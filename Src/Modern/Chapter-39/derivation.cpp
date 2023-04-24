
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      derivation.cpp
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
#include "derivation.hpp"
#include "../includes.hpp"

using namespace std;

namespace DERIVATION
{
    //39.1 使用自定义推导指引模板实例
    #if __MORE_THAN_CPP17__
    template<typename T1, typename T2>
    struct MyPair{
        MyPair(const T1& x, const T2& y):
            first(x), second(y) {}
        T1 first;
        T2 second;
        void print()
        {
            cout<<first<<" | "<<second<<" | ";
        }
    };

    template<typename T1, typename T2>
    MyPair(T1, T2)->MyPair<T1, T2>;

    MyPair(int, const char*) -> MyPair<int, std::string>;
    MyPair(unsigned int, const char*) -> MyPair<int, std::string>;
    #endif

    //39.2 聚合类型类模板的推导指引
    #if __MORE_THAN_CPP17__
    template<class T>
    struct Wrap
    {
        T t1; 
        T t2;
    };
    template<class...T> Wrap(T&& ...t) -> Wrap<std::common_type_t<T...>>;
    #endif

    void test_study()
    {
        FUNCTION_START()

        //39.1 使用自定义推导指引模板实例
        //指定让类型衰退，从而正确推导类型
        //当类模板的构造函数和用户自定义推导同时满足实例化要求时，编译器优先选择用户自定义推导。
        #if __MORE_THAN_CPP17__
        cout<<"\nchapter39.1\n  ";
        MyPair v1(5, 11.7);
        v1.print();

        MyPair f1(5, "hello");
        f1.print();

        MyPair mp1(1u, "hel");
        mp1.print();
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //39.2 聚合类型类模板的推导指引
        cout<<"\nchapter39.2\n  ";
        #if __MORE_THAN_CPP17__
        Wrap wp1{1, 2};
        std::cout<<typeid(wp1.t1).name()<<" | ";
        cout<<wp1.t1<<" | "<<wp1.t2;
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        FUNCTION_END()
    }
}
