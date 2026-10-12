// ---------------------------------------------------------------------------
// cppcoroutine/task.hpp
//
// `Task<T>` - the coroutine task type, its promise and its awaiter.
//
// Ownership
// ---------
//   * A freshly created `Task<T>` owns its coroutine frame.
//   * `Scheduler::spawn()` (or `co_await`ing it) transfers frame ownership to
//     the scheduler; the Task object is left empty.
//   * A Task that is destroyed while it still owns the frame destroys the
//     frame.  A Task that no longer owns anything is a no-op.  This is what
//     makes temporary Tasks safe: `sched.spawn(make_task())` can never pull a
//     running coroutine out from under the scheduler.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_TASK_HPP
#define CPPCOROUTINE_TASK_HPP

#include <coroutine>
#include <exception>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "cppcoroutine/config.hpp"
#include "cppcoroutine/detail/task_control_block.hpp"
#include "cppcoroutine/result.hpp"
#include "cppcoroutine/scheduler.hpp"
#include "cppcoroutine/task_state.hpp"

namespace cppcoroutine {

template <class T>
class Task;
template <class T>
class TaskAwaiter;

// ---------------------------------------------------------------------------
// detail
// ---------------------------------------------------------------------------
namespace detail {

/// Suspends the completing coroutine forever; the scheduler reclaims it once
/// nobody can observe the result anymore.
struct FinalAwaiter {
  TaskControlBlock* control = nullptr;

  [[nodiscard]] bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<>) const noexcept {
    if (control != nullptr && control->sched != nullptr) {
      control->sched->finish_task(*control);
    }
  }

  void await_resume() const noexcept {}
};

/// Shared promise state for `Task<T>` and `Task<void>`.
struct TaskPromiseBaseCommon {
  TaskControlBlock control{};
  bool cancelled_by_exception = false;
  bool completed_by_return = false;

  TaskPromiseBaseCommon() noexcept {
    control.id = next_task_id();
    ++global_counters().frames_created;
  }

  ~TaskPromiseBaseCommon() { ++global_counters().frames_destroyed; }

  TaskPromiseBaseCommon(const TaskPromiseBaseCommon&) = delete;
  TaskPromiseBaseCommon& operator=(const TaskPromiseBaseCommon&) = delete;

  /// Lazy start: the body does not run until the scheduler resumes the frame.
  std::suspend_always initial_suspend() noexcept { return {}; }

  FinalAwaiter final_suspend() noexcept {
    // Terminal state reflects the outcome the *caller* observes:
    //   * a non-cancellation exception escaped  -> Failed
    //   * cancellation was requested (or a CancellationError escaped)
    //                                           -> Cancelled
    //   * otherwise                             -> Completed
    // Note that a task which catches CancellationError and returns normally
    // still ends as Cancelled: the request was accepted, so the caller must
    // not mistake it for a successful run.
    if (control.exception && !cancelled_by_exception) {
      control.state = TaskState::Failed;
    } else if (cancelled_by_exception || control.cancel_requested) {
      control.state = TaskState::Cancelled;
    } else {
      control.state = TaskState::Completed;
    }
    return FinalAwaiter{&control};
  }

  void unhandled_exception() noexcept {
    control.exception = std::current_exception();
    try {
      std::rethrow_exception(control.exception);
    } catch (const CancellationError&) {
      cancelled_by_exception = true;
    } catch (...) {
      // A genuine failure: it is recorded in control.exception and reported
      // as TaskState::Failed.  It is never silently dropped.
    }
  }
};

template <class T>
struct TaskPromise : TaskPromiseBaseCommon {
  std::optional<T> value{};

  Task<T> get_return_object() noexcept;

  void return_value(T value_in) { value.emplace(std::move(value_in)); }
};

template <>
struct TaskPromise<void> : TaskPromiseBaseCommon {
  Task<void> get_return_object() noexcept;

  void return_void() noexcept { completed_by_return = true; }
};

}  // namespace detail

// ---------------------------------------------------------------------------
// TaskAwaiter
// ---------------------------------------------------------------------------

/// Awaiter produced by `co_await someTask`.
///
/// The awaiter *takes over* the child's coroutine handle: if the child has not
/// been spawned yet it is registered with the currently running scheduler.
template <class T>
class TaskAwaiter {
 public:
  explicit TaskAwaiter(std::coroutine_handle<detail::TaskPromise<T>> handle) noexcept
      : handle_(handle), owns_(handle != nullptr) {}

  TaskAwaiter(TaskAwaiter&& other) noexcept { move_from(other); }
  TaskAwaiter(const TaskAwaiter&) = delete;
  TaskAwaiter& operator=(const TaskAwaiter&) = delete;
  TaskAwaiter& operator=(TaskAwaiter&&) = delete;

  ~TaskAwaiter() {
    release();
    if (owns_ && handle_ != nullptr) {
      handle_.destroy();
    }
  }

  [[nodiscard]] bool await_ready() const noexcept { return false; }

  bool await_suspend(std::coroutine_handle<> parent) {
    if (handle_ == nullptr) {
      invalid_ = true;
      return false;
    }

    detail::TaskPromise<T>& promise = handle_.promise();
    TaskControlBlock& child = promise.control;

    Scheduler* sched = child.sched;
    if (sched == nullptr) {
      sched = Scheduler::current();
      if (sched == nullptr) {
        invalid_ = true;
        return false;
      }
      child.sched = sched;
      sched->register_task(child, std::coroutine_handle<>{handle_});
    } else if (sched != Scheduler::current()) {
      throw std::logic_error(
          "cppcoroutine: co_await on a Task that belongs to a different Scheduler");
    }
    owns_ = false;  // the scheduler owns the frame from here on

    sched_ = sched;
    child_id_ = child.id;

    if (is_terminal(child.state)) {
      return false;  // nothing to wait for: read the result immediately
    }

    if (child.continuation != nullptr) {
      throw std::logic_error(
          "cppcoroutine: a Task may be awaited by at most one coroutine");
    }

    parent_id_ = Scheduler::current_task_id();
    child.awaiter_refs += 1;
    child.continuation = parent;
    child.continuation_id = parent_id_;
    attached_ = true;

    if (TaskControlBlock* parent_control = sched->control_block(parent_id_);
        parent_control != nullptr) {
      parent_control->awaiting_child = child_id_;
    }
    sched->begin_wait(parent_id_, TaskState::WaitingEvent);
    return true;
  }

  T await_resume() {
    if (invalid_ || handle_ == nullptr) {
      throw std::logic_error(
          "cppcoroutine: co_await on an empty Task, or without a running Scheduler");
    }

    detail::TaskPromise<T>& promise = handle_.promise();
    TaskControlBlock& child = promise.control;
    Scheduler* sched = sched_;

    release();

    // A failed child always propagates - exceptions are never swallowed.
    if (child.exception) {
      std::rethrow_exception(child.exception);
    }
    if (sched != nullptr) {
      sched->check_cancelled(Scheduler::current_task_id());
    }

    if constexpr (std::is_void_v<T>) {
      return;
    } else {
      return std::move(promise.value).value();
    }
  }

 private:
  void move_from(TaskAwaiter& other) noexcept {
    handle_ = other.handle_;
    sched_ = other.sched_;
    child_id_ = other.child_id_;
    parent_id_ = other.parent_id_;
    attached_ = other.attached_;
    owns_ = other.owns_;
    invalid_ = other.invalid_;

    other.handle_ = {};
    other.sched_ = nullptr;
    other.child_id_ = invalid_task_id;
    other.parent_id_ = invalid_task_id;
    other.attached_ = false;
    other.owns_ = false;
    other.invalid_ = false;
  }

  void release() noexcept {
    if (attached_ && sched_ != nullptr) {
      if (TaskControlBlock* parent = sched_->control_block(parent_id_); parent != nullptr) {
        if (parent->awaiting_child == child_id_) {
          parent->awaiting_child = invalid_task_id;
        }
      }
      sched_->release_awaiter(child_id_);
      attached_ = false;
    }
  }

  std::coroutine_handle<detail::TaskPromise<T>> handle_{};
  Scheduler* sched_ = nullptr;
  TaskId child_id_ = invalid_task_id;
  TaskId parent_id_ = invalid_task_id;
  bool attached_ = false;
  bool owns_ = false;
  bool invalid_ = false;
};

// ---------------------------------------------------------------------------
// Task
// ---------------------------------------------------------------------------

template <class T>
class Task {
 public:
  using promise_type = detail::TaskPromise<T>;

  Task() noexcept = default;

  Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}

  Task& operator=(Task&& other) noexcept {
    if (this != &other) {
      destroy_if_owned();
      handle_ = std::exchange(other.handle_, {});
    }
    return *this;
  }

  Task(const Task&) = delete;
  Task& operator=(const Task&) = delete;

  ~Task() { destroy_if_owned(); }

  /// True while this object still owns a coroutine frame.
  [[nodiscard]] bool valid() const noexcept { return handle_ != nullptr; }

  /// Id of the underlying task (valid even before it is spawned).
  [[nodiscard]] TaskId id() const noexcept {
    return handle_ != nullptr ? handle_.promise().control.id : invalid_task_id;
  }

  /// Current state of the underlying task.
  [[nodiscard]] TaskState state() const noexcept {
    return handle_ != nullptr ? handle_.promise().control.state : TaskState::Created;
  }

  /// `co_await task;` - starts the task (if needed) and waits for it.
  TaskAwaiter<T> operator co_await() && noexcept {
    return TaskAwaiter<T>{std::exchange(handle_, {})};
  }

 private:
  friend class Scheduler;
  template <class U>
  friend struct detail::TaskPromise;

  explicit Task(std::coroutine_handle<promise_type> handle) noexcept : handle_(handle) {}

  std::coroutine_handle<promise_type> detach() noexcept {
    return std::exchange(handle_, {});
  }

  void destroy_if_owned() noexcept {
    if (handle_ != nullptr) {
      handle_.destroy();
      handle_ = {};
    }
  }

  std::coroutine_handle<promise_type> handle_{};
};

// ---------------------------------------------------------------------------
// Promise member definitions (need Task<T> to be complete)
// ---------------------------------------------------------------------------

template <class T>
inline Task<T> detail::TaskPromise<T>::get_return_object() noexcept {
  return Task<T>{std::coroutine_handle<TaskPromise<T>>::from_promise(*this)};
}

inline Task<void> detail::TaskPromise<void>::get_return_object() noexcept {
  return Task<void>{std::coroutine_handle<TaskPromise<void>>::from_promise(*this)};
}

// ---------------------------------------------------------------------------
// Scheduler::spawn
// ---------------------------------------------------------------------------

template <class T>
TaskId Scheduler::spawn(Task<T>&& task) {
  if (!task.valid()) {
    return invalid_task_id;
  }
  std::coroutine_handle<typename Task<T>::promise_type> handle = task.detach();
  TaskControlBlock& control = handle.promise().control;
  if (control.sched != nullptr) {
    // Double spawn: refuse, and release the frame we just took over.
    handle.destroy();
    return invalid_task_id;
  }
  return register_task(control, std::coroutine_handle<>{handle});
}

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_TASK_HPP
