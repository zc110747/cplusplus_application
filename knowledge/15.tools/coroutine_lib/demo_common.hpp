// ---------------------------------------------------------------------------
// demo/demo_common.hpp
//
// Tiny shared helpers for the demo programs.  Demos are meant to be readable
// documentation, so they print what they observe and return a non-zero exit
// code when an expectation does not hold - which is also what makes them usable
// as CTest smoke tests.
//
// This header is support code only; it is not part of the public API and is not
// installed.
// ---------------------------------------------------------------------------
#ifndef CPPCOROUTINE_DEMO_COMMON_HPP
#define CPPCOROUTINE_DEMO_COMMON_HPP

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>

#include "cppcoroutine/coroutine.hpp"

namespace demo {

/// Human-readable task state.
inline std::string state_name(cppcoroutine::TaskState state) {
  return std::string(cppcoroutine::to_string(state));
}

/// Collects pass/fail observations and reports them at the end.
struct Report {
  int checks = 0;
  int failures = 0;

  void check(bool condition, const char* what) {
    ++checks;
    if (condition) {
      std::printf("  [ ok ] %s\n", what);
    } else {
      ++failures;
      std::printf("  [FAIL] %s\n", what);
    }
  }

  template <class A, class B>
  void eq(A&& lhs_value, B&& rhs_value, const char* what) {
    check(static_cast<bool>(lhs_value == rhs_value), what);
  }
};

/// Prints the demo verdict and returns a process exit code.
inline int finish(const char* name, const Report& report) {
  std::printf("--------------------------------------------------------------\n");
  if (report.failures == 0) {
    std::printf("DEMO %s: PASS (%d checks)\n", name, report.checks);
    return 0;
  }
  std::printf("DEMO %s: FAIL (%d of %d checks failed)\n", name, report.failures, report.checks);
  return 1;
}

/// Process-wide coroutine-frame counters (see Scheduler::Stats).
inline std::uint64_t frames_created() {
  return cppcoroutine::global_counters().frames_created.load(std::memory_order_relaxed);
}

inline std::uint64_t frames_destroyed() {
  return cppcoroutine::global_counters().frames_destroyed.load(std::memory_order_relaxed);
}

using Clock = cppcoroutine::Scheduler::clock;

/// Milliseconds elapsed since `start`.
inline long long ms_since(Clock::time_point start) {
  return static_cast<long long>(
      std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count());
}

}  // namespace demo

#endif  // CPPCOROUTINE_DEMO_COMMON_HPP
