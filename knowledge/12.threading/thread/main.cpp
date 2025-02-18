/*
std::thread 
C++11 引入的标准库类，定义在 <thread> 头文件中，用于创建和管理线程。它提供了一种跨平台的方式来实现多线程编程，使得开发者可以方便地在 C++ 程序中创建新的执行线程。

std::scoped_lock 
C++17 引入的模板类，定义在 <mutex> 头文件中，用于在作用域内管理多个互斥锁，避免死锁问题。它可以同时锁定多个互斥锁，并且在其生命周期结束时自动解锁这些互斥锁。
*/
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>    // std::chrono::seconds
#include <unistd.h>
#include <atomic>
#include <mutex>

static volatile int val;
std::mutex mutex_lock;

void worker_task_0(std::string str, int n)
{
    mutex_lock.lock();
    for(int i=0; i<n; i++)
    {
        std::cout<<"times "<<i<<" : "<<str<<std::endl;
    }
    mutex_lock.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));

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

std::mutex mutex1;
std::mutex mutex2;

void threadFunction1() {
    std::scoped_lock lock(mutex1, mutex2);
    std::cout << "Thread 1 acquired both locks." << std::endl;
    // 模拟一些工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void threadFunction2() {
    std::scoped_lock lock(mutex2, mutex1);
    std::cout << "Thread 2 acquired both locks." << std::endl;
    // 模拟一些工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char* argv[])
{
    try
    {
        std::thread t0(worker_task_0, "hello world!", 3);
        std::thread t1(worker_task_1);

        //native_handle, get_id
        auto nh = t1.native_handle();
        mutex_lock.lock();
        std::cout<<"native_handle: 0x"<<std::hex<<nh<<std::endl;
        std::cout<<"get_id: 0x"<<t1.get_id()<<std::endl;

        //joinable, join
        std::cout<<"joinable:"<<std::boolalpha<<t1.joinable()<<std::endl;
        mutex_lock.unlock();

        //join
        t0.join();
        t1.join();

        //detach
        std::thread t2(worker_task_0, "detech task!", 5);
        t2.detach();

        sleep(2);

        mutex_lock.lock();
        std::cout<<"thread_study_handle leave!"<<std::endl;
        mutex_lock.unlock();

        //scoped_lock
        std::thread t3(threadFunction1);
        std::thread t4(threadFunction2);
    
        t3.join();
        t4.join(); 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
} 
