//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      typename.cpp
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
#include "typename.hpp"
#include "../includes.hpp"

using namespace std;

namespace TYPENAME
{
    //36.1 允许使用typename声明模板形参
    #if __MORE_THAN_CPP17__
    template<typename T> using A = int;
    template<template <typename> class T> struct B {
        void print(){
            cout<<"struct B"<<" | ";
        }
    };
    //第二个template告知T是一个模板
    template<template <typename> typename T> struct C {
        void print(){
            cout<<"struct C"<<" | ";
        }
    };
    #endif

    //36.2 减少typename使用的必要性
    //需要通过typename指定T为模板
    template<class T> void f(typename T::R){}

    struct Impl {
        Impl(){
            cout<<"Impl::Init"<<"\n  ";
        }
    };
    struct Wrap {
        using B = Impl;
    };

    template<class T>
    struct D: T::B{
        D() : T::B() {
            cout<<"D::Init"<<"\n  ";
        }
    };

    #if __MORE_THAN_CPP20__
    template<class T>
    using T1 = T::B;

    template<class T>
    auto func() -> T::B {
        return typename T::B();
    }

    template<class T>
    T::B f();

    template<class T>
    struct L{
        T::B b;
        T::B &f(){
            return b;
        }
    };
    #endif

    void test_run()
    {
        FUNCTION_START()

        //36.1 允许使用typename声明模板形参
        cout<<"\nchapter36.1\n  ";
        //c++17后允许用typename声明模板形参
        #if __MORE_THAN_CPP17__
            B<A> ba;
            ba.print();
            C<A> ca;
            ca.print();
        #else
            __LOWRE_THAN_CPP17_RUN
        #endif       
    
        //36.2 减少typename使用的必要性
        //C++20中可以省略typename的场景
        //1.在上下文仅可能时类型标识的情况下，可以省略typename
        //如static_cast, dynamic_cast, using定义的类型别名，
        //后置返回类型，模板类型形参的默认参数
        //2.全局空间或者命名空间中的简单声明
        //3.结构体的成员
        //4.作为成员函数或者lambada表达式形参声明
            D<Wrap> d1;
        #if __MORE_THAN_CPP20__
            T1<Wrap> t1;
            auto t2 = func<Wrap>();
            L<Wrap> c1;
            auto c2 = c1.f();
        #else
            __LOWRE_THAN_CPP20_RUN
        #endif
        FUNCTION_END()
    }
}