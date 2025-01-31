/***********************************************************************************
 * thread_local
 * thread_local 是一个存储类型说明符，用于声明一个变量，使其在每个线程中有独立的实例。
 * 这样，每个线程对该变量的修改都只会影响自己的副本，而不会影响其他线程的值。这对于多线程编程中
 * 避免数据竞争是非常有用的。
 * thread_local 变量可以在每个线程第一次访问时初始化，而不是在程序启动时
***********************************************************************************/
#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>

using std::cout;
using std::endl;

std::mutex sys_mutex;
thread_local static int val = 1;

void thread_task_0(void)
{
    int i = 0;

    for(int i=0; i<5; i++)
    {
        val++;

        {
            std::lock_guard<std::mutex> lock(sys_mutex);
            cout<<"thread_task_0: "<<val<<endl;
        }
    }

}

void foo(int n)
{
    thread_local int v = 1; //函数只有第一次执行初始化，后续不执行(类似局部静态变量)

    v += n;
    {
        std::lock_guard<std::mutex> lock(sys_mutex);
        cout<<"foo: "<<v<<endl;
    }
}

void thread_task_1(void)
{
    for(int i=0; i<5; i++)
    {
        val+=2;

        {
            std::lock_guard<std::mutex> lock(sys_mutex);
            cout<<"thread_task_1: "<<val<<endl;
        }
    }

    for(int i=0; i<5; i++)
    {
        foo(1);
    }
}


int main(int argc, char* argv[])
{
    std::thread th1(thread_task_0);
    std::thread th2(thread_task_1);

    th1.join();
    th2.join();
    return 0;
} 
