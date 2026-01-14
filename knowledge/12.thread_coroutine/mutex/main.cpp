//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::mutex
//      2. std::recursive_mutex
//      3. std::lock_guard
//      4. std::scoped_lock
//
// Author:
//      @公众号 <嵌入式技术总结>
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
#include <vector>

namespace MUTEX
{
    std::mutex mutex_1;
    std::mutex mutex_2;
    int val_1 = 0, val_2 = 0;

    void multi_func(int id)
    {
        if (mutex_1.try_lock()) {
            val_1++;
            mutex_1.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        mutex_2.lock();
        std::cout << "thread id is " << id << std::endl;
        val_2++;
        mutex_2.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    int test(void)
    {
        std::cout << "================== MUTEX ==================" << std::endl;
        std::vector<std::thread> vec;

        for (int i = 0; i < 100; i++) {
            vec.push_back(std::thread(multi_func, i));
        }

        for (auto& t : vec) {
            t.join();
        }

        std::cout << "val1 = " << val_1 << std::endl;
        std::cout << "val2 = " << val_2 << std::endl;

        return 0;
    }
}

namespace RECURSIVE_MUTEX
{
    std::recursive_mutex r_mutex;
    int val = 0;

    void func(int id)
    {   
        if (id == 0) return;

        r_mutex.lock();
        std::cout << "thread id is " << id << std::endl;
        val++;

        func(id - 1);   // recursive_mutex 支持递归调用申请

        r_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    int test(void)
    {
        std::cout << "================== RECURSIVE_MUTEX ==================" << std::endl;
        std::vector<std::thread> vec;

        for (int i = 0; i < 5; i++) {
            vec.push_back(std::thread(func, i));
        }

        for (int i = 0; i < 5; i++) {
            vec[i].join();
        }

        std::cout << "val = " << val << std::endl;

        return 0;
    }
};

namespace SPOCK_LOCK 
{
    std::mutex mutex_1;
    std::mutex mutex_2;
    std::vector<int> vec;

    void thread_function(int id) 
    {
        std::scoped_lock lock(mutex_1, mutex_2);
        vec.push_back(id);
    }

    int test(void) 
    { 
        std::cout << "================== SPOCK_LOCK_PROCESS ==================" << std::endl;
        std::vector<std::thread> vec_thread;

        // scoped_lock
        for (int i = 0; i < 10; i++) {
            vec_thread.push_back(std::thread(thread_function, i));
        }

        for (int i = 0; i < 10; i++) {
            vec_thread[i].join();
        }

        for (const auto& i : vec) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        return 0;
    }
}

namespace LOCK_GUARD
{
    std::mutex mutex_1;
    int val = 0;

    void func(int id)
    {
        std::lock_guard<std::mutex> lock_guard(mutex_1);
        val++;
    }

    int test(void)
    {
        std::cout << "================== LOCK_GUARD ==================" << std::endl;
        std::vector<std::thread> vec;

        for (int i = 0; i < 5; i++) {
            vec.push_back(std::thread(func, i));
        }

        for (int i = 0; i < 5; i++) {
            vec[i].join();
        }

        std::cout << "val = " << val << std::endl;

        return 0;
    }
}

namespace UNIQUE_LOCK
{
    int count = 0, count1 = 0;
    std::mutex mt, mt1;
    void work_thread(void)
    {
        {
            // 自动释放
            std::unique_lock<std::mutex> lck(mt);
            count++;
        }

        // 手动解锁
        std::unique_lock<std::mutex> lck(mt);
        count++;
        lck.unlock();

        if (lck.try_lock()) {
            count1++;
            lck.unlock();
        }

        // 手动释放
        std::unique_lock<std::mutex> lck1(mt);
        count1++;
        lck.release();  
    }

    int test(void)
    {
        std::cout << "================== UNIQUE_LOCK ==================" << std::endl;
        std::vector<std::thread> threads;

        for (int i = 0; i < 10; i++) {
            threads.push_back(std::thread(work_thread));
        }
        
        for (auto& t : threads) {
            t.join();
        }
        std::cout << "count: " << count << std::endl;
        std::cout << "count1: " << count1 << std::endl;
        return 0;
    }
}
int main(int argc, char* argv[])
{
    MUTEX::test();

    RECURSIVE_MUTEX::test();

    SPOCK_LOCK::test();

    LOCK_GUARD::test();

    UNIQUE_LOCK::test();

    return 0;
} 
