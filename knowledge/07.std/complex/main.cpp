//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::complex声明
//      2. std::complex成员函数
//      3. std::complex外部操作方法
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <complex>

int main(int argc, char* argv[])
{
    std::complex<double> c1(1.0, 2.0);
    std::complex<double> c2(3.0, 4.0);

    // operator+
    auto c3 = c1 + c2;
    
    // operator<<
    std::cout << "c1 + c2 = " << c3 << std::endl;

    // operator-
    auto c4 = c1 - c2;
    std::cout << "c1 - c2 = " << c4 << std::endl;

    // operator*
    auto c5 = c1 * c2;
    std::cout << "c1 * c2 = " << c5 << std::endl;

    // operator/
    auto c6 = c1 / c2;
    std::cout << "c1 / c2 = " << c6 << std::endl;

    // operator==
    if (c5 == c6) {
        std::cout << "c5 == c6" << std::endl;
    } else {
        std::cout << "c5 != c6" << std::endl;
    }

    // real、imag
    std::cout << "c1.real() = " << c1.real() << std::endl;
    std::cout << "c1.imag() = " << c1.imag() << std::endl;

    // 外部方法
    // abs: 复数的模长，实部虚部平方和的开方
    std::cout << "abs(c1) = " << std::abs(c1) << std::endl;

    // arg: 复数的相位，虚部为0时为0，否则为tan-1(虚部/实部)
    std::cout << "arg(c1) = " << std::arg(c1) << std::endl;

    // norm: 复数的范数，实部虚部平方和
    std::cout << "std::norm(c1) = " << std::norm(c1) << std::endl;

    // pow: 复数的幂，pow(a, b) = a^b
    std::cout << "std::pow(c1, 2) = " << std::pow(c1, 2) << std::endl;

    // sqrt: 复数的平方根，sqrt(a) = a^(1/2)
    std::cout << "std::sqrt(c1) = " << std::sqrt(c1) << std::endl;

    // sin: 复数的正弦函数，sin(a) = (e^(ia) - e^(-ia)) / (2i)
    std::cout << "std::sin(c1) = " << std::sin(c1) << std::endl;

    // cos: 复数的余弦函数，cos(a) = (e^(ia) + e^(-ia)) / 2
    std::cout << "std::cos(c1) = " << std::cos(c1) << std::endl;

    // sinh: 复数的双曲正弦函数，sinh(a) = (e^a - e^(-a)) / 2
    std::cout << "std::sinh(c1) = " << std::sinh(c1) << std::endl;

    // cosh: 复数的双曲余弦函数，cosh(a) = (e^a + e^(-a)) / 2
    std::cout << "std::cosh(c1) = " << std::cosh(c1) << std::endl;
    return 0;
} 
