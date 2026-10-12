// ---------------------------------------------------------------------------
// cppcoroutine/detail/task_control_block.hpp
//
// TaskControlBlock (TCB) - the stable, coroutine-type-erased record that the
// Scheduler uses to manage a task.
//
// Ownership model
// ---------------
//   * The TCB is a *member* of the task's promise object, therefore it lives
//     inside the coroutine frame and is destroyed together with it.
//   * A Scheduler keeps a `TaskRecord` per registered task holding a raw
//     pointer to the TCB plus the (type-erased) coroutine handle.  The record
//     also caches the last observed state so that `Scheduler::task_state(id)`
//     stays well-defined after the frame has been reclaimed.
//   * Nothing else owns a TCB.  Task objects hand their frame to the
//     scheduler on `spawn()`; after that the scheduler is the sole owner of
//     the coroutine frame.
//
// This header is an implementation detail.  It is visible from public headers
// only because `Task<T>` is a template and the compiler needs the complete
// type of its promise.  It is *not* a stable public interface.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_DETAIL_TASK_CONTROL_BLOCK_HPP
#define CPPCOROUTINE_DETAIL_TASK_CONTROL_BLOCK_HPP

#include <atomic>
#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <exception>

#include "cppcoroutine/config.hpp"
#include "cppcoroutine/task_state.hpp"

namespace cppcoroutine {

class Scheduler;

/// Per-task bookkeeping.  One instance per coroutine frame, living inside the
/// promise object.
struct TaskControlBlock {
  // -- identity -------------------------------------------------------------
  TaskId id = invalid_task_id;

  // -- state machine --------------------------------------------------------
  TaskState state = TaskState::Created;

  /// Type-erased handle to *this* coroutine.  Valid only while the frame is
  /// alive (`TaskRecord::alive`).
  std::coroutine_handle<> handle{};

  /// Captured exception for failed tasks (may hold CancellationError).
  std::exception_ptr exception{};

  /// Set by the destructor of an Event/Channel to fail a parked waiter
  /// (ObjectDestroyedError) or by channel teardown (ChannelClosedError).
  /// Consumed exactly once by the corresponding await point.
  std::exception_ptr wait_error{};

  // -- wake-up bookkeeping --------------------------------------------------
  /// Monotonically increasing generation counter.  Bumped on every suspend so
  /// that a stale registration (timer entry, event/waiter slot, channel slot)
  /// can never wake a task that has already moved on.  This is the mechanism
  /// that makes "only one wake source may win" hold without relying on locks.
  std::uint64_t wait_gen = 0;

  /// Set when a cancelled waiting task is resumed: the library await point
  /// turns it into a thrown CancellationError.
  bool wait_cancelled = false;

  /// Set as soon as a cancellation request is observed.  Sticky: every
  /// subsequent library await point re-throws.
  bool cancel_requested = false;

  // -- lifecycle ------------------------------------------------------------
  /// True once the coroutine body has been entered at least once.
  bool started = false;

  /// Result-consumer counter.  Non-zero while a parent coroutine is parked in
  /// a `co_await task` and still has to read this task's result out of the
  /// frame.  The scheduler refuses to reclaim the frame until it hits zero.
  std::size_t awaiter_refs = 0;

  /// Coroutine that is waiting for *this* task to finish (task join).
  std::coroutine_handle<> continuation{};
  TaskId continuation_id = invalid_task_id;

  /// If non-zero, this task is currently parked inside `co_await <child>`.
  /// Used as a safety net so that frames destroyed without stack unwinding
  /// still release the child's `awaiter_refs`.
  TaskId awaiting_child = invalid_task_id;

  /// Set while the task id sits in the internal reclaim queue (prevents
  /// duplicate entries).
  bool reclaim_scheduled = false;

  /// Set when the task reached a terminal state while a consumer still needed
  /// its result; the frame is reclaimed as soon as the last reference drops.
  bool reclaim_when_unref = false;

  /// The owning scheduler.  Null for a Task that has never been spawned.
  Scheduler* sched = nullptr;
};

// ---------------------------------------------------------------------------
// Process-wide counters.
//
// They are process-wide (not per scheduler) on purpose: the lifecycle tests
// assert `frames_created == frames_destroyed` at the end of a test binary,
// which is a stronger statement than a per-scheduler check.
//
// The library is header-only, so the accessors are `inline` and rely on the
// single-instance guarantee for locals of inline functions: every translation
// unit that includes this header shares one counter block.
// ---------------------------------------------------------------------------
struct GlobalCounters {
  std::atomic<std::uint64_t> frames_created{0};
  std::atomic<std::uint64_t> frames_destroyed{0};
  std::atomic<std::uint64_t> tasks_created{0};
};

inline GlobalCounters& global_counters() noexcept {
  static GlobalCounters counters{};
  return counters;
}

/// Allocates the next unique task id (also bumps `tasks_created`).
inline TaskId next_task_id() noexcept {
  static std::atomic<TaskId> next{1};
  global_counters().tasks_created.fetch_add(1, std::memory_order_relaxed);
  TaskId id = next.fetch_add(1, std::memory_order_relaxed);
  if (id == invalid_task_id) {
    id = next.fetch_add(1, std::memory_order_relaxed);
  }
  return id;
}

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_DETAIL_TASK_CONTROL_BLOCK_HPP
