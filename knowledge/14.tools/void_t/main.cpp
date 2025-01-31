/************************************************************************************
 * std::void_t
 * void_t 是一个C++中的模板元编程工具，通常用于检查某个类型是否具有特定的成员函数或类型别名.
 * declval 用于在不创建对象实例的情况下获取类型的右值引用，std::declval<class T>()
 * decltype 是 C++11 引入的一个关键字，用于在编译时推导出表达式的类型
***********************************************************************************/
#include <iostream>
#include <vector>
#include <type_traits>

// 定义一个辅助结构体来检查类型 T 是否有 member_function 成员函数
template <typename T, typename = void>
struct has_member_function : std::false_type {};

// 特化版本，当 T 有 member_function 成员函数时匹配
template <typename T>
struct has_member_function<T, std::void_t<decltype(std::declval<T>().member_function())>> : std::true_type {};

template <typename T, typename = void>
struct has_member_type: std::false_type {};

template <typename T>
struct has_member_type<T, std::void_t<typename T::example_type>> : std::true_type {};

// 示例类，有一个 member_function 成员函数
struct ExampleClass {

    using example_type = std::vector<int>;

    int member_function() {
        return 0;
    }
};

// 示例类，没有 member_function 成员函数
struct AnotherClass {
};

int main() {
    // 检查 ExampleClass 是否有 member_function 成员函数
    std::cout << std::boolalpha;
    std::cout << "ExampleClass has member_function: " << has_member_function<ExampleClass>::value << std::endl;
    std::cout << "ExampleClass has member_type: " << has_member_type<ExampleClass>::value << std::endl;

    // 检查 AnotherClass 是否有 member_function 成员函数
    std::cout << "AnotherClass has member_function: " << has_member_function<AnotherClass>::value << std::endl;
    std::cout << "ExampleClass has member_type: " << has_member_type<AnotherClass>::value << std::endl;
    return 0;
}