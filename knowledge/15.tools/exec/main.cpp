//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      exec执行代码
//
//  Author:
//     	@公众号 <嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <unistd.h>
#include <memory>
#include <array>
#include <vector>

std::string command = "curl 'https://devapi.qweather.com/v7/weather/now?location=101210301&key=***' --compressed";

int func_add(int a, int b) {
    return a+b;
}
int main(int argc, char *argv[])
{
    std::array<char, 256> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.data(), "r"), pclose);
    std::string result;
    if (!pipe) {
        std::cerr << "无法打开管道" << std::endl;
        return 1;
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    std::cout<<result<<std::endl;

    int a = ({
        int b = 3;
        func_add(b, 2);
    });
    std::cout<<a<<std::endl;
    return 0;
}


