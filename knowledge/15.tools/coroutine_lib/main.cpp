// ---------------------------------------------------------------------------
// demo/02_multiple_tasks.cpp
//
// Many tasks sharing one event loop.
//
// Shows:
//   * the ready queue dispatches in FIFO spawn order;
//   * a task that suspends does not block the others;
//   * `post()` runs a callback on the loop thread;
//   * finishing all work lets run() return without any explicit shutdown.
// ---------------------------------------------------------------------------
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "cppcoroutine/coroutine.hpp"
#include "demo_common.hpp"

using namespace std::chrono_literals;
using cppcoroutine::Scheduler;
using cppcoroutine::Task;
using cppcoroutine::TaskState;

namespace {

constexpr int kTaskCount = 6;

/// Records its start order, waits, then records its finish order.
Task<void> worker(Scheduler& scheduler, int index, std::vector<int>& start_order,
                  std::vector<int>& finish_order) {
  start_order.push_back(index);
  co_await scheduler.sleep_for(1ms);
  finish_order.push_back(index);
  co_return;
}

/// A task that never suspends: it runs to completion in a single resume.
Task<void> instant(std::vector<int>& order, int index) {
  order.push_back(index);
  co_return;
}

Task<void> loop_work(Scheduler& scheduler, std::chrono::_V2::steady_clock::duration duration = 1s)
{
    while (true) {
        co_await scheduler.sleep_for(duration);
        std::printf("loop_work: sleeping for %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
    }
}
}  // namespace

int main() {
  std::printf("=== Demo 02: multiple tasks on one loop ===\n");
  demo::Report report;

  Scheduler scheduler;
  std::vector<int> start_order;
  std::vector<int> finish_order;

  for (int i = 0; i < kTaskCount; ++i) {
    scheduler.spawn(worker(scheduler, i, start_order, finish_order));
  }

  // Queued from the outside: runs on the loop thread, on the next iteration.
  std::vector<int> posted;
  scheduler.post([&posted] { posted.push_back(1); });
  scheduler.post([&posted] { posted.push_back(2); });

  const auto start = demo::Clock::now();
  scheduler.run();
  const long long ms = demo::ms_since(start);

  std::printf("start order : ");
  for (const int index : start_order) {
    std::printf("%d ", index);
  }
  std::printf("\nfinish order: ");
  for (const int index : finish_order) {
    std::printf("%d ", index);
  }
  std::printf("\nelapsed     : %lld ms\n", ms);

  report.eq(start_order.size(), static_cast<std::size_t>(kTaskCount), "every task started");
  report.eq(finish_order.size(), static_cast<std::size_t>(kTaskCount), "every task finished");

  bool fifo = start_order.size() == static_cast<std::size_t>(kTaskCount);
  for (std::size_t i = 0; i < start_order.size(); ++i) {
    fifo = fifo && start_order[i] == static_cast<int>(i);
  }
  report.check(fifo, "the ready queue dispatched in FIFO spawn order");

  // Every task slept for the same 1 ms, so they woke as one batch in the order
  // they were parked - which is again the spawn order.
  bool same_order = finish_order.size() == start_order.size();
  for (std::size_t i = 0; i < finish_order.size() && same_order; ++i) {
    same_order = finish_order[i] == start_order[i];
  }
  report.check(same_order, "the batched timer wake-up preserved the parking order");

  report.eq(posted.size(), static_cast<std::size_t>(2), "both posted callbacks ran");
  report.check(posted.empty() == false && posted[0] == 1 && posted[1] == 2,
               "posted callbacks ran in submission order");

  // Tasks that never suspend still go through the ready queue one at a time.
  std::vector<int> instant_order;
  for (int i = 0; i < 4; ++i) {
    scheduler.spawn(instant(instant_order, i));
  }
  scheduler.run();

  bool instant_fifo = instant_order.size() == 4;
  for (std::size_t i = 0; i < instant_order.size() && instant_fifo; ++i) {
    instant_fifo = instant_order[i] == static_cast<int>(i);
  }
  report.check(instant_fifo, "non-suspending tasks also ran in spawn order");

  const auto stats = scheduler.stats();
  std::printf("loop iterations used: %llu tasks created, %llu completed\n",
              static_cast<unsigned long long>(stats.tasks_created),
              static_cast<unsigned long long>(stats.tasks_completed));
  report.eq(stats.tasks_completed, static_cast<std::uint64_t>(kTaskCount) + 4,
            "all ten tasks completed");
  report.eq(scheduler.active_task_count(), static_cast<std::size_t>(0), "nothing is left active");
  report.eq(scheduler.registry_size(), static_cast<std::size_t>(kTaskCount) + 4,
            "ten registry entries are kept for post-mortem inspection");
  report.check(scheduler.clear_finished() == static_cast<std::size_t>(kTaskCount) + 4,
               "clear_finished() drops exactly the finished entries");

  // run() returned on its own: no request_stop() was needed.
  report.check(!scheduler.stop_requested(), "run() returned when the work was done");
  report.check(stats.frames_created == stats.frames_destroyed, "no frame leaked");

  demo::finish("02_multiple_tasks", report);

  scheduler.spawn(loop_work(scheduler));  // runs forever, but doesn't block the others
  scheduler.spawn(loop_work(scheduler, 2s));  // runs forever, but doesn't block the others

  scheduler.run(); 

  return 0;
}
