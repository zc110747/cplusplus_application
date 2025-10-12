//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. atomic
//      2. atomic_flag
//      3. memory_order
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <atomic>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>    // std::chrono::seconds
#include <vector>

namespace ATOMIC_EXAMPLE
{
    struct Node { 
        int value; 
        Node* next; 
    };
    std::atomic<Node*> list_head (nullptr);

    void append (int val) 
    {
        Node* oldHead = list_head;
        Node* newNode = new Node {val,oldHead};

        // what follows is equivalent to: list_head = newNode, but in a thread-safe way:
        while (!list_head.compare_exchange_weak(oldHead,newNode)) {
            newNode->next = oldHead;
        }
    }

    std::atomic<int> val = 0;
    void add(void)
    {
        for (int index=0; index<10; index++) {
            val++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    int test(void)
    {
        std::cout << "================ Atomic test ================" << std::endl;

        std::atomic<int> a(0);

        // store, load, T()
        a.store(1);
        std::cout << a.load() << std::endl;
        if (a == 1) {
            std::cout << "a == 1" << std::endl;
        }

        a.fetch_add(1);
        std::cout << "fetch_add: " << a << std::endl;

        a = 3;
        std::cout << "operator=: " << a << std::endl;

        a.fetch_sub(1);
        std::cout << "fetch_sub=: " << a << std::endl;

        a++;
        std::cout << "operator++: " << a << std::endl;

        a--;
        std::cout << "operator--: " << a << std::endl;

        // spawn 10 threads to fill the linked list:
        std::vector<std::thread> threads;
        for (int i=0; i<10; ++i) {
            threads.push_back(std::thread(append,i));
        }
        for (auto& th : threads) {
            th.join();
        }
        // print contents:
        for (Node* it = list_head; it!=nullptr; it=it->next) {
            std::cout << ' ' << it->value;
        }
        std::cout << '\n';

        // cleanup:
        Node* it; 
        while (it=list_head) {
            list_head=it->next;
            delete it;
        }
        threads.clear();

        // threads for adding
        for (int i=0; i<10; ++i) {
            threads.emplace_back(std::thread(add));
        }
        for (auto& th : threads) {
            th.join();
        }
        std::cout << "add: " << val << std::endl;

        return 0;
    }
}

namespace ATOMIC_FLAG_EXAMPLE
{
    // 自旋锁类
    class SpinLock {
    private:
        std::atomic_flag _lock_flag{ATOMIC_FLAG_INIT};

    public:
        void lock() {
            while (_lock_flag.test_and_set(std::memory_order_acquire)) {
                // 自旋等待锁释放
            }
        }

        void unlock() {
            _lock_flag.clear(std::memory_order_release);
        }
    };

    // RAII 自旋锁守卫类
    template<typename T>
    class LockGuard {
    public:
        LockGuard(T& spinLock) : _spinLock(spinLock) {
            _spinLock.lock();
        }
        ~LockGuard() {
            _spinLock.unlock();
        }
    private:
        T& _spinLock;
    };

    SpinLock spinLock;
    int sharedData = 0;
    int sharedData2 = 0;

    void worker(void)
    {
        // 通过自旋锁保护sharedData
        for (int i = 0; i < 100; ++i) {
            spinLock.lock();
            ++sharedData;
            spinLock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // 通过自旋锁保护sharedData2
        for (int i = 0; i < 100; ++i) {
            {
                LockGuard<SpinLock> guard(spinLock);
                ++sharedData2;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    int test(void) 
    { 
        std::cout << "================ Atomic flag test ================" << std::endl;

        std::vector<std::thread> threads;

        // 创建多个线程
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back(worker);
        }
    
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }
    
        std::cout << "Shared data: " << sharedData << std::endl;    // 1000
        std::cout << "Shared data2: " << sharedData2 << std::endl;  // 1000
        return 0;
    }
}

namespace ATOMIC_MEMORY_ORDER_EXAMPLE
{
    std::atomic<int> data(0);
    std::atomic<bool> ready(false);

    void writer() 
    {
        data.store(42, std::memory_order_relaxed);
        ready.store(true, std::memory_order_relaxed);
    }

    void reader() 
    {
        while (!ready.load(std::memory_order_relaxed)) {
        }
        ATOMIC_FLAG_EXAMPLE::spinLock.lock();
        std::cout << "Data: " << data.load(std::memory_order_relaxed) << std::endl;
        ATOMIC_FLAG_EXAMPLE::spinLock.unlock();
    }

    int test(void)
    { 
        std::cout << "================ Memory order test ================" << std::endl;
        
        std::vector<std::thread> threads;

        for (int i = 0; i < 5; i++) {
            threads.emplace_back(reader);
        }
        std::thread t1(writer);

        for (auto& t : threads) {
            t.join();
        }
        t1.join();

        return 0;
    }
}

int main(int argc, char* argv[])
{
    ATOMIC_EXAMPLE::test();

    ATOMIC_FLAG_EXAMPLE::test();

    ATOMIC_MEMORY_ORDER_EXAMPLE::test();

    return 0;
} 
