//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <scoped_allocator>
#include <memory>

int main() 
{
    using InnerAlloc = std::allocator<int>;
    using OuterAlloc = std::allocator<std::vector<int>>;

    using ScopedAlloc =
        std::scoped_allocator_adaptor<
            OuterAlloc,
            InnerAlloc
        >;

    std::vector<std::vector<int>, ScopedAlloc> matrix;

    matrix.emplace_back();
    matrix.back().push_back(10);
    matrix.back().push_back(20);

    matrix.emplace_back();
    matrix.back().push_back(30);

    for (const auto& row : matrix) {
        for (int value : row) {
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }
}
