//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      inline.cpp
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
#include "inline.hpp"
#include "../includes.hpp"

using namespace std;

namespace INLINE
{
    class X
    {
    public:
        static std::string text;
        static const int num{5};
    };

    #if __MORE_THAN_CPP17__
    class extend
    {
    public:
        inline static std::string text{"inline"};
    };
    #endif

    std::string X::text{"hello"};

    void test_run()
    {
        FUNCTION_START()

        //26.1 定义非常量静态成员问题      
        //类的静态成员定义有且只有一个
        //再类内部初始化则需要定义为制度类型
        cout<<"\nchapter26.1\n  ";
        cout<<X::text<<" | ";
        cout<<X::num<<" | ";

        //26.2 使用inline说明符
        //使用inline说明，可以再多个头文件包含
        #if __MORE_THAN_CPP17__
        cout<<extend::text<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif
        FUNCTION_END()
    }
}

