//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::any声明
//      2. std::any方法
//      3. any_cast处理
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <any>
#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <complex>

int main(int argc, char* argv[])
{
    std::any data_0 = "a";
    std::any data_1 = 1;
    std::any data_2{std::in_place_type<std::string>, "c"};

    std::cout<< std::boolalpha;
    std::cout<< "data_0 has value: " << data_0.has_value() << std::endl;

    // type
    std::cout<< "data_0 type: " << data_0.type().name() << std::endl;

    // exception
    try
    {
        std::cout<< "data_0 value: " << std::any_cast<const char*>(data_0) << std::endl;
        std::cout<< "data_1 value: " << std::any_cast<int>(data_1) << std::endl;
        std::cout<< "data_2 value: " << std::any_cast<std::string>(data_2) << std::endl;
    }
    catch(const std::bad_any_cast& e)
    {
        std::cerr << e.what() << '\n';
    }

    // operator=
    data_0 = 1;
    std::cout<< "data_0 value: " << std::any_cast<int>(data_0) << std::endl;

    // reset
    data_0.reset();
    std::cout<< "data_0 has value: " << data_0.has_value() << std::endl;

    // copy constructor
    std::any data_3(data_1);
    std::cout<< "data_3 value: " << std::any_cast<int>(data_3) << std::endl;

    // move constructor
    std::any data_4(std::move(data_1));
    std::cout<< "data_4 value: " << std::any_cast<int>(data_4) << std::endl;

    // move assignment operator
    data_4 = std::move(data_3);
    std::cout<< "data_4 value: " << std::any_cast<int>(data_4) << std::endl;

    // any_cast vector
    std::any data_5{std::vector{1, 2, 3}};
    auto v = std::any_cast<std::vector<int>>(data_5);
    std::cout << "data_5 value: ";
    for (const auto &val : v) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // make_any
    std::any data_6 = std::make_any<int>(10);
    std::cout<< "data_6 value: " << std::any_cast<int>(data_6) << std::endl;

    auto data_7 = std::make_any<std::complex<double>>(0.1, 0.2);
    std::cout << "data_7 value: " << std::any_cast<std::complex<double>>(data_7) << std::endl;

    // emplace
    data_7.emplace<std::complex<double>>(0.2, 0.3);
    std::cout << "data_7 value: " << std::any_cast<std::complex<double>>(data_7) << std::endl;

    // swap
    std::any data_8 = 100;
    data_8.swap(data_7);
    std::cout << "data_7 value: " << std::any_cast<int>(data_7) << std::endl;
    std::cout << "data_8 value: " << std::any_cast<std::complex<double>>(data_8) << std::endl;

    return 0;
} 
