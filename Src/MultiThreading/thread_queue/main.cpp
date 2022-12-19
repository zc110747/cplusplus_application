
#include "thread_queue.hpp"
#include <iostream>
#include <thread>

Thread_Queue<int> ThreadQueue;

void SendTask(void)
{
    int i = 0;

    std::cout<<"Send Task Run"<<" | ";

    for(i=0; i<1000; i++)
    {
        ThreadQueue.QueueSend(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void ReceiveTask(void)
{
    int i, rx;

    std::cout<<"Receive Task Run"<<" | ";
    for(i=0; i<1000; i++)
    {
        if(ThreadQueue.QueueReceive(100, rx))
        {
            std::cout<<rx<<" | ";
        }
    }
}

int main(int argc, char* argv[])
{
    std::thread t_send(SendTask);
    std::thread t_receive(ReceiveTask);

    t_send.detach();
    t_receive.detach();
    std::this_thread::sleep_for(std::chrono::seconds(20));

    return 0;
}

