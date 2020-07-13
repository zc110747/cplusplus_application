/**********************************************************
 * 说明: 
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
    for(int i=0; i<10; i++)
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
    for(int i=0; i<10; i++)
    {   
        std::cout<<"task 1 val:"<<val--<<std::endl;
    }
    sys_mutex.unlock();

    for(int i=0; i<2; i++)
    {
        std::cout<<"thread task 1"<<std::endl;
    }
} 

int main(int argc, char *argv[])
{
    val = 100;

    std::thread thread0(thread_task0, "hello world!", 5);
    std::thread thread1(thread_task1);

    thread0.join();     //堵塞等待thread0完成
    thread1.detach();   //主线程继续执行，直到主线程回收时，才结束子线程

    return 0;
}


