//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      auto.cpp
//
//  Purpose:
//      第3章 auto占位符
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "auto.hpp"
#include "../includes.hpp"

using namespace std;

namespace AUTO
{
    //3.1 重新定义的auto关键字
    auto sum(int a1, int a2)->int
    {
        return a1+a2;
    }

    struct X
    {
        static const auto i = 9;
        #if __MORE_THAN_CPP17__
        static inline auto j = 42;
        #endif
    };

    #if __MORE_THAN_CPP20__
    void echo(auto str)
    {
        cout<<str<<" | ";
    }
    #endif

    //3.4 返回类型推导
    #if __MORE_THAN_CPP14__
    auto sum(long a1, long a2)
    {
        if(a1 < 0)
            return 0L;
        else
            return a1+a2;
    }
    #endif

    //3.6 非类型模板形参占位符
    #if __MORE_THAN_CPP17__
    template<auto N>
    void f()
    {
        cout<<N<<" | ";
    }

    #endif

    void test_study()
    {
        FUNCTION_START()

        //3.1 重新定义的auto关键字
        //auto声明变量时根据初始化表达式自动推断该变量的类型
        //声明函数时函数返回值的占位符
        //1.当用auto关键字声明多个变量时，按照从左到右原则
        //2.当使用条件表达式初始化auto声明变量时，编译器使用表达能力更强的类型
        //3.auto不能用于声明类的非静态变量，静态变量需要加const(C++17后不需要)
        //4.C++20支持函数的形参也可以为auto类型，C++14支持lambda的形参为auto
        cout<<"\nchapter3.1\n  ";
        auto i_1 = 5;
        auto str_1 = "hello world!";
        auto x_1 = sum(1, 2);
        int n_1 = 5;
        auto *pn_1 = &n_1, m_1 = 10;
        auto f_1 = true?5:10.1;            //double
        cout<<i_1<<" | "<<str_1<<" | ";
        cout<<x_1<<" | "<<n_1<<" | ";
        cout<<*pn_1<<" | "<<m_1<<" | ";
        cout<<f_1<<" | ";

        cout<<"\n  class static init for auto:\n  ";
        cout<<X::i<<" | ";
        #if __MORE_THAN_CPP17__
        cout<<X::j<<" | ";
        #else
            __LOWRE_THAN_CPP17_RUN
        #endif

        cout<<"\n  function parameter with auto:\n  ";
        #if __MORE_THAN_CPP20__
        echo("hello world!");
        auto func = [](auto val){
            return val;
        };
        cout<<func(5)<<" | ";
        #else
            __LOWRE_THAN_CPP20_RUN
        #endif

        //3.2 推导规则
        //1.如果auto声明的变量是按值初始化(没有引用或指针)，忽略CV限定符(const/volatile),
        //2.如果声明变量初始化时，目标对象是引用，引用属性被忽略
        //3.使用auto和万能引用声明变量时，对于左值会将auto推导为引用类型,右值推导为原类型
        //4.使用auto声明变量，如果目标是数组或者函数，auto推导为对应的指针类型
        //5.auto关键字和列表初始化配合
        //5.1 直接使用列表初始化，只能时单元素, auto被推导为单元素类型
        //5.2 用等于加列表初始化，可包含单个或多个元素，但类型必须相同
        cout<<"\nchapter3.2\n  ";
        const int a_2 = 5;
        auto b_2 = a_2;         //int
        auto& c_2 = a_2;        //const int
        auto d_2 = c_2;         //int 引用属性被忽略
        volatile int x_2 = 1;
        auto y_2 = x_2;         //int
        auto *p_2 = &x_2;       //volatile int

        auto &&e_2 = b_2;       //int &
        auto &&f_2 = 5;         //int
        cout<<a_2<<" | "<<c_2<<" | "<<d_2<<" | ";
        cout<<x_2<<" | "<<y_2<<" | "<<*p_2<<" | ";
        cout<<e_2<<" | "<<f_2<<" | ";

        int arr2[] = {1, 2, 3};
        auto parr = arr2;       //int *
        cout<<*parr<<" | ";

        cout<<"\n  auto use initializer_list:\n  ";
        #if __MORE_THAN_CPP17__
            auto g_2{1};
            auto h_2 = {1, 2, 3};
            cout<<g_2<<" | ";
            for(const auto& val:h_2)
            {
                cout<<val<<" | ";
            }
        #else
            __MORE_THAN_CPP17__;
        #endif
        
        //3.3 什么时候使用auto
        //1.当一眼能看出声明变量的初始化类型
        //2.对于复杂的类型，如lambda表达式，bind等
        cout<<"\nchapter3.3\n  ";
        std::map<int, std::string> map_3 = {
            {1, "first"},
            {2, "second"}
        };
        for(const auto& val:map_3)
        {
            cout<<val.first<<"="<<val.second<<" | ";
        }
        auto lambda_3 = [](){
            cout<<"hello world!"<<" | ";
        };
        lambda_3();

        //3.4 返回类型推导
        //如果多重返回值，需要保证返回值类型是相同的
        cout<<"\nchapter3.4\n  ";
        cout<<sum(2, 3)<<" | ";

        //3.5 lambda表达式中使用auto类型推导
        //C++14可以把auto写到lambda表达式
        //后置返回类型使用auto，是lambda返回引用类型的唯一方法。
        cout<<"\nchapter3.5\n  ";
        #if __MORE_THAN_CPP14__
            auto func1_4 = [](auto a1, auto a2){
                return a1+a2;
            };
            cout<<func1_4(1, 2.5)<<" | ";
            auto func2_4 = [](int &i)->auto& {return i;};
            auto x1 = 5;
            auto &x2 = func2_4(x1);
            x2 = 3;
            cout<<x1<<" | "<<x2<<" | ";
        #else
            __MORE_THAN_CPP14__;
        #endif

        //3.6 非类型模板形参占位符
        //对于非类型模板template<int N>这类，可以用auto占用
        //template<auto N>
        cout<<"\nchapter3.6\n  ";
        #if __MORE_THAN_CPP17__
            f<5>();
            f<'c'>();
        #else
            __MORE_THAN_CPP17__;
        #endif
        FUNCTION_END()
    }
}
