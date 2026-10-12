// ---------------------------------------------------------------------------
// cppcoroutine/detail/wait_queue.hpp
//
// Generic FIFO used by Event and Channel to park waiters.
//
// Slots are never removed eagerly when a waiter is cancelled: the owning
// object validates a slot with `Scheduler::is_active_wait(task, generation)`
// before honouring it.  `compact()` drops dead slots in bulk once they start
// to dominate.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_DETAIL_WAIT_QUEUE_HPP
#define CPPCOROUTINE_DETAIL_WAIT_QUEUE_HPP

#include <cstddef>
#include <cstdint>
#include <deque>
#include <utility>

#include "cppcoroutine/config.hpp"

namespace cppcoroutine {
namespace detail {

/// Minimal slot: "wake this (task, generation) pair".
struct WaitSlot {
  TaskId task = invalid_task_id;
  std::uint64_t generation = 0;
};

template <class Slot>
class WaitQueueT {
 public:
  void push(Slot slot) { slots_.push_back(std::move(slot)); }

  [[nodiscard]] bool empty() const noexcept { return slots_.empty(); }
  [[nodiscard]] std::size_t size() const noexcept { return slots_.size(); }

  void clear() noexcept { slots_.clear(); }

  Slot pop_front() noexcept {
    Slot slot = std::move(slots_.front());
    slots_.pop_front();
    return slot;
  }

  [[nodiscard]] Slot& front() noexcept { return slots_.front(); }
  [[nodiscard]] const Slot& front() const noexcept { return slots_.front(); }

  typename std::deque<Slot>::iterator begin() noexcept { return slots_.begin(); }
  typename std::deque<Slot>::iterator end() noexcept { return slots_.end(); }
  typename std::deque<Slot>::const_iterator begin() const noexcept { return slots_.begin(); }
  typename std::deque<Slot>::const_iterator end() const noexcept { return slots_.end(); }

  /// Removes every slot for which `predicate(slot)` is true.
  template <class Predicate>
  void remove_if(Predicate&& predicate) {
    for (auto it = slots_.begin(); it != slots_.end();) {
      if (predicate(*it)) {
        it = slots_.erase(it);
      } else {
        ++it;
      }
    }
  }

 private:
  std::deque<Slot> slots_{};
};

using WaitQueue = WaitQueueT<WaitSlot>;

}  // namespace detail
}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_DETAIL_WAIT_QUEUE_HPP
