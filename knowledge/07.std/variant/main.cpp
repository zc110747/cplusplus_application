/*
std::variant 是 C++17 标准库中引入的一个类型安全的联合体（union）替代品。
它允许你在一个变量中存储多种不同类型的值，并且在运行时可以安全地访问和操作这些值。
与传统的联合体不同，std::variant 会跟踪当前存储的是哪种类型的值，从而避免了类型不安全的问题。
*/
#include <vector>
#include <iostream>
#include <variant>
#include <type_traits>

using std::cout;
using std::endl;

// 处理 variant 中的值的函数
void processVariant(const std::variant<int, double, std::string>& v) {
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "int value: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << "double value: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "string value: " << arg << std::endl;
        }
    }, v);
}

// 从 variant 中获取值的函数
template <typename T>
T getValue(const std::variant<int, double, std::string>& v) {
    return std::get<T>(v);
}

int main(int argc, char* argv[])
{
    std::variant<int, double, std::string> v; // 默认构造，存储第一个类型（int）的值，初始化为 0
    std::variant<int, double, std::string> v2 = 42; // 存储 int 类型的值 42
    std::variant<int, double, std::string> v3 = 3.14; // 存储 double 类型的值 3.14
    std::variant<int, double, std::string> v4 = "Hello"; // 存储 std::string 类型的值 "Hello"

    auto i = std::get<int>(v2); // 获取存储的 int 值
    cout<<i<<endl;

    auto d = std::get<double>(v3); // 获取存储的 double 值
    cout<<d<<endl;

    auto s = std::get<std::string>(v4); // 获取存储的 std::string 值
    cout<<s<<endl;

    v = 3.14; // 修改为 double 类型的值
    std::cout<<std::get<double>(v)<<std::endl;

    v = "Hello"; // 修改为 std::string 类型的值
    std::cout<<std::get<std::string>(v)<<std::endl;

    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "int value: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << "double value: " << arg << std::endl;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "string value: " << arg << std::endl;
        }
    }, v);

    // 使用 std::get_if 来安全地获取 variant 中的值
    if (const int* pval = std::get_if<int>(&v2)) {
        std::cout << "v1 contains int: " << *pval << std::endl;
    } else if (const double* pval = std::get_if<double>(&v2)) {
        std::cout << "v1 contains double: " << *pval << std::endl;
    } else if (const std::string* pval = std::get_if<std::string>(&v2)) {
        std::cout << "v1 contains string: " << *pval << std::endl;
    }

    // 使用 std::monostate 来表示 variant 为空
    std::variant<std::monostate, int, double, std::string> v5;
    if (std::holds_alternative<std::monostate>(v5)) {
        std::cout << "v5 is empty" << std::endl;
    }

    // 使用 std::variant 作为容器元素
    std::vector<std::variant<int, double, std::string>> vec = {42, 3.14, "Hello, variant!"};
    for (const auto& v : vec) {
        processVariant(v);
    }

    // 使用 std::holds_alternative 来处理容器中的 variant 元素
    for (const auto& v : vec) {
        if (std::holds_alternative<int>(v)) {
            std::cout << "int value: " << getValue<int>(v) << std::endl;
        } else if (std::holds_alternative<double>(v)) {
            std::cout << "double value: " << getValue<double>(v) << std::endl;
        } else if (std::holds_alternative<std::string>(v)) {
            std::cout << "string value: " << getValue<std::string>(v) << std::endl;
        }
    }
    return 0;
} 
