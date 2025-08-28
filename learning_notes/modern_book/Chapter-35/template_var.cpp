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
    
    //35.3 sizeof...运算符
    template<class... Args> 
    void foo3(Args ...args)
    {
        std::cout<<sizeof...(args)<<" | ";
    }

    template<class... Args>
    class bar3
    {
    public:
        bar3(){
            cout<<sizeof...(Args)<<" | ";
        }
    };

    //35.4 可变参数模板的递归计算
    #if __MORE_THAN_CPP14__
    template<class T>
    T sum4(T arg)
    {
        return arg;
    }
    template<class T1, class... Args>
    auto sum4(T1 arg1, Args ...args)
    {
        return arg1+sum4(args...);
    }
    #endif

    //35.5 折叠表达式
    #if __MORE_THAN_CPP17__
    template<class... Args>
    auto sum5(Args ...args)
    {
        return (args + ...);
    }

    template<class... Args>
    auto sum51(Args ...args)
    {
        return (... + args);
    }
    #endif

    //35.6 一元折叠表达式中空参数包的特殊处理
    #if __MORE_THAN_CPP17__
    template<class... Args>
    auto sum6(Args ...args)
    {
        return (args && ...);
    }
    #endif

    //35.7 using声明中的包展开
    #if __MORE_THAN_CPP17__
    template<class T>
    class base7
    {
    public:
        base7() {}
        base7(T t): t_(t) {}

        void show()
        {
            cout<<t_<<" | ";
        }
    private:
        T t_;
    };

    template<class... Args>
    class derived7: public base7<Args>...
    {
    public:
        using base7<Args>::base7...;
    };
    #endif

    //35.8 lambda表达式初始化捕获的包展开
    #if __MORE_THAN_CPP17__
    template<class F, class... Args>
    auto delay_invoke8(F f, Args... args){
        return [f, args...]() -> decltype(auto) {
            return std::invoke(f, args...);
        };
    }
    #endif

    void test_run()
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

        bar<int, int> b1(1, 2);
        bar<int, int, std::string> b11(1, 2, "hello");

        dat<double, int> d1(1.1, 2);
        dat<double, int, std::string> d11(1.1, 2, "hello");
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
        bar2<int, double, int> b2(1, 5.2, 8);
        bar21<int, double, double> b21(1, 2.5, 3.5);
        foo21(add, sub);

        //35.3 sizeof...运算符
        cout<<"\nchapter35.3\n  ";
        foo3(1);
        foo3(1, 2);
        bar3<int, float> bar3_val;

        //35.4 可变参数模板的递归计算
        cout<<"\nchapter35.4\n  ";
        #if __MORE_THAN_CPP14__
        cout<<sum4(1, 2.5, 3.7)<<" | ";
        #else
        __LOWRE_THAN_CPP14_RUN;
        #endif

        //35.5 折叠表达式
        //一元向右折叠
        //(args op ...) (arg0 op (arg1 op ..(argN-1 op argN)))
        //一元向左折叠
        //(... op args) ((((arg0 op arg1) op arg2) op ...) op argn)
        cout<<"\nchapter35.5\n  ";
        #if __MORE_THAN_CPP17__
        cout<<sum5(1, 2.5, 3.7)<<" | ";
        cout<<sum51(std::string("1"), "2", "3")<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //35.6 一元折叠表达式中空参数包的特殊处理
        //1.只有&&， ||和，运算符能够在空参数包的一元折叠表达式中使用
        //2.&&的求值结果一定是true
        //3.||的求值一定是flase
        //4.，的求值结果一定是void()
        //5.其它运算符时非法的
        cout<<"\nchapter35.6\n  ";
        #if __MORE_THAN_CPP17__
        cout<<std::boolalpha<<sum6()<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //35.7 using声明中的包展开
        //多重继承的二义性问题
        cout<<"\nchapter35.7\n  ";
        #if __MORE_THAN_CPP17__
        derived7<int, std::string, bool> d71 = 11;
        d71.base7<int>::show();
        derived7<int, std::string, bool> d72 = true;
        d72.base7<bool>::show();
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //35.8 lambda表达式初始化捕获的包展开
        cout<<"\nchapter35.8\n  ";
        #if __MORE_THAN_CPP17__
        auto lambda8 = delay_invoke8([](int a, int b, int c){
            cout<<a<<b<<c<<" | ";
        }, 1, 2, 3);
        lambda8();
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        FUNCTION_END()
    }
}