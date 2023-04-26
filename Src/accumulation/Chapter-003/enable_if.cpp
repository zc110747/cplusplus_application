//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      enable_if.cpp
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
#include "enable_if.hpp"
#include "../includes.hpp"

using namespace std;

namespace ENABLE_IF
{
    // 1. the return type (bool) is only valid if T is an integral type:
    template<class T>
    typename std::enable_if<is_integral<T>::value, bool>::type
    is_odd(T i)
    {
        return bool(i%2);
    }

    // 2. the second template argument is only valid if T is an integral type:
    template<class T, class = typename std::enable_if<is_integral<T>::value>::type>
    bool is_even(T i)
    {
        return !bool(i%2);
    } 

    void test_study()
    {
        FUNCTION_START()

        cout<<"\nchapter3.1\n  ";
        cout<<std::boolalpha<<is_even(5)<<" | ";
        cout<<is_odd(6)<<" | ";

        FUNCTION_END()
    }
}

