//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::locale声明
//      2. std::locale方法
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <locale>

int main(int argc, char* argv[]) 
{
    // 设置全局的默认 locale 为用户的默认 locale
    std::locale::global(std::locale(""));

    // 默认locale输出数字
    std::cout.imbue(std::locale());
    std::cout << "Number in default locale: " << 1234.56 << std::endl;

    // 使用特定的 locale 输出数字
    try
    {
        std::locale german("C.utf8");
        std::cout.imbue(german);
        std::cout << "Number in German locale: " << 1234.56 << std::endl;

        std::locale loc1("C.utf8");
        std::locale loc2 = std::locale::global( loc1 );
        std::cout << "The name of the previous locale is: " << loc2.name( )
            << "." << std::endl;
        std::cout << "The name of the current locale is: " << loc1.name( )
            << "." << std::endl;
        std::cout << "number in C.utf8: " << 1234.56 << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::locale loc3("en_US.utf8");
    std::cout.imbue(loc3);
    std::cout << "Number in US locale: " << 1234.56 << std::endl;

    return 0;
}