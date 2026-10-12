// ---------------------------------------------------------------------------
// cppcoroutine/event.hpp
//
// Coroutine-friendly events.
//
// Two well-separated semantics (never mixed through an ambiguous flag):
//
//   ManualResetEvent  A notification is *persistent*.  Once notified the event
//                     stays signalled until reset(); every subsequent `wait()`
//                     returns immediately.  A notify that arrives when nobody
//                     is waiting is remembered.
//
//   AutoResetEvent    A notification is *consumed* by exactly one `wait()`.
//                     notify_one() hands the signal to one parked waiter (or
//                     remembers one pending signal); notify_all() releases
//                     only the waiters that are parked right now.
//
// Both variants are parked inside the owning Scheduler: no Event ever creates
// its own event loop.
//
// Header-only: the Event definitions below used to live in src/event.cpp.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_EVENT_HPP
#define CPPCOROUTINE_EVENT_HPP

#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <utility>

#include "cppcoroutine/config.hpp"
#include "cppcoroutine/detail/wait_queue.hpp"
#include "cppcoroutine/result.hpp"
#include "cppcoroutine/scheduler.hpp"
#include "cppcoroutine/task_state.hpp"

namespace cppcoroutine {

class Event;

namespace detail {
struct EventWaitAwaiter;
}

class Event {
 public:
  enum class Mode {
    /// Notifications persist until reset().
    ManualReset,
    /// Each notification releases exactly one waiter.
    AutoReset,
  };

  explicit Event(Scheduler& scheduler, Mode mode = Mode::ManualReset);
  ~Event();

  Event(const Event&) = delete;
  Event& operator=(const Event&) = delete;
  Event(Event&&) = delete;
  Event& operator=(Event&&) = delete;

  /// ManualReset: sets the signal (persistently) and releases one parked
  /// waiter if there is one.
  /// AutoReset: releases one parked waiter, or remembers a single pending
  /// signal when nobody is parked.
  void notify_one();

  /// ManualReset: sets the signal and releases every parked waiter.
  /// AutoReset: releases every *currently parked* waiter; if none are parked,
  /// remembers a single pending signal (it does not queue N signals).
  void notify_all();

  /// ManualReset: clears the persistent signal.
  /// AutoReset: clears a pending, not-yet-consumed signal.
  void reset();

  [[nodiscard]] bool signaled() const noexcept { return signaled_; }

  /// Number of live waiters currently parked.
  [[nodiscard]] std::size_t waiter_count() const;

  [[nodiscard]] detail::EventWaitAwaiter wait();

 private:
  friend struct detail::EventWaitAwaiter;

  /// Consumes an immediately available signal, if any.  Returns true when the
  /// caller must *not* park.
  bool try_acquire() noexcept;

  void enqueue(TaskId task, std::uint64_t generation);

  /// Pops the oldest live waiter into `task`/`generation`.
  bool take_live_waiter(TaskId& task, std::uint64_t& generation);

  /// Fails every parked waiter (used by the destructor).
  void fail_all_waiters(const char* reason);

  Scheduler* scheduler_ = nullptr;
  Mode mode_ = Mode::ManualReset;
  bool signaled_ = false;
  detail::WaitQueue waiters_{};
};

// ---------------------------------------------------------------------------
// Event implementation (was src/event.cpp)
// ---------------------------------------------------------------------------

inline Event::Event(Scheduler& scheduler, Mode mode) : scheduler_(&scheduler), mode_(mode) {}

inline Event::~Event() {
  fail_all_waiters("event destroyed while a coroutine was waiting on it");
}

inline bool Event::try_acquire() noexcept {
  if (!signaled_) {
    return false;
  }
  if (mode_ == Mode::AutoReset) {
    signaled_ = false;
  }
  return true;
}

inline void Event::enqueue(TaskId task, std::uint64_t generation) {
  waiters_.push(detail::WaitSlot{task, generation});
}

inline bool Event::take_live_waiter(TaskId& task, std::uint64_t& generation) {
  while (!waiters_.empty()) {
    const detail::WaitSlot slot = waiters_.pop_front();
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      task = slot.task;
      generation = slot.generation;
      return true;
    }
  }
  return false;
}

inline void Event::notify_one() {
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;

  if (mode_ == Mode::ManualReset) {
    signaled_ = true;
    if (take_live_waiter(task, generation)) {
      (void)scheduler_->wake(task, generation);
    }
    return;
  }

  // AutoReset: hand the token to one parked waiter, or remember it.
  if (take_live_waiter(task, generation)) {
    (void)scheduler_->wake(task, generation);
    return;
  }
  signaled_ = true;
}

inline void Event::notify_all() {
  if (mode_ == Mode::ManualReset) {
    signaled_ = true;
  }

  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;
  bool released_any = false;
  while (take_live_waiter(task, generation)) {
    (void)scheduler_->wake(task, generation);
    released_any = true;
  }

  if (mode_ == Mode::AutoReset && !released_any) {
    signaled_ = true;
  }
}

inline void Event::reset() { signaled_ = false; }

inline std::size_t Event::waiter_count() const {
  std::size_t count = 0;
  for (const auto& slot : waiters_) {
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      count += 1;
    }
  }
  return count;
}

inline void Event::fail_all_waiters(const char* reason) {
  if (scheduler_ == nullptr) {
    waiters_.clear();
    return;
  }
  const std::exception_ptr error = std::make_exception_ptr(ObjectDestroyedError(reason));
  while (!waiters_.empty()) {
    const detail::WaitSlot slot = waiters_.pop_front();
    if (scheduler_->is_active_wait(slot.task, slot.generation)) {
      (void)scheduler_->fail_wait(slot.task, slot.generation, error);
    }
  }
}

namespace detail {

struct EventWaitAwaiter {
  Event* event = nullptr;
  Scheduler* scheduler = nullptr;
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;

  [[nodiscard]] bool await_ready() const noexcept { return false; }

  bool await_suspend(std::coroutine_handle<>) {
    scheduler = Scheduler::current();
    task = Scheduler::current_task_id();
    if (scheduler == nullptr) {
      throw std::logic_error(
          "cppcoroutine: Event::wait() used outside of a running Scheduler");
    }
    generation = scheduler->begin_wait(task, TaskState::WaitingEvent);
    if (event->try_acquire()) {
      scheduler->abort_wait(task, generation);
      return false;  // signalled already: do not suspend
    }
    event->enqueue(task, generation);
    return true;
  }

  void await_resume() { scheduler->rethrow_wait_error(task); }
};

}  // namespace detail

inline detail::EventWaitAwaiter Event::wait() { return detail::EventWaitAwaiter{this}; }

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_EVENT_HPP
