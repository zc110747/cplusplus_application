//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. coroutine应用
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <chrono>
#include <future>
#include <thread>
#include <coroutine>

std::future<int> foo()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    co_return 5;
}

std::future<std::future<int>> bar()
{
    auto n = co_await std::async(std::launch::async, foo);
    co_return n;
}

int main(int argc, char* argv[])
{
    auto i = bar();
    i.wait();

    std::cout << i.get().get() << std::endl;
    return 0;
}