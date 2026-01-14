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
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <chrono>
#include <thread>
#include <coroutine>

using namespace std::chrono_literals;

// 简单的协程返回类型
struct SimpleCoroutine {
    struct promise_type {
        int value;
        
        SimpleCoroutine get_return_object() {
            return SimpleCoroutine{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_value(int v) { value = v; }
    };

    std::coroutine_handle<promise_type> coro_handle;
    
    SimpleCoroutine(std::coroutine_handle<promise_type> h) : coro_handle(h) {}
    ~SimpleCoroutine() {
        if (coro_handle) coro_handle.destroy();
    }
    
    // 获取结果
    int get() {
        if (!coro_handle.done()) {
            coro_handle.resume();
        }
        return coro_handle.promise().value;
    }
};

SimpleCoroutine foo(void)
{
    std::cout << "Starting coroutine\n";
    std::this_thread::sleep_for(1s);
    std::cout << "Coroutine completed\n";
    co_return 42;
}

int main(int argc, char* argv[])
{
    auto coro = foo();
    std::cout << "Coroutine created, getting result...\n";
    int result = coro.get();
    std::cout << "Result: " << result << std::endl;
    return 0;
}