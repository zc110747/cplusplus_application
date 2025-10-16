//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      lambda.cpp
//
//  Purpose:
//      第7章 lambda表达式(C++11~C++20)
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

#if __has_include(<future>)
#include <future>
#endif

using namespace std;

namespace LAMBDA
{
    //7.2 捕获列表
    class A
    {
    public:
        void print()
        {
            cout<<"class A"<<" | ";
        }

        void test()
        {
            auto foo=[this]{
                print();
                x = 5;
                return x;
            };
            foo();
        }
    private:
        int x;
    };

    //7.3 lambda表达式的实现原理
    class Bar
    {
    public:
        Bar(int x, int y):x_(x), y_(y) {}
        int operator() ()
        {
            return x_ * y_;
        }
    private:
        int x_;
        int y_;
    };

    //7.4 无状态lambda表达式
    void f(void(*func)(int a)){
        func(4);
    }
    void f1(void(&func)()){
        func();
    }

    //7.6 广义捕获
    #if __MORE_THAN_CPP14__
    #if __has_include(<future>)
        class Work
        {  
        public:
            std::future<int> spawn()
            {
                return std::async([=, tmp=*this]()->int
                {
                    return tmp.value;
                });
            }
        private:
            int value{42};
        };

        std::future<int> foo_work6()
        {
            Work tmp;
            return tmp.spawn();
        }
    #endif
    #endif

    //7.8 常量lambda表达式和捕获*this指针
    #if __MORE_THAN_CPP17__
    class work_e
    {
    public:
        std::future<int> spawn()
        {
            return std::async([=, *this]()->int
            {
                return value;
            });
        }
    private:
        int value{42};
    };

    std::future<int> foo_work8()
    {
        Work tmp;
        return tmp.spawn();
    }
    #endif

    //7.9 捕获[=, this]
    #if __MORE_THAN_CPP20__
    //在类中，=号隐式捕获this，不过c++20后需要显示捕获
    class T_9
    {
    public:
        void foo(){
            auto func = [=, this](){
                return value;
            };
            cout<<func()<<" | ";
        }
    private:
        int value{5};
    };
    #endif

    void test_run()
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
        //1.捕获列表的作用域存在两个作用域，表达式定义的作用域和函数体作用域
        cout<<"\nchapter7.2\n  ";
        int a2 = 1;
        static int b2 = 3;
        auto foo2 = [a2](){
            cout<<a2<<" | "<<b2<<" | ";
        };
        foo2();

        //捕获值和捕获引用
        int c2 = 4;
        //2.变量按值捕获，变量本身就是值，不可改变
        //3.在捕获引用的情况下，捕获变量实际上是一个引用, 修改会影响外部的值。
        //4.mutable可以移除lambda表达式的常量性，但不影响外部变量
        //5.值捕获时，在lambda定义时数值已经确定，修改外部不会改变
        //  引用捕获时，因为是参数的引用，所以外部的修改会影响到内部数值
        auto func2 = [a2, c2]{
            return a2 * c2;
        };
        auto funcl_2 = [&a2, &c2]{
            a2 += 1;
            c2 += 2;
            return a2 * c2;       
        };
        auto funcll_2 = [a2, c2]() mutable{
            a2 += 3;
            c2 += 5;
            return a2 * c2;
        };
        cout<<func2()<<" | ";
        cout<<funcl_2()<<" | ";
        cout<<funcll_2()<<" | ";
        cout<<a2<<" | "<<c2<<" | ";
        cout<<func2()<<" | ";
        cout<<funcl_2()<<" | ";

        //特殊的捕获方法
        //[this] 捕获this指针, 可以使用this类型的成员变量和函数
        //[=] 捕获作用域的全部变量数值
        //[&] 捕获作用域的全部变量引用
        auto funclll_2 = [=]{
            return a2 + c2;
        };
        cout<<funclll_2()<<" | ";
        auto funcllll_2 = [&]{
            return a2 + c2;
        };
        cout<<funcllll_2()<<" | ";

        A a;
        a.test();

        //7.3 lambda表达式的实现原理
        cout<<"\nchapter7.3\n  ";
        int a3 = 1, b3 = 3;
        Bar bar3(a3, b3);
        cout<<bar3()<<" | ";

        //7.4 无状态lambda表达式
        //无状态lambda可以隐式转换为函数指针
        //没有捕获的lambda表达式被称为无状态表达式
        //[]{} -> void(*)()
        //*[]{} -> vod(&)()
        cout<<"\nchapter7.4\n  ";
        auto func4 = [](int a){
            cout<<"func4"<<" | "<<a<<" |";
        };
        f(func4);
        f([](int a){
            cout<<"no status lambda!"<<" | "<<a<<" | ";
        });
        f1(*[]{
            cout<<"no status ref lambda!"<<" | ";
        });

        //7.5 在STL中使用lambda表达式
        cout<<"\nchapter7.5\n  ";
        std::vector<int> x5 = {1, 2, 3};
        auto ptr5 = std::find_if(x5.cbegin(), x5.cend(), [](int i){
            return i%3 == 0;
        });
        cout<<*ptr5<<" | ";

        //7.6 广义捕获
        //C++14支持初始化捕获
        cout<<"\nchapter7.6\n  ";
        #if __MORE_THAN_CPP14__
            int x6 = 5;
            auto foo6 = [r = x6 + 1]{
                return r;
            };
            cout<<foo6()<<" | ";

            std::string x="hello";
            auto func6 = [x = std::move(x)]{
                return x + " world!";
            };
            cout<<func6()<<" | ";

            auto f6 = foo_work6();
            f6.wait();
            cout<<f6.get()<<" | ";
        #else
            __LOWRE_THAN_CPP14_RUN
        #endif

        //7.7 泛型lambda表达式
        //使用auto占位符，即可实现泛型lambda
        cout<<"\nchapter7.7\n  ";
        #if __MORE_THAN_CPP14__
            auto foo7 = [](auto a){
                return a;
            };
            cout<<foo7(5)<<" | "<<foo7("hello")<<" | ";
        #else
            __LOWRE_THAN_CPP14_RUN
        #endif 

        //7.8  常量lambda表达式和捕获*this指针
        //lambda表达式可以声明constexpr
        cout<<"\nchapter7.8\n  ";
        #if __MORE_THAN_CPP17__
            auto f8 = foo_work8();
            f8.wait();
            cout<<f8.get()<<" | ";

            auto foo8 = []() constexpr -> int {
                int x = 5;
                return x;
            };
            cout<<foo8()<<" | ";
        #else
            __LOWRE_THAN_CPP17_RUN
        #endif

        //7.9 捕获[=, this]
        //this捕获指针，*this捕获对象
        cout<<"\nchapter7.9\n  ";
        #if __MORE_THAN_CPP20__
            T_9 t9;
            t9.foo();
        #else
            __LOWRE_THAN_CPP20_RUN
        #endif

        //7.10 模板语法的泛型lambda表达式
        cout<<"\nchapter7.10\n  ";
        #if __MORE_THAN_CPP20__
            auto func10 = []<typename T>(std::vector<T> vec){
                for(const auto& val:vec)
                {
                    cout<<val<<" | ";
                }
            };
            vector<int> vec{1, 2, 3};
            func10(vec);
        #else
            __LOWRE_THAN_CPP20_RUN
        #endif

        //7.11 可构造和可赋值的无状态lambda表达式
        cout<<"\nchapter7.11\n  ";
        #if __MORE_THAN_CPP20__   
        auto greater = [](auto x, auto y){
            return x>y;
        };
        std::vector<decltype(greater)> vec7 = {greater};
        cout<<(*vec7.begin())(3, 2)<<" | ";
        #else
            __LOWRE_THAN_CPP20_RUN
        #endif
        FUNCTION_END()
    }
}

