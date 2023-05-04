//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      thread_local.cpp
//
//  Purpose:
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "thread_local.hpp"
#include "../includes.hpp"

using namespace std;

namespace THREAD_LOCAL
{
    thread_local int inc = 1;
    std::mutex sys_mutex;

    class B
    {
    public:
        thread_local static int inc;
    };
    thread_local int B::inc = 10;

    void test_study()
    {
        FUNCTION_START()

        //25.1 操作系统和编译器对线程局部存储的支持
        cout<<"\nchapter25.1\n  ";
        auto func1 = [](const std::string& thread_name){
            for(auto i=0; i<3; i++)
            {
                inc++; //thread_local, 每个线程独立
                std::lock_guard<std::mutex> lock(sys_mutex);
                std::cout<<thread_name<<":"<<inc<<" | ";
            }
        };
        std::thread t1(func1, "t1");
        std::thread t2(func1, "t2");
        t1.join();
        t2.join();
        
        //25.2　thread_local说明符
        cout<<"\nchapter25.2\n  ";
        auto func2 = [](const std::string& thread_name){
            B b;
            for(auto i=0; i<3; i++)
            {
                b.inc--;
                std::lock_guard<std::mutex> lock(sys_mutex);
                std::cout<<thread_name<<":"<<b.inc<<" | ";
            }
        };
        std::thread t21(func2, "t5");
        std::thread t22(func2, "t6");
        t21.join();
        t22.join();
        FUNCTION_END()
    }
}
