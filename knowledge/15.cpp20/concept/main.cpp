/*
concept和reguires
C++20中，concept 和 requires 是两个重要的特性，它们用于在编译时检查模板参数是否满足特定的要求。
concepts 是一种用于描述类型要求的语法，而 requires 则是用于在模板定义中指定这些要求的关键字。
*/
#include <iostream>
#include <vector>
#include <concepts>
#include <algorithm>

// 定义一个 concept，要求类型 T 必须支持加法操作
template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

// 使用 concept 约束模板参数
template <Addable T>
T add(T a, T b) {
    return a + b;
}


int main() 
{
    int x = 5, y = 10;
    double a = 3.14, b = 2.71;

    // 调用 add 函数，传入 int 类型参数
    std::cout << "Sum of integers: " << add(x, y) << std::endl;

    // 调用 add 函数，传入 double 类型参数
    std::cout << "Sum of doubles: " << add(a, b) << std::endl;

    // 以下代码会导致编译错误，因为 std::string 类型不支持加法操作
    // std::string s1 = "Hello", s2 = "World";
    // std::cout << "Concatenated string: " << add(s1, s2) << std::endl;


    return 0;
}
