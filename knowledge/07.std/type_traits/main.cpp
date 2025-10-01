//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::enable_if
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <map>
#include <iostream>
#include <tuple>
#include <type_traits>

namespace TYPE_CATEGORY
{
    class Test
    {
    public:
        Test() = default;
    };

    int test(void)
    {
        std::cout << "========================== TYPE CATEGORY ==========================" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "is_integral<int>: " << std::is_integral<int>::value << std::endl;
        std::cout << "is_floating_point<float>: " << std::is_floating_point<float>::value << std::endl;
        std::cout << "is_pointer<int*>: " << std::is_pointer<int*>::value << std::endl;
        std::cout << "is_reference<int&>: " << std::is_reference<int&>::value << std::endl;
        std::cout << "is_class<std::vector<int>>: " << std::is_class<std::vector<int>>::value << std::endl;
        std::cout << "is_union<std::pair<int, float>>: " << std::is_union<std::pair<int, float>>::value << std::endl;
        std::cout << "is_enum<std::ios_base::fmtflags>: " << std::is_enum<std::ios_base::fmtflags>::value << std::endl;
  
        int arr1[10];
        std::cout << "is_array<arr1>: " << std::is_array<decltype(arr1)>::value << std::endl;

        Test t1;
        std::cout << "is_class<Test>: " << std::is_class<decltype(t1)>::value << std::endl;

        int v1 = 0;
        int &lv1 = v1;
        int &&rv1 = std::move(v1);

        std::cout << "is_lvalue_reference<int&>: " << std::is_lvalue_reference<decltype(lv1)>::value << std::endl;
        std::cout << "is_rvalue_reference<int&&>: " << std::is_rvalue_reference<decltype(rv1)>::value << std::endl;

        std::cout << "is_function<void(int)>: " << std::is_function<void(int)>::value << std::endl;
        std::cout << "is_pod<Test>: " << std::is_pod<Test>::value << std::endl;
        return 0;
    } 
}

namespace TEMPLATE_FEATURES
{
    template <typename T>
    void process(T value) {
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "Processing an integer: " << value << std::endl;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "Processing a string: " << value << std::endl;
        } else {
            // 使用std::conditional来选择不同的类型
            using result_type = typename std::conditional<std::is_floating_point_v<T>, double, int>::type;
            auto result = static_cast<result_type>(value);
            std::cout << "Processing a generic type: " << result << std::endl;
        }
    }

    // 模板参数默认值约束
    template <class T, class = typename std::enable_if<std::is_integral<T>::value>::type>
    void print_integer(T value) {
        std::cout << "Integer value: " << value << std::endl;
    }

    template <class T, typename = std::enable_if_t<std::is_integral_v<T>, void>>
    struct info {
        T val;
    };

    // 约束函数返回值
    template <class T>
    typename std::enable_if<std::is_integral<T>::value, T>::type // 限制为整数类型
    is_odd(T x) {
        return bool(x % 2);
    }

    template <class T>
    struct is_integral_type {
        static constexpr bool value = std::false_type::value;
    };

    template <>
    struct is_integral_type<int> {
        static constexpr bool value = std::true_type::value;
    };

    // 模板参数值
    template <class T>
    constexpr bool is_integral_type_v = is_integral_type<T>::value;

    template <class T>
    typename std::enable_if<is_integral_type_v<T>, T>::type test_odd(T x) {
        return bool(x % 2);
    }

    int test(void)
    {
        std::cout << "========================== TEMPLATE FEATURES ==========================" << std::endl;
        int a = 42;
        print_integer(a);
        //print_integer(10.123f); 异常，不符合限制

        std::tuple<int, float, std::string> t1(1, 2.0f, "hello");
        process(std::get<0>(t1));
        process(std::get<1>(t1));
        process(std::get<2>(t1));

        std::cout << "is_odd(3): " << is_odd(3) << std::endl;
        std::cout << "is_odd(4): " << is_odd(4) << std::endl;

        info<int> i1;

        std::cout << is_integral_type<int>::value << std::endl;
        std::cout << is_integral_type<float>::value << std::endl;
        std::cout << test_odd(3) << std::endl;
        std::cout << test_odd(4) << std::endl;
        return 0;
    }
}

int main(int argc, char* argv[])
{
    TYPE_CATEGORY::test();

    TEMPLATE_FEATURES::test();

    return 0;
} 
