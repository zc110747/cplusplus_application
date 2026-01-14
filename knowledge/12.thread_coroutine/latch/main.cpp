//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::latch
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <vector>
#include <latch>

int main(int argc, const char* argv[])
{
    const int num_threads = 3;

    std::latch work_done(num_threads);
    std::vector<std::thread> threads;

    // 创建多个线程
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&work_done](int id){
            std::cout << "线程 " << id << " 开始工作..." << std::endl;
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