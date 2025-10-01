/*
在C++20中，ranges 是一个重要的特性，它提供了一种更简洁、更强大的方式来处理容器和序列。
ranges 库引入了一系列新的概念和算法，使得代码更加直观和易于理解

1. 范围(Ranges)
ranges 库的核心是范围（Ranges）的概念。
范围是一个可以迭代的对象，它可以是一个容器（如 std::vector、std::list 等），也可以是一个视图（View）。
视图是一种轻量级的对象，它不拥有数据，而是对现有数据的一种引用或转换。

2. 视图(Views)
视图是 ranges 库中的一个重要概念。
视图是一种轻量级的对象，它不拥有数据，而是对现有数据的一种引用或转换。
视图可以通过各种算法和操作符进行组合，形成复杂的操作链。

3. 算法(Algorithms)
ranges 库还引入了一系列新的算法，这些算法可以直接作用于范围，而不需要显式地指定迭代器。
这些算法的名称与标准库中的算法类似，但它们接受范围作为参数，而不是迭代器。

4. ranges 库引入了管道操作符（|），它允许你将多个视图和算法组合在一起，形成一个操作链。管道操作符使得代码更加简洁和易于理解。
*/
#include <iostream>
#include <vector>
#include <algorithm>

#if __cplusplus > 202002L
#include <ranges>

// 模板元编程：计算乘法表的一行
template<int N>
struct MultiplicationRow {
    static std::string value() {
        std::string row;
        for (int i = 1; i <= N; ++i) {
            row += std::to_string(i) + " x " + std::to_string(N) + " = " + std::to_string(i * N) + "\t";
        }
        return row;
    }
};

// 模板元编程：生成乘法表
template<int... Ns>
struct MultiplicationTable {
    static std::vector<std::string> value() {
        return { MultiplicationRow<Ns>::value()... };
    }
};

// 辅助函数：生成整数序列
template<int... Ns>
constexpr auto index_sequence = std::integer_sequence<int, Ns...>{};

// 辅助函数：生成乘法表
template<int N, int... Ns>
constexpr auto make_multiplication_table(index_sequence<Ns...>) {
    return MultiplicationTable<Ns...>::value();
}
#endif

int main(int argc, char* argv[])
{
#if __cplusplus > 202002L
    std::vector<int> numbers = {1, 4, 3, 6, 5, 2, 4, 1, 0, 8};

    // 使用视图过滤偶数并转换为字符串
    auto even_strings = numbers | std::views::filter([](int n) { return n % 2 == 0; })
                                | std::views::transform([](int n) { return std::to_string(n); });

    // 输出结果
    for (const auto& str : even_strings) {
        std::cout << str << ' ';
    }
    std::cout<<std::endl;

    std::ranges::sort(numbers);
    for (const auto& str : numbers) {
        std::cout << str <<' ';
    }
    std::cout<<std::endl;

    // 使用 ranges 库输出乘法表
    std::ranges::for_each(make_multiplication_table<9>(index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9>{}), [](const std::string& row) {
        std::cout << row << std::endl;
    });
#else
    std::cout << "C++20 or later is required for this example." << std::endl;
#endif
    return 0;
}