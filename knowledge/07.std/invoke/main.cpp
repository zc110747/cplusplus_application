/*
std::invoke
std::invoke 是 C++17 引入的一个标准库函数模板，位于 <functional> 头文件中。它提供了一种统一的方式来调用可调用对象，包括函数、函数指针、成员函数指针、成员对象指针、lambda 表达式等。
*/

#include <iostream>
#include <functional>

void base_function() {
    std::cout << "base_function called." << std::endl;
}

int add(int a, int b) {
    return a + b;
}

class MyClass {
public:
    void memberFunction() {
        std::cout << "Member function called." << std::endl;
    }
    int memberValue = 42;
};

int main() 
{
    // 调用普通函数
    std::invoke(base_function);

    // 调用带参数函数
    int result = std::invoke(add, 3, 5);
    std::cout << "Result of add: " << result << std::endl;

    // 访问成员函数
    MyClass obj;
    std::invoke(&MyClass::memberFunction, obj);

    // 使用成员对象指针获取成员值
    int value = std::invoke(&MyClass::memberValue, obj);
    std::cout << "member value: " << value << std::endl;

    // 使用 lambda 表达式
    auto lambda = [](int x) { return x * 2; };
    int lambdaResult = std::invoke(lambda, 10);
    std::cout << "Result of lambda: " << lambdaResult << std::endl;

    return 0;
}