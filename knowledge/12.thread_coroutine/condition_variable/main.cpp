//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::condition_variable
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <iomanip>
#include <chrono>
#include <condition_variable>

namespace CV_PROCESS
{
    std::condition_variable cv;
    std::mutex mt;
    std::atomic<bool> ready(false);

    void work_thread(void)
    {
        std::unique_lock<std::mutex> lock(mt);
        
        cv.wait(lock, [] { return ready.load(); });

        std::cout << "work_thread run" << std::endl;

        lock.unlock();
        ready.store(true);
        cv.notify_all();
    }

    int test(void)
    {
        std::thread t1(work_thread);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ready = true;

        std::cout << "ready is: " << ready << std::endl;

        cv.notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        {
            std::unique_lock<std::mutex> lock(mt);
            bool ret = cv.wait_for(lock, std::chrono::milliseconds(1000), 
                        []()->bool { return ready.load(); });
            if (ret) {
                std::cout << "work_thread ready" << std::endl;
            } else {
                std::cout << "work_thread timeout" << std::endl;
            }
        }
        t1.join();
        
        std::cout << "work_thread join" << std::endl;
        return 0;
    }
}

namespace USE_SIGNAL
{
    class semaphore
    {
    public:
        semaphore(int count = 0):wakeups(count) {}
        virtual ~semaphore() {}
        
        void signal(void) {
            std::lock_guard<std::mutex> lock(mt);
            ++wakeups;
            cv.notify_one();
        }
        
        bool wait(uint32_t timeout = 0xffffffff) {
            bool ret = false;
            std::unique_lock<std::mutex> lock(mt);
            ret = cv.wait_for(lock, 
                std::chrono::milliseconds(timeout),
                [this]() -> bool {
                    return wakeups > 0;
                });

            if (ret && wakeups > 0) {
                --wakeups;
            }
            return ret;
        }

    private:
        std::mutex mt;
        std::condition_variable cv;
        int wakeups;
    };

    template<typename T>
    class thread_queue
    {
    private:
        std::queue<T> Queue_;
        std::mutex  mut;
        semaphore semaphore_;
    public:
        size_t size() {
            return Queue_.size();
        }

        void send(T& Object)
        {
            {
                std::unique_lock<std::mutex> lock(mut);
                Queue_.push(Object);
            }
            //std::cout<<Queue_.size()<<" | ";
            semaphore_.signal();
        }

        bool receive(T& Object, uint32_t timeout = 0xffffffff)
        {
            int ret = false;

            if (semaphore_.wait(timeout)) {
                std::unique_lock<std::mutex> lock(mut);
                if (Queue_.size() != 0){
                    Object = Queue_.front();
                    Queue_.pop();
                    ret = true;
                }
            } else {
                std::cout<<"rx failed"<<" | ";
            }

            return ret;
        }
    };
}

USE_SIGNAL::thread_queue<int> ThreadQueue;
USE_SIGNAL::semaphore ThreadSemaphore;

void thread_send_task(void)
{
    int i = 0;

    std::cout << "Send Task Run" << std::endl;

    for (i = 0; i < 100; i++) {
        ThreadQueue.send(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    ThreadSemaphore.wait();

    std::cout << "Send Task End" << std::endl;
}

void thread_receive_task(void)
{
    int i, rx;
    int last_rx = 0;

    std::cout << "Receive Task Run" << std::endl;

    for (i = 0; i < 100; i++) {
        if(ThreadQueue.receive(rx, 100)) {
            std::cout << std::setw(2) << std::setfill('0') << rx <<" | ";
            if (rx != 0 && rx%16 == 15) {
                std::cout<<std::endl;
            }

            if(rx != last_rx) {
                std::cout<<"queue lost!"<<std::endl;
                break;
            }
            last_rx++;
        }
    }

    std::cout << "\nReceive Task End" << std::endl;
    ThreadSemaphore.signal();
}

int main(int argc, char* argv[])
{
    std::thread t1(thread_send_task);
    std::thread t2(thread_receive_task);

    t1.join();
    t2.join();

    CV_PROCESS::test();
    return 0;
}
