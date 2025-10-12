# exception

C++ 异常处理是一种机制，用于在程序运行时处理错误和异常情况。它允许程序在遇到错误时抛出异常，并在适当的地方捕获和处理这些异常，从而避免程序崩溃或产生不可预测的行为。

- [异常处理基本语法](#except_grammer)
- [throw关键字](#throw)
- [异常传递](#noexcept)
- [系统定义异常](#execpt_class)
- [用户自定义异常](#user_define_except)
- [std::expected(c++20)](#expected)

## except_grammer

异常的基本语法如下。

```cpp
try {
    // 可能会抛出异常的代码
} catch (ExceptionType1& e) {
    // 处理 ExceptionType1 类型的异常
} catch (ExceptionType2& e) {
    // 处理 ExceptionType2 类型的异常
} catch (std::exception& e) {
    // 处理其他类型的异常
}
```

具体示例如下。

```cpp
#include <iostream>

int main(int argc, char* argv[]) 
{
    try {
        // 可能会抛出异常的代码
        throw std::runtime_error("error occurred");
    }
    catch (std::runtime_error& e) {
        // 处理 exception
        std::cout << "runtime_error: " << e.what() << std::endl;   
    }
    catch (std::exception& e) {
        // 处理 exception
        std::cout << "exception: " << e.what() << std::endl;
    }
}
```

## throw

throw关键字，在C++中，throw关键字用于抛出异常。throw关键字后面跟着一个表达式，该表达式的值将作为异常对象被抛出。异常对象可以是任何类型，但通常是std::exception以及从此类派生的类的实例。

```cpp
#include <iostream>
#include <string>

class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return message.c_str();
    }

    MyException(const std::string& message) : message(message) {}
private:
    std::string message{""};
};

int main(int argc, char* argv[])
{
    // 抛出自定义异常
    try {
        throw MyException("user defined exception");    
    } catch (MyException e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

    // 抛出内置类型异常
    try {
        throw 1;
    } catch (int e) {
        std::cout << "exception: " << e << std::endl;
    }
    return 0;
}
```

## noexcept

在C++中，异常传递是指在函数调用链中，异常从抛出点向上传递，直到被捕获并处理的过程。在C++中，noexcept关键字用于指定一个函数是否可能抛出异常。它是C++11引入的一个特性，用于优化代码和提高程序的健壮性；根据noexcept关键字的使用情况，编译器可以对代码进行优化。例如，对于不会抛出异常的函数，编译器可以进行一些优化。

- noexcept：声明函数不会抛出任何异常，编译器可以对其进行优化；并不保证一定会不发生异常，如果发生异常，则程序将调用std::terminate()函数，程序将终止运行。
- noexcept(true/false)：表示函数是否抛出异常取决于expression的值。如果expression为true，则声明函数不会抛出异常；如果expression为false，则函数可能抛出异常。
- noexcept(expression): 计算函数或者表达式执行是否会产生异常，结果是true或者false。可以和上面语句组合起来使用，格式为noexcept(noexcept(expression))。

对于C++11来说，标准规定类中以下函数默认为noexcept(ture)。

1. 默认构造函数、默认复制构造函数、默认赋值函数、默认移动构造函数和默认移动赋值函数的类型为noexcept(true)。
2. 类型的析构函数以及delete运算符默认带有noexcept声明，请注意即使自定义实现的析构函数也会默认带有noexcept声明，除非类型本身或者其基类和成员明确使用noexcept(false)声明析构函数。

具体示例如下所示。

```cpp
#include <iostream>
#include <type_traits>

void func1() noexcept {
    throw std::runtime_error("func1 exception");
}

void func2() noexcept(false) {
    throw std::runtime_error("func2 exception");
}

// 声明为int时，不会抛出异常
template<typename T>
void func3(T t) noexcept(std::is_same<T, int>::value) {

    if constexpr (std::is_same<T, int>::value) {
        std::cout << "func3<int>()" << std::endl;
    } else {
        std::cout << "func3<T>()" << std::endl;
        throw std::runtime_error("func3 exception");
    }
}

template<typename T>
void func4(T t) noexcept(noexcept(T(t))) {
    throw std::runtime_error("func4 exception");
}

int do_something() {
    std::cout << "do_something()" << std::endl;
    return 1;
}
void func5() noexcept(noexcept(do_something())) {
    do_something();
}

int main(int argc, char* argv[])
{
    try {
        //func1();              // 执行报错中止，而非继续执行
    } catch (std::runtime_error& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

    try {
        func2();                // 执行继续，捕获异常并打印
    } catch (std::runtime_error& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

    try {
        func3<int>(1);           // 执行继续，捕获异常并打印
    } catch (std::runtime_error& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

    try {
        func5();                // 执行继续，不产生异常
    } catch (std::runtime_error& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

    try {
        std::cout << std::boolalpha;
        std::cout << noexcept(int(1)) << std::endl;
        func4<int>(1);           // 执行报错中止，而非继续执行
    } catch (std::runtime_error& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}
```

## execpt_class

1. std::exception：所有标准异常类的基类，提供了一个虚函数what()，用于返回异常的描述信息
2. std::bad_alloc：当new操作符无法分配内存时抛出。
3. std::bad_cast: 当dynamic_cast转换失败时抛出
4. std::bad_typeid: 当typeid操作符无法获取类型信息时抛出
5. std::bad_function_call: 当std::function对象无法调用时抛出
6. std::invalid_argument: 当传递给函数的参数无效时抛出
7. std::out_of_range: 当访问容器或数组时超出范围时抛出
8. std::length_error: 当字符串长度超过允许的最大值时抛出
9. std::logic_error: 当逻辑错误发生时抛出
10. std::runtime_error: 当运行时错误发生时抛出
11. std::overflow_error: 当数值溢出时抛出
12. std::underflow_error: 当数值下溢时抛出
13. std::system_error: 当系统错误发生时抛出

这里举部分例子说明。

```cpp
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    //std::length_error
    try
    {
        std::vector<int> v(100 + static_cast<size_t>(-1) / sizeof(int)); // 这里会抛出std::length_error异常
    }
    catch (const std::length_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }  
}
```

## user_define_except

用户自定义异常类，需要继承自std::exception类，并重写what()方法。

```cpp
#include <iostream>
#include <string>

class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return message.c_str();
    }

    MyException(const std::string& message) : message(message) {}
private:
    std::string message{""};
};

int main(int argc, char* argv[])
{
    try {
        throw MyException("user defined exception");    
    } catch (MyException e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
    return 0;
}
```

## expected

std::expected是 C++23 标准库引入的一个模板类，定义在 <expected> 头文件中。它用于表示一个操作可能成功返回一个值，也可能失败并返回一个错误信息，为处理函数可能失败的情况提供了一种更安全、更清晰的方式，替代了传统的错误码或者异常处理机制。

```cpp
//-std=c++23
#include <iostream>
#include <expected>

// 模拟一个可能失败的函数
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}

int main(int argc, char* argv[]) 
{
    // 调用可能失败的函数
    std::expected<int, std::string> result = divide(10, 2);

    // 检查结果是否成功
    if (result.has_value()) {
        std::cout << "Result: " << result.value() << std::endl;
    } else {
        std::cout << "Error: " << result.error() << std::endl;
    }
}
```
