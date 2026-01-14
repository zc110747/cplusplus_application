//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <semaphore>
#include <mutex>
#include <queue>
#include <thread>
#include <chrono>

namespace COUNTING_SEMAPHORE
{
    std::counting_semaphore<1> sem(0);

    void reader(void)
    {
        sem.acquire();
        std::cout << "reader semaphore" << std::endl;
    }

    void writer(void)
    {
        std::cout << "writer semaphore" << std::endl;
        sem.release();
    }

    int test(void)
    {
        std::cout << "============= COUNTING SEMAPHORE ==============" << std::endl;
        std::thread t1(reader);
        std::thread t2(writer);
        
        t1.join();
        t2.join();
        return 0;
    }
}

namespace BINARY_SEMAPHORE
{
    std::binary_semaphore sem(0);

    void reader(void)
    {
        sem.acquire();
        std::cout << "reader semaphore" << std::endl;
    }

    void writer(void)
    {
        std::cout << "writer semaphore" << std::endl;
        sem.release();
    }

    int test(void)
    {
        std::cout << "============= BINARY SEMAPHORE ==============" << std::endl;
        std::thread t1(reader);
        std::thread t2(writer);
        
        t1.join();
        t2.join();
        return 0;
    }
}

namespace EVENT
{
    template<typename T, int sn = 10000>
    class thread_queue
    {
    public:
        void send(T &Object)
        {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                queue_.push(Object);
            }

            semaphore_.release();
        }

        bool receive(T& Object, uint32_t timeout = 0xffffffff)
        {
            int ret = false;
            if (semaphore_.try_acquire_for(std::chrono::milliseconds(timeout))) {
                std::unique_lock<std::mutex> lock(mutex_);
                if (queue_.size() != 0) {
                    Object = queue_.front();
                    queue_.pop();
                    ret = true;
                }
            }
            return ret;
        }
    private:
        /// @brief queue_
        /// - the queue to store the object.
        std::queue<T> queue_;

        /// @brief mutex_
        /// - the mutex to protect the queue.
        std::mutex  mutex_;

        /// @brief semaphore_
        /// - the semaphore to control the queue.
        std::counting_semaphore<sn> semaphore_{0};
    };

    thread_queue<int, 1000> queue;

    void sender(void)
    {
        for (int i = 0; i < 1000; ++i) {
            queue.send(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void receiver(void)
    {
        int data = 0;

        for (int i = 0; i < 1000; ++i) {
            if (queue.receive(data)) {
                std::cout << std::setw(2) << std::setfill('0') << data <<" | ";
                if (data != 0 && data%16 == 15) {
                    std::cout<<std::endl;
                }
            } else {
                std::cout << "timeout" << std::endl;
            }
        }
    }

    int test(void)
    {
        std::cout << "============= EVENT SEMAPHORE ==============" << std::endl;
        std::thread t1(sender);
        std::thread t2(receiver);
        
        t1.join();
        t2.join();
        return 0;
    }
}
int main(int argc, const char* argv[])
{
    COUNTING_SEMAPHORE::test();

    BINARY_SEMAPHORE::test();

    EVENT::test();
    return 0;
}