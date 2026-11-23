//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      exec执行代码: 通过 popen 读取子进程输出
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//      popen/pclose 是 POSIX 接口, 需要 POSIX 环境(或 MinGW/MSYS2);
//      MSVC 下不可用.
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//命令只依赖最基础的回显, 不依赖外网与任何密钥
static const char *command = "echo exec pipe works";

int func_add(int a, int b) {
    return a+b;
}

int main()
{
    std::array<char, 256> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    std::string result;
    if (!pipe) {
        std::cerr << "无法打开管道" << std::endl;
        return 1;
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    std::cout<<result<<std::endl;

    //立即调用 lambda: 等价于原来 GCC 的语句表达式 ({ ... }), 但标准可移植
    int a = [b = 3]() {
        return func_add(b, 2);
    }();
    std::cout<<a<<std::endl;
    return 0;
}
