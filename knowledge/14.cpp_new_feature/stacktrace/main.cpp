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
#include <iostream>
#include <stacktrace>

void print_stacktrace()
{
    auto trace = std::stacktrace::current();

    for (const auto& entry : trace)
    {
        std::cout
            << "Description: "
            << entry.description()
            << '\n';

        std::cout
            << "Source file: "
            << entry.source_file()
            << '\n';

        std::cout
            << "Source line: "
            << entry.source_line()
            << "\n\n";
    }
}

int main()
{
    print_stacktrace();
    return 0;
}