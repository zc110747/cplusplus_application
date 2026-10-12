//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. coroutine应用
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <coroutine>
#include <cstdint>
#include <deque>
#include <exception>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// ============================================================
// Configuration
// ============================================================

#ifndef PRODUCER_COUNT
#define PRODUCER_COUNT 3
#endif

static_assert(PRODUCER_COUNT > 0,
              "PRODUCER_COUNT must be greater than zero");

using namespace std::chrono_literals;
using Clock = std::chrono::steady_clock;

// ============================================================
// Coroutine Task
// The scheduler owns every coroutine handle after spawn().
// ============================================================

class Task {
public:
    struct promise_type;
    using Handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        std::exception_ptr exception;

        Task get_return_object() noexcept {
            return Task{Handle::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept {
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        void return_void() noexcept {}

        void unhandled_exception() noexcept {
            exception = std::current_exception();
        }
    };

    explicit Task(Handle h) noexcept : handle_(h) {}

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&& other) noexcept
        : handle_(std::exchange(other.handle_, {})) {}

    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }

    Handle release() noexcept {
        return std::exchange(handle_, {});
    }

private:
    Handle handle_{};
};

// ============================================================
// EventLoop Scheduler
// ============================================================

class EventLoop {
public:
    using Handle = Task::Handle;

    struct Timer {
        Clock::time_point deadline;
        std::uint64_t sequence;
        Handle handle;
    };

    struct TimerLater {
        bool operator()(const Timer& a, const Timer& b) const {
            if (a.deadline != b.deadline) {
                return a.deadline > b.deadline;
            }
            return a.sequence > b.sequence;
        }
    };

    // Put a task into the initial ready queue.
    void spawn(Task&& task) {
        Handle h = task.release();
        if (!h) {
            return;
        }

        {
            std::lock_guard lock(mutex_);
            all_tasks_.push_back(h);
            ready_.push_back(h);
        }
        cv_.notify_one();
    }

    // Timer awaiter: suspend now, resume at the deadline.
    struct SleepAwaiter {
        EventLoop& loop;
        std::chrono::milliseconds duration;

        bool await_ready() const noexcept {
            return duration.count() <= 0;
        }

        void await_suspend(Handle h) {
            loop.add_timer(h, duration);
        }

        void await_resume() const noexcept {}
    };

    SleepAwaiter sleep_for(std::chrono::milliseconds duration) {
        return SleepAwaiter{*this, duration};
    }

    // Wait until the shared data queue contains an item.
    struct DataAwaiter {
        EventLoop& loop;

        bool await_ready() const {
            return loop.has_data();
        }

        void await_suspend(Handle h) {
            loop.wait_for_data(h);
        }

        void await_resume() const noexcept {}
    };

    DataAwaiter wait_for_data() {
        return DataAwaiter{*this};
    }

    // Called by a producer after publishing data.
    void notify_data_available() {
        {
            std::lock_guard lock(mutex_);

            // Wake one waiting consumer.
            if (!waiting_consumers_.empty()) {
                Handle h = waiting_consumers_.front();
                waiting_consumers_.pop_front();
                ready_.push_back(h);
            }
        }
        cv_.notify_one();
    }

    bool has_data() const {
        std::lock_guard lock(data_mutex_);
        return !data_.empty();
    }

    void push_data(std::string value) {
        {
            std::lock_guard lock(data_mutex_);
            data_.push_back(std::move(value));
        }
        notify_data_available();
    }

    bool try_pop_data(std::string& value) {
        std::lock_guard lock(data_mutex_);

        if (data_.empty()) {
            return false;
        }

        value = std::move(data_.front());
        data_.pop_front();
        return true;
    }

    // Can be called from another thread.
    void request_stop() {
        stopping_.store(true);
        cv_.notify_all();
    }

    void run() {
        std::cout << "EventLoop started. Press Enter to stop.\n";

        while (!stopping_.load()) {
            // First, move every expired timer into the ready queue.
            const auto now = Clock::now();

            while (!timers_.empty() &&
                   timers_.top().deadline <= now) {
                Handle h = timers_.top().handle;
                timers_.pop();
                enqueue_ready(h);
            }

            Handle current{};

            {
                std::unique_lock lock(mutex_);

                if (!ready_.empty()) {
                    current = ready_.front();
                    ready_.pop_front();
                } else if (!stopping_.load()) {
                    // No ready task: sleep until a task is queued,
                    // stop is requested, or the next timer expires.
                    if (timers_.empty()) {
                        cv_.wait(lock, [this] {
                            return stopping_.load() ||
                                   !ready_.empty();
                        });
                    } else {
                        const auto deadline = timers_.top().deadline;
                        cv_.wait_until(lock, deadline, [this] {
                            return stopping_.load() ||
                                   !ready_.empty();
                        });
                    }
                }
            }

            if (stopping_.load()) {
                break;
            }

            if (current && !current.done()) {
                current.resume();

                if (current.done()) {
                    auto error = current.promise().exception;
                    if (error) {
                        try {
                            std::rethrow_exception(error);
                        } catch (const std::exception& e) {
                            std::cerr << "Coroutine error: "
                                      << e.what() << '\n';
                        } catch (...) {
                            std::cerr << "Unknown coroutine error\n";
                        }
                    }
                }
            }
        }

        std::cout << "EventLoop stopping...\n";
        cleanup();
        std::cout << "EventLoop stopped.\n";
    }

private:
    void enqueue_ready(Handle h) {
        {
            std::lock_guard lock(mutex_);
            ready_.push_back(h);
        }
        cv_.notify_one();
    }

    void add_timer(
        Handle h,
        std::chrono::milliseconds duration) {

        timers_.push(Timer{
            Clock::now() + duration,
            next_sequence_++,
            h
        });
    }

    void wait_for_data(Handle h) {
        std::lock_guard lock(mutex_);
        waiting_consumers_.push_back(h);
    }

    void cleanup() {
        // No coroutine is resumed after stopping is requested.
        // Destroy each owned handle exactly once.
        for (Handle h : all_tasks_) {
            if (h) {
                h.destroy();
            }
        }

        all_tasks_.clear();

        {
            std::lock_guard lock(mutex_);
            ready_.clear();
            waiting_consumers_.clear();
        }

        while (!timers_.empty()) {
            timers_.pop();
        }
    }

    mutable std::mutex data_mutex_;
    std::deque<std::string> data_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::deque<Handle> ready_;
    std::deque<Handle> waiting_consumers_;
    std::vector<Handle> all_tasks_;

    // Only the event-loop thread modifies the timer heap.
    std::priority_queue<
        Timer,
        std::vector<Timer>,
        TimerLater
    > timers_;

    std::uint64_t next_sequence_ = 0;
    std::atomic<bool> stopping_{false};
};

// ============================================================
// Producer / Consumer
// ============================================================

Task producer(
    EventLoop& loop,
    int producer_id,
    std::chrono::milliseconds period) {

    std::uint64_t count = 0;

    while (true) {
        // Wait without blocking the event-loop thread.
        co_await loop.sleep_for(period);

        ++count;

        const auto now = Clock::now().time_since_epoch();
        const auto ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now).count();

        std::string item =
            "P" + std::to_string(producer_id) +
            "#" + std::to_string(count) +
            " @ " + std::to_string(ms) + "ms";

        std::cout << "[Produce] " << item << '\n';

        loop.push_data(std::move(item));
    }
}

Task consumer(EventLoop& loop) {
    std::uint64_t count = 0;

    while (true) {
        // Suspend if no data is available.
        co_await loop.wait_for_data();

        std::string item;

        // Drain available items before waiting again.
        while (loop.try_pop_data(item)) {
            ++count;
            std::cout << "[Consume] #" << count
                      << " <- " << item << '\n';
        }
    }
}

// ============================================================
// Main
// ============================================================

int main() {
    EventLoop loop;

    constexpr int periods[] = {1000, 1500, 2000};

    // Consumer and producers all enter the ready queue.
    loop.spawn(consumer(loop));

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        const auto period =
            std::chrono::milliseconds(periods[i % 3]);

        loop.spawn(producer(loop, i + 1, period));

        std::cout << "Producer " << (i + 1)
                  << ", period = " << period.count()
                  << " ms\n";
    }

    // Input thread requests stop; it does not run coroutines.
    std::thread input_thread([&loop] {
        std::string line;
        std::getline(std::cin, line);
        loop.request_stop();
    });

    loop.run();

    input_thread.join();
    return 0;
}
