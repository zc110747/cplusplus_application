// ---------------------------------------------------------------------------
// cppcoroutine/cancellation.hpp
//
// Unified cancellation.
//
//   * Requesting cancellation: `scheduler.cancel(task_id)` / `handle.cancel()`
//     / `scheduler.cancel_all()`.  Requesting and completing are distinct:
//     `cancel()` only records the request.
//   * Observing it: every library await point is a cancellation checkpoint.
//     A cancelled task that is parked in a wait is unregistered and resumed
//     with a thrown CancellationError; a running task observes it at its next
//     await point (`co_await cppcoroutine::cancellation_point()` is one).
//   * Cancellation never force-destroys a suspended coroutine: the frame is
//     always unwound normally, so local destructors run in the right order.
//   * All cancellation operations are idempotent.
//
// Header-only: the TaskHandle definitions below used to live in
// src/cancellation.cpp.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_CANCELLATION_HPP
#define CPPCOROUTINE_CANCELLATION_HPP

#include <coroutine>
#include <cstdint>

#include "cppcoroutine/config.hpp"
#include "cppcoroutine/result.hpp"
#include "cppcoroutine/scheduler.hpp"
#include "cppcoroutine/task_state.hpp"

namespace cppcoroutine {

/// Scheduler-mediated, type-erased handle to a task.
///
/// It deliberately stores only (Scheduler*, TaskId) - never a raw coroutine
/// pointer - so it remains safe to query after the task has finished and its
/// frame has been reclaimed.
class TaskHandle {
 public:
  TaskHandle() noexcept = default;
  TaskHandle(Scheduler& scheduler, TaskId task) noexcept
      : scheduler_(&scheduler), id_(task) {}

  [[nodiscard]] bool valid() const noexcept {
    return scheduler_ != nullptr && id_ != invalid_task_id;
  }

  [[nodiscard]] TaskId id() const noexcept { return id_; }

  /// Current state; well-defined after completion and after reclamation.
  [[nodiscard]] TaskState state() const noexcept;

  [[nodiscard]] bool finished() const noexcept { return is_terminal(state()); }

  /// Requests cancellation.  Returns true if the request was accepted.
  [[nodiscard]] bool cancel() const;

  [[nodiscard]] bool cancel_requested() const noexcept;

 private:
  Scheduler* scheduler_ = nullptr;
  TaskId id_ = invalid_task_id;
};

// ---------------------------------------------------------------------------
// TaskHandle implementation (was src/cancellation.cpp)
// ---------------------------------------------------------------------------

inline TaskState TaskHandle::state() const noexcept {
  if (scheduler_ == nullptr || id_ == invalid_task_id) {
    return TaskState::Created;
  }
  return scheduler_->task_state(id_);
}

inline bool TaskHandle::cancel() const {
  if (scheduler_ == nullptr || id_ == invalid_task_id) {
    return false;
  }
  return scheduler_->cancel(id_);
}

inline bool TaskHandle::cancel_requested() const noexcept {
  if (scheduler_ == nullptr || id_ == invalid_task_id) {
    return false;
  }
  return scheduler_->is_cancel_requested(id_);
}

namespace detail {

/// Awaitable that never suspends; it only performs a cancellation check.
struct CancellationPointAwaiter {
  TaskId task = invalid_task_id;

  bool await_ready() {
    task = Scheduler::current_task_id();
    if (Scheduler* scheduler = Scheduler::current(); scheduler != nullptr) {
      scheduler->check_cancelled(task);
    }
    return true;
  }

  void await_suspend(std::coroutine_handle<>) const noexcept {}
  void await_resume() const noexcept {}
};

}  // namespace detail

/// `co_await cppcoroutine::cancellation_point();`
[[nodiscard]] inline detail::CancellationPointAwaiter cancellation_point() noexcept {
  return detail::CancellationPointAwaiter{};
}

/// `co_await scheduler.cancellation_point();` - identical to the free function
/// above; provided so the scheduler-centric style reads consistently.
[[nodiscard]] inline detail::CancellationPointAwaiter Scheduler::cancellation_point() noexcept {
  return detail::CancellationPointAwaiter{};
}

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_CANCELLATION_HPP
