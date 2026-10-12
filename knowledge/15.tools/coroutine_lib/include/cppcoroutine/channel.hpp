// ---------------------------------------------------------------------------
// cppcoroutine/channel.hpp
//
// Bounded, type-safe value channel for communication between coroutines that
// share one Scheduler.
//
// Semantics
// ---------
//   * FIFO, bounded by `capacity` elements and (optionally) by a byte budget.
//   * `capacity == 0` builds a rendezvous channel: a sender hands the value
//     straight to a parked receiver.
//   * A sender blocks when the channel is full, a receiver blocks when it is
//     empty.  Neither ever busy-waits: blocked coroutines are parked in the
//     scheduler's wait machinery.
//   * `close()` stops *sending*: parked senders fail with ChannelClosedError.
//     Values already accepted by the channel are still delivered to receivers
//     (drain semantics); once drained, `receive()` fails with
//     ChannelClosedError.
//   * Errors are never masked by a fabricated default value.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_CHANNEL_HPP
#define CPPCOROUTINE_CHANNEL_HPP

#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <exception>
#include <optional>
#include <stdexcept>
#include <utility>

#include "cppcoroutine/config.hpp"
#include "cppcoroutine/detail/wait_queue.hpp"
#include "cppcoroutine/result.hpp"
#include "cppcoroutine/scheduler.hpp"
#include "cppcoroutine/task_state.hpp"

namespace cppcoroutine {

template <class T>
class Channel;

namespace detail {

enum class ChannelOp { Delivered, Buffered, Blocked, Closed };

template <class T>
struct ChannelSendSlot {
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;
  T* value = nullptr;
};

template <class T>
struct ChannelReceiveSlot {
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;
  std::optional<T>* out = nullptr;
};

template <class T>
struct ChannelSendAwaiter;
template <class T>
struct ChannelReceiveAwaiter;

}  // namespace detail

template <class T>
class Channel {
 public:
  using value_type = T;
  using SizeFunction = std::size_t (*)(const T&);

  /// `size_function` (optional) returns the "cost" of a value; together with
  /// `byte_budget` it bounds the memory a channel may retain.
  explicit Channel(Scheduler& scheduler,
                   std::size_t capacity = default_channel_capacity,
                   std::size_t byte_budget = 0,
                   SizeFunction size_function = nullptr)
      : scheduler_(&scheduler),
        capacity_(capacity),
        byte_budget_(byte_budget),
        size_function_(size_function) {}

  ~Channel() { fail_waiters("channel destroyed while coroutines were waiting on it"); }

  Channel(const Channel&) = delete;
  Channel& operator=(const Channel&) = delete;
  Channel(Channel&&) = delete;
  Channel& operator=(Channel&&) = delete;

  // -------------------------------------------------------------------------
  // Coroutine API
  // -------------------------------------------------------------------------

  /// `co_await channel.send(std::move(value));`
  [[nodiscard]] detail::ChannelSendAwaiter<T> send(T value) {
    return detail::ChannelSendAwaiter<T>{this, std::move(value)};
  }

  /// `auto value = co_await channel.receive();`
  [[nodiscard]] detail::ChannelReceiveAwaiter<T> receive() {
    return detail::ChannelReceiveAwaiter<T>{this};
  }

  // -------------------------------------------------------------------------
  // Non-blocking API
  // -------------------------------------------------------------------------

  Result<void> try_send(T value) {
    const detail::ChannelOp op = send_impl(value);
    if (op == detail::ChannelOp::Delivered || op == detail::ChannelOp::Buffered) {
      return Result<void>::success();
    }
    if (op == detail::ChannelOp::Closed) {
      return Result<void>{
          std::make_exception_ptr(ChannelClosedError("send on a closed channel"))};
    }
    return Result<void>{std::make_exception_ptr(WouldBlockError{})};
  }

  Result<T> try_receive() {
    std::optional<T> out{};
    const detail::ChannelOp op = receive_impl(out);
    if (op == detail::ChannelOp::Delivered) {
      return Result<T>{std::move(*out)};
    }
    if (op == detail::ChannelOp::Closed) {
      return Result<T>{
          std::make_exception_ptr(ChannelClosedError("receive on a closed channel"))};
    }
    return Result<T>{std::make_exception_ptr(WouldBlockError{})};
  }

  // -------------------------------------------------------------------------
  // State
  // -------------------------------------------------------------------------

  /// Stops further sends and fails every parked sender with
  /// ChannelClosedError.  Idempotent.
  void close();

  [[nodiscard]] bool closed() const noexcept { return closed_; }
  [[nodiscard]] std::size_t size() const noexcept { return buffer_.size(); }
  [[nodiscard]] std::size_t capacity() const noexcept { return capacity_; }
  [[nodiscard]] std::size_t buffered_bytes() const noexcept { return buffered_bytes_; }

  /// Number of coroutines currently parked in `send`.
  [[nodiscard]] std::size_t sender_count() const;
  /// Number of coroutines currently parked in `receive`.
  [[nodiscard]] std::size_t receiver_count() const;

  [[nodiscard]] Scheduler& scheduler() const noexcept { return *scheduler_; }

 private:
  template <class U>
  friend struct detail::ChannelSendAwaiter;
  template <class U>
  friend struct detail::ChannelReceiveAwaiter;

  std::size_t measure(const T& value) const noexcept {
    return size_function_ != nullptr ? size_function_(value) : 0U;
  }

  bool has_room(const T& value) const noexcept {
    if (buffer_.size() >= capacity_) {
      return false;
    }
    if (byte_budget_ == 0 || size_function_ == nullptr) {
      return true;
    }
    return buffered_bytes_ + size_function_(value) <= byte_budget_;
  }

  bool peek_live_sender(detail::ChannelSendSlot<T>& out);
  bool pop_live_sender(detail::ChannelSendSlot<T>& out);
  bool pop_live_receiver(detail::ChannelReceiveSlot<T>& out);

  detail::ChannelOp send_impl(T& value);
  detail::ChannelOp receive_impl(std::optional<T>& out);

  void take_from_buffer(std::optional<T>& out);
  void refill_from_buffer();

  void park_sender(TaskId task, std::uint64_t generation, T* value) {
    senders_.push(detail::ChannelSendSlot<T>{task, generation, value});
  }

  void park_receiver(TaskId task, std::uint64_t generation, std::optional<T>* out) {
    receivers_.push(detail::ChannelReceiveSlot<T>{task, generation, out});
  }

  void fail_waiters(const char* reason);

  Scheduler* scheduler_ = nullptr;
  std::size_t capacity_ = default_channel_capacity;
  std::size_t byte_budget_ = 0;
  SizeFunction size_function_ = nullptr;
  std::size_t buffered_bytes_ = 0;
  bool closed_ = false;
  std::deque<T> buffer_{};
  detail::WaitQueueT<detail::ChannelSendSlot<T>> senders_{};
  detail::WaitQueueT<detail::ChannelReceiveSlot<T>> receivers_{};
};

// ---------------------------------------------------------------------------
// Channel<T> out-of-line members
// ---------------------------------------------------------------------------

template <class T>
bool Channel<T>::pop_live_receiver(detail::ChannelReceiveSlot<T>& out) {
  while (!receivers_.empty()) {
    detail::ChannelReceiveSlot<T>& front = receivers_.front();
    if (scheduler_->is_active_wait(front.task, front.generation)) {
      out = front;
      receivers_.pop_front();
      return true;
    }
    receivers_.pop_front();
  }
  return false;
}

template <class T>
bool Channel<T>::peek_live_sender(detail::ChannelSendSlot<T>& out) {
  while (!senders_.empty()) {
    detail::ChannelSendSlot<T>& front = senders_.front();
    if (scheduler_->is_active_wait(front.task, front.generation)) {
      out = front;
      return true;
    }
    senders_.pop_front();
  }
  return false;
}

template <class T>
bool Channel<T>::pop_live_sender(detail::ChannelSendSlot<T>& out) {
  if (!peek_live_sender(out)) {
    return false;
  }
  senders_.pop_front();
  return true;
}

template <class T>
detail::ChannelOp Channel<T>::send_impl(T& value) {
  if (closed_) {
    return detail::ChannelOp::Closed;
  }
  detail::ChannelReceiveSlot<T> slot;
  if (pop_live_receiver(slot)) {
    *slot.out = std::move(value);
    scheduler_->wake(slot.task, slot.generation);
    return detail::ChannelOp::Delivered;
  }
  if (has_room(value)) {
    T& stored = buffer_.emplace_back(std::move(value));
    buffered_bytes_ += measure(stored);
    return detail::ChannelOp::Buffered;
  }
  return detail::ChannelOp::Blocked;
}

template <class T>
void Channel<T>::take_from_buffer(std::optional<T>& out) {
  buffered_bytes_ -= measure(buffer_.front());
  out = std::move(buffer_.front());
  buffer_.pop_front();
}

template <class T>
void Channel<T>::refill_from_buffer() {
  if (capacity_ == 0 || buffer_.size() >= capacity_) {
    return;
  }
  detail::ChannelSendSlot<T> slot;
  if (!peek_live_sender(slot)) {
    return;
  }
  if (!has_room(*slot.value)) {
    return;
  }
  senders_.pop_front();
  T& stored = buffer_.emplace_back(std::move(*slot.value));
  buffered_bytes_ += measure(stored);
  scheduler_->wake(slot.task, slot.generation);
}

template <class T>
detail::ChannelOp Channel<T>::receive_impl(std::optional<T>& out) {
  if (!buffer_.empty()) {
    take_from_buffer(out);
    refill_from_buffer();
    return detail::ChannelOp::Delivered;
  }
  detail::ChannelSendSlot<T> slot;
  if (pop_live_sender(slot)) {
    out = std::move(*slot.value);
    scheduler_->wake(slot.task, slot.generation);
    return detail::ChannelOp::Delivered;
  }
  if (closed_) {
    return detail::ChannelOp::Closed;
  }
  return detail::ChannelOp::Blocked;
}

template <class T>
void Channel<T>::close() {
  if (closed_) {
    return;
  }
  closed_ = true;

  const std::exception_ptr closed_error =
      std::make_exception_ptr(ChannelClosedError("send on a closed channel"));
  while (!senders_.empty()) {
    detail::ChannelSendSlot<T> slot = senders_.pop_front();
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      scheduler_->fail_wait(slot.task, slot.generation, closed_error);
    }
  }

  const std::exception_ptr drained =
      std::make_exception_ptr(ChannelClosedError("receive on a closed channel"));
  while (!receivers_.empty()) {
    detail::ChannelReceiveSlot<T> slot = receivers_.pop_front();
    if (!scheduler_->is_active_wait(slot.task, slot.generation)) {
      continue;
    }
    if (!buffer_.empty()) {
      take_from_buffer(*slot.out);
      scheduler_->wake(slot.task, slot.generation);
    } else {
      scheduler_->fail_wait(slot.task, slot.generation, drained);
    }
  }
}

template <class T>
void Channel<T>::fail_waiters(const char* reason) {
  const std::exception_ptr error =
      std::make_exception_ptr(ObjectDestroyedError(reason));
  for (auto& slot : senders_) {
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      scheduler_->fail_wait(slot.task, slot.generation, error);
    }
  }
  for (auto& slot : receivers_) {
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      scheduler_->fail_wait(slot.task, slot.generation, error);
    }
  }
  senders_.clear();
  receivers_.clear();
}

template <class T>
std::size_t Channel<T>::sender_count() const {
  std::size_t count = 0;
  for (const auto& slot : senders_) {
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      ++count;
    }
  }
  return count;
}

template <class T>
std::size_t Channel<T>::receiver_count() const {
  std::size_t count = 0;
  for (const auto& slot : receivers_) {
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      ++count;
    }
  }
  return count;
}

// ---------------------------------------------------------------------------
// Awaiters
// ---------------------------------------------------------------------------

namespace detail {

template <class T>
struct ChannelSendAwaiter {
  Channel<T>* channel = nullptr;
  std::optional<T> value{};
  Scheduler* scheduler = nullptr;
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;
  bool closed_immediately = false;

  ChannelSendAwaiter(Channel<T>* channel_in, T value_in)
      : channel(channel_in), value(std::move(value_in)) {}

  ChannelSendAwaiter(const ChannelSendAwaiter&) = delete;
  ChannelSendAwaiter& operator=(const ChannelSendAwaiter&) = delete;
  ChannelSendAwaiter(ChannelSendAwaiter&&) = delete;
  ChannelSendAwaiter& operator=(ChannelSendAwaiter&&) = delete;

  /// Always go through await_suspend: the send may complete synchronously
  /// there (in which case we return false and never suspend).
  [[nodiscard]] bool await_ready() const noexcept { return false; }

  bool await_suspend(std::coroutine_handle<>) {
    scheduler = Scheduler::current();
    if (scheduler == nullptr) {
      throw std::logic_error("cppcoroutine: Channel::send() outside of a running Scheduler");
    }
    task = Scheduler::current_task_id();
    generation = scheduler->begin_wait(task, TaskState::WaitingChannel);

    const ChannelOp op = channel->send_impl(*value);
    if (op == ChannelOp::Delivered || op == ChannelOp::Buffered) {
      scheduler->abort_wait(task, generation);
      return false;
    }
    if (op == ChannelOp::Closed) {
      scheduler->abort_wait(task, generation);
      closed_immediately = true;
      return false;
    }
    channel->park_sender(task, generation, &*value);
    return true;
  }

  void await_resume() {
    if (closed_immediately) {
      throw ChannelClosedError("send on a closed channel");
    }
    scheduler->rethrow_wait_error(task);
  }
};

template <class T>
struct ChannelReceiveAwaiter {
  Channel<T>* channel = nullptr;
  std::optional<T> result{};
  Scheduler* scheduler = nullptr;
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;
  bool closed_immediately = false;

  explicit ChannelReceiveAwaiter(Channel<T>* channel_in) noexcept : channel(channel_in) {}

  ChannelReceiveAwaiter(const ChannelReceiveAwaiter&) = delete;
  ChannelReceiveAwaiter& operator=(const ChannelReceiveAwaiter&) = delete;
  ChannelReceiveAwaiter(ChannelReceiveAwaiter&&) = delete;
  ChannelReceiveAwaiter& operator=(ChannelReceiveAwaiter&&) = delete;

  [[nodiscard]] bool await_ready() const noexcept { return false; }

  bool await_suspend(std::coroutine_handle<>) {
    scheduler = Scheduler::current();
    if (scheduler == nullptr) {
      throw std::logic_error("cppcoroutine: Channel::receive() outside of a running Scheduler");
    }
    task = Scheduler::current_task_id();
    generation = scheduler->begin_wait(task, TaskState::WaitingChannel);

    const ChannelOp op = channel->receive_impl(result);
    if (op == ChannelOp::Delivered) {
      scheduler->abort_wait(task, generation);
      return false;
    }
    if (op == ChannelOp::Closed) {
      scheduler->abort_wait(task, generation);
      closed_immediately = true;
      return false;
    }
    channel->park_receiver(task, generation, &result);
    return true;
  }

  T await_resume() {
    if (closed_immediately) {
      throw ChannelClosedError("receive on a closed channel");
    }
    scheduler->rethrow_wait_error(task);
    if (!result.has_value()) {
      throw std::logic_error("cppcoroutine: channel receiver woken without a value");
    }
    return std::move(*result);
  }
};

}  // namespace detail
}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_CHANNEL_HPP
