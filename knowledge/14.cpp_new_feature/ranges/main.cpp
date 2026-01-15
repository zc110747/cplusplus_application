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
//      @公众号：<嵌入式技术总结>
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

namespace RANGES
{
    template<int... Is>
    constexpr auto generate_row_fold(int row) {
        std::string result;
        ((result += Is <= row?(result.empty() ? "" : "\t") + 
            std::to_string(Is) + "*" + std::to_string(row) + "=" + std::to_string(row * Is):""), ...);
        return result;
    }

    template<int... Rs>
    constexpr auto generate_table_fold() {
        std::vector<std::string> table;
        (table.push_back(generate_row_fold<1, 2, 3, 4, 5, 6, 7, 8, 9>(Rs)), ...);
        return table;
    }

    // 便捷函数：生成9x9乘法表
    inline std::vector<std::string> generate_9x9_multiplication_table() {
        return generate_table_fold<1, 2, 3, 4, 5, 6, 7, 8, 9>();
    }

    int test(void)
    {
        std::vector<int> numbers = {1, 4, 3, 6, 5, 2, 4};

        // begin、cbegin、rbegin、crbegin, end、cend、rend、crend
        std::cout << "begin: " << *std::ranges::begin(numbers) << std::endl;
        std::cout << "cbegin: " << *std::ranges::cbegin(numbers) << std::endl;
        std::cout << "rbegin: " << *std::ranges::rbegin(numbers) << std::endl;
        std::cout << "crbegin: " << *std::ranges::crbegin(numbers) << std::endl;
        for (auto it = std::ranges::begin(numbers); it != std::ranges::end(numbers); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        std::cout << "size: " << std::ranges::size(numbers) << std::endl;
        std::cout << "ssize: " << std::ranges::ssize(numbers) << std::endl;
        std::cout << "empty: " << std::ranges::empty(numbers) << std::endl;

        std::string str = "hello world";
        std::cout << "str: " << std::ranges::data(str) << std::endl;
        std::cout << "cstr: " << std::ranges::cdata(str) << std::endl;

        // sort
        std::ranges::sort(numbers);
        for (const auto& str : numbers) {
            std::cout << str <<' ';
        }
        std::cout<<std::endl;

        // find
        auto it = std::ranges::find(numbers, 4);
        if (it != std::ranges::end(numbers)) {
            std::cout << "4 found at index: " << std::distance(std::ranges::begin(numbers), it) << std::endl;
        } else {
            std::cout << "4 not found" << std::endl;
        }

        // count
        auto count = std::ranges::count(numbers, 4);
        std::cout << "count: " << count << std::endl;

        // for_each
        std::ranges::for_each(numbers, [](int n) { std::cout << n << ' '; });
        std::cout<<std::endl;
        
        // copy
        std::vector<int> numbers_copy(numbers.size());
        std::ranges::copy(numbers, numbers_copy.begin());
        show_container(numbers_copy);
        
        auto table = generate_9x9_multiplication_table();
        for (const auto& row : table) {
            std::cout << row << std::endl;
        }
        return 0;
    }
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

int main(int argc, char* argv[])
{
    //VIEWS::test();

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