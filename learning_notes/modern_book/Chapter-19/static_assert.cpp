//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      static_assert.cpp
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
#include "static_assert.hpp"
#include "../includes.hpp"
#include "cassert"

using namespace std;

namespace STATIC_ASSERT
{
    class X
    {
    public:
        int a;
    };

    class VX
    {
        virtual void func(){
        }
    };

    //19.2 静态断言的需求
    template<bool>
    struct static_assert_st;
    template<>
    struct static_assert_st<true> {};

    #define STATIC_ASSERT2(expr) \
        static_assert_st<(expr) != 0>()

    //19.3 静态断言
    class A{
    };
    class B:public A{
    };
    class C{
    };
    template<class T>
    class E{
        static_assert(std::is_base_of<A, T>::value, "T is not Base of A");
    public:
        void print(void)
        {
            std::cout<<std::is_base_of<A, T>::value<<" | ";
        }
    };

    void test_run()
    {
        FUNCTION_START()

        //19.1 运行时断言
        cout<<"\nchapter19.1\n  ";
        int i = 0;
        auto p = &i;   

        assert(i == 0);
        assert(p != nullptr);

        *p = 3;
        cout<<i<<" | ";

        //19.2 静态断言的需求
        cout<<"\nchapter19.2\n  ";
        STATIC_ASSERT2(true);

        //19.3 静态断言
        //1.所有处理必须在编译期执行，不允许有空间或时间上运行时成本
        //2.他必须具有简单的语法
        //3.断言失败可以显示丰富的错误诊断信息
        //4.它可以在命名空间、类或代码块内使用
        //5.失败的断言会在编译阶段报错
        //传入两个实参，常量表达式和诊断消息字符串
        cout<<"\nchapter19.3\n  ";
        static_assert(true, "Not Right!");
        static_assert(sizeof(X) == sizeof(X::a), "Not Equal!");
        static_assert(sizeof(VX) == sizeof(void *), "not equal ptr");

        E<B> x1;
        //E<C> x2;
        x1.print();

        FUNCTION_END()
    }
}
