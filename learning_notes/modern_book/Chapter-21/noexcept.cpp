//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      noexcept.cpp
//
//  Purpose:
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "noexcept.hpp"
#include "../includes.hpp"

using namespace std;


namespace NOEXCEPT
{
    //21.1 使用noexcept代替throw
    int foo_1() noexcept
    {
        return 42;
    }

    int func_1() noexcept(false)
    {
        return 32;
    }

    //is_fundamental检查类型是否是基础类型
    template<class T>
    T func1_1(const T& o) noexcept(std::is_fundamental<T>::value)
    {
        cout<<"func1_1"<<" | ";
        return o;
    }
    
    void funct_1() {}
    void functh_1() throw()
    {
    }

    //21.2 用noexcept来解决移动构造问题
    //检查移动构造和移动赋值函数是否会抛出异常
    template<class T>
    void Swap(T& a, T& b)
    noexcept(noexcept(T(std::move(a))) && noexcept(a.operator=(std::move(b))))
    {
        T tmp(std::move(a));
        a = std::move(b);
        b = std::move(tmp);
    }

    //21.4 noexcept或throw()
    struct X{
    };
    struct M{
        ~M(){
        }
    };
    struct ML{
        ~ML() noexcept(false){
        }
    };
    #define PRINT_NOEXCEPT(x) {cout<<#x<<"="<<x<<" | ";}

    //21.6 将异常规范作为类型的一部分
    void (*fp)() = nullptr;
    void foo() noexcept {}

    class Base{
    public:
        virtual void foo(){
            cout<<"Base"<<" | ";
        }
    };
    class Derived:public Base
    {
    public:
        void foo() noexcept override{
            cout<<"Derived"<<" | ";
        }
    };

    void test_run()
    {
        FUNCTION_START()

        //21.1 使用noexcept代替throw
        //noexcept是一个与异常相关的关键字，即是说明符，也是运算符
        //声明了noexcept的函数抛出异常，会直接中止程序
        //noexcept可以接受表达式参数，并返回true或false
        //npexcept默认相当于true，表示不会抛出异常
        cout<<"\nchapter21.1\n  ";
        cout<<foo_1()<<" | ";
        cout<<func_1()<<" | ";
        int i = 5;
        cout<<func1_1<int>(i)<<" | ";
        cout<<noexcept(funct_1())<<" | ";
        cout<<noexcept(functh_1())<<" | ";

        //21.2 用noexcept来解决移动构造问题
        cout<<"\nchapter21.2\n  ";
        std::vector<int> a2 = {1, 2};
        std::vector<int> b2 = {2, 4};
        Swap(a2, b2);
        auto func_print = [](std::vector<int> t){
            for(auto& val:t)
            {
                cout<<val<<" | ";
            }
        };
        func_print(a2);
        func_print(b2);        
        cout<<std::boolalpha<<noexcept(Swap(a2, b2))<<" | ";
        cout<<std::is_nothrow_constructible<vector<int> >::value<<" | ";
        cout<<std::is_nothrow_move_constructible<vector<int> >::value<<" | ";
        cout<<std::is_nothrow_copy_assignable<vector<int> >::value<<" | ";

        //21.3 noexcept或throw()
        //throw()和noexcept, C++17后具有相同行为，C++20后throw()移除
        cout<<"\nchapter21.3\n  ";
        auto func3 = []() throw() {
        };
        cout<<noexcept(func3())<<" | ";
        
        //21.4 noexcept或throw()
        //1.默认构造函数，默认复制构造函数，默认赋值函数，默认移动构造函数和默认移动赋值函数，默认带有noexcept声明
        //  需要类型的基类和成员中也具有noexcept声明
        //2.类型的析构函数和delete运算符默认带有noexcept运算符，即使自己定义也默认带有noexcept
        //  除非声明noexcept(false)
        cout<<"\nchapter21.4\n  ";
        X x4;
        PRINT_NOEXCEPT(noexcept(X()));
        PRINT_NOEXCEPT(noexcept(X(x4)));
        PRINT_NOEXCEPT(noexcept(X(std::move(x4))));
        PRINT_NOEXCEPT(noexcept(x4.operator=(x4)));
        PRINT_NOEXCEPT(noexcept(x4.operator=(std::move(x4))));
        cout<<"\n  ";
    
        X *px4 = new X;
        M *m4 = new M;
        ML *ml4 = new ML;
        PRINT_NOEXCEPT(noexcept(px4->~X()));
        PRINT_NOEXCEPT(noexcept(m4->~M()));
        PRINT_NOEXCEPT(noexcept(ml4->~ML()));
        PRINT_NOEXCEPT(noexcept(delete px4));
        PRINT_NOEXCEPT(noexcept(delete m4));
        PRINT_NOEXCEPT(noexcept(delete ml4));

        //21.5 使用noexcept的时机
        //1.一定不会出现异常的函数, 函数很简短，例如求整数的绝对值，对基本类型的初始化
        //2.目标是提供不会失败或者不会抛出异常的函数可以使用noexcept声明
        cout<<"\nchapter21.5\n  ";
        auto foo5 = [](int a, int b) noexcept {
            return a+b;
        };
        cout<<noexcept(foo5(5, 6))<<" | ";
        
        //21.6 将异常规范作为类型的一部分(C++17后)
        //1.允许用非noexcept声明的函数指针接受noexcept声明的函数，反之不允许
        //2.允许用noexcept函数重载基类的非noexcept函数，反之不允许
        cout<<"\nchapter21.6\n  ";
        #if __MORE_THAN_CPP17__
        fp = &foo;
        cout<<std::is_same_v<decltype(fp), decltype(&foo)><<" | ";
        Derived d6;
        d6.foo();
        #else
        __LOWRE_THAN_CPP17_RUN
        #endif

        FUNCTION_END()
    }
}
