//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      lambda.cpp
//
//  Purpose:
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "lambda.hpp"
#include "../includes.hpp"

using namespace std;

namespace LAMBDA
{
    void test_study()
    {
        FUNCTION_START()

        //7.1 lambda表达式语法
        //[captures] (params) specifiers exception -> ret { body }
        //captures - 捕获列表，可以捕获当前函数作用域的零个或多个变量(值捕获和引用捕获)
        //params - 可选参数列表
        //specifiers - 可选限定符 C++可以用mutable, 允许我们值函数体内改变值捕获的变量
        //exception - 可选异常说明符，指明是否会抛弃异常
        //ret - 可选返回值类型，如果没有返回值(void), 可以忽略包括->在内的整个部分
        //body - lambda表达式的函数体
        cout<<"\nchapter7.1\n  ";
        int a_1 = 5;
        auto func0_1 = [a_1](int x) -> int
        {
            return a_1+x;
        };
        cout<<func0_1(5)<<" | ";
        auto func1_1 = [a_1]() mutable -> int
        {
            a_1 = 2;
            return a_1;
        };
        cout<<func1_1()<<" | ";
        auto func2_1 = []() noexcept-> int
        {
            return 5;
        };
        cout<<func2_1()<<" | ";

        //7.2 捕获列表
        FUNCTION_END()
    }
}

