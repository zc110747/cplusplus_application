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
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <vector>
#include <latch>
#include <barrier>

void completion_callback() {
    std::cout << "completion! " << std::endl;
}

int main(int arg, const char* argv[]) 
{
    const int num_threads = 3;

    std::barrier sync_point(num_threads, completion_callback);
    std::vector<std::thread> threads;

    // max
    std::cout << sync_point.max() << std::endl;

    // arrive_and_wait
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&sync_point, i]() {
            std::cout << "thread " << i << " run..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "thread " << i << " arrive barrier." << std::endl;
            sync_point.arrive_and_wait();
            std::cout << "thread " << i << " over barrier." << std::endl;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}