//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by Person Inc.  
//  All Rights Reserved
//
//  Name:
//      thread_pool.hpp
//
//  Purpose:
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <iostream>

namespace THREAD_POOL 
{
class thread_pool {
public:
    /// @brief constructor
    thread_pool() : stop_(false) {
    }

    /// @brief destructor
    ~thread_pool() {
        {
            std::unique_lock<std::mutex> lock(qmutex_);
            stop_ = true;
        }

        condition_.notify_all();
        for (std::thread &worker : workers_) {
            worker.join();
        }
    }

    /// @brief delete copy constructor and operator== 
    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    /// @brief init
    /// - init thread pool
    /// @param threadCount thread count in thread pool, default is std::thread::hardware_concurrency()
    /// @return NULL
    void init(size_t threadCount = std::thread::hardware_concurrency()) { 
        thread_count_ = threadCount;
        if (thread_count_ == 0) {
            thread_count_ = 4;
        }
        for (size_t i = 0; i < thread_count_; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->qmutex_);
                        
                        this->condition_.wait(lock, [this] { return this->stop_ || !this->tasks_.empty(); });
                        if (this->stop_ && this->tasks_.empty()) {
                            return;
                        }

                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    
                    task();
                }
            });
        }
    }

    /// @brief enqueue
    /// - enqueue a task to thread pool
    /// @param f task function
    /// @param args arguments of task function
    /// @return return value of function
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(qmutex_);

            if (stop_) {
                throw std::runtime_error("enqueue on stopped thread_pool");
            }

            // 将任务添加到任务队列中
            tasks_.emplace([task]() { (*task)(); });
        }

        condition_.notify_one();
        return res;
    }

private:
    /// @brief thread_count_
    /// thread count in thread pool
    uint32_t thread_count_{0};

    /// @brief workers_
    /// worker threads vector, store all worker threads
    std::vector<std::thread> workers_;

    /// @brief tasks
    /// task queue, store all tasks
    std::queue<std::function<void()>> tasks_;
    
    /// @brief qmutex_
    /// mutex for tasks queue
    std::mutex qmutex_;

    /// @brief condition_
    /// condition variable for tasks queue
    std::condition_variable condition_;

    /// @brief stop_
    /// stop flag, if true, thread pool will stop
    std::atomic<bool> stop_;
};
}
#endif
