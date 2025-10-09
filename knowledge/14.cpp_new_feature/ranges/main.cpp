//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::views::filter / std::views::transform
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <concepts>

template<typename T>
requires requires(T a) {
    { a.begin() } -> std::convertible_to<typename T::iterator>;
    { a.end() } -> std::convertible_to<typename T::iterator>;
}
void show_container(const T& container)
{
    for (const auto& str : container) {
        std::cout << str <<' ';
    }
    std::cout<<std::endl;
}

namespace VIEWS
{
    int test(void)
    {
        std::vector<int> numbers = {1, 4, 3, 6, 5, 2, 4, 1, 0, 8};
        show_container(numbers);

        auto even_numbers = numbers | std::views::filter([](int n) { return n % 2 == 0; });
        auto iterator = even_numbers.begin();
        std::cout << "first even number: " << *iterator << std::endl;
        
        // show_container(even_numbers);

        
        return 0;
    }
}

namespace RANGES
{
    int test(void)
    {
        std::vector<int> numbers = {1, 4, 3, 6, 5, 2, 4, 1, 0, 8};

        std::ranges::sort(numbers);
        for (const auto& str : numbers) {
            std::cout << str <<' ';
        }
        std::cout<<std::endl;

        auto count = std::ranges::count(numbers, 4);
        std::cout << "count: " << count << std::endl;

        std::ranges::for_each(numbers, [](int n) { std::cout << n << ' '; });
        std::cout<<std::endl;
        
        return 0;
    }
}

// 模板元编程：计算乘法表的一行
// template<int N>
// struct MultiplicationRow {
//     static std::string value() {
//         std::string row;
//         for (int i = 1; i <= N; ++i) {
//             row += std::to_string(i) + " x " + std::to_string(N) + " = " + std::to_string(i * N) + "\t";
//         }
//         return row;
//     }
// };

// // 模板元编程：生成乘法表
// template<int... Ns>
// struct MultiplicationTable {
//     static std::vector<std::string> value() {
//         return { MultiplicationRow<Ns>::value()... };
//     }
// };

// // 辅助函数：生成整数序列
// template<int... Ns>
// constexpr auto index_sequence = std::integer_sequence<int, Ns...>{};

// // 辅助函数：生成乘法表
// template<int N, int... Ns>
// constexpr auto make_multiplication_table(index_sequence<Ns...>) {
//     return MultiplicationTable<Ns...>::value();
// }

int main(int argc, char* argv[])
{
    VIEWS::test();

    RANGES::test();

    // std::vector<int> numbers = {1, 4, 3, 6, 5, 2, 4, 1, 0, 8};

    // // 使用视图过滤偶数并转换为字符串
    // auto even_strings = numbers | std::views::filter([](int n) { return n % 2 == 0; })
    //                             | std::views::transform([](int n) { return std::to_string(n); });

    // // 输出结果
    // for (const auto& str : even_strings) {
    //     std::cout << str << ' ';
    // }
    // std::cout<<std::endl;

    // std::ranges::sort(numbers);
    // for (const auto& str : numbers) {
    //     std::cout << str <<' ';
    // }
    // std::cout<<std::endl;

    // // 使用 ranges 库输出乘法表
    // std::ranges::for_each(make_multiplication_table<9>(index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9>{}), [](const std::string& row) {
    //     std::cout << row << std::endl;
    // });
    return 0;
}