//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::shared_mutex
//、    2. std::shared_lock
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>    // std::chrono::seconds
#include <mutex>
#include <unistd.h>
#include <ctime>
#include <shared_mutex>
#include <vector>

namespace SHARD_MUTEX
{
    std::shared_mutex shared_mutex_1;
    int val_ = 0;
    
    void read_thread(void)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::shared_lock<std::shared_mutex> lock(shared_mutex_1);
        std::cout << "read 1 val: " << val_ << std::endl;
    }

    void read_thread_2(void)
    {
        for (int i = 0; i < 10; i++) {
            shared_mutex_1.lock_shared();       // lock shared，允许读操作同时进行
            std::cout << "read 2 val: " << val_ << std::endl;
            shared_mutex_1.unlock_shared();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::this_thread::yield();
        }
    }

    void write_thread(void)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock<std::shared_mutex> lock(shared_mutex_1);
        val_++;
    }

    void write_thread_2(void)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        shared_mutex_1.lock();       // lock unique，独占访问
        val_++;
        shared_mutex_1.unlock();
    }

    int test(void) 
    {
        std::cout << "================= Shared Mutex ===================" << std::endl;
        std::vector<std::thread> threads;

        for (int i = 0; i < 10; i++) {
            threads.push_back(std::thread(write_thread));
        }
        threads.push_back(std::thread(read_thread));
        threads.push_back(std::thread(write_thread_2));
        threads.push_back(std::thread(read_thread_2));

        for (auto& thread : threads) {
            thread.join();
        }
        return 0;
    }
}

int main(int argc, const char *argv[])
{
    SHARD_MUTEX::test();

    return 0;
}
