//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      union.cpp
//
//  Purpose:
//
//  Author:
//     	@公众号 <嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "union.hpp"
#include "../includes.hpp"
#include <new>

using namespace std;

namespace UNION
{
    //11.1 联合类型在C++中的局限性
    union U1
    {
        int x;
        float y;
        //string str1; //string为非平凡类型，不支持
    };

    //11.2 使用非受限联合体
    union U2
    {
        U2(){}
        ~U2(){}
        int x;
        float y;
        string str;
        vector<int> vec;
    };    

    union U3
    {
        static int x;
    };
    int U3::x = 5;

    void test_run()
    {
        FUNCTION_START()

        //11.1 联合类型在C++中的局限性
        //联合类型的成员变量不能是非平凡类型，不能有自定义构造函数
        cout<<"\nchapter11.1\n  ";
        U1 u;
        u.x = 5;
        cout<<u.x<<" | "<<u.y<<" | ";

        u.y = 5.0;
        cout<<u.x<<" | "<<u.y<<" | ";
        
        //11.2 使用非受限联合体
        cout<<"\nchapter11.1\n  ";
        U2 u2;
        new(&u2.str) std::string("hello");
        std::cout<<u2.str<<" | ";
        u2.str.~basic_string();

        new(&u2.vec) std::vector<int>({1, 2, 3});
        for(const auto& val:u2.vec)
        {
            cout<<val<<" | ";
        }
        u2.vec.~vector();

        cout<<U3::x<<" | ";
        FUNCTION_END()
    }
}
