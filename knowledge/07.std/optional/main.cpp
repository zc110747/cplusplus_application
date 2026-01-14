//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::optional声明
//      2. std::optional方法
//      3. std::nullopt
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <optional>

std::optional<int> find_value(int value)
{
    if (value == 46) {
        return value;
    } else {
        return std::nullopt;
    }
}

int main(int argc, char *argv[]) 
{
    std::optional<int> opt = find_value(46);
    
    // has_value、value
    // operator*、operator->
    if (opt.has_value()) {
        std::cout << "opt has value: " << opt.value() << std::endl;
        std::cout << "opt:" << *opt << std::endl;
        std::cout << "opt->:" << *(opt.operator->()) << std::endl;
    } else {
        std::cout << "opt has no value" << std::endl;
    }

    // value_or
    opt = find_value(42);
    std::cout << "opt has value: " << opt.value_or(-1) << std::endl;

    std::optional<int> opt_1{42};
    
    // and_then
    std::cout << "opt_1:" << opt_1.and_then([](int value) -> std::optional<int> { 
        return value * 2; 
    }).value_or(0) << std::endl;

    // transform
    std::cout << "opt_1:" << opt_1.transform([](int value) -> int { 
        return value * 2; 
    }).value_or(0) << std::endl;

    // or_else
    std::cout << "opt_1:" << opt_1.or_else([]() -> std::optional<int> {
        std::cout << "Valueless: ";
        return std::optional<int>{0};
    }).value_or(0) << std::endl;

    // swap
    std::optional<int> opt_2{std::nullopt};
    opt_1.swap(opt_2);
    std::cout << "opt_1:" << opt_1.value_or(0) << std::endl;
    std::cout << "opt_2:" << opt_2.value_or(0) << std::endl;

    // reset
    opt_2.reset();
    std::cout << "opt_2:" << opt_2.value_or(0) << std::endl;

    // emplace
    opt_2.emplace(51);
    std::cout << "opt_2:" << opt_2.value_or(0) << std::endl;

    return 0;
}
