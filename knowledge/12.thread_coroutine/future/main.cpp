//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::future
//      2. std::async
//      3. std::promise
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex g_print_mutex;

template <typename... Args>
void safe_print(Args&&... args)
{
    std::lock_guard<std::mutex> lock(g_print_mutex);
    (std::cout << ... << args);
}

namespace ASYNC
{
    int long_running_task(int value)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return value;
    }

    bool is_prime(int x)
    {
        safe_print("Calculating. Please, wait...\n");
        for (int i = 2; i < x; ++i) {
            if (x % i == 0) return false;
        }
        return true;
    }

    class work
    {
    public:
        work(int value) : value_(value) {}

        std::future<int> spawn()
        {
            return std::async(std::launch::async, [=, tmp = *this] {
                return tmp.value_;
            });
        }
    private:
        int value_;
    };

    std::future<int> foo(int value)
    {
        work w(value);
        return w.spawn();
    }

    int test()
    {
        safe_print("==================== ASYNC ====================\n");

        std::future<int> result = std::async(std::launch::async, long_running_task, 10);
        safe_print("Main thread is doing other work...\n");
        int final_result = result.get();
        safe_print("Async task result: ", final_result, "\n");

        std::future<bool> result2 = std::async(std::launch::async, is_prime, 17);
        bool prime = result2.get();
        safe_print("Async task result: ", prime, "\n");

        std::future<int> result3 = foo(5);
        result3.wait();
        safe_print("Async task result: ", result3.get(), "\n");
        return 0;
    }
}

namespace PROMISE
{
    int test()
    {
        safe_print("==================== PROMISE ====================\n");

        std::promise<int> _promise;
        std::future<int> _future = _promise.get_future();
        std::thread t([&_promise](int value) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            _promise.set_value(value);
        }, 123);

        int final_result = _future.get();
        safe_print("Async task result: ", final_result, "\n");
        t.join();

        std::promise<int> _promise2;
        _future = _promise2.get_future();
        auto func = [](std::future<int>& future) {
            int result = future.get();
            safe_print("Async task result: ", result, "\n");
        };

        std::thread t2(func, std::ref(_future));
        _promise2.set_value(64);
        t2.join();

        return 0;
    }
}

int main(int argc, char* argv[])
{
    ASYNC::test();
    PROMISE::test();
    return 0;
}
