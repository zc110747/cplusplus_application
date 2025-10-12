//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::regex
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <regex>

int main(int argc, char* argv[])
{
    // 创建一个正则表达式对象，用于匹配一个或多个数字
    std::regex pattern("\\d+");

    // 要匹配的字符串
    std::string text = "The answer is 42 and the next number is 123";

    // 检查字符串中是否存在匹配的模式
    if (std::regex_search(text, pattern)) {
        std::cout << "Match found!" << std::endl;
    }

    // 提取匹配的子串
    std::smatch matches;
    if (std::regex_search(text, matches, pattern)) {
        std::cout << "Match found: " << matches[0] << std::endl;
    }

    // 替换字符串中的匹配项
    std::string new_text = std::regex_replace(text, pattern, "forty-two");
    std::cout << "Replaced text: " << new_text << std::endl;

    // 使用正则表达式迭代器查找所有匹配项
    std::sregex_iterator it(text.begin(), text.end(), pattern);
    std::sregex_iterator end;
    while (it != end) {
        std::cout << "Match: " << it->str() << std::endl;
        ++it;
    }

    return 0;
} 