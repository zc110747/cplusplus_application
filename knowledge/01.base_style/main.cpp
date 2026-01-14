//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//     1. c++兼容c语言的实现，主要包含基础数据类型，循环，指针，sizeof等, 这部分可以参考《C Primer Plus》学习。
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstdio>

extern void basic_type_process(void);
extern void standard_improve(void);
extern void c_standard_lib(void);

int main(int argc, char **argv)
{
    basic_type_process();

    standard_improve();
    
    c_standard_lib();
    
    return 0;
}
