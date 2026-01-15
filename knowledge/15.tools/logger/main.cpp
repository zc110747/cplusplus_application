//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      基于fmt库封装的日志库，支持日志级别，多线程安全，格式化输出等功能。
//      是对fmt的转发封装，因此支持fmt的所有功能。
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
//#define PROGRAM_NAME    "logger_manage"
#define LOG_MODULE_ON   1

#include "logger.hpp"

namespace LOGGER
{
    template<typename... Args>
    void logger_manage::logger_message(LOGGER_LEVEL level, const char* f_str, Args... args)
    {
        if (level >= level_) {
            std::lock_guard<std::mutex> lock(log_mutex_);
            const char* level_str = "";
            switch(level) {
                case LOGGER_LEVEL::DEBUG: level_str = "D"; break;
                case LOGGER_LEVEL::INFO:  level_str = "I";  break;
                case LOGGER_LEVEL::WARN:  level_str = "W";  break;
                case LOGGER_LEVEL::ERROR: level_str = "E"; break;
                case LOGGER_LEVEL::FATAL: level_str = "F"; break;
            }
            auto now = std::chrono::system_clock::now();
            auto now_time = std::chrono::system_clock::to_time_t(now);
            std::tm* local_time = std::localtime(&now_time);
            std::stringstream ss;
            ss << std::put_time(local_time, "%Y-%m-%d %H:%M");
            #ifndef PROGRAM_NAME
            fmt::println("[{}][{}] {}", level_str, ss.str(), fmt::format(f_str, std::forward<Args>(args)...));
            #else
            fmt::println("[{}][{}][{}] {}", level_str, ss.str(), PROGRAM_NAME, fmt::format(f_str, std::forward<Args>(args)...));    
            #endif
        }
    }
}
struct Point {
    int x;
    int y;
};

// 为 Point 类型实现格式化功能
template <>
struct fmt::formatter<Point> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Point& p, FormatContext& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

struct Network {
    std::string ipaddr;
    int port;
};

template <>
struct fmt::formatter<Network> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }
    template <typename FormatContext>
    auto format(const Network& net, FormatContext& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "{}:{}", net.ipaddr, net.port);
    }
};

int main(void)
{
    LOG_MESSAGE("Hello, {}!", "World");
    LOG_MESSAGE("The answer is {}", 42);
    LOG_MESSAGE("Multiple arguments: {}, {}, {}", 1, "B", "C");
    
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARN("This is a warning message");
    LOG_ERROR("This is an error message");
    LOG_FATAL("This is a fatal message");

    LOG_SET_LEVEL(LOGGER::logger_manage::LOGGER_LEVEL::INFO);
    LOG_DEBUG("This is a debug message");       // not logger_manage
    LOG_INFO("This is an info message");

    LOG_SET_LEVEL(LOGGER::logger_manage::LOGGER_LEVEL::FATAL);
    LOG_WARN("This is a warning message");     // not logger_manage
    LOG_FATAL("This is a fatal message")

    LOG_SET_LEVEL(LOGGER::logger_manage::LOGGER_LEVEL::DEBUG);
    LOG_DEBUG("This is a debug message");
    LOG_FATAL("float:{0:.2f}, Scientific: {1:.2e}", 123.456, 123.456);  //数字格式化

    LOG_SET_LEVEL(LOGGER::logger_manage::LOGGER_LEVEL::DEBUG);
    LOG_FATAL("Character: {0}, Boolean:{1}", 'a', true);                //字符和布尔值格式化
    LOG_FATAL("Left aligned: {:<10}, Right aligned: {:>10}", "left", "right"); //对齐和宽度控制
    LOG_FATAL("{1}, {0}", "test", "hello");

    // 自定义类型的格式化
    Point p{1, 2};
    LOG_FATAL("{0}", p);

    Network net{"192.168.1.1", 8080};
    LOG_FATAL("{0}", net);

    return 0;
}

