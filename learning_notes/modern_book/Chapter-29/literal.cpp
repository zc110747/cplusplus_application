//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      literal.cpp
//
//  Purpose:
//      第29章 字面量优化
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "literal.hpp"
#include "../includes.hpp"

using std::cout;

namespace LITERAL
{
    //将数字转成字符串的字面量
    template <char...c>
    std::string operator "" _w()
    {
        std::string str;

        //折叠表达式
        using unused = int[];
        unused{(str.push_back(c), 0)...};
        return str;
    }

    //字符串转换成string对象
    std::string operator "" _string(const char* str, size_t len)
    {
        return std::string(str, len);
    }

    void test_run()
    {
        FUNCTION_START() 

        //29.1 十六进制浮点字面量
        //std::hexfloat - 将浮点数格式化成十六进制字符串
        //std::defaultfloat - 将浮点数还原成十进制字符串
        cout<<"chapter 29.1\n  ";
        double double_array[] = {1.5, 35.35, 600.25};
        for(const auto &value:double_array)
        {
            cout<<std::hexfloat<<value<<"="<<std::defaultfloat<<value<<" | ";
        }
        
        //在源代码中使用十六进制浮点字面量
        #if __MORE_THAN_CPP17__
        double float_array[]{0xb.8p-1, 0xf.ap+6, 0x9.61p+6};
        for(const auto &value:float_array)
        {
            cout<<std::hexfloat<<value<<"="<<std::defaultfloat<<value<<" | ";
        }
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //29.2 二进制整数字面量
        //二进制 0b或0B开头
        //八进制 0开头
        //十六进制 0x或0X开头
        cout<<"\nchapter 29.2\n  ";
        unsigned int binary_array[] = {0b11011101L, 063L, 25L, 0xf1L}; 
        for(const auto &value:binary_array)
        {
            cout<<value<<" | ";
        } 

        //29.3 单引号作为整数分隔符
        //单引号作为界定符，用于整数分隔符, 可任意间隔，不影响原数值.
        cout<<"\nchapter 29.3\n  ";
        #if __MORE_THAN_CPP14__
            unsigned int separation_array[] = {0b111'111L, 2'50'00L, 0x23'1f1L}; 
            for(const auto &value:separation_array)
            {
                cout<<value<<" | ";
            }
        #else
            __LOWRE_THAN_CPP14_RUN;
        #endif


        //29.4 原生字符串字面量
        //C++可使用R"cpp(字符串)cpp支持原生字符串字面量
        //外部添加cpp可以支持内部嵌套
        //另外支持u8-char8_t, u-char16_t, U-char32_t, L-wchar-t
        //格式用于表示字面量存储格式
        cout<<"\nchapter 29.4\n  ";
        char hello_HTML[] = R"cpp(<!DOCTYPE HTML
            <html lang="en">
            <head>
                <title>Hello World!</title>
            </head>
            <body>
                "(Hello World!)"
            </body>
            </html>
        )cpp";
        cout<<hello_HTML;

        #if __MORE_THAN_CPP20__
        char8_t u8v[] = u8R"(hello world!)";
        #endif
        char16_t u16v[] = uR"(hello world!)";
        char32_t u32v[] = UR"(hello world!)";

        //29.5 用户自定义字面量
        //可通过自定义后缀，将整数、浮点数、字符及字符串转成特定对象
        cout<<"\nchapter 29.5\n  ";
        auto x = 12.3_w;
        auto y = "123"_string;
        cout<<x<<" | "<<y<<" | ";

        FUNCTION_END() 
    }
}