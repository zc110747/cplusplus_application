/*
C++ 异常处理是一种机制，用于在程序运行时处理错误和异常情况。
它允许程序在遇到错误时抛出异常，并在适当的地方捕获和处理这些异常，从而避免程序崩溃或产生不可预测的行为。

1. 异常的基本语法：
try {
    // 可能会抛出异常的代码
} catch (ExceptionType1& e) {
    // 处理 ExceptionType1 类型的异常
} catch (ExceptionType2& e) {
    // 处理 ExceptionType2 类型的异常
} catch (...) {
    // 处理其他类型的异常
}

2. throw关键字
在C++中，throw关键字用于抛出异常。throw关键字后面跟着一个表达式，该表达式的值将作为异常对象被抛出。
异常对象可以是任何类型，但通常是从std::exception类派生的类的实例

3. 异常传递和noexcept关键字
在C++中，异常传递是指在函数调用链中，异常从抛出点向上传递，直到被捕获并处理的过程
在C++中，noexcept关键字用于指定一个函数是否可能抛出异常。它是C++11引入的一个特性，用于优化代码和提高程序的健壮性
- noexcept：表示函数不会抛出任何异常。
- noexcept(expression)：表示函数是否抛出异常取决于expression的值。如果expression为true，则函数不会抛出异常；如果expression为false，则函数可能抛出异常。

4. 异常类和用户自定义异常类
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
*/

#include <iostream>
#include <vector>
#include <exception>
#include <bitset>
#include <typeinfo>
#include <locale>

void exception_variable()
{
    std::cout<<"["<<__func__<<"] ";

    try
    {
        throw -1;
    }
    catch(const int& e)
    {
        std::cerr<< "Caught: " << e << "; ";
    }

    try
    {
        throw std::string("throw string");
    }
    catch(const std::string& e)
    {
        std::cerr << "Caught: " << e << '\n';
    }  
}

//std::domain_error此类用作所有引发报告域错误（按数学方式，而不是网络）的异常的基类。
void exception_domain_error()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        throw std::domain_error("Your domain is in error!");
    }
    catch (const std::domain_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }    
}

//std::invalid_argument 此类用作引发报告无效自变量的所有异常的基类。
void exception_invalid_argument()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        std::bitset<32> b("11001010101100001b100101010110000");
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }    
}

//std::length_error 此类用作引发报告尝试生成对象太长而难以指定的所有异常的基类。
void exception_length_error()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        std::vector<int> v(100 + static_cast<size_t>(-1) / sizeof(int));
    }
    catch (const std::length_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }   
}

//const std::logic_error 此类用作引发报告执行程序前大概可检测的错误（例如，违反逻辑前提条件）的所有异常的基类。
void exception_logic_error()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        throw std::logic_error("this is logic error!");
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }
}

//std::out_of_range 此类用作引发报告无效自变量的所有异常的基类
void exception_out_of_range()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        std::vector<int> val(10);
        val.at(100);
    }
    catch(const std::out_of_range& e)
    {
        std::cerr <<"Caught: " << e.what() << '\n';
    }  
}

//std::overflow_error 此类用作引发报告算数溢出的所有异常的基类。
void exception_overflow()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        std::bitset<66> b;
        b[65] = 1;
        b[0] = 1;
        unsigned long x = b.to_ulong();

        std::cout<<x<<" | "<<sizeof(unsigned long)<<std::endl;
    }
    catch (const std::overflow_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }
}

//std::range_error 此类用作所有引发报告范围错误（按数学方式，而不是迭代器）的异常的基类。
void exception_range()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        throw std::range_error("range error!");
    }
    catch (const std::range_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }
}

//std::runtime_erro此类用作引发报告仅在执行程序时大概可检测的错误的所有异常的基类。
void exception_runtime()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        std::locale loc("test");
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }    
}

//std::underflow_error 此类用作引发报告算数下溢的所有异常的基类。
void exception_underflow()
{
    try
    {
        std::cout<<"["<<__func__<<"] ";
        throw std::underflow_error("num is bit small");
    }
    catch (const std::underflow_error& e)
    {
        std::cerr << "Caught: " << e.what() << std::endl;
    }
}

// 自定义异常类，继承自std::exception
class MyException : public std::exception {
public:
    // 构造函数，接受一个描述信息作为参数
    MyException(const std::string& message) : msg(message) {}

    // 重写what()方法，返回异常的描述信息
    const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};

// 函数，可能抛出自定义异常
void exception_user(void) {
    try
    {
        throw MyException("An error occurred in func");
    }
    catch(const MyException& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught unknown exception" << std::endl;
    } 
}

int main(int argc, char *argv[])
{
    exception_variable();

    exception_domain_error();

    exception_invalid_argument();
    
    exception_length_error();

    exception_logic_error();

    exception_out_of_range();

    exception_overflow();

    exception_range();

    exception_runtime();

    exception_underflow();

    exception_user();
    return 0;
}