/******************************************************************
 * 第二十五章 线程局部存储 
********************************************************************/
#include "thread_process.hpp"
#include <thread>
#include <mutex>
#include <iostream>

thread_local int inc = 1;
std::mutex sys_mutex;

class B
{
public:
    thread_local static int inc;
};

thread_local int B::inc = 10;

void thread_process(void)
{
    {
        auto func0 = [](const std::string& thread_name){
            for(auto i=0; i<3; i++)
            {
                inc++; //局部变量
                std::lock_guard<std::mutex> lock(sys_mutex);
                std::cout<<thread_name<<":"<<inc<<std::endl;
            }
        };
        std::thread t1(func0, "t1");
        std::thread t2(func0, "t2");
        t1.join();
        t2.join();
    }

    {
        class A
        {
        public:
            int count = 0;
            int get_value(){
                return count;
            }
            void inc() {
                count++;
            }
        };
        auto func0 = [](const std::string& thread_name){
            for(auto i=0; i<3; i++)
            {
                thread_local A a;
                std::lock_guard<std::mutex> lock(sys_mutex);
                a.inc();
                std::cout<<thread_name<<":"<<a.get_value()<<std::endl;
            }
        };
        std::thread t3(func0, "t3");
        std::thread t4(func0, "t4");
        t3.join();
        t4.join();
    }

    {
        auto func0 = [](const std::string& thread_name){
            B b;
            for(auto i=0; i<3; i++)
            {
                b.inc--;
                std::lock_guard<std::mutex> lock(sys_mutex);
                std::cout<<thread_name<<":"<<b.inc<<std::endl;
            }
        };
        std::thread t5(func0, "t5");
        std::thread t6(func0, "t6");
        t5.join();
        t6.join();
    }
}