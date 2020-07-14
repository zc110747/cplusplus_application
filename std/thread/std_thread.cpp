/**********************************************************
 * 说明: thread 线程相关的接口
 * joinable         检查线程是否可执行
 * join             堵塞等待线程终止的函数，回收线程资源
 * detach           分离子线程，与主进程分开
 * swap             交换线程资源，被交换对象内部资源转移
 * native_handle    在主线程中获取设备的id
 * std::this_thread::get_id 
 * std::this_thread::sleep_for 
************************************************************/
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>    // std::chrono::seconds
#include <mutex>  

static volatile int val;
std::mutex sys_mutex;

void thread_task0(std::string str, int n)
{
    std::cout<<n<<std::endl;
    sys_mutex.lock();
    for(int i=0; i<3; i++)
    {   
        std::cout<<"task 0 val:"<<val++<<std::endl;
    }
    sys_mutex.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(n));
    std::cout<<str<<"id:0x"<<std::hex<<std::this_thread::get_id()<<std::endl;
} 

void thread_task1(void)
{
    sys_mutex.lock();
    for(int i=0; i<3; i++)
    {   
        std::cout<<"task 1 val:"<<val--<<std::endl;
    }
    sys_mutex.unlock();

    std::cout<<"thread task 1"<<std::endl;
} 

int main(int argc, char *argv[])
{
    val = 100;

    std::thread thread0(thread_task0, "hello world!", 1);
    std::thread thread1(thread_task1);
    std::thread thread2;

    thread2.swap(thread0);
    std::cout<<"thread0 joinable:"<<thread0.joinable()<<std::endl;
    if(thread2.joinable())
    {
        std::cout<<"thread2 can join"<<std::endl;
        auto tn = thread2.native_handle();  //获取thread的id，可进行其它处理
        std::cout<<"native_handle:0x"<<std::hex<<tn<<std::endl;
        thread2.join();                     //堵塞等待thread0完成
    }
    thread1.detach();   //主线程继续执行，直到主线程回收时，才结束子线程

    return 0;
}


