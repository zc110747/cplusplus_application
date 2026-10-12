// ---------------------------------------------------------------------------
// cppcoroutine/timer.hpp
//
// Non-blocking timers.
//
//   * Coroutine timers: `co_await scheduler.sleep_for(100ms);`
//   * Callback timers:  `auto t = scheduler.set_timeout(1s, fn);`
//
// There is deliberately no separate "TimerHandle" for coroutine sleeps:
// a coroutine timer is owned by the task that created it, so it is cancelled
// through the task (`scheduler.cancel(task_id)`), which unregisters the wait
// relation atomically with the state transition.  `TimerHandle` exists for
// callback timers, where there is no task to cancel.
//
// Header-only: the TimerHandle definitions below used to live in src/timer.cpp.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_TIMER_HPP
#define CPPCOROUTINE_TIMER_HPP

#include <chrono>
#include <coroutine>
#include <cstdint>
#include <memory>
#include <utility>

#include "cppcoroutine/config.hpp"
#include "cppcoroutine/detail/timer_queue.hpp"
#include "cppcoroutine/scheduler.hpp"
#include "cppcoroutine/task_state.hpp"

namespace cppcoroutine {

/// Move-only, RAII handle for a callback timer.
///
///   * Destroying a handle that still owns a pending timer cancels it.
///   * `cancel()` is idempotent and thread-safe.
///   * Timers fire on the event-loop thread, inside the loop iteration.
class TimerHandle {
 public:
  TimerHandle() noexcept = default;
  TimerHandle(TimerHandle&& other) noexcept = default;
  /// Move-assignment cancels the timer this handle used to own, then adopts
  /// `other`'s timer.
  TimerHandle& operator=(TimerHandle&& other) noexcept;
  TimerHandle(const TimerHandle&) = delete;
  TimerHandle& operator=(const TimerHandle&) = delete;

  ~TimerHandle();

  /// Cancels the timer.  Returns true if it was still pending.
  bool cancel() noexcept;

  [[nodiscard]] bool valid() const noexcept { return control_ != nullptr; }

  /// True until the timer fires or is cancelled.
  [[nodiscard]] bool pending() const noexcept;

  [[nodiscard]] bool cancelled() const noexcept;

  /// How many times the callback ran (always <= 1 for one-shot timers).
  [[nodiscard]] std::uint64_t fire_count() const noexcept;

 private:
  friend class Scheduler;
  explicit TimerHandle(std::shared_ptr<detail::TimerControl> control) noexcept
      : control_(std::move(control)) {}

  std::shared_ptr<detail::TimerControl> control_{};
};

// ---------------------------------------------------------------------------
// TimerHandle implementation (was src/timer.cpp)
// ---------------------------------------------------------------------------

inline TimerHandle::~TimerHandle() {
  if (control_) {
    (void)cancel();
  }
}

inline TimerHandle& TimerHandle::operator=(TimerHandle&& other) noexcept {
  if (this != &other) {
    (void)cancel();
    control_ = std::move(other.control_);
  }
  return *this;
}

inline bool TimerHandle::cancel() noexcept {
  if (!control_) {
    return false;
  }
  if (control_->scheduler == nullptr) {
    return !control_->cancelled.exchange(true, std::memory_order_acq_rel);
  }
  return control_->scheduler->cancel_callback_timer(control_);
}

inline bool TimerHandle::pending() const noexcept {
  if (!control_) {
    return false;
  }
  if (control_->cancelled.load(std::memory_order_acquire)) {
    return false;
  }
  if (!control_->periodic && control_->fire_count.load(std::memory_order_acquire) > 0) {
    return false;
  }
  return true;
}

inline bool TimerHandle::cancelled() const noexcept {
  return control_ != nullptr && control_->cancelled.load(std::memory_order_acquire);
}

inline std::uint64_t TimerHandle::fire_count() const noexcept {
  return control_ != nullptr ? control_->fire_count.load(std::memory_order_acquire) : 0;
}

// ---------------------------------------------------------------------------
// detail::SleepAwaiter
// ---------------------------------------------------------------------------
namespace detail {

/// Awaitable returned by `Scheduler::sleep_for` / `sleep_until`.
struct SleepAwaiter {
  Scheduler* scheduler = nullptr;
  Scheduler::clock::time_point deadline{};
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;

  /// A deadline that has already passed does not cost a suspension round-trip,
  /// but it *is* still a cancellation checkpoint.
  bool await_ready() noexcept {
    task = Scheduler::current_task_id();
    return deadline <= Scheduler::clock::now();
  }

  void await_suspend(std::coroutine_handle<>) {
    generation = scheduler->begin_wait(task, TaskState::WaitingTimer);
    scheduler->add_timer(deadline, task, generation);
  }

  void await_resume() { scheduler->rethrow_wait_error(task); }
};

}  // namespace detail

// ---------------------------------------------------------------------------
// Scheduler timer entry points (the coroutine ones; the callback-timer
// factory functions live here too, next to TimerHandle)
// ---------------------------------------------------------------------------

inline detail::SleepAwaiter Scheduler::sleep_for(clock::duration duration) {
  return detail::SleepAwaiter{this, clock::now() + duration, invalid_task_id, 0};
}

inline detail::SleepAwaiter Scheduler::sleep_until(clock::time_point deadline) {
  return detail::SleepAwaiter{this, deadline, invalid_task_id, 0};
}

inline TimerHandle Scheduler::set_timeout(clock::duration delay, Callback callback) {
  return set_timeout_at(clock::now() + delay, std::move(callback));
}

inline TimerHandle Scheduler::set_timeout_at(clock::time_point deadline, Callback callback) {
  auto control = std::make_shared<detail::TimerControl>();
  control->scheduler = this;
  control->periodic = false;
  control->callback = std::move(callback);
  add_callback_timer(control, deadline);
  return TimerHandle{std::move(control)};
}

inline TimerHandle Scheduler::set_interval(clock::duration period, Callback callback) {
  if (period <= clock::duration::zero()) {
    period = std::chrono::milliseconds(1);
  }
  auto control = std::make_shared<detail::TimerControl>();
  control->scheduler = this;
  control->periodic = true;
  control->period = period;
  control->callback = std::move(callback);
  add_callback_timer(control, clock::now() + period);
  return TimerHandle{std::move(control)};
}

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_TIMER_HPP
