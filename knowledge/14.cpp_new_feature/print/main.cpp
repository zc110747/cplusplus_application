//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. print
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <print>

int main()
{
    std::print("Hello World\n");

    std::print("{} + {} = {}\n", 1, 2, 1 + 2);

    std::print("{0:<10} {1:>10}\n", "Left", "Right");

    std::print("{0}:{1}:{0}\n", 1, 2);
    return 0;
}

