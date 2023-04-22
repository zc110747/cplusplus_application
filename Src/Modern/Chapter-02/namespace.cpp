//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      namespace.cpp
//
//  Purpose:
//      第2章 内联和嵌套命名空间
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "namespace.hpp"
#include "../includes.hpp"

using namespace std;

namespace NAMESPACE
{
    //2.1 内联命名空间的定义和使用
    namespace S1 {
        void foo() {
            cout<<"S1::foo"<<" | ";
        }

        namespace Child1 {
            void foo() {
                cout<<"S1::Child1::foo"<<" | ";
            }
        }

        inline namespace Child2 {
            void foo_inline() {
                cout<<"S2::Child2::foo"<<" | ";
            }
        }
    }

    namespace S2 {
        namespace V1{
            void foo() {
                cout<<"S2::V1::foo"<<" | ";
            }
        }

        inline namespace V2{
            void foo() {
                cout<<"S2::V2::foo"<<" | ";
            }
        }
    }

    //2.2 嵌套命名空间的简化语法
    namespace A::B::C{
        void foo() {
            cout<<"A::B::C::foo"<<endl;
        }
    }

    void test_study()
    {
        FUNCTION_START()

        //2.1 内联命名空间的定义和使用
        cout<<"\nchapter2.1\n  ";
        S1::foo();
        S1::Child1::foo();
        S1::foo_inline();
        S2::foo();
        S2::V1::foo();
        
        //2.2 嵌套命名空间的简化语法
        cout<<"\nchapter2.2\n  ";  
        A::B::C::foo();

        FUNCTION_END()
    }
}
