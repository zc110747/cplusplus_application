/*
std::async:
C++11引入的一个函数模板，用于启动异步任务。它的主要作用是在一个独立的线程中执行一个函数，并返回一个std::future对象，用于获取函数的返回值。

std::future: 
std::future是C++11引入的一个类模板，用于表示异步操作的结果。它提供了一种机制，允许在一个线程中等待另一个线程的结果。

std::promise:
std::promise是C++11引入的一个类模板，用于在一个线程中设置值或异常，然后在另一个线程中通过关联的 std::future 对象获取这个值或异常，以此实现线程间的同步和数据传递。它允许线程之间进行数据交换。
*/
#include <iostream>
#include <future>
#include <chrono>

// 一个简单的函数，模拟耗时操作
int long_running_task(int value) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟耗时2秒
    return value * 2;
}

int main() 
{
    // 使用 std::async 启动异步任务
    std::future<int> result = std::async(std::launch::async, long_running_task, 10);
    std::cout << "主线程正在执行其他操作..." << std::endl;

    int final_result = result.get();
    std::cout << "异步任务的结果: " << final_result << std::endl;

    std::promise<int> _promise;
    std::future<int> _future = _promise.get_future();

    std::thread t([&_promise](int value) {
        // 模拟耗时操作
        std::this_thread::sleep_for(std::chrono::seconds(2));
        _promise.set_value(value);
    }, 123);

    final_result = _future.get();
    std::cout << "异步任务的结果: " << final_result << std::endl;

    t.join(); // 等待线程结束

    return 0;
}