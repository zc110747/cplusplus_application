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
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

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

            if (ret && wakeups>0) {
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

        bool receive(uint32_t timeout, T& Object)
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

    for (i=0; i<1000; i++) {
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

    for(i=0; i<1000; i++) {
        if(ThreadQueue.receive(100, rx)) {
            std::cout<<rx<<" | ";
            if (rx !=0 && rx%16 == 0) {
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

    return 0;
}
