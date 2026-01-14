//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      preprocess.cpp
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
#include "preprocess.hpp"
#include "../includes.hpp"
#include <cstdio>

using namespace std;

namespace PREPROCESS
{
    #if __MORE_THAN_CPP17__
    #if __has_include(<optional>)
        constexpr auto have_optional = true;
    #else
        constexpr auto have_optional = false;
    #endif
    #endif

    void test_run()
    {
        FUNCTION_START()

        //32.1 预处理器__has_include
        //通过__has_include(<file>)格式，可以判断某个头文件十否存在(C++17支持)
        //只能用于预处理器中
        cout<<"\nchapter32.1\n  ";
        #if __MORE_THAN_CPP17__
        cout<<std::boolalpha;
        cout<<have_optional<<" | "; 
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif
        
        //32.2 特性测试宏
        //属性测试宏__has_cpp_attribute，获取某个属性加入cpp的时间
        cout<<"\nchapter32.2\n  ";
        cout<<"attribute\n  ";
        #if __MORE_THAN_CPP17__
        cout<<__has_cpp_attribute(carries_dependency)<<" | "; 
        cout<<__has_cpp_attribute(deprecated)<<" | "; 
        cout<<__has_cpp_attribute(fallthrough)<<" | "; 
        cout<<__has_cpp_attribute(likely)<<" | "; 
        cout<<__has_cpp_attribute(maybe_unused)<<" | "; 
        cout<<__has_cpp_attribute(no_unique_address)<<" | "; 
        cout<<__has_cpp_attribute(nodiscard)<<" | "; 
        cout<<__has_cpp_attribute(noreturn)<<" | "; 
        cout<<__has_cpp_attribute(unlikely)<<" | "; 
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        cout<<"\n  cpp feature:\n  ";
        //语言功能测试宏
        #if __MORE_THAN_CPP17__
        cout<<__cpp_aggregate_bases<<" | ";
        cout<<__cpp_aggregate_nsdmi<<" | ";
        cout<<__cpp_alias_templates<<" | ";
        cout<<__cpp_aligned_new<<" | ";
        cout<<__cpp_attributes<<" | ";
        cout<<__cpp_binary_literals<<" | ";
        cout<<__cpp_capture_star_this<<" | ";
        cout<<__cpp_constexpr<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        cout<<"\n  ";
        #if __MORE_THAN_CPP20__
        cout<<__cpp_aggregate_paren_init<<" | ";
        cout<<__cpp_char8_t<<" | ";
        cout<<__cpp_concepts<<" | ";
        cout<<__cpp_conditional_explicit<<" | ";
        cout<<__cpp_consteval<<" | ";
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif

        cout<<"\n  library feature:\n  ";
        #if __MORE_THAN_CPP17__
        cout<<__cpp_lib_addressof_constexpr<<" | ";  //<memory>
        cout<<__cpp_lib_apply<<" | ";
        cout<<__cpp_lib_array_constexpr<<" | ";
        cout<<__cpp_lib_as_const<<" | ";
        #else
        __LOWRE_THAN_CPP17_RUN;
        #endif

        //32.3 新增宏VA_OPT
        cout<<"\nchapter32.3\n  ";
        #define LOG(msg, ...) printf("[" __FILE__ ":%d]" msg"\n  ", __LINE__, ##__VA_ARGS__)
        LOG("Hello %d", 2023);
        LOG("Hello 2023");
        #if __MORE_THAN_CPP20__
        #define LOG_OPT(msg, ...) printf("[" __FILE__ ":%d]" msg"\n  ", __LINE__ __VA_OPT__(,) __VA_ARGS__)
        LOG_OPT("Hello %d", 2023);
        LOG_OPT("Hello 2023");
        #else
        __LOWRE_THAN_CPP20_RUN;
        #endif
        FUNCTION_END()
    }
}
