//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::span声明
//      2. std::span方法
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <span>
#include <spanstream>
#include <string>
#include <iomanip>
#include <cstdint>

template <class T>
void print_span(const std::span<T> &arr, std::string extra="")
{
    std::cout << extra << ": ";
    
    for (auto x : arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

namespace SPAN
{
    int test(void)
    {
        int arr[] = {1, 2, 3, 4};

        std::cout <<"============================ SPAN ============================" << std::endl;

        // 静态span，编译时固定，不能修改
        // 数组、vector、array都可以创建span
        std::span<int, 4> s0(arr);
        print_span<int>(s0, "s0");

        std::array<int, 4> a = {1, 2, 3, 4};
        std::span<int> s1(a);
        print_span<int>(s1, "s1");

        // 动态span
        std::span<int> s2(arr);
        print_span<int>(s2, "s2");

        std::vector vec = {1, 4, 2, 3, 5};
        std::span<int> s3(vec);
        print_span<int>(s3, "s3");

        // span方法
        // 迭代器iterator
        std::cout << "iter: ";
        for (auto iter = s0.begin(); iter != s0.end(); ++iter) {
            std::cout << *iter << " ";
        }
        std::cout << std::endl;

        // size、empty
        std::cout << "size: " << s0.size() << std::endl;
        std::cout << "empty: " << s0.empty() << std::endl;
        std::cout << "size_bytes: " << s0.size_bytes() << std::endl;

        // 元素访问
        // at、front、back、data
        //std::cout << "at: " << s0.at(2) << std::endl; //(C++26)
        std::cout << "front: " << s0.front() << std::endl;
        std::cout << "back: " << s0.back() << std::endl;
        std::cout << "data: " << *s0.first(2).data() << std::endl;

        // span和原始元素同步修改
        *s0.begin() = 10;
        std::cout << "arr[0]: " << arr[0] << std::endl;

        // operator[]
        std::cout << "s0[0]: " << s0[0] << std::endl;

        // subspan
        auto s4 = s0.subspan(0, 2);
        print_span<int>(s4, "s4");

        auto s5 = s0.subspan<0, 2>();
        print_span<int>(s5, "s5");

        // first、last
        auto s6 = s0.first(2);
        print_span<int>(s6, "s6");

        auto s7 = s0.last(2);
        print_span<int>(s7, "s7");

        // 将span转换成bytes存储
        auto const bytes0 = std::as_bytes(s0);
        for (auto const b : bytes0)
            std::cout << std::setw(2) << std::to_integer<int>(b) << ' ';
        std::cout << std::endl;

        auto const bytes1 = std::as_writable_bytes(s0);
        for (auto const b : bytes1)
            std::cout << std::setw(2) << std::to_integer<int>(b) << ' ';
        std::cout << std::endl;
        
        return 0;
    } 
}

namespace SPANSTREAM
{
    int test(void)
    {
        std::cout <<"============================ SPANSTREAM ============================" << std::endl;
        
        // ospanstream(用ospanstream输出字符串)
        char out_buf[16];
        std::ospanstream ost{std::span<char>{out_buf}};
        
        // span
        ost << "123456";
        auto sp = ost.span();
        print_span<char>(sp, "sp");

        // operator=
        std::ospanstream&& ost1 = std::move(ost);
        print_span<char>(ost1.span(), "ost1");

        // rdbuf
        std::spanbuf* rbuf = ost1.rdbuf();
        print_span<char>(rbuf->span(), "ost1");

        // ispanstream(用ispanstream解析字符串)
        const char in_buf[] = "a b 24";
        std::ispanstream ist{std::span<const char>{in_buf}};
        char c;
        ist >> c;
        std::cout << "c: " << c << std::endl;
        ist >> c;
        std::cout << "c: " << c << std::endl;
        int i;
        ist >> i;
        std::cout << "i: " << i << std::endl;
        
        // 以span方式获取解析的字符串
        auto span_1 = ist.span();
        for (auto x : span_1) {
            std::cout << x << " ";
        }
        std::cout << std::endl;

        return 0; 
    }
}

int main(int argc, char* argv[])
{
    SPAN::test();

    SPANSTREAM::test();
    return 0;
}