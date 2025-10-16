//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::variant, std::get、std::get_if
//      3. std::visit和std::decay_t
//      4. std::holds_alternative
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <iostream>
#include <string>
#include <variant>

namespace VARIANT
{
    // 从 variant 中获取值的函数
    template <typename T>
    T get_value(const std::variant<int, double, std::string>& v) {
        return std::get<T>(v);
    }

    void test(void) {
        std::cout << "============================ VARIANT ============================" << std::endl;

        std::variant<int, double, std::string> v;               // 默认构造，存储第一个类型（int）的值，初始化为 0
        std::variant<int, double, std::string> v2 = 42;         // 存储 int 类型的值 42
        std::variant<int, double, std::string> v3 = 3.14;       // 存储 double 类型的值 3.14
        std::variant<int, double, std::string> v4 = "Hello";    // 存储 std::string 类型的值 "Hello"
        
        auto i = std::get<int>(v2);            
        std::cout << i << std::endl;        // 输出 42

        auto d = std::get<double>(v3);          
        std::cout << d << std::endl;

        auto s = std::get<std::string>(v4); 
        std::cout << s << std::endl;

        v = 3.14; 
        std::cout << std::get<double>(v) << std::endl;

        v = "Hello";
        std::cout << std::get<std::string>(v) << std::endl;

        try {
            std::cout << std::get<int>(v4) << std::endl;    // v4不存在int类型，抛出异常
        } catch (const std::bad_variant_access& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        std::cout << get_value<std::string>(v) << std::endl;

        // std::get_if函数
        if (std::get_if<int>(&v)) {
            std::cout << "v is an int, value:" << std::get<int>(v) << std::endl;
        } else if (std::get_if<double>(&v)) {
            std::cout << "v is a double, value:" << std::get<double>(v) << std::endl;
        } else if (std::get_if<std::string>(&v)) {
            std::cout << "v is a string, value:" << std::get<std::string>(v) << std::endl;
        }
    }
}

namespace VISIT
{
    // 处理 variant 中的值的函数
    void print_variant(const std::variant<int, double, std::string>& v) {

        auto func = [](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
                std::cout << "int value: " << arg << std::endl;
            } else if constexpr (std::is_same_v<T, double>) {
                std::cout << "double value: " << arg << std::endl;
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "string value: " << arg << std::endl;
            }
        };
        std::visit(func, v);
    }
    
    std::variant<int, std::string> parse_number(const std::string& input) {
        try {
            return std::stoi(input);
        } catch (...) {
            return "Invalid number format";
        }
    }

    template <typename T>
    void show_type(T&& t) {
        using T1 = std::decay_t<decltype(t)>;
        if constexpr (std::is_same_v<T1, int>) {
            std::cout << "int" << std::endl;
        } else if constexpr (std::is_same_v<T1, double>) {
            std::cout << "double" << std::endl;
        } else if constexpr (std::is_same_v<T1, std::string>) {
            std::cout << "string" << std::endl;
        } else {
            std::cout << "Unknown type" << std::endl;
        }
    }

    void test(void) 
    {
        std::cout << "============================ VISIT ============================" << std::endl;

        std::variant<int, double, std::string> v;               // 默认构造，存储第一个类型（int）的值，初始化为 0
        
        auto func = [](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
                std::cout << "int value: " << arg << std::endl;
            } else if constexpr (std::is_same_v<T, double>) {
                std::cout << "double value: " << arg << std::endl;
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "string value: " << arg << std::endl;
            } else {
                std::cout << "Unknown type" << std::endl;
            }
        };
        
        auto func1 = [](auto&& arg) {
            std::cout << arg << std::endl;
        };

        std::visit(func, v);

        v = 3.14;
        std::visit(func, v);    // 转变为double类型

        v = "Hello";
        std::visit(func, v);    // 转变为string类型

        std::visit(func1, v);   // 输出 Hello

        print_variant(v);

        // 使用 std::vector来处理多个variant
        std::vector<std::variant<int, double, std::string>> vec = {42, 3.14, "Hello, variant!"};
        for (const auto& ref : vec) {
            print_variant(ref);
        }

        // std::holds_alternative函数
        for (const auto& ref : vec) {
            if (std::holds_alternative<int>(ref)) {
                std::cout << "int value: " << std::get<int>(ref) << std::endl;
            } else if (std::holds_alternative<double>(ref)) {
                std::cout << "double value: " << std::get<double>(ref) << std::endl;
            } else if (std::holds_alternative<std::string>(ref)) {
                std::cout << "string value: " << std::get<std::string>(ref) << std::endl;
            } else {
                std::cout << "Unknown type" << std::endl;
            }
        }

        // std::monostate
        std::variant<int, double, std::string, std::monostate> v2;
        v2 = std::monostate();
        if (std::holds_alternative<std::monostate>(v2)) {
            std::cout << "v2 is empty" << std::endl;
        }

        // std::variant返回值
        std::variant<int, std::string> result = parse_number("42");
        if (std::holds_alternative<int>(result)) {
            std::cout << "Parsed number: " << std::get<int>(result) << std::endl;
        } else if (std::holds_alternative<std::string>(result)) {
            std::cout << "Error: " << std::get<std::string>(result) << std::endl;
        }

        // std::decay_t获取原始类型
        int i1 = 42;
        int& i2 = i1;
        int&& i3 = std::move(i1);
        show_type(i1);
        show_type(i2);
        show_type(i3);
    }
}

int main(int argc, char* argv[])
{
    VARIANT::test();

    VISIT::test();
    return 0;
} 
