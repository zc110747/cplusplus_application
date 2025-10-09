//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. barrier
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <vector>
#include <latch>
#include <barrier>

// 完成回调函数
void completion_callback() {
    std::cout << "所有线程已到达屏障，继续执行。" << std::endl;
}

int main() 
{
    const int num_threads = 3;

    std::barrier sync_point(num_threads, completion_callback);
    std::vector<std::thread> threads;

    // 创建多个线程
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&sync_point, i]() {
            std::cout << "线程 " << i << " 正在工作..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "线程 " << i << " 已到达屏障。" << std::endl;
            sync_point.arrive_and_wait();
            std::cout << "线程 " << i << " 越过屏障，继续执行。" << std::endl;
        });
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}