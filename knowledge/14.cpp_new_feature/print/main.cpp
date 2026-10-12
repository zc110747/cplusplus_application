//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. print
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////

#include <print>
#include <format>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <cstdint>
#include <limits>
#include <cstdio>
#include <stdexcept>
#include <exception>

// ============================================================
// 1. 自定义类型：版本号
// ============================================================
struct Version
{
    int major;
    int minor;
    int patch;
};

// 为自定义类型提供 std::format / std::print 格式化支持
template <>
struct std::formatter<Version> : std::formatter<std::string_view>
{
    auto format(const Version& v, std::format_context& ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::format("{}.{}.{}", v.major, v.minor, v.patch), ctx);
    }
};

// ============================================================
// 2. 自定义类型：传感器数据
// ============================================================
struct SensorData
{
    std::uint32_t id;
    float temperature;
    float pressure;
    bool valid;
};

template <>
struct std::formatter<SensorData> : std::formatter<std::string_view>
{
    auto format(const SensorData& data, std::format_context& ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::format(
                "Sensor[id={}, temp={:.2f} C, pressure={:.2f} kPa, valid={}]",
                data.id,
                data.temperature,
                data.pressure,
                data.valid ? "true" : "false"),
            ctx);
    }
};

// ============================================================
// 3. 测试函数
// ============================================================
void test_basic()
{
    std::println("\n========== 1. Basic output ==========");

    std::print("Hello");
    std::print(", ");
    std::println("C++23 std::print!");

    std::println("整数：{}", 123);
    std::println("浮点数：{}", 3.1415926);
    std::println("字符串：{}", "Hello World");
    std::println("布尔值：{}", true);
}

void test_integer_formats()
{
    std::println("\n========== 2. Integer formats ==========");

    std::uint32_t value = 0x1234ABCD;

    std::println("decimal     : {}", value);
    std::println("hex lowercase: {:x}", value);
    std::println("hex uppercase: {:X}", value);
    std::println("hex prefix   : {:#x}", value);
    std::println("octal        : {:o}", value);
    std::println("binary       : {:b}", value);
    std::println("binary prefix: {:#b}", value);

    std::println("zero padded  : {:08}", 123);
    std::println("hex padded   : {:#010x}", 0x1234ABCD);
    std::println("left aligned : {:<10}", 123);
    std::println("right aligned: {:>10}", 123);
    std::println("centered     : {:^10}", 123);

    std::int32_t signed_value = -12345;
    std::println("signed value : {}", signed_value);
    std::println("with sign    : {:+d}", signed_value);
    std::println("positive sign: {:+d}", 12345);
}

void test_floating_point()
{
    std::println("\n========== 3. Floating-point formats ==========");

    double value = 123.456789;

    std::println("default      : {}", value);
    std::println("fixed 2      : {:.2f}", value);
    std::println("fixed 4      : {:.4f}", value);
    std::println("scientific   : {:.3e}", value);
    std::println("general      : {:.5g}", value);
    std::println("percent      : {:.2f}%", 98.765);

    std::println("positive sign: {:+.2f}", value);
    std::println("zero padded  : {:010.2f}", value);

    std::println("infinity     : {}", std::numeric_limits<double>::infinity());
    std::println("quiet NaN    : {}", std::numeric_limits<double>::quiet_NaN());
}

void test_string_formats()
{
    std::println("\n========== 4. String formats ==========");

    std::string name = "MeArmPilot";

    std::println("normal       : {}", name);
    std::println("left aligned : '{:<16}'", name);
    std::println("right aligned: '{:>16}'", name);
    std::println("centered     : '{:^16}'", name);
    std::println("custom fill  : '{:*^16}'", name);
    std::println("truncate     : '{:.5}'", name);

    std::string_view view = "RobotForge";
    std::println("string_view  : {}", view);
}

void test_argument_index()
{
    std::println("\n========== 5. Argument indexing ==========");

    int a = 10;
    int b = 20;

    std::println("automatic: {} + {} = {}", a, b, a + b);
    std::println("indexed  : {1} - {0} = {2}", a, b, b - a);
    std::println("repeated : {0}, {0}, {1}", a, b);

    // 注意：同一个格式字符串中，不要混用自动索引与显式索引。
}

void test_escape_braces()
{
    std::println("\n========== 6. Escaped braces ==========");

    std::println("{{}} produces literal braces: {{}}");
    std::println("JSON-like output: {{\"id\": {}, \"ok\": {}}}", 1001, true);
}

void test_containers()
{
    std::println("\n========== 7. Containers ==========");

    std::array<int, 5> values{10, 20, 30, 40, 50};
    std::vector<std::string> names{"Alice", "Bob", "Charlie"};

    // std::print 不会自动格式化任意容器
    std::print("array: ");
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        if (i != 0)
            std::print(", ");

        std::print("{}", values[i]);
    }
    std::println();

    std::print("vector: ");
    for (std::size_t i = 0; i < names.size(); ++i)
    {
        if (i != 0)
            std::print(", ");

        std::print("{}", names[i]);
    }
    std::println();
}

void test_custom_types()
{
    std::println("\n========== 8. Custom types ==========");

    Version version{1, 2, 3};
    SensorData sensor{1001, 36.5F, 101.325F, true};

    std::println("Version: {}", version);
    std::println("Sensor : {}", sensor);
}

void test_file_output()
{
    std::println("\n========== 9. File output ==========");

    // std::println(FILE*, ...) 是 C++23 的文件输出重载。
    // 这里仅用于演示；文件打开失败时应检查返回值。
    std::FILE* file = std::fopen("std_print_test.log", "w");

    if (file == nullptr)
    {
        std::println(stderr, "Failed to open log file");
        return;
    }

    std::println(file, "C++23 std::print file test");
    std::println(file, "Device ID: {}", 1001);
    std::println(file, "Temperature: {:.2f} C", 36.5);
    std::println(file, "Hex register: {:#010x}", 0x1234ABCD);

    std::fclose(file);

    std::println("File output completed: std_print_test.log");
}

void test_error_handling()
{
    std::println("\n========== 10. Error handling ==========");

    try
    {
        // std::format 用于生成字符串，不直接输出到控制台。
        std::string text = std::format(
            "device={}, status={}",
            1001,
            "OK");

        std::println("Formatted string: {}", text);
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Formatting error: {}", e.what());
    }
}

int main()
{
    std::println("========================================");
    std::println(" C++23 std::print comprehensive test");
    std::println("========================================");

    test_basic();
    test_integer_formats();
    test_floating_point();
    test_string_formats();
    test_argument_index();
    test_escape_braces();
    test_containers();
    test_custom_types();
    test_file_output();
    test_error_handling();

    std::println("\n========== All tests completed ==========");
    return 0;
}

