//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. constexpr定义变量，函数
//      2. constexpr支持if/switch语句
//      3. if constexpr
//      4. constexpr lambda表达式
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <type_traits>
#include <memory>
#include <variant>

namespace CONSTEXPR_BASIC
{
    constexpr int val = 2;
    const int val1 = 2;
    constexpr int val2 = val1;
    constexpr int val3 = val2;

    template<class T>
    constexpr auto add(T x, T y) -> decltype(x+y)
    {
        return x+y;
    }
    constexpr int ADD_SUM = add<int>(1, 2);

    // 递归，支持if/switch语句，C++14
    constexpr int fubonacci(int n) {
        if (n == 1 || n == 2) {
            return 1;
        }
        return fubonacci(n-1) + fubonacci(n-2);
    }

    // switch 编译期switch条件判断
    template <int N>
    constexpr int get_result() {
        switch (N) {
            case 1:
                return 10;
            case 2:
                return 20;
            default:
                return 0;
        }
    }

    //constexpr, 结构体和类
    struct const_type
    {
        static constexpr int number1_ = 2;
        inline static const int number2_ = 2;
    };

    template <long num>
    struct Fibonacci{
        static constexpr long val = Fibonacci<num-1>::val + Fibonacci<num-2>::val;
    };
    template<> struct Fibonacci<2> {static constexpr long val = 1;};
    template<> struct Fibonacci<1> {static constexpr long val = 1;};


    int test(void)
    {
        std::cout << " ====================== CONSTEXPR_BASIC ======================" << std::endl;

        std::cout << "val: " << val << std::endl;
        std::cout << "val1: " << val1 << std::endl;
        std::cout << "val2: " << val2 << std::endl;
        std::cout << "val3: " << val3 << std::endl;

        int arr1[val1];
        int arr2[val2];

        std::cout << "arr1: " << sizeof(arr1)/sizeof(int) << std::endl;
        std::cout << "arr2: " << sizeof(arr2)/sizeof(int) << std::endl;

        int arr3[ADD_SUM];
        int arr4[add<int>(1, 2)];
        std::cout << "arr3: " << sizeof(arr3)/sizeof(int) << std::endl;
        std::cout << "arr4: " << sizeof(arr4)/sizeof(int) << std::endl;

        int arr5[fubonacci(10)];
        std::cout << "arr5: " << sizeof(arr5)/sizeof(int) << std::endl;
        std::cout << "Fibonacci<10>::val: " << Fibonacci<10>::val << std::endl;
        
        std::cout << "const_type::number1_: " << const_type::number1_ << std::endl;
        std::cout << "const_type::number2_: " << const_type::number2_ << std::endl;

        int arr6[get_result<1>()];
        std::cout << "arr6: " << sizeof(arr6)/sizeof(int) << std::endl;

        return 0;
    }
}

namespace CONSTEXPR_TEMPLATE
{ 
    // 模板函数和constexpr
    template<int First>
    constexpr int add_sum(void)
    {
        return First;
    }
    template<int First, int Second, int... Rest>
    constexpr int add_sum(void)
    {
        return add_sum<First>() + add_sum<Second, Rest...>();
    }
    constexpr int ADD_SUM = add_sum<1, 2, 3>();

    // 模板参数包展开
    template<int... Nums>
    struct mul_all_
    {
        static constexpr int value = (Nums * ...);
    };

    // 模板参数包展开
    template<int First, int... Rest>
    struct add_all_;

    template<int First>
    struct add_all_<First>:std::integral_constant<int, First>
    {};

    template<int First, int Second, int... Rest>
    struct add_all_<First, Second, Rest...>:std::integral_constant<int, First + add_all_<Second, Rest...>::value>
    {};

    //C++14放宽限制
    // 支持局部变量和赋值语句
    constexpr int sum_nums(int n)
    {
        int result = 0;
        for (int i = 1; i <= n; i++) {
            result += i;
        }
        return result;
    }

    //模板特化
    template <typename T>
    struct is_pointer {
        static constexpr bool value = false;
    };

    template <typename T>
    struct is_pointer<T*> {
        static constexpr bool value = true;
    };

    template <typename T>
    struct is_pointer<std::unique_ptr<T>> {
        static constexpr bool value = true;
    };

    int test(void)
    {
        std::cout << " ====================== CONSTEXP_TEMPLATE ======================" << std::endl;

        int arr1[add_sum<1, 2, 3>()];
        int arr2[ADD_SUM];
        int arr3[mul_all_<1, 2, 3>::value];

        std::cout << "arr1: " << sizeof(arr1)/sizeof(int) << std::endl;
        std::cout << "arr2: " << sizeof(arr2)/sizeof(int) << std::endl;
        std::cout << "arr3: " << sizeof(arr3)/sizeof(int) << std::endl;

        std::cout << "mul_all_<1, 2, 3>::value: " << mul_all_<1, 2, 3>::value << std::endl;
        std::cout << "add_all_<1, 2, 3>::value: " << add_all_<1, 2, 3>::value << std::endl;

        std::unique_ptr<char[]> up0{new char[sum_nums(10)]};
        up0[0] = 'a';
        std::cout << "up0: " << up0[0] << std::endl;

        std::cout << "is_pointer<int>::value: " << is_pointer<int>::value << std::endl;
        std::cout << "is_pointer<int*>::value: " << is_pointer<int*>::value << std::endl;
        std::cout << "is_pointer<char*>::value: " << is_pointer<std::unique_ptr<int>>::value << std::endl;
        return 0;
    }
}

namespace CONSTEXPR_EXTEND
{ 
    void show_variant(std::variant<int, double, std::string>& v)
    {
        auto func = [](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
                std::cout << "int value: " << arg << std::endl;
            } else if constexpr (std::is_same_v<T, double>) {
                std::cout << "double value: " << arg << std::endl;
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "string value: " << arg << std::endl;
            }
        };
        std::visit(func, v);
    }

    // if constexpr 编译期if条件判断
    template <typename T>
    constexpr auto get_mul_int(T value) {
        if constexpr (std::is_integral_v<T>) {
            return value * 2;
        } else {
            return value;
        }
    }

    struct Point {
        int x;
        int y;

        // C++14 constexpr 成员函数可修改非 const 成员
        constexpr void move(int dx, int dy) {
            x += dx;
            y += dy;
        }
    };

    int test(void)
    {
        std::cout << " ====================== CONSTEXPR_EXTEND ======================" << std::endl;

        std::variant<int, double, std::string> v0{1};
        show_variant(v0);

        std::cout << "get_mul_int(1): " << get_mul_int(1) << std::endl;
        std::cout << "get_mul_int(1.0): " << get_mul_int(1.0) << std::endl;

        // if constexpr
        int arr1[get_mul_int(1)];
        std::cout << "arr1: " << sizeof(arr1)/sizeof(int) << std::endl;

        // constexpr指定结构体内部函数
        constexpr Point p1{1, 2};
        constexpr Point p2 = [&] {
            Point p = p1;
            p.move(1, 2);
            return p;
        }();
        int arr2[p2.x];
        std::cout << "arr2: " << sizeof(arr2)/sizeof(int) << std::endl;
        
        // constexpr lambda
        constexpr auto add = [](int x, int y) constexpr {
            return x + y;
        };
        static_assert(add(1, 2) == 3, "add(1, 2) should be 3");

        // constexpr lambda
        constexpr auto add1 = [](int x, int y) {
            auto x_func = [&] {return x;};
            auto y_func = [&] {return y;};
            return x_func() + y_func();
        };
        static_assert(add1(1, 2) == 3, "add1(1, 2) should be 3");

        return 0;
    }
}

int main(int argc, char *argv[])
{
    CONSTEXPR_BASIC::test();

    CONSTEXPR_TEMPLATE::test();

    CONSTEXPR_EXTEND::test();

    return 0;
}
