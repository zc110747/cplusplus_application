//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      postf_return.cpp
//
//  Purpose:
//
// Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "postf_return.hpp"
#include "../includes.hpp"

using namespace std;
namespace POSTF_RETURN
{
    //5.1 使用函数返回类型后置声明
    auto foo_1()->int
    {
        return 42;
    }

    auto foo_func_1() -> int(*)()
    {
        return foo_1;
    }

    //5.2 推动函数模板返回类型值
    //通过decltype推导出表达式结果类型
    template<class T1, class T2>
    auto sum_2(T1 t1, T2 t2)->decltype(t1 + t2)
    {
        return t1+t2;
    }

    //declval获取类型的引用，从而不需要默认构造函数
    template<class T1, class T2>
    decltype(declval<T1>() + declval<T2>()) sumo_2(T1 t1, T2 t2)
    {
        return t1+t2;
    } 

    void test_run()
    {
        FUNCTION_START()

        //5.1 使用函数返回类型后置声明函数
        cout<<"\nchapter5.1\n  ";
        cout<<foo_1()<<" | ";
        cout<<foo_func_1()()<<" | ";

        //5.2 推导函数模板返回类型值
        cout<<"\nchapter5.2\n  ";
        auto x_2 = sum_2(4, 5);
        cout<<x_2<<" | ";
        auto xo_2 = sumo_2(6, 7);
        cout<<xo_2<<" | ";

        FUNCTION_END()
    }
}
