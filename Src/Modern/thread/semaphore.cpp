
#include "semaphore.hpp"
#include "timer.hpp"
#include <chrono>    // std::chrono::seconds
#include <iostream>
#include <thread>

Semaphore sem(0);
Timer timer;

void thread_wait(void)
{
    int i;

    for(i=0; i<10; i++)
    {
        sem.wait();
        std::cout<<i<<" ";
    }
    timer.stop();
    std::cout<<"\n";
}

void semaphore_process(void)
{   
    std::thread threadW(thread_wait);
    timer.start(1000, [](){
        sem.signal();
    });

    threadW.join();
    
    std::cout<<"thread finished"<<std::endl;
}

