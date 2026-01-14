//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::shared_mutex
//、    2. std::shared_lock
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include <vector>

namespace SHARD_MUTEX
{
    class demo
    {
    public:
        demo() = default;
        ~demo() = default;

        void show_val(void)
        {
            // lock shared、unlock_shared
            smtx_.lock_shared();       // lock shared，允许读操作同时进行
            std::cout << "read val: " << val_ << std::endl;
            smtx_.unlock_shared();
        }

        void increment(void)
        {  
            // lock、unlock
            smtx_.lock();
            val_++;
            smtx_.unlock();
        }

        void reset(void)
        {   
            // try_lock
            if(smtx_.try_lock())
            {
                val_ = 0;
                smtx_.unlock();
            }
        }

        int get(void)
        {   
            // lock shared、unlock_shared
            smtx_.lock_shared();       // lock shared，允许读操作同时进行
            int val = val_;
            smtx_.unlock_shared();
            return val;
        }

    private:
        /// @brief val_
        /// @brief 共享变量val_，多个线程可以同时读取
        int val_{0};

        /// @brief smtx_
        /// @brief 共享互斥锁smtx_，用于保护val_
        std::shared_mutex smtx_;
    };

    int test(void) 
    {
        std::cout << "================= SHARD_MUTEX ===================" << std::endl;
        std::vector<std::thread> threads;
        demo d1;

        for (int i = 0; i < 5; i++) {
            threads.push_back(std::thread(&demo::increment, &d1));
            threads.push_back(std::thread(&demo::show_val, &d1));
        }

        std::cout << "get: " << d1.get() << std::endl;
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "get: " << d1.get() << std::endl;
        d1.reset();
        std::cout << "reset val: " << d1.get() << std::endl;
        return 0;
    }
}

namespace SHARD_MUTEX_RAII
{ 
    class demo
    {
    public:
        demo() = default;
        ~demo() = default;

        void show_val(void)
        {
            std::shared_lock<std::shared_mutex> lck(smtx_); //shared_lock，允许读操作同时进行
            std::cout << "read val: " << val_ << std::endl;
        }

        void increment(void)
        {  
            std::lock_guard<std::shared_mutex> lck(smtx_); // lock_guard，自动lock、unlock
            val_++;
        }

        void reset(void)
        {   
            // try_lock
            if(smtx_.try_lock())
            {
                val_ = 0;
                smtx_.unlock();
            }
        }

        int get(void)
        {   
            std::shared_lock<std::shared_mutex> lck(smtx_);
            int val = val_;
            return val;
        }

    private:
        /// @brief val_
        /// @brief 共享变量val_，多个线程可以同时读取
        int val_{0};

        /// @brief smtx_
        /// @brief 共享互斥锁smtx_，用于保护val_
        std::shared_mutex smtx_;
    };

    int test(void) 
    {
        std::cout << "================= SHARD_MUTEX_RAII ===================" << std::endl;
        std::vector<std::thread> threads;
        demo d1;

        for (int i = 0; i < 5; i++) {
            threads.push_back(std::thread(&demo::increment, &d1));
            threads.push_back(std::thread(&demo::show_val, &d1));
        }

        std::cout << "get: " << d1.get() << std::endl;
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "get: " << d1.get() << std::endl;
        d1.reset();
        std::cout << "reset val: " << d1.get() << std::endl;
        return 0;
    }    
}

namespace SHARD_TIME_MUTEX
{
    class demo
    {
    public:
        demo() = default;
        ~demo() = default;

        void show_val(void)
        {
            if (smtx_.try_lock_shared_for(std::chrono::seconds(1)))
            {
                std::cout << "read val: " << val_ << std::endl;
                smtx_.unlock_shared();
            }
        }

        void increment(void)
        {  
            if (smtx_.try_lock_for(std::chrono::seconds(1)))
            {
                val_++;
                smtx_.unlock();
            }
        }

        void reset(void)
        {   
            // try_lock
            if(smtx_.try_lock())
            {
                val_ = 0;
                smtx_.unlock();
            }
        }

        int get(void)
        {   
            std::shared_lock<std::shared_timed_mutex> lck(smtx_, std::defer_lock);
            if (lck.try_lock_for(std::chrono::seconds(1)))
            {
                int val = val_;
                lck.unlock();
                return val;
            }
            return -1;
        }

    private:
        /// @brief val_
        /// @brief 共享变量val_，多个线程可以同时读取
        int val_{0};

        /// @brief smtx_
        /// @brief 共享互斥锁smtx_，用于保护val_
        std::shared_timed_mutex smtx_;
    };

    int test(void) 
    {
        std::cout << "================= SHARD_TIME_MUTEX ===================" << std::endl;
        std::vector<std::thread> threads;
        demo d1;

        for (int i = 0; i < 5; i++) {
            threads.push_back(std::thread(&demo::increment, &d1));
            threads.push_back(std::thread(&demo::show_val, &d1));
        }

        std::cout << "get: " << d1.get() << std::endl;
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "get: " << d1.get() << std::endl;
        d1.reset();
        std::cout << "reset val: " << d1.get() << std::endl;
        return 0;
    }
}
int main(int argc, const char *argv[])
{
    SHARD_MUTEX::test();

    SHARD_MUTEX_RAII::test();

    SHARD_TIME_MUTEX::test();

    return 0;
}
