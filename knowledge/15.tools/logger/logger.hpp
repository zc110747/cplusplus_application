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
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#if __cplusplus < 201103L
#error "This library requires C++11 or later. Please update your compiler."
#endif

#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <unistd.h>
#include <fcntl.h>
#include <mutex>
#include <string>
#include <chrono>
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
        /// @param format - the string base format.
        /// @param args - args process int the format   
        template<typename... Args>
        void logger_message(LOGGER_LEVEL level, const std::string& f_str, Args... args) const {
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

#if LOG_MODULE_ON == 1
#define LOG_MESSAGE(...)    do {  \
    LOGGER::logger_manage::get_instance()->logger_message(__VA_ARGS__); \
}while(0);
#define LOG_DEBUG(...)      do { \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::DEBUG, __VA_ARGS__); \
}while(0);          
#define LOG_INFO(...)       do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::INFO, __VA_ARGS__); \
}while(0);
#define LOG_WARN(...)       do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::WARN, __VA_ARGS__); \
}while(0);
#define LOG_ERROR(...)      do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::ERROR, __VA_ARGS__); \
}while(0);
#define LOG_FATAL(...)      do {  \
    LOGGER::logger_manage::get_instance()->logger_message(LOGGER::logger_manage::LOGGER_LEVEL::FATAL, __VA_ARGS__); \
}while(0);
#define LOG_SET_LEVEL(level)     do {  \
    LOGGER::logger_manage::get_instance()->set_log_level(level); \
}while(0);
#else
#define LOG_MESSAGE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#endif
#endif