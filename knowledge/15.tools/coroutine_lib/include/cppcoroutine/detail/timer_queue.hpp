// ---------------------------------------------------------------------------
// cppcoroutine/detail/timer_queue.hpp
//
// Min-heap ordered by (deadline, insertion sequence).
//
// Two kinds of entries share the queue:
//   * task timers    - `task`/`generation` identify a parked coroutine; the
//                      scheduler validates the pair before waking it, which is
//                      how stale entries are handled without eager removal.
//   * callback timers- `control` carries a user callback; `TimerHandle`
//                      cancels by flipping `control->cancelled`.
//
// Header-only: this file used to live under src/internal/ and is now part of
// the installed include tree.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_DETAIL_TIMER_QUEUE_HPP
#define CPPCOROUTINE_DETAIL_TIMER_QUEUE_HPP

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "cppcoroutine/config.hpp"

namespace cppcoroutine {
class Scheduler;

namespace detail {

using steady_clock = std::chrono::steady_clock;

/// Shared state of a callback timer.  Held by the TimerHandle and by the
/// queued entry, so the callback stays alive until it fires or is cancelled.
struct TimerControl {
  Scheduler* scheduler = nullptr;
  std::uint64_t id = 0;
  std::atomic<bool> cancelled{false};
  bool periodic = false;
  steady_clock::duration period{};
  std::function<void()> callback{};
  std::atomic<std::uint64_t> fire_count{0};
};

struct TimerEntry {
  steady_clock::time_point deadline{};
  std::uint64_t sequence = 0;
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;
  std::shared_ptr<TimerControl> control{};
};

/// Comparator that turns `std::push_heap`/`std::pop_heap` into a min-heap.
struct TimerEntryLater {
  bool operator()(const TimerEntry& lhs, const TimerEntry& rhs) const noexcept {
    if (lhs.deadline != rhs.deadline) {
      return lhs.deadline > rhs.deadline;
    }
    return lhs.sequence > rhs.sequence;
  }
};

class TimerQueue {
 public:
  void push(TimerEntry entry) {
    entry.sequence = next_sequence_++;
    heap_.push_back(std::move(entry));
    std::push_heap(heap_.begin(), heap_.end(), TimerEntryLater{});
  }

  [[nodiscard]] const TimerEntry& top() const noexcept { return heap_.front(); }
  [[nodiscard]] bool empty() const noexcept { return heap_.empty(); }
  [[nodiscard]] std::size_t size() const noexcept { return heap_.size(); }

  TimerEntry pop() {
    std::pop_heap(heap_.begin(), heap_.end(), TimerEntryLater{});
    TimerEntry entry = std::move(heap_.back());
    heap_.pop_back();
    return entry;
  }

  void clear() noexcept { heap_.clear(); }

  /// Counts entries matching a predicate.  Used to distinguish "entries in the
  /// heap" from "entries that still refer to a live wait" (lazy deletion keeps
  /// stale entries around until they surface).
  template <class Predicate>
  [[nodiscard]] std::size_t count_if(Predicate&& predicate) const {
    std::size_t count = 0;
    for (const TimerEntry& entry : heap_) {
      if (predicate(entry)) {
        count += 1;
      }
    }
    return count;
  }

  /// Drops every entry the predicate marks as dead and re-heapifies.
  /// Lazy deletion keeps wake-ups correct without eager removal; compaction
  /// keeps a long-running scheduler from accumulating garbage.
  ///
  /// The scan must not disturb live entries: an earlier revision collected the
  /// survivors into a scratch vector with `std::move` and then took an early
  /// return when nothing had died, leaving `heap_` full of moved-from entries
  /// (a moved-from `shared_ptr` is null, which made `expire_timers` mistake a
  /// live callback timer for a stale task entry and silently drop it).
  /// Compacting in place with `std::remove_if`-style writes keeps the "nothing
  /// dead" path completely side-effect free.
  template <class Predicate>
  void compact(Predicate&& is_dead) {
    const std::size_t before = heap_.size();
    std::size_t write = 0;
    for (std::size_t read = 0; read < before; ++read) {
      if (is_dead(heap_[read])) {
        continue;
      }
      if (write != read) {
        heap_[write] = std::move(heap_[read]);
      }
      ++write;
    }
    if (write == before) {
      return;  // nothing was dead, and no element was touched
    }
    heap_.resize(write);
    std::make_heap(heap_.begin(), heap_.end(), TimerEntryLater{});
  }

 private:
  std::vector<TimerEntry> heap_{};
  std::uint64_t next_sequence_ = 0;
};

}  // namespace detail
}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_DETAIL_TIMER_QUEUE_HPP
