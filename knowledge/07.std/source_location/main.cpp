//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      C++20 引入的标准库特性，定义在 <source_location> 头文件中，
//      用于在程序运行时获取源代码的位置信息，如文件名、行号、列号和函数名等。这在调试、日志记录和错误报告等场景中非常有用。
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>

#if __cplusplus >= 202002L
#include <source_location>

void log(const std::string& message, const std::source_location& location = std::source_location::current()) {
    std::cout << "File: " << location.file_name() << std::endl;
    std::cout << "Line: " << location.line() << std::endl;
    std::cout << "Function: " << location.function_name() << std::endl;
    std::cout << "Message: " << message << std::endl;
}
#else
void log(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
}
#endif

int main() {
    log("This is a test message");
    return 0;
}
