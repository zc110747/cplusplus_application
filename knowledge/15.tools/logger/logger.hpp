//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      logger.hpp
//
//  Purpose:
//      基于fmt库封装的日志库，支持日志级别，多线程安全，格式化输出等功能
//      C++版本需要至少高于C++11
//
//  Author:
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#if __cplusplus < 201103L
#error "This library requires C++11 or later. Please update your compiler."
#endif

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <mutex>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace LOGGER {
    class logger_manage final
    {
    public:
        enum class LOGGER_LEVEL {
            DEBUG = 0,
            INFO,
            WARN,
            ERROR,
            FATAL,
        };

        /// \brief constructor
        logger_manage() {};

        /// \brief destructor
        ~logger_manage() {};

        static logger_manage *get_instance() {
            static logger_manage instance;
            return &instance;
        }

        /// @brief logger_message
        /// - This method is used to print the log message.
        /// - 模板定义必须留在头文件里: 只放在某一个 .cpp 中, 其他翻译单元
        ///   使用 LOG_DEBUG/INFO/WARN/ERROR/FATAL 会直接链接失败.
        /// @param level - the log level of this message.
        /// @param f_str - the string base format.
        /// @param args - args process int the format   
        template<typename... Args>
        void logger_message(LOGGER_LEVEL level, const char* f_str, Args... args)
        {
            if (level < level_) {
                return;
            }

            std::lock_guard<std::mutex> lock(log_mutex_);

            const char* level_str = "D";
            switch(level) {
                case LOGGER_LEVEL::DEBUG: level_str = "D"; break;
                case LOGGER_LEVEL::INFO:  level_str = "I"; break;
                case LOGGER_LEVEL::WARN:  level_str = "W"; break;
                case LOGGER_LEVEL::ERROR: level_str = "E"; break;
                case LOGGER_LEVEL::FATAL: level_str = "F"; break;
            }

            auto now = std::chrono::system_clock::now();
            auto now_time = std::chrono::system_clock::to_time_t(now);
            std::tm local_time{};
#if defined(_WIN32)
            localtime_s(&local_time, &now_time);
#else
            localtime_r(&now_time, &local_time);
#endif
            std::stringstream ss;
            ss << std::put_time(&local_time, "%Y-%m-%d %H:%M");

#ifndef PROGRAM_NAME
            fmt::println("[{}][{}] {}", level_str, ss.str(), fmt::format(f_str, std::forward<Args>(args)...));
#else
            fmt::println("[{}][{}][{}] {}", level_str, ss.str(), PROGRAM_NAME, fmt::format(f_str, std::forward<Args>(args)...));
#endif
        }

        /// @brief logger_message
        /// - This method is used to print the log message.
        /// @param format - the string base format.
        /// @param args - args process int the format   
        template<typename... Args>
        void logger_message(const std::string& f_str, Args... args) const {
                std::lock_guard<std::mutex> lock(log_mutex_);
                fmt::println("{}", fmt::format(f_str, std::forward<Args>(args)...));
        }

        /// @brief set_log_level
        /// - This method is used to set the log level.
        /// @param level - the log level.
        void set_log_level(LOGGER_LEVEL level) {
            level_ = level;
        }
    private:
        /// @brief error level
        /// - the error level for the log message.
        LOGGER_LEVEL level_{LOGGER_LEVEL::DEBUG};

        /// @brief log_mutex_
        /// - mutex for thread-safe logging
        mutable std::mutex log_mutex_;
    };
}

//不引入 using namespace: 头文件里展开命名空间会污染所有包含者, 统一写 LOGGER:: 前缀
//(LOG_* 宏内部已经是全限定名, 使用宏的地方不需要前缀)

//默认开启日志: 否则忘记 #define 时所有 LOG_* 静默失效, 故障很难定位
#ifndef LOG_MODULE_ON
#define LOG_MODULE_ON 1
#endif

#if LOG_MODULE_ON == 1
#define LOG_MESSAGE(...)    do {  \
    LOGGER::logger_manage::get_instance()->logger_message(__VA_ARGS__); \
} while(0)
#define LOG_DEBUG(...)      do { \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::DEBUG, __VA_ARGS__); \
} while(0)
#define LOG_INFO(...)       do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::INFO, __VA_ARGS__); \
} while(0)
#define LOG_WARN(...)       do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::WARN, __VA_ARGS__); \
} while(0)
#define LOG_ERROR(...)      do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::ERROR, __VA_ARGS__); \
} while(0)
#define LOG_FATAL(...)      do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::FATAL, __VA_ARGS__); \
} while(0)
#define LOG_SET_LEVEL(level)     do {  \
    LOGGER::logger_manage::get_instance()->set_log_level(level); \
} while(0)
#else
#define LOG_MESSAGE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#define LOG_SET_LEVEL(level)
#endif
#endif