/*
1. auto类型推导占位符
auto关键字用于自动类型推导，它可以根据变量的初始化表达式来推断变量的类型。auto通常用于简化复杂类型的声明，特别是在使用模板或迭代器时
不过对于auto来说也有些限制:
- 不能作函数的形参推导 void fun(auto i = 1){}
- 对于结构体来说，非静态成员不能用auto. struct str{auto var = 10;}
- 不能声明auto数组 auto d[3] = {1, 2, 3};
- auto不能用作模板参数 vector<auto> demo{1, 2, 3, 4};

2. decltype类型推导表达式
decltype关键字用于获取表达式的类型，而不是变量的类型。它可以用于声明函数返回值的类型，或者在模板中推导类型参数的类型
对于decltype(e)来获取类型时，编译器将依序判断以下4个规则：
- 如果e时一个没有带括号的标记符表达式或者类成员访问表达式，那么decltype就是e所命名的实体的类型，另外，如果e是一个被重载的函数，则会导致编译时错误。
- 否则，假如e的类型是T，如果e是一个将亡值(xvalue), 那么decltype(e)为T&&
- 否则，假如e的类型是T，如果e是一个左值，则decltype(e)为T&
- 否则， 假如e的类型是T，则decltype(e)为T

3. 函数返回类型后置语法
在C++11之前，函数的返回类型必须在函数名之前声明。C++11引入了函数返回类型后置语法，允许在函数参数列表之后声明返回类型。
这种语法通常与auto和decltype结合使用，特别是在模板编程中
*/
#include <iostream>
#include <vector>

auto add(int a, int b) -> int {
    return a + b;
}

// 使用decltype推导返回类型
template <typename T, typename U>
auto multiply(T t, U u) -> decltype(t * u) {
    return t * u;
}

int main(int argc, char* argv[]) 
{   
    // auto类型推导占位符
    auto x = 42; // x的类型被推导为int
    auto y = 3.14; // y的类型被推导为double
    auto z = "hello"; // z的类型被推导为const char*

    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "z: " << z << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5};

    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // decltype类型推导表达式
    int a = 10;
    double b = 3.14;
    decltype(a + b) c = a + b; // c的类型被推导为double
    std::cout << "c: " << c << std::endl;

    auto sum_val = add(1, 2); // sum_val的类型被推导为int
    std::cout << "sum_val: " << sum_val << std::endl;

    auto product = multiply(3, 4.5); // product的类型被推导为double
    std::cout << "product: " << product << std::endl;

    return 0;
}
