/*
std::type_traits 是 C++ 标准库中的一个模板库，它提供了一系列的模板类和模板函数，用于在编译时获取和操作类型的特性。
这些特性包括类型是否是整数、浮点数、指针、引用、类、联合体、枚举等，
以及类型的大小、对齐方式、是否有默认构造函数、是否有拷贝构造函数、是否有移动构造函数等。

std::type_traits 中的模板类和模板函数都是通过模板特化和模板元编程技术实现的，
它们可以在编译时进行类型检查和类型转换，从而提高代码的效率和安全性

std::type_traits 中常用的模板类和模板函数：
1. std::is_integral<T>：判断类型 T 是否是整数类型。
2. std::is_floating_point<T>：判断类型 T 是否是浮点数类型。
3. std::is_pointer<T>：判断类型 T 是否是指针类型。
4. std::is_reference<T>：判断类型 T 是否是引用类型。
5. std::is_class<T>：判断类型 T 是否是类类型。
6. std::is_union<T>：判断类型 T 是否是联合体类型。
7. std::is_enum<T>：判断类型 T 是否是枚举类型。
8. std::is_same<T, U>：判断类型 T 和 U 是否相同。
9. std::enable_if<B, T>：根据条件 B 来选择是否启用类型 T。
10. std::conditional<B, T, F>：根据条件 B 来选择类型 T 或类型 F。
*/
    #include <vector>
    #include <iostream>
#include <type_traits>

using std::cout;
using std::endl;

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
