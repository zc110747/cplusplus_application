
#include "asio_server.hpp"
#include <thread>
#include <chrono>
#include <stdio.h>

AsioServer server;

void server_thread()
{
    server.init("192.168.204.1", "15061", [](char* ptr, int length){
        printf("rx:%s", ptr);
    });
    server.run();
}

int main()
{
    auto server = std::thread(server_thread);
    server.detach();

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}