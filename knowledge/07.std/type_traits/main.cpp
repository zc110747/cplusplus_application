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
#include <iostream>
#include <type_traits>

// 模板函数，仅接受整数类型的参数
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
void printInteger(T value) {
    std::cout << "Integer value: " << value << std::endl;
}

// 模板函数，根据传入的类型选择不同的行为
template <typename T>
void process(T value) {
    // 使用 std::is_same 来判断类型是否为 int
    if constexpr (std::is_same_v<T, int>) {
        std::cout << "Processing an integer: " << value << std::endl;
    }
    // 使用 std::is_same 来判断类型是否为 double
    else if constexpr (std::is_same_v<T, double>) {
        std::cout << "Processing a double: " << value << std::endl;
    }
    // 使用 std::conditional 来选择不同的行为
    else {
        using result_type = typename std::conditional<std::is_floating_point_v<T>, double, int>::type;
        result_type result = static_cast<result_type>(value);
        std::cout << "Processing a generic type: " << result << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::cout << "is_integral<int>: " << std::is_integral<int>::value << std::endl;
    std::cout << "is_floating_point<float>: " << std::is_floating_point<float>::value << std::endl;
    std::cout << "is_pointer<int*>: " << std::is_pointer<int*>::value << std::endl;
    std::cout << "is_reference<int&>: " << std::is_reference<int&>::value << std::endl;
    std::cout << "is_class<std::vector<int>>: " << std::is_class<std::vector<int>>::value << std::endl;
    std::cout << "is_union<std::pair<int, float>>: " << std::is_union<std::pair<int, float>>::value << std::endl;
    std::cout << "is_enum<std::ios_base::fmtflags>: " << std::is_enum<std::ios_base::fmtflags>::value << std::endl;
    std::cout << "sizeof(int): " << sizeof(int) << std::endl;
    std::cout << "alignof(int): " << alignof(int) << std::endl;

    int a = 42;
    double b = 3.14;
    char c = 'A';

    printInteger(a); // 正确，a 是整数类型
    // printInteger(b); // 错误，b 不是整数类型

    process(a); // 处理整数
    process(b); // 处理浮点数
    process(c); // 处理其他类型
    return 0;
} 
