
#include "event.hpp"
#include "thread_queue.hpp"
#include <cstring>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

static EVENT::Thread_Queue<EventBufMessage> high_priority_queue;
static EVENT::Thread_Queue<EventBufMessage> low_priority_queue;

int main(int argc, char *argv[])
{
    std::thread print_thread([&]() {
        while (true) {
            EventBufMessage message;
            if (high_priority_queue.receive(message, 1)) {
                std::cout<<message.get_data().get_buffer()<<std::endl;
                continue;
            }

            if (low_priority_queue.receive(message, 1)) {
                std::cout<<message.get_data().get_buffer()<<std::endl;
            }
        }
    });

    std::thread tx_thread_high([&]() {
        size_t index = 0, size;
        char buffer[24];

        while (true) {
            EventBufMessage message(1);        
            size = sprintf(buffer, "high thread %ld", index);
            memcpy(message.get_data().buffer, buffer, size+1);
            high_priority_queue.send(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            index++;
        }
    });

    std::thread tx_thread_low([&]() {
        size_t index = 0, size;
        char buffer[24];

        while (true) {
            EventBufMessage message(2);
            size = sprintf(buffer, "low thread %ld", index);
            memcpy(message.get_data().buffer, buffer, size+1);

            low_priority_queue.send(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            index++;
        }
    });

    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}