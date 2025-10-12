//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. format
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdint>
#include <format>
#include <chrono>
#include <vector>

struct Point {
    int x;
    int y;
};

// 为 Point 类型实现格式化功能
template <>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }
    
    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

// 扩展输出vector
template <>
struct std::formatter<std::vector<int>> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }
    
    auto format(const std::vector<int>& v, std::format_context& ctx) const {
        for (const auto& i : v) {
            std::format_to(ctx.out(), "{} ", i);
        }
        return ctx.out();
    }
};

int main(int argc, const char* argv[])
{
    // 简单占位符
    std::cout << std::format("{} + {} = {}", 1, 2, 3) << std::endl;

    // 指定索引
    std::cout << std::format("{1} {0}", "World", "Hello") << std::endl;

    // 数据格式化
    // 浮点数格式化
    double pi = 3.14159;
    std::cout << std::format("Default: {}", pi) << std::endl;
    std::cout << std::format("两位小数: {:.2f}", pi) << std::endl;
    std::cout << std::format("科学计数法: {:.2e}", pi) << std::endl;

    // 整数格式化
    std::cout << std::format("Decimal: {:d}", 42) << std::endl;
    std::cout << std::format("Hexadecimal: {:x}", 42) << std::endl;
    std::cout << std::format("Octal: {:o}", 42) << std::endl;
    std::cout << std::format("Binary: {:b}", 42) << std::endl;
    std::cout << std::format("有符号: {:+} {:+}", 42, -42) << std::endl;
    
    // 输出数学常数
    std::cout << std::format("数学常数: π = {:.5f}, e = {:.5f}\n", 
                           std::numbers::pi, std::numbers::e);
    
    // 字符串格式化
    std::string s = "Hello, World!";
    std::cout << std::format("{:16}", s) << "Default" << std::endl; //指定宽度为16
    std::cout << std::format("{:>16}", s) << "Right" << std::endl; //右对齐，指定宽度为16
    std::cout << std::format("{:<16}", s) << "Left" << std::endl; //左对齐，指定宽度为16
    std::cout << std::format("{:^16}", s) << "Centered" << std::endl; //居中对齐，指定宽度为16

    std::cout << std::format("{:16.5}", s) << "Truncated" << std::endl; //指定宽度为16，截断为10个字符

    // 填充字符
    std::cout << std::format("{:*^16}", s) << "Centered with *" << std::endl; //居中对齐，指定宽度为16，填充字符为*

    // 多行文本
    std::cout << std::format("{:>16}\n{:>16}", "Line 1", "Line 2") << std::endl; //多行文本右对齐，指定宽度为16

    // 时间格式化
    std::cout << std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now()) << std::endl;

    // 自定义结构体格式化
    Point p{1, 2};
    std::cout << std::format("Point: {}", p) << std::endl;

    // 格式化到输出迭代器
    std::string output;
    std::format_to(std::back_inserter(output), "迭代器输出: {}", "测试");
    std::cout << output << std::endl;

    // 格式化重复调用
    std::cout << std::format("重复调用: {0} {1} {0}", "A", "B") << std::endl;

    // 格式化带颜色
    std::cout << std::format("\033[31m{}\033[0m", "红色文本") << std::endl;
    
    // 扩展格式化vector
    std::vector<int> v{1, 2, 3, 4, 5};
    std::cout << std::format("Vector: {}", v) << std::endl;
    return 0;
}
