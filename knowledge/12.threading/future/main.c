#include <iostream>
#include <future>
#include <chrono>

// 一个简单的函数，模拟耗时操作
int long_running_task(int value) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟耗时2秒
    return value * 2;
}

int main() {
    // 使用 std::async 启动异步任务
    std::future<int> result = std::async(std::launch::async, long_running_task, 10);

    // 在异步任务执行的同时，主线程可以继续执行其他操作
    std::cout << "主线程正在执行其他操作..." << std::endl;

    // 获取异步任务的结果
    int final_result = result.get();

    std::cout << "异步任务的结果: " << final_result << std::endl;

    return 0;
}