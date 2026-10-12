// ---------------------------------------------------------------------------
// cppcoroutine/detail/ready_queue.hpp
//
// FIFO queue of task ids that are eligible to run.  Deliberately trivial -
// the interesting policy lives in the Scheduler.
//
// Header-only: the whole library is header-only, so this file used to live
// under src/internal/ and is now part of the installed include tree.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_DETAIL_READY_QUEUE_HPP
#define CPPCOROUTINE_DETAIL_READY_QUEUE_HPP

#include <cstddef>
#include <deque>

#include "cppcoroutine/config.hpp"

namespace cppcoroutine {
namespace detail {

class ReadyQueue {
 public:
  void push(TaskId id) { queue_.push_back(id); }

  TaskId pop() noexcept {
    const TaskId id = queue_.front();
    queue_.pop_front();
    return id;
  }

  [[nodiscard]] bool empty() const noexcept { return queue_.empty(); }
  [[nodiscard]] std::size_t size() const noexcept { return queue_.size(); }
  void clear() noexcept { queue_.clear(); }

 private:
  std::deque<TaskId> queue_{};
};

}  // namespace detail
}  // namespace cppcoroutine

#endif  // CPPCOROUTINE_DETAIL_READY_QUEUE_HPP
