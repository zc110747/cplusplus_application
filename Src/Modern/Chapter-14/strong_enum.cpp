//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      strong_enum.cpp
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
#include "strong_enum.hpp"
#include "../includes.hpp"

using namespace std;

namespace STRONG_ENUM
{
    //14.1 枚举类型的弊端
    enum School{
        principal,
        teacher,
        student
    };
    enum Company{
        chairman,
        manage,
        employee,
    };
    enum E{
        e1 = 1,
        e2 = 2,
        e3 = 0xfffffff0
    };

    template<int a, int b>
    struct add{
        enum {
            result = a+b
        };
    };

    void test_study()
    {
        FUNCTION_START()

        //14.1 枚举类型的弊端
        //不同的枚举类型在比较时会隐式转换为整型
        cout<<"\nchapter14.1\n  ";
        School x1 = student;
        Company y1 = manage;
        auto b1 = (x1 == y1);  
        cout<<b1<<" | ";
        auto c1 = e1<e3; //不同平台到enum存储不一致，输出可能不一致
        cout<<c1<<" | ";
        cout<<add<3, 5>::result<<" | ";

        FUNCTION_END()
    }
}

