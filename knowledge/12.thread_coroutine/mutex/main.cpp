/*
std::mutex是C++标准库中用于实现线程同步的互斥锁类，定义在 <mutex> 头文件中。它的主要作用是保护共享资源，防止多个线程同时访问这些资源而导致数据竞争和不一致的问题

std::lock_guard是C++标准库中提供的一种RAII（资源获取即初始化）风格的互斥锁包装器，定义在 <mutex> 头文件中。它的主要作用是在构造函数中获取互斥锁，在析构函数中释放互斥锁，确保互斥锁的正确获取和释放。

std::shared_mutex是C++17引入的一种读写锁，位于 <shared_mutex> 头文件中。与普通的互斥锁（如 std::mutex）不同，std::shared_mutex 允许多个线程同时进行读操作，但在写操作时会独占资源，确保数据的一致性。这种特性使得它在多读少写的场景下能显著提高并发性能。

std::recursive_mutex是C++标准库中提供的一种递归互斥锁，定义在 <mutex> 头文件中。与普通的互斥锁不同，递归互斥锁允许同一个线程多次获取同一个互斥锁，而不会发生死锁。这在某些情况下非常有用，比如在递归函数或嵌套的锁保护下。
*/
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

static volatile int val = 0;

namespace SPOCK_LOCK_PROCESS 
{ 
    std::mutex mutex_1;
    std::mutex mutex_2;
    std::vector<int> vec;
    
    // std::scoped_lock 
    void thread_function_1(void) 
    {
        for (int i = 0; i < 10; i++) {
            {
                std::scoped_lock lock(mutex_1, mutex_2);
                vec.push_back(i);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    }

    void thread_function_2(void)
    {
        for (int i = 0; i < 10; i++) {
            {
                std::scoped_lock lock(mutex_2, mutex_1);
                vec.push_back(i + 100);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    int test(void) 
    { 
        std::cout << "================== SPOCK_LOCK_PROCESS ==================" << std::endl;

        // scoped_lock
        std::thread t3(thread_function_1);
        std::thread t4(thread_function_2);
    
        t3.join();
        t4.join();

        for (const auto& i : vec) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        return 0;
    }
}

// 普通互斥锁 std::mutex
std::mutex _mutex;

// 读写锁 std::shared_mutex
std::shared_mutex _shared_mutex;
int shared_data = 0;

void reader(int id) 
{
    std::shared_lock<std::shared_mutex> lock(_shared_mutex); // 获取共享锁
    std::cout << "Reader " << id << " reads: " << shared_data << std::endl;
}

void writer(int id)
{
    std::unique_lock<std::shared_mutex> lock(_shared_mutex); // 获取独占锁
    shared_data++;
    std::cout << "Writer " << id << " writes: " << shared_data << std::endl;
}

// 递归互斥锁 std::recursive_mutex
std::recursive_mutex recursive_mtx;

void recursiveFunction(int depth) {
    if (depth == 0) return;

    recursive_mtx.lock();
    std::cout << "Locked at depth " << depth << " by thread " << std::this_thread::get_id() << std::endl;

    recursiveFunction(depth - 1);

    std::cout << "Unlocking at depth " << depth << " by thread " << std::this_thread::get_id() << std::endl;
    recursive_mtx.unlock();
}

void worker_task(int n)
{
    auto id = std::this_thread::get_id();

    //lock, unlock
    _mutex.lock();
    for(int i=0; i<n; i++)
    {
        std::cout<<"lock id is 0x"<<std::hex<<id<<std::endl;
    }
    _mutex.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));  

    //lock_guard
    {
        std::lock_guard<std::mutex> lock_guard(_mutex);
        for(int i=0; i<n; i++)
        {
            std::cout<<"lock_guard id is 0x"<<std::hex<<id<<std::endl;
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));  

    //try_lock, native_handle
    if(_mutex.try_lock())
    {
        std::cout<<"try_lock id is 0x:"<<std::hex<<id<<std::endl;
        std::cout<<"native_handle:"<<_mutex.native_handle()<<std::endl;
        _mutex.unlock();
    }
    else
    {
        _mutex.lock();
        std::cout<<"try_lock failed!, id is 0x"<<std::hex<<id<<std::endl;
        _mutex.unlock();
    }
}

int main(int argc, char* argv[])
{
    try
    {
        std::cout<< "\n====== std::mutex ======\n";
        std::thread t1(worker_task, 3);
        std::thread t2(worker_task, 2);

        t1.join();
        t2.join();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    {
        std::cout<< "\n====== std::chrono ======\n";
        std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm *ptm = std::localtime(&tt);
        ptm->tm_sec += 2;
        std::cout<<std::oct<<"hour:"<<ptm->tm_hour<<",minute:"<<ptm->tm_min<<",second:"<<ptm->tm_sec<<std::endl;
        std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(mktime(ptm)));
        std::cout<<"sleep timeout finished!"<<std::endl;
    }

    {
        std::vector<std::thread> threads;

        std::cout<< "\n====== std::shared_mutex ======\n";

        // 创建3个读线程，一个写线程
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(reader, i);
        }
        threads.emplace_back(writer, 0);
        for (auto& t : threads) {
            t.join();
        }
    }

    {
        std::cout<< "\n====== std::recursive_mutex ======\n";
        std::thread t1(recursiveFunction, 3);
        std::thread t2(recursiveFunction, 3);

        t1.join();
        t2.join();
    }

    return 0;
} 
