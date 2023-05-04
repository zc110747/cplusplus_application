//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      template_optimize.hpp
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
_Pragma("once")

namespace TEMPLATE_OPTIMIZE
{
    template<class T>
    class bar
    {
    public:
        T foo(T t){
            return t;
        };
    };

#ifndef _EXTERN
    extern template class bar<int>;
#else
    template class bar<int>;
#endif

    void test_study();
}