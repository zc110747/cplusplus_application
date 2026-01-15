//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. thread
//      2. thread_local
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

namespace THREAD_PROCESS
{
    std::mutex mutex_lock;

    void worker_task_0(std::string str, int n)
    {
        mutex_lock.lock();
        for (int i = 0; i < n; i++) {
            std::cout<<"times "<<i<<" : "<<str<<std::endl;
        }
        mutex_lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        //get_id
        mutex_lock.lock();
        std::cout<<str<<" id:0x"<<std::hex<<std::this_thread::get_id()<<std::endl;
        mutex_lock.unlock();
    } 

    void worker_task_1(void)
    {
        mutex_lock.lock();
        std::cout<<"worker_task_1 id: 0x"<<std::hex<<std::this_thread::get_id()<<std::endl;
        mutex_lock.unlock();
    } 

    std::atomic<bool> ready{false};

    void worker_task_2(void)
    {
        while (!ready.load()) {
            std::this_thread::yield();
        }
        
        std::lock_guard<std::mutex> lock(mutex_lock);
        std::cout << "worker_task_2 is running" << std::endl;
    }

    int test(void) 
    {
        std::cout << "================== THREAD_PROCESS ==================" << std::endl;

        std::thread t0(worker_task_0, "hello world!", 3);
        std::thread t1(worker_task_1);
        std::thread t3(worker_task_2);

        //native_handle, get_id
        auto nh = t1.native_handle();
        mutex_lock.lock();
        std::cout << "hardware_concurrency: " << std::thread::hardware_concurrency() << std::endl;
        std::cout << "native_handle: 0x" << std::hex << nh <<std::endl;
        std::cout << "get_id: 0x" << t1.get_id() << std::endl;

        //joinable, join
        std::cout << "joinable:" << std::boolalpha << t1.joinable() <<std::endl;

        mutex_lock.unlock();

        t0.join();
        t1.join();

        std::thread t2(worker_task_0, "detech task!", 5);
        t2.detach();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // ready设置标志位，通知worker_task_2线程
        ready.store(true);
        t3.join();

        return 0;
    }
}

namespace THREAD_LOCAL
{
    std::mutex mutex_lock;
    thread_local int lcoal_var = 1;

    void thread_task_0(void)
    {
        int i = 0;

        for (int i = 0; i < 5; i++){
            
            lcoal_var++;

            {
                std::lock_guard<std::mutex> lock(mutex_lock);
                std::cout << "thread_task_0: "<< lcoal_var << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    void foo(int n)
    {
        thread_local int v = 1; //函数只有第一次执行初始化，后续不执行(类似局部静态变量)

        v += n;

        {
            std::lock_guard<std::mutex> lock(mutex_lock);
            std::cout << "foo: "<< v << std::endl;
        }
    }

    void thread_task_1(void)
    {
        for (int i = 0; i < 5; i++) {
            
            lcoal_var += 100;

            {
                std::lock_guard<std::mutex> lock(mutex_lock);
                std::cout << "thread_task_1: "<< lcoal_var <<std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        for(int i = 0; i < 4; i++) {
            foo(1);
        }
    }

    int test(void)
    {
        std::cout << "================== THREAD_LOCAL ==================" << std::endl;

        std::cout << std::dec; //

        std::thread th1(thread_task_0);
        std::thread th2(thread_task_1);

        th1.join();
        th2.join();
        return 0;
    } 

}

int main(int argc, char* argv[])
{
    try {
        // std::thread
        THREAD_PROCESS::test();

        // std::thread_local
        THREAD_LOCAL::test();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
} 
