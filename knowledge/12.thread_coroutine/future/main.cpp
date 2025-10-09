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
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <future>
#include <chrono>
#include <thread>

namespace ASYNC
{
    int long_running_task(int value) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return value;
    }

    bool is_prime (int x) {
        std::cout << "Calculating. Please, wait...\n";
        for (int i=2; i<x; ++i) {
            if (x%i==0) {
                return false;
            }
        }
        return true;
    }

    class work
    {
    public:
        work(int value) : value_(value) {};

        std::future<int> spawn(void)
        {
            return std::async(std::launch::async, [=, tmp=*this] {
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

    int test(void)  
    {
        std::cout << "==================== ASYNC ====================" << std::endl;
        
        std::future<int> result = std::async(std::launch::async, long_running_task, 10);
        std::cout << "主线程正在执行其他操作..." << std::endl;
        int final_result = result.get();
        std::cout << "异步任务的结果: " << final_result << std::endl;

        std::future<bool> result2 = std::async(std::launch::async, is_prime, 17);
        std::cout << "异步任务的结果: " << result2.get() << std::endl;

        std::future<int> result3 = foo(5);
        result3.wait();
        std::cout << "异步任务的结果: " << result3.get() << std::endl;
        return 0;
    }
}

namespace PROMISE
{
    int test(void)
    {
        std::cout << "==================== PROMISE ====================" << std::endl;

        std::promise<int> _promise;
        std::future<int> _future = _promise.get_future();
        std::thread t([&_promise](int value) {
            // 模拟耗时操作
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            _promise.set_value(value);
        }, 123);

        int final_result = _future.get();
        std::cout << "异步任务的结果: " << final_result << std::endl;
        t.join(); // 等待线程结束

        // std::promise
        std::promise<int> _promise2;
        _future = _promise2.get_future();
        auto func = [](std::future<int> &future) {
            int result = future.get();
            std::cout << "异步任务的结果: " << result << std::endl;
        };

        std::thread t2(func, std::ref(_future));
        _promise2.set_value(64);

        t2.join();

        return 0;
    }
}

int main(int argc, char *argv[])
{
    ASYNC::test();

    PROMISE::test();
    return 0;
}
