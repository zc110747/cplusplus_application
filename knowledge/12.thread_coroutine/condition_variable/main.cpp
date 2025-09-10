
#include "thread_queue.hpp"
#include <iostream>
#include <thread>

Thread_Queue<int> ThreadQueue;

void SendTask(void)
{
    int i = 0;

    std::cout<<"Send Task Run\n";

    for(i=0; i<1000; i++)
    {
        ThreadQueue.QueueSend(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ReceiveTask(void)
{
    int i, rx;
    int last_rx = 0;

    std::cout<<"Receive Task Run\n";

    for(i=0; i<1000; i++)
    {
        if(ThreadQueue.QueueReceive(100, rx))
        {
            std::cout<<rx<<" | ";
            if (rx !=0 && rx%16 == 0) {
                std::cout<<std::endl;
            }
            if(rx != last_rx) {
                std::cout<<"queue lost!"<<std::endl;
                break;
            }
            last_rx++;
        }
    }
}

int main(int argc, char* argv[])
{
    std::thread t_send(SendTask);
    std::thread t_receive(ReceiveTask);

    t_send.join();
    t_receive.join();

    return 0;
}

