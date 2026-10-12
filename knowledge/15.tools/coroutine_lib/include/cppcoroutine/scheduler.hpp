// ---------------------------------------------------------------------------
// cppcoroutine/scheduler.hpp
//
// The single-threaded, cooperative event loop.
//
// Threading contract
// ------------------
//   * All coroutine execution, all state transitions and all frame
//     destruction happen on the thread that called run() ("the loop thread").
//   * Other threads may only call the explicitly thread-safe entry points:
//     post(), request_stop(), cancel(), TimerHandle::cancel().
//     Those enqueue a request; they never resume a coroutine themselves.
//
// Header-only
// -----------
// This library ships as headers only: there is no .cpp to compile and no
// static library to link.  Everything below is `inline`, so any number of
// translation units may include this header without violating the ODR.
//
// The event loop, the task registry and the single point where every state
// transition happens live in this file.
//
// Invariants enforced here (see docs/lifecycle.md):
//   I1. A task id sits in the ready queue at most once.
//   I2. A wake source may only move a task out of a wait if BOTH the task id
//       and the wait generation still match - so cancellation and a timer
//       firing at the same time can never resume the same coroutine twice.
//   I3. A coroutine frame is destroyed only when the task is terminal, no
//       consumer still needs its result, and it is not sitting in any queue.
//   I4. Every frame is destroyed exactly once, either by the reclaimer or by
//       the Scheduler destructor.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_SCHEDULER_HPP
#define CPPCOROUTINE_SCHEDULER_HPP

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cppcoroutine/config.hpp"
#include "cppcoroutine/detail/ready_queue.hpp"
#include "cppcoroutine/detail/task_control_block.hpp"
#include "cppcoroutine/detail/timer_queue.hpp"
#include "cppcoroutine/result.hpp"
#include "cppcoroutine/task_state.hpp"

namespace cppcoroutine {

class TimerHandle;

template <class T>
class Task;

struct TaskControlBlock;

namespace detail {
struct SleepAwaiter;
struct CancellationPointAwaiter;
struct TimerControl;
}  // namespace detail

class Scheduler {
 public:
  using clock = std::chrono::steady_clock;
  using Callback = std::function<void()>;

  /// Aggregate diagnostic counters (see README 12.7).
  ///
  /// Scope: the task counters (`tasks_*`, `active_tasks`, `registry_entries`,
  /// `pending_timers`, `waiting_tasks`, `ready_tasks`) describe *this*
  /// scheduler.  The frame counters (`frames_created`, `frames_destroyed`) are
  /// process-wide totals across every Scheduler instance ever constructed - they
  /// exist so that a leaked coroutine frame is detectable (`frames_created !=
  /// frames_destroyed` means a frame was never destroyed), which no per-instance
  /// counter could show.
  struct Stats {
    std::uint64_t tasks_created = 0;
    std::uint64_t tasks_completed = 0;
    std::uint64_t tasks_failed = 0;
    std::uint64_t tasks_cancelled = 0;
    std::uint64_t frames_created = 0;
    std::uint64_t frames_destroyed = 0;
    std::size_t active_tasks = 0;
    std::size_t registry_entries = 0;
    std::size_t pending_timers = 0;
    std::size_t waiting_tasks = 0;
    std::size_t ready_tasks = 0;
  };

  Scheduler();
  ~Scheduler();

  Scheduler(const Scheduler&) = delete;
  Scheduler& operator=(const Scheduler&) = delete;
  Scheduler(Scheduler&&) = delete;
  Scheduler& operator=(Scheduler&&) = delete;

  // -------------------------------------------------------------------------
  // Task submission
  // -------------------------------------------------------------------------

  /// Registers a task and makes it ready to run.  Ownership of the coroutine
  /// frame moves to the scheduler; `task` is left empty.  Returns the new
  /// task id, or `invalid_task_id` if `task` was already empty.
  template <class T>
  TaskId spawn(Task<T>&& task);

  // -------------------------------------------------------------------------
  // Event loop
  // -------------------------------------------------------------------------

  /// Runs until every task has reached a terminal state, or until
  /// request_stop() has been observed and all tasks have been cleaned up.
  /// Throws `std::logic_error` if called while already running.
  void run();

  /// Thread-safe.  Asks the loop to stop; all remaining tasks are cancelled
  /// and drained before run() returns.
  void request_stop();

  [[nodiscard]] bool is_running() const noexcept;
  [[nodiscard]] bool stop_requested() const noexcept;

  /// Thread-safe.  Runs `fn` on the loop thread (at the next loop iteration,
  /// or at the start of the next run() if the loop is not running).
  void post(Callback fn);

  // -------------------------------------------------------------------------
  // Timers
  // -------------------------------------------------------------------------

  /// `co_await scheduler.sleep_for(100ms);`
  [[nodiscard]] detail::SleepAwaiter sleep_for(clock::duration duration);

  /// `co_await scheduler.sleep_until(deadline);`
  [[nodiscard]] detail::SleepAwaiter sleep_until(clock::time_point deadline);

  /// Callback timers.  The returned handle owns the timer: dropping it
  /// cancels the timer (RAII).  Keep the handle alive for as long as the
  /// timer should be able to fire.
  TimerHandle set_timeout(clock::duration delay, Callback callback);
  TimerHandle set_timeout_at(clock::time_point deadline, Callback callback);
  TimerHandle set_interval(clock::duration period, Callback callback);

  // -------------------------------------------------------------------------
  // Cancellation
  // -------------------------------------------------------------------------

  /// Requests cancellation of `task`.  Idempotent.  Returns true if the
  /// request was accepted (the task existed and was not already terminal).
  ///
  /// Thread-safe: when called from a foreign thread the request is queued and
  /// applied by the loop thread.
  bool cancel(TaskId task);

  /// Requests cancellation of every non-terminal task.
  void cancel_all();

  [[nodiscard]] bool is_cancel_requested(TaskId task) const noexcept;

  /// `co_await scheduler.cancellation_point();`
  /// Yields nothing; it throws CancellationError when the calling task has
  /// been cancelled.  Useful inside long running, await-free loops.
  [[nodiscard]] detail::CancellationPointAwaiter cancellation_point() noexcept;

  // -------------------------------------------------------------------------
  // Queries
  // -------------------------------------------------------------------------

  /// Valid also after the task's frame has been reclaimed.
  [[nodiscard]] TaskState task_state(TaskId task) const noexcept;

  /// Number of registry entries (finished entries included until
  /// clear_finished() is called).
  [[nodiscard]] std::size_t registry_size() const noexcept;

  /// Number of registered tasks that are not in a terminal state.
  [[nodiscard]] std::size_t active_task_count() const noexcept;

  [[nodiscard]] std::size_t pending_timer_count() const noexcept;

  /// Number of queued timers that still refer to a live wait (stale entries
  /// scheduled for lazy deletion are not counted).
  [[nodiscard]] std::size_t live_timer_count() const;

  [[nodiscard]] std::size_t waiting_task_count() const;

  /// Drops registry entries for tasks that reached a terminal state and whose
  /// frame has already been reclaimed.
  std::size_t clear_finished();

  [[nodiscard]] Stats stats() const;

  /// The scheduler whose run() is executing on the calling thread, or null.
  [[nodiscard]] static Scheduler* current() noexcept;

  /// Id of the task currently executing on the calling thread, or
  /// `invalid_task_id`.
  [[nodiscard]] static TaskId current_task_id() noexcept;

  // =========================================================================
  // Library-internal services.
  //
  // These are used by the awaiters in the public headers.  They are not part
  // of the stable API and may change between minor versions.
  // =========================================================================

  /// Marks the task as waiting and bumps its wait generation.
  std::uint64_t begin_wait(TaskId task, TaskState wait_state) noexcept;

  /// Undoes begin_wait() when an awaiter decides not to suspend.
  void abort_wait(TaskId task, std::uint64_t generation) noexcept;

  /// Wakes a waiting task if (task, generation) still matches.  Returns true
  /// when the wake-up actually happened - i.e. when this call won the race
  /// against any other wake source / cancellation.
  bool wake(TaskId task, std::uint64_t generation) noexcept;

  /// Wakes a waiting task and makes its await point throw `error`.
  bool fail_wait(TaskId task, std::uint64_t generation, std::exception_ptr error) noexcept;

  /// Called at the end of every library await point.  Throws if a cancellation
  /// was requested or the wait was failed.
  void rethrow_wait_error(TaskId task);

  /// Called by an awaiter that decided not to suspend: throws if the task was
  /// cancelled in the meantime.
  void check_cancelled(TaskId task) const;

  [[nodiscard]] TaskControlBlock* control_block(TaskId task) noexcept;
  [[nodiscard]] const TaskControlBlock* control_block(TaskId task) const noexcept;

  /// True while `task` is parked in a wait with generation `generation`.
  [[nodiscard]] bool is_active_wait(TaskId task, std::uint64_t generation) const noexcept;

  /// Drops the `awaiter_refs` reference a parent held on a child task.
  void release_awaiter(TaskId child) noexcept;

  /// Enqueues a timer wake-up for `task`.
  void add_timer(clock::time_point deadline, TaskId task, std::uint64_t generation);

  /// Enqueues a callback timer.
  void add_callback_timer(const std::shared_ptr<detail::TimerControl>& control,
                          clock::time_point deadline);

  /// Cancels a callback timer (idempotent).  Returns true if it was pending.
  bool cancel_callback_timer(const std::shared_ptr<detail::TimerControl>& control) noexcept;

  /// Called by the final awaiter of every task coroutine.
  void finish_task(TaskControlBlock& control) noexcept;

  /// Registers an already-created frame and queues it as ready.
  TaskId register_task(TaskControlBlock& control, std::coroutine_handle<> handle);

  /// Moves a task id into the ready queue (used by `post`-style wake-ups).
  void push_ready(TaskId task) noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

// ---------------------------------------------------------------------------
// Loop-thread bookkeeping.
//
// These used to be file-scope `thread_local` objects inside scheduler.cpp.
// Now that they are reachable from every translation unit that includes this
// header they must be `inline` variables: an anonymous namespace would give
// every translation unit its own copy, which would break `Scheduler::current()`
// and `current_task_id()` (an awaiter compiled in one TU would no longer see
// the scheduler entered from another).
// ---------------------------------------------------------------------------
namespace detail {

inline thread_local Scheduler* tls_current_scheduler = nullptr;
inline thread_local TaskId tls_current_task = invalid_task_id;

/// Registry entry.  Keeps the TCB alive (by identity, not ownership) and
/// caches the last observed state so `task_state()` stays meaningful after
/// the frame has been reclaimed.
struct TaskRecord {
  TaskControlBlock* control = nullptr;
  std::coroutine_handle<> handle{};
  bool alive = false;
  TaskState last_state = TaskState::Created;
};

}  // namespace detail

// ---------------------------------------------------------------------------
// Scheduler::Impl
//
// Previously defined in src/scheduler.cpp.  It has to be complete in the header
// now because `std::unique_ptr<Impl>` is destroyed by the inline destructor.
// ---------------------------------------------------------------------------

struct Scheduler::Impl {
  Scheduler* owner = nullptr;

  detail::ReadyQueue ready{};
  detail::TimerQueue timers{};
  std::unordered_map<TaskId, detail::TaskRecord> registry{};
  std::deque<TaskId> reclaim_queue{};

  std::size_t active_tasks = 0;
  std::uint64_t created_count = 0;
  std::uint64_t completed_count = 0;
  std::uint64_t failed_count = 0;
  std::uint64_t cancelled_count = 0;

  bool running = false;
  std::thread::id owner_thread{};

  std::mutex external_mutex{};
  std::vector<Scheduler::Callback> external_queue{};
  std::condition_variable external_cv{};
  std::atomic<bool> stop_requested{false};

  // -- lookup ---------------------------------------------------------------

  [[nodiscard]] detail::TaskRecord* find(TaskId id) noexcept {
    const auto it = registry.find(id);
    return it == registry.end() ? nullptr : &it->second;
  }

  [[nodiscard]] const detail::TaskRecord* find(TaskId id) const noexcept {
    const auto it = registry.find(id);
    return it == registry.end() ? nullptr : &it->second;
  }

  [[nodiscard]] TaskControlBlock* control_of(TaskId id) noexcept {
    detail::TaskRecord* record = find(id);
    if (record == nullptr || !record->alive) {
      return nullptr;
    }
    return record->control;
  }

  [[nodiscard]] const TaskControlBlock* control_of(TaskId id) const noexcept {
    const detail::TaskRecord* record = find(id);
    if (record == nullptr || !record->alive) {
      return nullptr;
    }
    return record->control;
  }

  // -- queues ---------------------------------------------------------------

  void schedule_reclaim(TaskId id) noexcept {
    TaskControlBlock* control = control_of(id);
    if (control == nullptr || control->reclaim_scheduled) {
      return;
    }
    control->reclaim_scheduled = true;
    reclaim_queue.push_back(id);
  }

  void release_awaiter(TaskId child) noexcept {
    TaskControlBlock* control = control_of(child);
    if (control == nullptr) {
      return;
    }
    if (control->awaiter_refs > 0) {
      control->awaiter_refs -= 1;
    }
    if (control->awaiter_refs == 0 && control->reclaim_when_unref &&
        is_terminal(control->state)) {
      schedule_reclaim(child);
    }
  }

  bool wake(TaskId id, std::uint64_t generation) noexcept {
    TaskControlBlock* control = control_of(id);
    if (control == nullptr) {
      return false;
    }
    if (!is_waiting(control->state) || control->wait_gen != generation) {
      return false;  // stale registration, or somebody else won the race
    }
    control->state = TaskState::Ready;
    ready.push(id);
    return true;
  }

  /// Destroys every frame whose task is terminal and unreferenced.
  void reclaim_pending() {
    while (!reclaim_queue.empty()) {
      const TaskId id = reclaim_queue.front();
      reclaim_queue.pop_front();

      detail::TaskRecord* record = find(id);
      if (record == nullptr || !record->alive) {
        continue;
      }
      TaskControlBlock* control = record->control;
      if (!is_terminal(control->state)) {
        control->reclaim_scheduled = false;
        continue;
      }
      if (control->awaiter_refs != 0) {
        control->reclaim_when_unref = true;
        control->reclaim_scheduled = false;
        continue;
      }

      // Safety net: a frame destroyed without stack unwinding still has to
      // release the reference it held on a child task.
      if (control->awaiting_child != invalid_task_id) {
        const TaskId child = control->awaiting_child;
        control->awaiting_child = invalid_task_id;
        release_awaiter(child);
      }

      const TaskState final_state = control->state;
      control->reclaim_scheduled = false;

      record->last_state = final_state;
      record->alive = false;
      record->control = nullptr;
      std::coroutine_handle<> handle = record->handle;
      record->handle = {};

      if (handle) {
        handle.destroy();
      }
    }
  }

  // -- timers ---------------------------------------------------------------

  void expire_timers() {
    const auto now = detail::steady_clock::now();
    while (!timers.empty() && timers.top().deadline <= now) {
      detail::TimerEntry entry = timers.pop();

      if (entry.control != nullptr) {
        const std::shared_ptr<detail::TimerControl>& control = entry.control;
        if (control->cancelled.load(std::memory_order_acquire)) {
          continue;
        }
        if (control->periodic && control->period > detail::steady_clock::duration::zero()) {
          detail::TimerEntry next;
          next.control = control;
          next.deadline = entry.deadline + control->period;
          if (next.deadline <= now) {
            next.deadline = now + control->period;
          }
          timers.push(std::move(next));
        }
        control->fire_count.fetch_add(1, std::memory_order_relaxed);
        if (control->callback) {
          try {
            control->callback();
          } catch (...) {
            // A throwing callback must never take the event loop down.
          }
        }
        continue;
      }

      if (entry.task != invalid_task_id) {
        (void)wake(entry.task, entry.generation);
      }
    }
  }

  // -- cancellation ---------------------------------------------------------

  void finish_before_start(TaskControlBlock& control) {
    control.state = TaskState::Cancelled;
    cancelled_count += 1;
    notify_continuation(control);
    if (active_tasks > 0) {
      active_tasks -= 1;
    }
    control.reclaim_when_unref = true;
    schedule_reclaim(control.id);
  }

  void notify_continuation(TaskControlBlock& control) {
    if (control.continuation == nullptr) {
      return;
    }
    const TaskId parent_id = control.continuation_id;
    control.continuation = {};
    control.continuation_id = invalid_task_id;

    TaskControlBlock* parent = control_of(parent_id);
    if (parent != nullptr && parent->state == TaskState::WaitingEvent) {
      parent->state = TaskState::Ready;
      ready.push(parent_id);
    }
  }

  bool cancel_impl(TaskId id) {
    TaskControlBlock* control = control_of(id);
    if (control == nullptr || is_terminal(control->state)) {
      return false;
    }
    if (control->cancel_requested) {
      return true;  // idempotent
    }
    control->cancel_requested = true;

    switch (control->state) {
      case TaskState::Created:
        finish_before_start(*control);
        break;
      case TaskState::Ready:
        if (control->started) {
          // Already woken by a wait: let it resume and unwind normally, where
          // the await point turns this into a thrown CancellationError.
          control->wait_cancelled = true;
        } else {
          finish_before_start(*control);
        }
        break;
      case TaskState::Running:
      case TaskState::Cancelling:
        // Cooperative: the running coroutine notices at its next await point.
        control->state = TaskState::Cancelling;
        break;
      case TaskState::WaitingTimer:
      case TaskState::WaitingEvent:
      case TaskState::WaitingChannel:
        control->wait_cancelled = true;
        (void)wake(id, control->wait_gen);
        break;
      default:
        break;
    }
    return true;
  }

  void cancel_all() {
    std::vector<TaskId> pending;
    pending.reserve(registry.size());
    for (const auto& entry : registry) {
      if (entry.second.alive && !is_terminal(entry.second.control->state)) {
        pending.push_back(entry.first);
      }
    }
    for (const TaskId id : pending) {
      (void)cancel_impl(id);
    }
  }

  // -- external wake-ups ----------------------------------------------------

  void drain_external() {
    std::vector<Scheduler::Callback> local;
    {
      std::lock_guard<std::mutex> lock(external_mutex);
      local.swap(external_queue);
    }
    for (auto& fn : local) {
      fn();
    }
  }

  void wait_for_work() {
    std::unique_lock<std::mutex> lock(external_mutex);
    if (stop_requested.load(std::memory_order_acquire) || !external_queue.empty()) {
      return;
    }
    if (timers.empty()) {
      external_cv.wait(lock, [this] {
        return stop_requested.load(std::memory_order_acquire) || !external_queue.empty();
      });
      return;
    }
    const auto deadline = timers.top().deadline;
    (void)external_cv.wait_until(lock, deadline, [this, deadline] {
      if (stop_requested.load(std::memory_order_acquire) || !external_queue.empty()) {
        return true;
      }
      return !timers.empty() && timers.top().deadline < deadline;
    });
  }

  void notify_external() { external_cv.notify_all(); }

  /// Number of queued, still-cancellable *one-shot* callback timers.
  ///
  /// One-shot callback timers count as pending work: a scheduler with no
  /// tasks but a pending one-shot timer keeps running until the timer fires.
  /// Periodic timers deliberately do NOT keep the loop alive - they are
  /// background sources that have to be stopped explicitly (drop the
  /// TimerHandle or call request_stop()).
  [[nodiscard]] std::size_t pending_one_shot_callbacks() const {
    return timers.count_if([](const detail::TimerEntry& entry) {
      return entry.control != nullptr && !entry.control->periodic &&
             !entry.control->cancelled.load(std::memory_order_acquire);
    });
  }

  /// Drops timer entries that can never fire.  Lazy deletion keeps the heap
  /// correct without eager removal, but a long-running scheduler that
  /// constantly cancels timers should not accumulate garbage.  `force` runs the
  /// pass regardless of the heap size (used once the loop is about to exit).
  void prune_timers(bool force = false) {
    if (!force && timers.size() < 128) {
      return;
    }
    timers.compact([this](const detail::TimerEntry& entry) {
      if (entry.control != nullptr) {
        return entry.control->cancelled.load(std::memory_order_acquire);
      }
      const TaskControlBlock* control = control_of(entry.task);
      return control == nullptr || !is_waiting(control->state) ||
             control->wait_gen != entry.generation;
    });
  }
};

// ---------------------------------------------------------------------------
// Lifetime
// ---------------------------------------------------------------------------

inline Scheduler::Scheduler() : impl_(std::make_unique<Impl>()) { impl_->owner = this; }

inline Scheduler::~Scheduler() {
  if (!impl_) {
    return;
  }
  impl_->cancel_all();
  impl_->timers.clear();
  impl_->ready.clear();
  impl_->reclaim_queue.clear();

  for (auto& entry : impl_->registry) {
    detail::TaskRecord& record = entry.second;
    if (!record.alive) {
      continue;
    }
    record.alive = false;
    record.control = nullptr;
    std::coroutine_handle<> handle = record.handle;
    record.handle = {};
    if (handle) {
      handle.destroy();
    }
  }
  impl_->registry.clear();
}

// ---------------------------------------------------------------------------
// Event loop
// ---------------------------------------------------------------------------

inline void Scheduler::run() {
  Impl& self = *impl_;
  if (self.running) {
    throw std::logic_error("cppcoroutine: Scheduler::run() is already running");
  }
  self.running = true;
  self.owner_thread = std::this_thread::get_id();

  Scheduler* const previous = detail::tls_current_scheduler;
  detail::tls_current_scheduler = this;

  std::uint64_t iterations = 0;
  while (true) {
    self.drain_external();
    self.expire_timers();
    self.reclaim_pending();
    iterations += 1;
    if ((iterations % 64U) == 0U) {
      self.prune_timers();
    }

    if (self.stop_requested.load(std::memory_order_acquire)) {
      self.cancel_all();
    }

    if (!self.ready.empty()) {
      const TaskId id = self.ready.pop();
      TaskControlBlock* control = self.control_of(id);
      if (control == nullptr || is_terminal(control->state)) {
        continue;
      }
      if (!control->started && control->cancel_requested) {
        self.finish_before_start(*control);
        continue;
      }
      control->state = TaskState::Running;
      control->started = true;
      detail::tls_current_task = id;
      control->handle.resume();
      detail::tls_current_task = invalid_task_id;
      continue;
    }

    // Nothing runnable right now.
    if (self.active_tasks == 0 && self.pending_one_shot_callbacks() == 0) {
      // Reap entries that can never fire (cancelled callback timers, timers of
      // tasks that are no longer waiting).  Lazy deletion is what keeps wake-ups
      // correct; without this forced pass a scheduler that ran mostly callback
      // timers would leave garbage in the heap, and the post-run diagnostics
      // (`pending_timer_count()`) would report entries that can never fire.
      self.prune_timers(true);
      break;
    }
    self.wait_for_work();
  }

  self.reclaim_pending();
  detail::tls_current_scheduler = previous;
  detail::tls_current_task = invalid_task_id;
  self.running = false;
  self.owner_thread = std::thread::id{};
  self.stop_requested.store(false, std::memory_order_release);
}

inline void Scheduler::request_stop() {
  impl_->stop_requested.store(true, std::memory_order_release);
  impl_->notify_external();
}

inline bool Scheduler::is_running() const noexcept { return impl_->running; }

inline bool Scheduler::stop_requested() const noexcept {
  return impl_->stop_requested.load(std::memory_order_acquire);
}

inline void Scheduler::post(Callback fn) {
  if (!fn) {
    return;
  }
  {
    std::lock_guard<std::mutex> lock(impl_->external_mutex);
    impl_->external_queue.push_back(std::move(fn));
  }
  impl_->notify_external();
}

// ---------------------------------------------------------------------------
// Task registration / completion
// ---------------------------------------------------------------------------

inline TaskId Scheduler::register_task(TaskControlBlock& control,
                                       std::coroutine_handle<> handle) {
  Impl& self = *impl_;
  control.sched = this;
  control.handle = handle;
  control.state = TaskState::Ready;

  detail::TaskRecord record;
  record.control = &control;
  record.handle = handle;
  record.alive = true;
  record.last_state = TaskState::Ready;
  self.registry.emplace(control.id, record);

  self.created_count += 1;
  self.active_tasks += 1;
  self.ready.push(control.id);
  return control.id;
}

inline void Scheduler::finish_task(TaskControlBlock& control) noexcept {
  Impl& self = *impl_;

  switch (control.state) {
    case TaskState::Completed:
      self.completed_count += 1;
      break;
    case TaskState::Failed:
      self.failed_count += 1;
      break;
    case TaskState::Cancelled:
      self.cancelled_count += 1;
      break;
    default:
      break;
  }

  self.notify_continuation(control);
  if (self.active_tasks > 0) {
    self.active_tasks -= 1;
  }
  control.reclaim_when_unref = true;
  if (control.awaiter_refs == 0) {
    self.schedule_reclaim(control.id);
  }
}

inline void Scheduler::push_ready(TaskId task) noexcept { impl_->ready.push(task); }

inline void Scheduler::release_awaiter(TaskId child) noexcept { impl_->release_awaiter(child); }

// ---------------------------------------------------------------------------
// Wait services used by the awaiters
// ---------------------------------------------------------------------------

inline std::uint64_t Scheduler::begin_wait(TaskId task, TaskState wait_state) noexcept {
  TaskControlBlock* control = impl_->control_of(task);
  if (control == nullptr) {
    return 0;
  }
  control->wait_gen += 1;
  if (control->wait_gen == 0) {
    control->wait_gen = 1;
  }
  control->state = wait_state;
  return control->wait_gen;
}

inline void Scheduler::abort_wait(TaskId task, std::uint64_t generation) noexcept {
  TaskControlBlock* control = impl_->control_of(task);
  if (control == nullptr) {
    return;
  }
  if (control->wait_gen == generation && is_waiting(control->state)) {
    control->state = TaskState::Running;
  }
}

inline bool Scheduler::wake(TaskId task, std::uint64_t generation) noexcept {
  return impl_->wake(task, generation);
}

inline bool Scheduler::fail_wait(TaskId task, std::uint64_t generation,
                                 std::exception_ptr error) noexcept {
  TaskControlBlock* control = impl_->control_of(task);
  if (control == nullptr) {
    return false;
  }
  if (!is_waiting(control->state) || control->wait_gen != generation) {
    return false;
  }
  control->wait_error = std::move(error);
  control->state = TaskState::Ready;
  impl_->ready.push(task);
  return true;
}

inline void Scheduler::rethrow_wait_error(TaskId task) {
  TaskControlBlock* control = impl_->control_of(task);
  if (control == nullptr) {
    return;
  }
  if (control->wait_error) {
    std::exception_ptr error = control->wait_error;
    control->wait_error = nullptr;
    std::rethrow_exception(error);
  }
  if (control->cancel_requested || control->wait_cancelled) {
    throw CancellationError(task);
  }
}

inline void Scheduler::check_cancelled(TaskId task) const {
  const TaskControlBlock* control = impl_->control_of(task);
  if (control == nullptr) {
    return;
  }
  if (control->cancel_requested || control->wait_cancelled) {
    throw CancellationError(task);
  }
}

inline bool Scheduler::is_active_wait(TaskId task, std::uint64_t generation) const noexcept {
  const TaskControlBlock* control = impl_->control_of(task);
  if (control == nullptr) {
    return false;
  }
  return is_waiting(control->state) && control->wait_gen == generation;
}

inline TaskControlBlock* Scheduler::control_block(TaskId task) noexcept {
  return impl_->control_of(task);
}

inline const TaskControlBlock* Scheduler::control_block(TaskId task) const noexcept {
  return impl_->control_of(task);
}

// ---------------------------------------------------------------------------
// Timers
// ---------------------------------------------------------------------------

inline void Scheduler::add_timer(clock::time_point deadline, TaskId task,
                                 std::uint64_t generation) {
  detail::TimerEntry entry;
  entry.deadline = deadline;
  entry.task = task;
  entry.generation = generation;
  impl_->timers.push(std::move(entry));
}

inline void Scheduler::add_callback_timer(const std::shared_ptr<detail::TimerControl>& control,
                                          clock::time_point deadline) {
  detail::TimerEntry entry;
  entry.deadline = deadline;
  entry.control = control;
  impl_->timers.push(std::move(entry));
}

inline bool Scheduler::cancel_callback_timer(
    const std::shared_ptr<detail::TimerControl>& control) noexcept {
  if (!control) {
    return false;
  }
  const bool was_pending = !control->cancelled.exchange(true, std::memory_order_acq_rel);
  impl_->notify_external();
  return was_pending;
}

// ---------------------------------------------------------------------------
// Cancellation
// ---------------------------------------------------------------------------

inline bool Scheduler::cancel(TaskId task) {
  Impl& self = *impl_;
  if (!self.running || std::this_thread::get_id() == self.owner_thread) {
    return self.cancel_impl(task);
  }
  {
    std::lock_guard<std::mutex> lock(self.external_mutex);
    self.external_queue.push_back([&self, task]() { (void)self.cancel_impl(task); });
  }
  self.notify_external();
  return true;
}

inline void Scheduler::cancel_all() { impl_->cancel_all(); }

inline bool Scheduler::is_cancel_requested(TaskId task) const noexcept {
  const TaskControlBlock* control = impl_->control_of(task);
  return control != nullptr && control->cancel_requested;
}

// ---------------------------------------------------------------------------
// Queries
// ---------------------------------------------------------------------------

inline TaskState Scheduler::task_state(TaskId task) const noexcept {
  const detail::TaskRecord* record = impl_->find(task);
  if (record == nullptr) {
    return TaskState::Created;
  }
  if (record->alive) {
    return record->control->state;
  }
  return record->last_state;
}

inline std::size_t Scheduler::registry_size() const noexcept { return impl_->registry.size(); }

inline std::size_t Scheduler::active_task_count() const noexcept { return impl_->active_tasks; }

inline std::size_t Scheduler::pending_timer_count() const noexcept { return impl_->timers.size(); }

inline std::size_t Scheduler::live_timer_count() const {
  return impl_->timers.count_if([this](const detail::TimerEntry& entry) {
    if (entry.control != nullptr) {
      return !entry.control->cancelled.load(std::memory_order_acquire);
    }
    const TaskControlBlock* control = impl_->control_of(entry.task);
    return control != nullptr && is_waiting(control->state) &&
           control->wait_gen == entry.generation;
  });
}

inline std::size_t Scheduler::waiting_task_count() const {
  std::size_t count = 0;
  for (const auto& entry : impl_->registry) {
    if (entry.second.alive && is_waiting(entry.second.control->state)) {
      count += 1;
    }
  }
  return count;
}

inline std::size_t Scheduler::clear_finished() {
  std::size_t removed = 0;
  for (auto it = impl_->registry.begin(); it != impl_->registry.end();) {
    if (!it->second.alive) {
      it = impl_->registry.erase(it);
      removed += 1;
    } else {
      ++it;
    }
  }
  return removed;
}

inline Scheduler::Stats Scheduler::stats() const {
  Stats result;
  result.tasks_created = impl_->created_count;
  result.tasks_completed = impl_->completed_count;
  result.tasks_failed = impl_->failed_count;
  result.tasks_cancelled = impl_->cancelled_count;
  result.frames_created = global_counters().frames_created.load(std::memory_order_relaxed);
  result.frames_destroyed = global_counters().frames_destroyed.load(std::memory_order_relaxed);
  result.active_tasks = impl_->active_tasks;
  result.registry_entries = impl_->registry.size();
  result.pending_timers = impl_->timers.size();
  result.waiting_tasks = waiting_task_count();
  result.ready_tasks = impl_->ready.size();
  return result;
}

inline Scheduler* Scheduler::current() noexcept { return detail::tls_current_scheduler; }

inline TaskId Scheduler::current_task_id() noexcept { return detail::tls_current_task; }

}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_SCHEDULER_HPP
