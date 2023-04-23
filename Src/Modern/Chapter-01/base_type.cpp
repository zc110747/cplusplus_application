//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      base_type.cpp
//
//  Purpose:
//      第1章 新基础类型
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "base_type.hpp"
#include "../includes.hpp"

#if __has_include(<format>)
#include <format>
#endif

using namespace std;

namespace BASE_TYPE
{
    //1.1　整数类型long long
    enum X:long long{
        ENUM_A,
        ENUM_B
    };

    void test_study()
    {
        FUNCTION_START()

        //1.1　整数类型long long/unsigned long long
        //C++标准定义，long long至少为64位数据
        //支持LL或ULL声明指定长度数据
        cout<<"\nchapter1.1\n  ";
        long long x1 = 65536LL;
        long long y1 = 65536<<16;
        long long z1 = 65536LL<<16;
        cout<<x1<<" | "<<y1<<" | "<<z1<<" | ";

        X a1 = ENUM_A;
        cout<<a1<<" | ";

        unsigned long long b1 = 65536ULL<<16;
        cout<<b1<<"\n  ";
        
        cout<<LLONG_MAX<<" | "<<LLONG_MIN<<" | "<<ULLONG_MAX<<"\n  ";
        cout<<std::numeric_limits<long long>::min()<<" | ";
        cout<<std::numeric_limits<long long>::max()<<" | ";
        cout<<std::numeric_limits<unsigned long long>::max()<<" | ";

        //1.2 新字符类型char16_t和char32_t
        cout<<"\nchapter1.2\n  ";
        char16_t ch1 = u'好';
        char32_t ch2 = U'好';
        u16string str1 = {u"测试"};
        u32string str2 = {U"测试"};

        //1.3 新字符类型char8_t
        cout<<"\nchapter1.3\n  ";
        #if __MORE_THAN_CPP20__
            char ch0 = u8'a';       //c++20可使用char8_t
            char8_t char8_3[] = u8"text";
            u8string str3(char8_3);
            cout<<str3.length()<<" | ";
            #if __has_include(<format>)
            for(auto c:str3)
            {
                cout<<fmt::format("{0:08b}", char(c))<<" ";
            }
            #endif
        #else
            __LOWRE_THAN_CPP20_RUN
        #endif
        FUNCTION_END()
    }
}

