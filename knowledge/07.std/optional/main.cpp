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
//      @zc
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
    std::optional<int> opt_1;
    std::optional<int> opt_2 = 42;

    // has_value
    if (opt_1.has_value()) {
        std::cout << "opt_1 has value: " << opt_1.value() << std::endl;
    } else {
        std::cout << "opt_1 has no value" << std::endl;
    }

    //value
    if (opt_2.has_value()) {
        std::cout << "opt_2 has value: " << opt_2.value() << std::endl;
    } else {
        std::cout << "opt_2 has no value" << std::endl;
    }

    //vlaue_or
    std::cout << "opt_1:" << opt_1.value_or(0) << std::endl;
    std::cout << "opt_2:" << opt_2.value_or(0) << std::endl;

    opt_1.swap(opt_2);
    std::cout << "opt_1:" <<  opt_1.value_or(0) << std::endl;

    //reset
    opt_2.reset();

    //emplace
    opt_2.emplace(51);
    std::cout << "opt_2:" << opt_2.value_or(0) << std::endl;

    return 0;
}