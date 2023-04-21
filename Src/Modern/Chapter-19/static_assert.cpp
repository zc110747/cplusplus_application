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
//     	@听心跳的声音
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

    void test_study()
    {
        FUNCTION_START()

        //19.1 运行时断言
        cout<<"\nchapter19.1\n  ";
        int i = 0;
        auto p = &i;   

        assert("i != 0");
        assert("p != nullptr");

        *p = 3;
        cout<<i<<" | ";

        cout<<"\nchapter19.2\n  ";
        static_assert(true, "Not Right!");
        static_assert(sizeof(X) == sizeof(X::a), "Not Equal!");
        static_assert(sizeof(VX) == sizeof(void *), "not equal ptr");

        FUNCTION_END()
    }
}
