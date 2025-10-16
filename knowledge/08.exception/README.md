# C++异常处理机制

C++ 异常处理是一种机制，用于在程序运行时处理错误和异常情况。它允许程序在遇到错误时抛出异常，并在适当的地方捕获和处理这些异常，从而避免程序崩溃或产生不可预测的行为。

- [异常处理基本语法](#except_grammer)
- [throw关键字](#throw)
- [exception](#exception)
- [异常传递](#noexcept)
- [系统定义异常](#execpt_class)
- [用户自定义异常](#user_define_except)
- [std::expected(c++20)](#expected)
- [system_error(c++11)](#system_error)

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

## exception

exception是C++中处理异常的一系列功能的组合，用于表示异常。异常是一种运行时错误，通常由程序员错误或外部环境错误导致。异常对象包含错误信息，用于描述异常的原因。

exception参考网址: <https://en.cppreference.com/w/cpp/header/exception.html>

对于exception，支持一下类型说明。

- std::exception：所有异常的基类。
- std::nested_exception：捕获并存储当前异常，用于在异常处理中嵌套异常。
- std::bad_exception：无法复制异常对象时抛出的异常。

另外，exception中也提供了一系列外部方法，用于产生异常对象，具体如下所示。

| 方法 | 描述 |
| --- | --- |
| make_exception_ptr(const T& e) | 创建一个异常对象 |
| current_exception() | 获取当前异常对象 |
| rethrow_exception(std::exception_ptr p) | 抛出异常对象 |
| throw_with_nested(const char* message) | 抛出带有std::nested_exception类型的异常对象 |
| rethrow_if_nested() | 判断时nested异常，继续抛出 |
| terminate | 异常抛出出错时调用中止程序 |
| get_terminate() | 获取当前异常处理函数 |
| set_terminate(terminate_handler) | 设置异常处理函数 |

具体示例如下。

```cpp
#include <iostream>
#include <exception>

int main(int argc, const char* argv[])
{
    try {
        throw std::runtime_error("throw exception");
    } catch(const std::exception& e) {
        std::cerr << "catch exception: " << e.what() << '\n';
    }

    // throw nested exception
    try {
        std::throw_with_nested(std::runtime_error("run() failed"));
    } catch(const std::exception& e) {
        std::cerr << "catch exception: " << e.what() << '\n';
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

## stdexcept

C++中定义了一系列exception类，这些异常类都继承自std::exception类。用于在程序运行时报告错误或异常情况。这些异常定义在头文件stdexcept中。

stdexcept参考网址：<https://en.cppreference.com/w/cpp/header/stdexcept.html>。

- std::exception：所有标准异常类的基类，提供了一个虚函数what()，用于返回异常的描述信息
- std::bad_alloc：当new操作符无法分配内存时抛出。
- std::bad_cast: 当dynamic_cast转换失败时抛出
- std::bad_typeid: 当typeid操作符无法获取类型信息时抛出
- std::bad_function_call: 当std::function对象无法调用时抛出
- std::invalid_argument: 当传递给函数的参数无效时抛出
- std::out_of_range: 当访问容器或数组时超出范围时抛出
- std::length_error: 当字符串长度超过允许的最大值时抛出
- std::logic_error: 当逻辑错误发生时抛出
- std::runtime_error: 当运行时错误发生时抛出
- std::overflow_error: 当数值溢出时抛出
- std::underflow_error: 当数值下溢时抛出
- std::system_error: 当系统错误发生时抛出

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

expected是C++23标准库引入头文件中。它用于表示一个操作可能成功返回一个值，也可能失败并返回一个错误信息，为处理函数可能失败的情况提供了一种更安全、更清晰的方式，替代了传统的错误码或者异常处理机制。

expected参考网址: <https://en.cppreference.com/w/cpp/header/expected.html>

std::expected支持以下类模板。

- std::expected<T, E>: 表示一个操作可能成功返回一个值，也可能失败并返回一个错误信息。T表示成功返回的值的类型，E表示错误信息的类型。
- std::unexpected<E>: 表示一个操作可能失败并返回一个错误信息。E表示错误信息的类型。
- std::bad_expected_access: 表示对std::expected对象的访问失败。

std::expected类模板提供了以下成员函数。

| 成员函数 | 描述 |
| --- | --- |
| operator= | 重载赋值运算符 |
| operator* | 重载解引用运算符，返回存储的值 |
| operator-> | 重载箭头运算符，返回存储的值的指针 |
| operator bool | 重载布尔运算符，返回存储的值是否存在 |
| has_value | 返回存储的值是否存在 |
| value | 返回存储的值 |
| error | 获取存储的错误信息 |
| value_or | 返回存储的值，如果存储的值不存在则返回默认值 |
| error_or | 获取存储的错误信息，如果存储的错误信息不存在则返回默认值 |
| and_then | 如果存储的值存在，则返回一个包含新值和错误信息的expected对象 |
| transform | 如果存储的值存在，则返回一个包含新值和错误信息的expected对象 |
| or_else | 如果存储的值不存在，则返回一个包含新值和错误信息的expected对象 |
| transform_error | 如果存储的错误信息存在，则返回一个包含新错误信息的expected对象 |
| emplace | 创建一个新的expected对象，将存储的值替换为指定值 |

具体使用示例如下。

```cpp
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

    // 使用value_or和error_or获取结果
    std::cout << result.value_or(-1) << std::endl;
    std::cout << result.error_or("No error") << std::endl;

    std::cout << std::boolalpha << result.operator bool() << std::endl;
}
```

## system_error

system_error是C++11中引入的头文件中。它用于处理操作系统和库函数的错误。它提供了一系列错误相关的类，包含std::error_category、std::error_condition、std::errc、std::error_code、std::system_error等。

system_error参考网址：<https://en.cppreference.com/w/cpp/header/system_error.html>

- error_category类是错误类别的基类，它定义了错误类别的接口。具体来说，它包含以下成员函数。
- error_condition类型用于存储一个与平台无关的错误条件标识值。与 `std::error_code` 类型一样，它通过一个整数值和一个 `std::error_category` 来唯一标识，但与 `std:：error_code` 不同的是，其值不受平台影响。
- errc: 通过强枚举类型列出了所有标准的“<cerrno>”宏常量。
- error_code: 错误代码类，用于存储一个错误代码，该错误代码与平台相关。
- system_error: 用于报告具有错误代码的异常情况的异常类。

关于error_category类模板的成员函数。

| 成员函数 | 描述 |
| --- | --- |
| operator= | 重载赋值运算符 |
| name | 返回错误类别名称 |
| default_error_condition | 返回默认的错误条件 |
| equivalent | 检查两个错误是否相等 |
| message | 返回错误消息 |
| operator== | 重载相等运算符 |
| operator!= | 重载不相等运算符 |
| operator< | 重载小于运算符 |
| operator<=> | 三向比较运算符(C++20) |
| generate_category | 生成错误类别 |
| system_category | 返回系统错误类别 |
| iostream_category | 返回输入输出错误类别 |
| future_category | 返回future错误类别 |

关于system_error类模板的成员函数。

| 函数 | 描述 |
| --- | --- |
| operator= | 重载赋值运算符 |
| code | 获取错误代码 |
| what | 获取错误消息 |

关于error_code类模板的成员函数。

| 函数 | 描述 |
| --- | --- |
| operator= | 重载赋值运算符 |
| assign | 设置错误代码 |
| clear | 清空错误代码 |
| value | 获取错误代码的值 |
| category | 获取错误代码的错误类别 |
| default_error_condition | 获取默认的错误条件 |
| message | 获取错误消息 |
| operator bool | 重载布尔运算符 |

具体使用示例如下。

```cpp
#include <system_error>
#include <fstream>
#include <thread>
#include <iostream>

int main(int argc, const char* argv[])
{
    // iostream_category
    std::ifstream f("doesn't exist");
    try {
        f.exceptions(f.failbit);
    } catch (const std::ios_base::failure& e) {
        std::cout << "Caught an ios_base::failure." << std::endl;

        const std::error_category& error = e.code().category();
        std::cout << "Error category: " << error.name() << std::endl;
    }

    // error_condition
    for (int const code : {EDOM, 10001})
    {
        const std::error_condition econd =
            std::system_category().default_error_condition(code);

        std::cout << "Category: " << econd.category().name() << '\n'
                << "Value:    " << econd.value() << '\n'
                << "Message:  " << econd.message() << "\n\n";
    }

    // system_error
    try {
        std::thread().detach(); 
    } catch(const std::system_error& e) {
        std::cout << "Caught system_error with code " << e.code() << " meaning " << e.what() << std::endl;
    }

    // std::errc声明了和平台相关的错误码
    std::errc e = std::errc::address_family_not_supported;
    if (e == std::errc::address_family_not_supported) {
        std::cout << "Address family not supported\n" << std::endl;
    }
    
    // error_code可以保存平台相关的错误码
    std::error_code ec = std::make_error_code(e);
    std::cout << "Error code: " << ec.value() << std::endl;
    std::cout << "Error message: " << ec.message() << std::endl;

    std::error_condition econd = ec.default_error_condition();
    std::cout << "Error condition: " << econd.value() << std::endl;
    std::cout << "Error message: " << econd.message() << std::endl;
    return 0;
} 
```