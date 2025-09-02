<<<<<<< HEAD
﻿/*
std::barrier 是C++20引入的同步原语，定义在 <barrier> 头文件，用于多线程同步，确保一组线程在特定点同步，所有线程到达该点后才能继续执行后续操作

std::latch 是 C++20 引入的同步原语，定义在 <latch> 头文件中，用于多线程同步，可让一组线程等待直到某个事件完成。它的工作模式是一次性的，一旦计数器减到 0，就不能再重置
*/
#include <iostream>
#include <thread>
#include <vector>

#if __cplusplus > 202002L
#include <latch>
#include <barrier>
#endif

// 完成回调函数
void completion_callback() {
    std::cout << "所有线程已到达屏障，继续执行。" << std::endl;
}

int main() 
{
#if __cplusplus > 202002L
    // std::barrier 示例
    {
        const int num_threads = 3;
        std::barrier sync_point(num_threads, completion_callback);
    
        std::vector<std::thread> threads;
    
        // 创建多个线程
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&sync_point, i]() {
                std::cout << "线程 " << i << " 正在工作..." << std::endl;
                // 模拟一些工作
                std::this_thread::sleep_for(std::chrono::seconds(1));
    
                std::cout << "线程 " << i << " 已到达屏障。" << std::endl;
                // 到达同步点并等待其他线程
                sync_point.arrive_and_wait();
    
                std::cout << "线程 " << i << " 越过屏障，继续执行。" << std::endl;
            });
        }
    
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }
    }

    // std::latch 示例
    {
        const int num_threads = 3;
        std::latch work_done(num_threads);
    
        std::vector<std::thread> threads;
    
        // 创建多个线程
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&work_done](int id){
                std::cout << "线程 " << id << " 开始工作..." << std::endl;
                // 模拟一些工作
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "线程 " << id << " 完成工作，计数器减 1。" << std::endl;
                work_done.count_down();  
            }, i);
        }
    
        std::cout << "主线程等待所有线程完成工作..." << std::endl;
        work_done.wait();
        std::cout << "所有线程完成工作，主线程继续执行。" << std::endl;
    
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }     
    }
#else
    std::cout << "当前 C++ 版本不支持 std::barrier 和 std::latch" << std::endl;
#endif
    return 0;
=======
﻿/*
std::barrier 是C++20引入的同步原语，定义在 <barrier> 头文件，用于多线程同步，确保一组线程在特定点同步，所有线程到达该点后才能继续执行后续操作

std::latch 是 C++20 引入的同步原语，定义在 <latch> 头文件中，用于多线程同步，可让一组线程等待直到某个事件完成。它的工作模式是一次性的，一旦计数器减到 0，就不能再重置
*/
#include <iostream>
#include <thread>
#include <vector>

#if __cplusplus > 202002L
#include <latch>
#include <barrier>
#endif

// 完成回调函数
void completion_callback() {
    std::cout << "所有线程已到达屏障，继续执行。" << std::endl;
}

int main() 
{
#if __cplusplus > 202002L
    // std::barrier 示例
    {
        const int num_threads = 3;
        std::barrier sync_point(num_threads, completion_callback);
    
        std::vector<std::thread> threads;
    
        // 创建多个线程
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&sync_point, i]() {
                std::cout << "线程 " << i << " 正在工作..." << std::endl;
                // 模拟一些工作
                std::this_thread::sleep_for(std::chrono::seconds(1));
    
                std::cout << "线程 " << i << " 已到达屏障。" << std::endl;
                // 到达同步点并等待其他线程
                sync_point.arrive_and_wait();
    
                std::cout << "线程 " << i << " 越过屏障，继续执行。" << std::endl;
            });
        }
    
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }
    }

    // std::latch 示例
    {
        const int num_threads = 3;
        std::latch work_done(num_threads);
    
        std::vector<std::thread> threads;
    
        // 创建多个线程
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&work_done](int id){
                std::cout << "线程 " << id << " 开始工作..." << std::endl;
                // 模拟一些工作
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "线程 " << id << " 完成工作，计数器减 1。" << std::endl;
                work_done.count_down();  
            }, i);
        }
    
        std::cout << "主线程等待所有线程完成工作..." << std::endl;
        work_done.wait();
        std::cout << "所有线程完成工作，主线程继续执行。" << std::endl;
    
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }     
    }
#else
    std::cout << "当前 C++ 版本不支持 std::barrier 和 std::latch" << std::endl;
#endif
    return 0;
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
}