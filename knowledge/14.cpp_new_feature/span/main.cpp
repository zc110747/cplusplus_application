<<<<<<< HEAD
﻿//////////////////////////////////////////////////////////////////////////////
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
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <span>
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

int main(int argc, char* argv[])
{
    int arr[] = {1, 2, 3, 4};

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

=======
﻿/*
std::span
std::span 是 C++20 引入的一个容器视图类，它提供了一种轻量级的、非拥有的方式来访问连续的对象序列

主要特点：
1. 非拥有性：std::span 不拥有它所指向的数据，它只是提供了对数据的访问接口。这意味着 std::span 不会分配或释放内存，它只是引用了已经存在的内存。
2. 连续内存访问：std::span 要求它所指向的数据是连续存储的，这使得它可以高效地进行随机访问和迭代。
3. 大小和范围：std::span 可以在编译时或运行时指定其大小，这使得它可以灵活地适应不同的使用场景。
4. 与标准库的集成：std::span 可以与标准库中的其他容器和算法无缝集成，使得代码更加简洁和高效。

注意:
span是对其它内存的视图引用，因此使用时不要修改原始的数据，如引用已经释放的内存或者改变原数组的大小
*/

#include <iostream>
#include <vector>
#include <algorithm>

#if __cplusplus >= 202002L
#include <span>
template <class T>
void print_span(const std::span<T> &arr)
{
    for (auto x : arr) {
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;
}
#endif

int main(int argc, char* argv[])
{
#if __cplusplus >= 202002L
    int arr[] = {1, 2, 3, 4};

    //静态span，编译时固定，不能修改
    std::span<int, 4> s(arr);
    print_span<int>(s);
    std::cout<<s.size()<<std::endl;

    //动态span
    std::span<int> s1(arr);
    
    //获取子视图
    auto s2 = s1.subspan(0, 2);
    print_span(s2);

    auto s3 = s1.subspan<0, 2>();
    print_span<int>(s3);

    //span是对其它内存的视图引用，不要修改原始的数据
    //vec修改，无论是push还是pop都影响span的应用
    std::vector vec = {1, 4, 2, 3, 5};
    std::span<int> s4(vec);
    print_span(s4);

    std::sort(s4.begin(), s4.end());
    print_span(s4);
#else
    std::cout<<"C++20 not support"<<std::endl;
#endif
>>>>>>> 30b84540516477c29747dece63510e1e03010626
    return 0;
} 