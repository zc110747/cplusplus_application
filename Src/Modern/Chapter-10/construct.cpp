//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      construct.cpp
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
#include "construct.hpp"
#include "../includes.hpp"

using namespace std;

namespace CONSTRUCT
{
    //10.1 类的特殊成员函数
    class User
    {
    public:
        void print()
        {
            cout<<name<<" | "<<age<<" | ";
        }
    private:
        string name{"default"};
        int age{10};
    };

    struct type
    {
        type() = default;
        virtual ~type() = delete;
        type(const type&);
        void print()
        {
            cout<<"type"<<" | ";
        }
    };
    type::type(const type&) = default;

    class Trivial
    {
    public:
        Trivial(int n):i_(n) {}
        Trivial() = default;
    private:
        int i_;
    };

    //10.3 显示删除的其他用途
    struct type_delete
    {
        void* operator new(std::size_t) = delete;
        void print()
        {
            cout<<"type_delete"<<" | ";
        }
    };

    //10.4 explict和=delete
    struct type_explict
    {
        type_explict(long long) {}
        explicit type_explict(long) = delete;
        void print()
        {
            cout<<"type_explict"<<" | ";
        }
    };
    void foo(type_explict t)
    {
        t.print();
    }

    void test_study()
    {
        FUNCTION_START()

        //10.1 类的特殊成员函数
        //C++11后支持6个默认构造函数
        //1.默认构造函数
        //2.析构函数
        //3.复制构造函数
        //4.复制赋值运算符函数 
        //5.移动构造函数
        //6.移动赋值运算符函数
        //声明任何构造函数都会抑制默认构造函数，类转变为非平凡类型
        cout<<"\nchapter10.1\n  ";
        User a1, b1;    //定义了默认构造函数
        a1 = b1;
        User c1(a1);    //定义了复制构造函数
        a1.print();
        c1.print();

        //10.2 显示默认和显示删除
        //显示默认和显示删除的，在声明函数的尾部添加=default和=delete
        //=default可以添加到类内部，也可以添加到类外部
        //=delete必须添加到类内部的函数声明
        cout<<"\nchapter10.2\n  ";
        auto *ptr = new type;
        ptr->print();
        cout<<is_trivial<Trivial>::value<<" | ";

        //10.3 显示删除的其他用途
        //可以删除普通函数或者类的成员函数，用于阻止某些动作
        //type_delete type3 = new type_delete;  //编译出错
        cout<<"\nchapter10.3\n  ";
        type_delete type1_3;
        type1_3.print();

        //10.4 explict和=delete
        //explict阻止隐式转换，混合使用会有异常
        cout<<"\nchapter10.4\n  ";
        foo(10);
        //foo(type(10));         //编译出错

        FUNCTION_END()
    }
}

