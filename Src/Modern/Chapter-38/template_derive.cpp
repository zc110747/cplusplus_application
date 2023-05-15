//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      template_derive.cpp
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
#include "template_derive.hpp"
#include "../includes.hpp"

using namespace std;

namespace TEMPLATE_DERIVE
{
    //38.1 通过初始化构造推导类模板的模板实参(C++17)
    #if __MORE_THAN_CPP17__    
    template<class T, auto N>
    struct MyCountOf
    {
        MyCountOf(T(&)[N]) {}
        std::size_t value = N;
    };

    template<class T1, class T2>
    void foo(T1 t1, T2 t2)
    {
        cout<<t1<<" "<<t2<<" | ";
    }
    #endif

    //38.2 拷贝初始化优先
    template<class T>
    struct LambdaWarp
    {   
        LambdaWarp(T t): func(t) {};

        template<class ...Args>
        void operator() (Args&& ... arg)
        {
            func(std::forward<Args>(arg) ...);
        }

        T func;
    };

    //38.4 别名模板的类模板参数推导
    #if __MORE_THAN_CPP20__
    template <class T, class U>
    struct C4
    {
        C4(T t1, U u1)
        {
            cout<<*t1<<" "<<*u1<<" | ";
        }
    };
    template<class V>
    using A4 = C4<V*, V*>;
    #endif

    //38.5 聚合类型的类模板实参推导
    #if __MORE_THAN_CPP20__
    template<class T>
    struct S5{
        T x;
        T y;
    };

    template<class T, class U>
    struct X5{
        S5<T> s1;
        U x;
        T y;    //必须由y推导整个类型，S<T>不参与推导
    };
    #endif

    void test_study()
    {
        FUNCTION_START()

        //38.1 通过初始化构造推导类模板的模板实参(C++17)
        cout<<"\nchapter38.1\n  ";
        auto v1 = std::make_tuple(5, 11.7, "hello world");
        auto v1_0 = std::get<0>(v1);
        auto v1_1 = std::get<1>(v1);
        auto v1_2 = std::get<2>(v1);
        cout<<v1_0<<" "<<v1_1<<" "<<v1_2<<" | ";

        #if __MORE_THAN_CPP17__
        std::tuple a1{1, 2.5, "test"};
        cout<<get<0>(a1)<<" "<<get<1>(a1)<<" "<<get<2>(a1)<<" | ";
        
        //使用new表达式可以触发类模板的实参推导
        auto vp1 = new vector{4, 3, 2};
        cout<<(*vp1)[0]<<" | ";
        delete vp1;

        //非类型的类模板也可以触发实参推导
        MyCountOf c("hello"); 
        cout<<c.value<<" | ";

        //函数模板支持部分推导，类模板不支持
        foo(1, 1.5);
        foo<>(1, 1.5);
        foo<int>(1, 1.5);
        foo<int, double>(1, 1.5);
        #else
        cout<<"\n  ";
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //38.2 拷贝初始化优先
        cout<<"\nchapter38.2\n  ";
        #if __MORE_THAN_CPP17__
        std::vector v2{1, 3, 5};
        std::vector c2{v2};     //vector<int>
        cout<<c2[0]<<" "<<c2[1]<<" "<<c2[2]<<" | ";

        std::vector s2 = {v2};      //vector<int>
        auto f2 = std::vector{v2};  //vector<int>
        std::vector l2{v2, v2};     //vector<vector<int>>
        cout<<s2[0]<<" | ";
        cout<<f2[0]<<" | ";
        cout<<l2[0][0]<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //38.3 lambda类型的用途
        cout<<"\nchapter38.3\n  ";
        auto l3 = [](int a, int b){
            cout<<a+b<< " | ";
        };
        LambdaWarp<decltype(l3)> y3(l3);
        y3(2, 4);

        #if __MORE_THAN_CPP17__
        LambdaWarp x3([](int a, int b){
            cout<<a+b<< " | ";
        });
        x3(1, 5);
        #else
        cout<<"\n  ";
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //38.4 别名模板的类模板参数推导
        cout<<"\nchapter38.4\n  ";    
        #if __MORE_THAN_CPP20__
        int i4 = 1;
        double d4 = 2.5;
        A4 a4_1(&i4, &i4);
        A4 a4_2(&d4, &d4);
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        //38.5 聚合类型的类模板实参推导
        cout<<"\nchapter38.5\n  ";    
        #if __MORE_THAN_CPP20__
        S5 s51{1, 2};
        cout<<s51.x<<" "<<s51.y<<" | ";

        X5 x51{{1, 2}, 3, 4};
        cout<<x51.s1.x<<" "<<x51.s1.y<<" | ";  
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        FUNCTION_END()
    }
}
