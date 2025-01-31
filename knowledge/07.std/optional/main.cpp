/*
std::optional 是 C++17 引入的一个标准库类，用于表示一个可能包含值的对象。它类似于其他语言中的 Maybe 或 Option 类型。
std::optional 可以用来避免使用空指针或特殊值（如 -1 或 nullptr）来表示缺失值

std::optional 提供了一种更安全、更清晰的方式来处理可能缺失的值，避免了使用特殊值或空指针带来的潜在问题。

成员函数：
emplace：在 std::optional 对象中构造一个值。
has_value：判断 std::optional 对象是否包含值。
reset：将 std::optional 对象中的值重置为未定义状态。
swap：交换两个 std::optional 对象中的值。
value：返回 std::optional 对象中包含的值。如果 std::optional 对象不包含值，则会抛出 std::bad_optional_access 异常。
value_or(default_value)：返回 std::optional 对象中包含的值，如果 std::optional 对象不包含值，则返回默认值 default_value。
*/
#include <iostream>
#include <optional>

std::optional<int> find_value(int value)
{
    if (value == 46) {
        return value;
    } else {
        return std::nullopt;
    }
}

int main(int argc, char *argv[]) 
{
    std::optional<int> opt1;
    std::optional<int> opt2 = 42;

    // has_value
    if (opt1.has_value()) {
        std::cout << "opt1 has value: " << opt1.value() << std::endl;
    } else {
        std::cout << "opt1 has no value" << std::endl;
    }

    //value
    if (opt2.has_value()) {
        std::cout << "opt2 has value: " << opt2.value() << std::endl;
    } else {
        std::cout << "opt2 has no value" << std::endl;
    }

    //vlaue_or
    std::cout << "opt1:" << opt1.value_or(0) << std::endl;
    std::cout << "opt2:" << opt2.value_or(0) << std::endl;

    opt1.swap(opt2);
    std::cout << "opt1:" <<  opt1.value_or(0) << std::endl;

    //reset
    opt2.reset();

    //emplace
    opt2.emplace(51);
    std::cout << "opt2:" << opt2.value_or(0) << std::endl;

    return 0;
}