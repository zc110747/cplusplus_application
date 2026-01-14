//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      inherit.cpp
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
#include "inherit.hpp"
#include "../includes.hpp"

using namespace std;

namespace INHERIT
{
    //13.1 继承关系中构造函数的困局
    class Base
    {
    public:
        Base() : x_(0), y_(1.2) {}
        Base(int x, double y) : x_(x), y_(y) {}
        Base(int x) : x_(x), y_(0) {}
        Base(double y) : x_(0), y_(y) {}
        void show() {
            cout<<x_<<" | "<<y_<<" | ";
        }
    private:
        int x_{0};
        double y_{1.2};
    };

    class Derived : public Base
    {
    public:
        Derived() {}
        Derived(int x, double y):Base(x, y) {}
        Derived(int x):Base(x) {}
        Derived(double y):Base(y) {}
    };

    class Derived_Extend : public Base
    {
    public:
        using Base::Base;
        Derived_Extend(int x){
            cout<<"Derived_Extend"<<" | ";
        }   
    };    

    void test_run()
    {
        FUNCTION_START()

        //13.1 继承关系中构造函数的困局
        cout<<"\nchapter13.1\n  ";
        Derived d1;
        d1.show();

        Derived d2(2, 5);
        d2.show();
        
        //13.2 使用继承构造函数
        //1.派生类是隐式继承基类的构造函数，使用才会生成
        //2.派生类不会继承基类的默认构造函数和复制构造函数
        //3.继承构造函数不会影响派生类默认构造函数的隐式声明
        //4.在派生类种声明签名相同的构造函数会禁止继承相应的构造函数
        //5.派生类继承多个签名相同的构造函数会导致编译失败
        //6.继承构造函数的基类构造函数不能为私有。
        cout<<"\nchapter13.2\n  ";
        Derived_Extend d3(2);
        d3.show();
        Derived_Extend d4(1.5);
        d4.show();        

        FUNCTION_END()
    }
}