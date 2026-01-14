//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. requires表达式定义和使用
//      2. concept定义和使用
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <type_traits>
#include <concepts>
#include <vector>
#include <algorithm>
#include <initializer_list>

namespace REQUIRES
{
    template <typename T>
    requires requires(T a, T b) {
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
    }
    void show_container(T &container) {
        for (auto &item : container) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // requires支持直接约束，也支持通过concept进行组合约束
    constexpr bool bar() {
        return true;
    }

    template <typename T>
    requires (bar()) &&std::integral<T> && (sizeof(T) >= 4)
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

    // 容器增加concepts约束
    template<typename T>
    class demo {
    public:
        demo(T t) : m_data(t) {
            std::cout << "demo constructor" << std::endl;
        }
        demo(T t) requires requires(T t){
            { t.size() } -> std::convertible_to<std::size_t>;
        }  : m_data(t) {
            std::cout << "demo constructor requires" << std::endl;
        }
    private:
        T m_data;
    };
    
    template <int N>
    requires (N > 0 && N <= 10)
    class demo1 {
    public:
        demo1() {
            std::cout << "demo1 constructor:" << N << std::endl;
        }
    private:
        int data[N];
    };

    int test(void)
    {
        std::cout << "============== REQUIRES ==============" << std::endl;

        // vector<char> 不满足complex_concept，要求类型长度大于1

        // vector<int> 满足complex_concept
        std::vector<int> vec = {1, 2, 3, 4, 5};

        // T = std::vector<int>
        show_container(vec);

        std::cout << "max_value(5, 10): " << max_value<int>(5, 10) << std::endl;

        // T = std::vector<int>
        process_container(vec);

        demo<int> d1(10);
        demo<std::vector<int>> d2(vec);

        demo1<5> d3;
        return 0;
    }
}

namespace CONCEPTS
{
    template <typename T> 
    concept limit_integer = std::is_integral_v<T>;

    template <typename T>
    concept limit_floating =  std::is_floating_point_v<T>;

    // 限定print_integer只能用于整数类型
    template <limit_integer T>
    void print_nums(T t) {
        std::cout << "print_nums: " << t << std::endl;
    }

    // 限定print_floating只能用于浮点数类型
    template <limit_floating T>
    void print_nums(T t) {
        std::cout << "print_floating: " << t << std::endl;
    }

    // && 表示同时满足两个requires表达式
    // || 表示满足其中一个requires表达式即可
    template <typename T>
    concept is_printable = requires(T t) {
        { std::cout << t } -> std::same_as<std::ostream&>;
    };

    template <typename T>
    concept is_addable = requires(T t) {
        { t + t } -> std::convertible_to<T>;
    };

    // 同时满足is_printable和is_addable
    template <typename T>
    concept is_printable_and_addable = is_printable<T> && is_addable<T>;

    template <is_printable_and_addable T>
    T add_func(T t1, T t2) {
        return t1 + t2;
    }

    // 限定T只能用于可打印的类型
    template <is_printable T>
    class add_demo {
    public:
        using type_value = T;

        add_demo(int a) : val_(a) {}

        add_demo operator+(const add_demo& other) const {
            return add_demo(val_ + other.val_);
        }

        T get_val(void) const {
            return val_;
        }

        T operator()(void) const {
            return val_;
        }
    private:
        T val_;
    };

    template <is_printable T>
    std::ostream& operator<<(std::ostream& os,  add_demo<T>& demo) {
        os << demo.get_val();
        return os;
    };

    // requires定义组合表达式
    // || 和 && 可以组合多个requires表达式
    // concept可以用组合表达式来定义模板类型
    template <typename T>
    concept is_for_extend = requires(T t) {
        // 支持begin和end成员函数，基础类型
        { t.begin() } -> std::same_as<typename T::type_value *>;
        { t.end() } -> std::same_as<typename T::type_value *>;
    } || requires(T t) {
        // 支持begin和end成员函数，容器类型
        { t.begin() } -> std::same_as<typename T::iterator>;
        { t.end() } -> std::same_as<typename T::iterator>;

        // 添加operator++和++operator
        ++std::declval<typename T::iterator>(); // 前置operator++
        std::declval<typename T::iterator>()++; // 后置operator++
        
        // 支持解引用operator*
        *std::declval<typename T::iterator>(); // 解引用operator*
    };

    template <is_for_extend T>
    void print_for_extend(T t) {
        for (const auto& e : t) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }

    template <typename T>
    class for_demo {
    public:
        // 定义类型别名，用于表示元素类型
        using type_value = T;

        // 支持列表初始化
        for_demo(std::initializer_list<int> il) {
            std::copy_n(il.begin(), std::min(il.size(), sizeof(m_data) / sizeof(m_data[0])), m_data);
        }
        T *begin() { 
            return m_data;
        }
        T *end() { 
            return m_data + 5; 
        }

    private:
        T m_data[5];
    };

    template <typename T>
    concept is_operator_run = requires(T t) {
        // 支持operator()
        { t() } -> std::convertible_to<typename T::type_value>;
    };
    
    template <is_operator_run T>
    void run_operator(T t) {
        std::cout << "run_operator: " << t() << std::endl;
    }

    // 用于可变参数模板, 打印多个参数
    template <is_printable T, is_printable ...Args>
    void print(T first, Args... args) {
        std::cout << first << " ";
        if constexpr (sizeof...(args) > 0) {
            print(args...);
        } else {
            std::cout << std::endl;
        }
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

    // 约束auto类型
    template <typename T>
    concept limit_integral = std::is_integral_v<T>;

    template <typename T>
    concept limit_iterator = requires(T t) {
        ++std::declval<T>(); // 前置operator++
        std::declval<T>()++; // 后置operator++

        *std::declval<T>();  // 解引用operator*
    };

    limit_integral auto i1 = 10;

    int test(void) {
        std::cout << "============== CONCEPTS ==============" << std::endl;

        // 基于concepts和SFINAE实现打印类型限制
        print_nums(10);
        print_nums(3.14);

        // 基于concept和requires实现add_demo类的加法操作
        add_demo<int> a(10);
        add_demo<int> b(20);
        auto c = add_func(a, b);
        std::cout << "c: " << c << std::endl;
        run_operator(a);
        run_operator(b);

        // 基于concept和requires实现现在for扩展支持
        std::vector<int> v = {1, 2, 3, 4, 5};
        std::cout << "v: ";
        print_for_extend(v);

        for_demo<int> fd = {1, 2, 3, 4, 5};
        std::cout << "fd: ";
        print_for_extend<for_demo<int>>(fd);

        auto add_result = add_func(1, 2);
        std::cout << "add_result: " << add_result << std::endl;

        print(1, "test", 1.5);

        // 限定square为is_arithmetic类型
        std::cout << "square(5): " << square<int>(5) << std::endl;

        // concept可以作为auto的类型约束
        std::cout << "i1: " << i1 << std::endl;
        auto func = [](auto a, auto b) -> limit_integral auto {
            return a + b;
        };
        std::cout << "func(1, 2): " << func(1, 2) << std::endl;

        limit_iterator auto iter = v.begin();
        std::cout << "*iter: " << *iter << std::endl;
        return 0;
    }
}

int main(int argc, const char* argv[]) 
{
    REQUIRES::test();
    
    CONCEPTS::test();

    return 0;
}
