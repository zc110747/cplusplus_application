//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      template_var.cpp
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
#include "template_var.hpp"
#include "../includes.hpp"

using namespace std;

namespace TEMPLATE_VAR
{
    //35.1 可变参数的概念和语法
    template<class ...Args>
    void foo(Args ...args)
    {
        cout<<sizeof...(args)<<" | ";
    }

    template<class ...Args>
    class bar{
    public:
        bar(Args ...args)
        {
            foo(args...);
        }
    };

    template<class ...Args, class T>
    void foo11(T t, Args ...args)
    {
        cout<<t<<":";
        cout<<sizeof...(args)<<" | ";
    }

    template<class T, class ...Args>
    class dat{
    public:
        dat(T t, Args ...args)
        {
            foo11(t, args...);
        }
    };

    template<int ...Args>
    void foo12()
    {
        cout<<sizeof...(Args)<<" | ";
    }

    //35.2 形参包展开
    template<class T, class U>
    T baz(T t, U u)
    {
        cout<<t<<":"<<u<<" | ";
        return t;
    }
    template<class ...Args>
    void foo2(Args ...args)
    {}

    template<class ...Args>
    class bar2
    {
    public:
        bar2(Args ...args)
        {   
            //解包格式
            //foo2(baz(&a1, a1), baz(&a2, a2), baz(&a3, a3))
            foo2(baz(&args, args)...);
        }
    };

    template<class ...T>
    int baz21(T ...t)
    {
        return 0;
    }

    template<class ...Args>
    class bar21
    {
    public:
        bar21(Args ...args)
        {
            //baz21(&a1, &a2, &a3)+a1, baz21(&a1, &a2, &a3)+a2, baz21(&a1, &a2, &a3)+a3
            foo(baz21(&args...) + args...);
        }
    };
    
    int add(int a, int b) {return a+b;}
    int sub(int a, int b) {return a-b;}
    template<class ...Args>
    void foo21(Args (*...args)(int, int))
    {
        //在括号中用逗号分隔的表达式会以从左往右的顺序执行
        //最后返回最右表达式的结果
        int tmp[] = {(cout<<args(7, 11)<<" | ", 0)...};
    }
    
    void test_study()
    {
        FUNCTION_START()

        //35.1 可变参数的概念和语法
        //class ...Args是类型模板形参包，可接受0个或者多个类型的模板形参
        //Args ...args是函数形参包，可以接受0个或多个函数实参
        //args...是形参包的展开，可通过sizeof...获取内部数据个数
        //对于类来说，类型模板形参包必须是模板参数的最后一个
        //函数来说没有要求
        cout<<"\nchapter35.1\n  ";
        foo(1);
        foo(1, 2.5);
        foo(1, 2, "hello");

        bar b1(1, 2);
        bar b11(1, 2, "hello");

        dat d1(1.1, 2);
        dat d11(1.1, 2, "hello");
        foo12<1, 2>();

        //35.2 形参包展开
        //形参包展开允许的条件如下
        //1.表达式列表 
        //2.初始化列表
        //3.基类描述 
        //4.成员初始化列表
        //5.函数参数列表
        //6.模板参数列表
        //7.动态异常列表(C++17不在使用)
        //8.lambda表达式捕获列表
        //9.sizeof...运算符
        //10.对齐运算符
        //11.属性列表
        cout<<"\nchapter35.2\n  ";
        bar2 b2(1, 5.2, 8);
        bar21 b21(1, 2.5, 3.5);
        foo21(add, sub);
        FUNCTION_END()
    }
}