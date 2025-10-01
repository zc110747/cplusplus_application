//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. concept定义和使用
//      2. requires表达式定义和使用
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
#include <concepts>
#include <type_traits>

namespace CONCEPTS
{   
    // 基本定义concept
    template <typename T>
    concept is_inter = std::is_integral_v<T>;

    // 复合定义concept，调用基本定义concept并组合
    template <typename T>
    concept is_signed_inter = is_inter<T> && std::is_signed_v<T>;

    // 自定义concept
    template <typename T>
    concept add_able = requires(T a, T b) {
        { a + b } -> std::convertible_to<T>;
    };

    // 模板函数约束
    template <is_signed_inter T>
    T mul_int(T &a, T &b) {
        return a * b;
    }

    template <add_able T>
    T add(T &a, T &b) {
        return a + b;
    }

    // 类模板约束
    template <is_inter T>
    class Interger
    {
    public:
        Interger(T value) : m_value(value) {}
        T get_value() const { return m_value; }
    private:
        T m_value;
    };

    int test(void)
    {
        int x = 5, y = 10;
        
        std::cout << "mul_int(x, y): " << mul_int<int>(x, y) << std::endl;

        std::cout << "add(x, y): " << add<int>(x, y) << std::endl;

        Interger<int> a(5);
        std::cout << "a.get_value(): " << a.get_value() << std::endl;
        return 0;
    }
}

namespace REQUIRES
{
    template <typename T>
    concept complex_concept = requires(T a, T b) {

        // 类型要求
        typename T::iterator;
        typename T::value_type;
        
        // 表达式要求(方法要求，返回值类型要求)
        { a.size() } -> std::convertible_to<std::size_t>;
        { a.begin() } -> std::convertible_to<typename T::iterator>;
        { a.end() } -> std::convertible_to<typename T::iterator>;
        { a.empty() } -> std::convertible_to<bool>;

        // 嵌套要求
        requires std::copy_constructible<T>;
        requires std::move_constructible<T>;
        requires sizeof(typename T::value_type) > 1;
    };

    template <complex_concept T>
    void show_container(T &container) {
        for (auto &item : container) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // requires支持直接约束，也支持通过concept进行组合约束
    template <typename T>
    requires std::integral<T> && (sizeof(T) >= 4)
    T max_value(T a, T b) {
        return (a > b) ? a : b;
    }

    // 支持通过reguires定义组合约束，并通过requires来进行管理函数和类
    template <typename T>
    requires requires(T a, typename T::value_type v) {
        { a.push_back(v) } -> std::convertible_to<void>;
        { a.size() } -> std::convertible_to<std::size_t>;
        { a.begin() } -> std::convertible_to<typename T::iterator>;
        { a.end() } -> std::convertible_to<typename T::iterator>;
    }
    void process_container(T &container) {
        for (auto con_iter = container.begin(); con_iter != container.end(); ++con_iter) {
            std::cout << *con_iter << " ";
        }
        std::cout << std::endl;
    }

    template <typename T>
    concept is_numeric = std::integral<T> || std::floating_point<T>;

    template <typename T>
    concept is_arithmetic = is_numeric<T> && requires(T a, T b) {
        { a + b } -> std::convertible_to<T>;
        { a - b } -> std::convertible_to<T>;
        { a * b } -> std::convertible_to<T>;
    };

    template <is_arithmetic T>
    T square(T a) {
        return a * a;
    }

    int test(void)
    {
        // vector<char> 不满足complex_concept，要求类型长度大于1

        // vector<int> 满足complex_concept
        std::vector<int> vec = {1, 2, 3, 4, 5};

        // T = std::vector<int>
        show_container(vec);

        std::cout << "max_value(5, 10): " << max_value<int>(5, 10) << std::endl;

        // T = std::vector<int>
        process_container(vec);

        // 限定square为is_arithmetic类型
        std::cout << "square(5): " << square<int>(5) << std::endl;
        return 0;
    }
}

int main() 
{
    CONCEPTS::test();

    REQUIRES::test();

    return 0;
}
