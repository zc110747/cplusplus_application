//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by Person Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include "thread_pool.hpp"

static std::mutex iomutex;

void schedular_loop_task(int id) {

    {
        std::lock_guard<std::mutex> lock(iomutex);
        std::cout << "Task " << id << " is running on thread " << std::this_thread::get_id() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lock(iomutex);
        std::cout << "Task " << id << " is finished." << std::endl;
    }
}

int main(int argc, char* argv[]) 
{
    THREAD_POOL::thread_pool pool;

    pool.init(4);

    for (int i = 0; i < 10; ++i) {
        pool.enqueue(schedular_loop_task, i);
    }

    // 主线程继续执行其他任务
    std::cout << "Main thread is doing other work..." << std::endl;

    return 0;
}
