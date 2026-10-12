// ---------------------------------------------------------------------------
// cppcoroutine/task_state.hpp
//
// The task state machine.  Every state transition is performed by the
// Scheduler (see docs/lifecycle.md for the full transition table).
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_TASK_STATE_HPP
#define CPPCOROUTINE_TASK_STATE_HPP

#include <string_view>

namespace cppcoroutine {

enum class TaskState {
  /// The coroutine frame exists but has never been handed to a scheduler.
  Created,

  /// Queued in the ready queue, waiting for its turn on the event loop.
  Ready,

  /// Currently executing on the event-loop thread.
  Running,

  /// Suspended inside a timer wait (`sleep_for` / `sleep_until`).
  WaitingTimer,

  /// Suspended waiting for an Event, or waiting for another Task to finish
  /// (task join).  Both are "an external party must wake me" waits.
  WaitingEvent,

  /// Suspended waiting for channel data / channel buffer space.
  WaitingChannel,

  /// A cancellation request arrived while the coroutine was running.
  /// Cancellation is cooperative: the task keeps running until it reaches
  /// a cancellation checkpoint (any library await point).
  Cancelling,

  /// Terminal: the coroutine body returned normally.
  Completed,

  /// Terminal: the coroutine exited through CancellationError.
  Cancelled,

  /// Terminal: the coroutine exited through an unhandled exception.
  Failed,
};

/// Terminal states never move back to a non-terminal state.
constexpr bool is_terminal(TaskState s) noexcept {
  return s == TaskState::Completed || s == TaskState::Cancelled || s == TaskState::Failed;
}

/// True for the states in which the task sits in a wait queue owned by
/// another object (timer queue / event / channel).
constexpr bool is_waiting(TaskState s) noexcept {
  return s == TaskState::WaitingTimer || s == TaskState::WaitingEvent ||
         s == TaskState::WaitingChannel;
}

/// Human readable name, used by diagnostics and tests.
constexpr std::string_view to_string(TaskState s) noexcept {
  switch (s) {
    case TaskState::Created:        return "Created";
    case TaskState::Ready:          return "Ready";
    case TaskState::Running:        return "Running";
    case TaskState::WaitingTimer:   return "WaitingTimer";
    case TaskState::WaitingEvent:   return "WaitingEvent";
    case TaskState::WaitingChannel: return "WaitingChannel";
    case TaskState::Cancelling:     return "Cancelling";
    case TaskState::Completed:      return "Completed";
    case TaskState::Cancelled:      return "Cancelled";
    case TaskState::Failed:         return "Failed";
  }
  return "Unknown";
}

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_TASK_STATE_HPP
