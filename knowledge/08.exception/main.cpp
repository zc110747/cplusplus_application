<<<<<<< HEAD
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. C++异常处理
//      2. noexcept指定函数是否可能抛出异常
//      3. 用户自定义异常
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <exception>
#include <bitset>
#include <typeinfo>
#include <locale>

#if __cplusplus >= 202302L
#include <expected>
#endif

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

#if __cplusplus >= 202302L
// 模拟一个可能失败的函数
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}
#endif

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

    #if __cplusplus >= 202302L
    {
        auto result = divide(10, 2);
        if (result) {
            std::cout << "Result: " << *result << std::endl;
        } else {
            std::cerr << "Error: " << result.error() << std::endl;
        }
    
        auto bad_result = divide(10, 0);
        if (bad_result) {
            std::cout << "Result: " << *bad_result << std::endl;
        } else {
            std::cerr << "Error: " << bad_result.error() << std::endl;
        }
    
        return 0;
    }
    #endif
    return 0;
=======
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. C++异常处理
//      2. noexcept指定函数是否可能抛出异常
//      3. 用户自定义异常
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <exception>
#include <bitset>
#include <typeinfo>
#include <locale>

#if __cplusplus >= 202302L
#include <expected>
#endif

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

#if __cplusplus >= 202302L
// 模拟一个可能失败的函数
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");
    }
    return a / b;
}
#endif

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

    #if __cplusplus >= 202302L
    {
        auto result = divide(10, 2);
        if (result) {
            std::cout << "Result: " << *result << std::endl;
        } else {
            std::cerr << "Error: " << result.error() << std::endl;
        }
    
        auto bad_result = divide(10, 0);
        if (bad_result) {
            std::cout << "Result: " << *bad_result << std::endl;
        } else {
            std::cerr << "Error: " << bad_result.error() << std::endl;
        }
    
        return 0;
    }
    #endif
    return 0;
>>>>>>> 6d4de4e420b65a5489b112a5738b854b7c121dfb
}