//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::ratio声明
//      2. std::ratio的成员变量
//      3. std::ratio的操作方法
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ratio>

int main(int argc, char const *argv[])
{
    using r1 = std::ratio<1, 2>;
    using r2 = std::ratio<2, 3>;

    std::cout << "r1: " << r1::num << "/" << r1::den << std::endl;

    // 计算两个比例的和、差、积、商
    using sum = std::ratio_add<r1, r2>;
    using diff = std::ratio_subtract<r1, r2>;
    using prod = std::ratio_multiply<r1, r2>;
    using quot = std::ratio_divide<r1, r2>;

    std::cout << "sum: " << sum::num << "/" << sum::den << std::endl;
    std::cout << "diff: " << diff::num << "/" << diff::den << std::endl;
    std::cout << "prod: " << prod::num << "/" << prod::den << std::endl;
    std::cout << "quot: " << quot::num << "/" << quot::den << std::endl;

    using eq = std::ratio_equal<r1, r2>;
    std::cout << "eq: " << eq::value << std::endl;

    using neq = std::ratio_not_equal<r1, r2>;
    std::cout << "neq: " << neq::value << std::endl;

    using less = std::ratio_less<r1, r2>;
    std::cout << "less: " << less::value << std::endl;

    using less_equal = std::ratio_less_equal<r1, r2>;
    std::cout << "less_equal: " << less_equal::value << std::endl;

    using greater = std::ratio_greater<r1, r2>;
    std::cout << "greater: " << greater::value << std::endl;

    using greater_equal = std::ratio_greater_equal<r1, r2>;
    std::cout << "greater_equal: " << greater_equal::value << std::endl;

    return 0;
}


