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
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <stop_token>
#include <string>
#include <thread>

using namespace std::chrono_literals;

class TaskQueue
{
public:
    void push(std::string task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.push(std::move(task));

            std::cout << "[Queue] Task added.\n";
        }

        condition_.notify_one();
    }

    void worker(std::stop_token token)
    {
        // This callback runs when a stop request is made.
        std::stop_callback callback(token, [] {
            std::cout << "[Callback] Stop request received.\n";
        });

        std::cout << "[Worker] Worker started.\n";

        while (true)
        {
            std::string task;

            {
                std::unique_lock<std::mutex> lock(mutex_);

                // Wait until a task is available or stop is requested.
                const bool ready = condition_.wait(
                    lock,
                    token,
                    [this] {
                        return !tasks_.empty();
                    });

                if (!ready && token.stop_requested())
                {
                    std::cout
                        << "[Worker] Stop requested and queue is empty.\n";
                    break;
                }

                if (tasks_.empty())
                {
                    continue;
                }

                task = std::move(tasks_.front());
                tasks_.pop();
            }

            // Process the task without holding the queue mutex.
            std::cout << "[Worker] Processing: " << task << '\n';
            std::this_thread::sleep_for(200ms);
            std::cout << "[Worker] Completed: " << task << '\n';
        }

        std::cout << "[Worker] Worker stopped.\n";
    }

private:
    std::mutex mutex_;
    std::condition_variable_any condition_;
    std::queue<std::string> tasks_;
};

int main()
{
    TaskQueue queue;

    std::cout << "[Main] Starting task processor.\n";

    std::jthread worker([&queue](std::stop_token token) {
        queue.worker(token);
    });

    queue.push("Task A");
    queue.push("Task B");
    queue.push("Task C");

    std::this_thread::sleep_for(350ms);

    std::cout << "[Main] Requesting shutdown.\n";
    worker.request_stop();

    std::cout << "[Main] Waiting for worker to exit.\n";

    // The jthread destructor joins the worker.
    return 0;
}
