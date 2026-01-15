//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////

#include "basic_any.hpp"
#include "enable_share.hpp"
#include "enable_if.hpp"
#include "lambda.hpp"
#include "singleton.hpp"

int main()
{
    BASIC_ANY::test_run();
    
    ENABLE_SHARE::test_run();

    ENABLE_IF::test_run();

    TEMPLATE_LAMBDA::test_run();

    SINGLETON::test_run();
    return 0;
}