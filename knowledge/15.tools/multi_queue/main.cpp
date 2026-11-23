
#include "event.hpp"
#include "thread_queue.hpp"
#include <cstring>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

static EVENT::thread_queue<EventBufMessage> high_priority_queue;
static EVENT::thread_queue<EventBufMessage> low_priority_queue;

int main()
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
        size_t index = 0;
        char buffer[24];

        while (true) {
            EventBufMessage message(1);        
            int len = std::snprintf(buffer, sizeof(buffer), "high thread %zu", index);
            if (len > 0 && static_cast<size_t>(len) < sizeof(message.get_data().buffer)) {
                std::memcpy(message.get_data().buffer, buffer, static_cast<size_t>(len) + 1);
            }
            high_priority_queue.send(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            index++;
        }
    });

    std::thread tx_thread_low([&]() {
        size_t index = 0;
        char buffer[24];

        while (true) {
            EventBufMessage message(2);
            int len = std::snprintf(buffer, sizeof(buffer), "low thread %zu", index);
            if (len > 0 && static_cast<size_t>(len) < sizeof(message.get_data().buffer)) {
                std::memcpy(message.get_data().buffer, buffer, static_cast<size_t>(len) + 1);
            }

            low_priority_queue.send(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            index++;
        }
    });

    //持续观察双队列的优先级调度, Ctrl+C 结束
    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}