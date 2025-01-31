//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      Chapter-001  basic_any      
//          实现一个any结构
//      Chapter-002  enable_share   
//          学习enable_share_from_this
//
//  Author:
//     	@听心跳的声音
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