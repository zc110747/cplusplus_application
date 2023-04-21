//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      x.cpp
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
#include "initialization.hpp"
#include "../includes.hpp"

using namespace std;

namespace INITIALIZATION
{
    class X
    {
    friend ostream& operator<<(ostream& os, const X& x);
    public:
        X() {}
        X(int a) : a_(a) {}
        X(double b) : b_(b) {}
        X(const std::string &c) : c_(c) {}
        

    private:
        int a_ = 0;
        double b_{0};
        std::string c_{"hello world"};
    };

    ostream& operator<<(ostream& os, const X& x)
    {
        os<<x.a_<<" | "<<x.b_<<" | "<<x.c_<<" | ";
        return os;
    }

    //8.2 位域的默认初始化
    #if __MORE_THAN_CPP20__
    struct S{
        int y:8 = 11;
        int z:4 = 7;
    };
    #endif

    void test_study()
    {
        FUNCTION_START()

        //8.1 使用默认初始化
        //初始化列表对数据成员的初始化总是优先于声明时默认初始化
        //1.不用使用括号()对非静态数据成员进行初始化
        //2.不要用auto来声明和初始化非静态数据成员
        cout<<"\nchapter8.1\n  ";
        X x1(1);
        X x2(1.5);
        X x3("hello");
        cout<<x1<<x2<<x3<<" | ";
        
        //8.2 位域的默认初始化
        cout<<"\nchapter8.1\n  ";
        #if __MORE_THAN_CPP20__
        S s1;
        cout<<s1.y<<" | "<<s1.z<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        FUNCTION_END()
    }
}
