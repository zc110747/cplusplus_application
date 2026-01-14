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
//      @公众号 <嵌入式技术总结>
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

namespace ANY_DECLARATION
{
    int test(void)
    {
        std::cout << "===================== ANY_DECLARATION =======================" << std::endl;

        std::any a = 42;
        std::cout << "a: " << std::any_cast<int>(a) << std::endl;            // any(int)

        a = "hello world";
        std::cout << "a: " << std::any_cast<const char*>(a) << std::endl;    // any(const char *)

        a = std::string("hello world");
        std::cout << "a: " << std::any_cast<std::string>(a) << std::endl;    // any(string)

        a = 3.14;
        std::cout << "a: " << std::any_cast<double>(a) << std::endl;         // any(double)

        a = std::vector<int>{1, 2, 3};
        const auto&vec = std::any_cast<std::vector<int>>(a);        // any(vector<int>)
        for (const auto& i : vec) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        a = std::complex<double>(1.0, 2.0);
        std::cout << "a: " << std::any_cast<std::complex<double>>(a) << std::endl; // any(complex<double>)

        // make_any构造any对象
        std::any a1 = std::make_any<int>(10);
        std::cout << "a1: " << std::any_cast<int>(a1) << std::endl;

        a1 = std::make_any<std::string>("hello world");
        std::cout << "a1: " << std::any_cast<std::string>(a1) << std::endl;
        return 0;
    }
}

namespace ANY_METHOD
{
    int test(void)
    {
        std::cout << "===================== ANY_METHOD =======================" << std::endl;

        std::any a1 = 1;
        std::any a2 = "hello world";
        std::any a3 = std::string("hello world");
        std::any a4{std::in_place_type<int>, 1};

        std::cout << std::boolalpha;
        std::cout << "has value: " << a1.has_value() << std::endl;
        std::cout << "type: " << a1.type().name() << std::endl;

        try
        {
            std::cout << "a1: " << std::any_cast<int>(a1) << std::endl;
            std::cout << "a2: " << std::any_cast<const char*>(a2) << std::endl;
            std::cout << "a3: " << std::any_cast<std::string>(a3) << std::endl;
            std::cout << "a4: " << std::any_cast<int>(a4) << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        // reset
        a1.reset();
        std::cout << "has value: " << a1.has_value() << std::endl;

        // operator=
        a1 = 1.35;
        std::cout << "a1: " << std::any_cast<double>(a1) << std::endl;
      
        // copy constructor
        std::any a5(a1);
        std::cout << "a5: " << std::any_cast<double>(a5) << std::endl;
    
        // move constructor
        std::any a6(std::move(a1));
        std::cout << "a6: " << std::any_cast<double>(a6) << std::endl;
        std::cout << "a1 has value: " << a1.has_value() << std::endl;

        // move assignment operator
        std::any a7 = std::move(a6);
        std::cout << "a7: " << std::any_cast<double>(a7) << std::endl;
        std::cout << "a6 has value: " << a6.has_value() << std::endl;

        // emplace
        a7.emplace<std::string>("hello world");
        std::cout << "a7: " << std::any_cast<std::string>(a7) << std::endl;

        // swap
        std::any a8 = 1;
        a8.swap(a7);

        std::cout << "a8: " << std::any_cast<std::string>(a8) << std::endl;
        std::cout << "a7: " << std::any_cast<int>(a7) << std::endl;

        return 0;
    } 
}

int main(int argc, char* argv[])
{
    ANY_DECLARATION::test();

    ANY_METHOD::test();

    return 0;
} 
