//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <chrono>
#include <iostream>
#include <syncstream>
#include <string>
#include <thread>
#include <vector>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

const char* to_string(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    }

    return "UNKNOWN";
}

class Logger
{
public:
    explicit Logger(std::ostream& output = std::cout)
        : output_(output)
    {
    }

    void log(LogLevel level, const std::string& message)
    {
        const auto now = std::chrono::system_clock::now();
        const auto time = std::chrono::system_clock::to_time_t(now);

        std::osyncstream stream(output_);

        stream << "["
               << to_string(level)
               << "] [Thread "
               << std::this_thread::get_id()
               << "] ["
               << time
               << "] "
               << message
               << '\n';
    }

private:
    std::ostream& output_;
};

void worker(Logger& logger, int id)
{
    logger.log(LogLevel::Info, "Worker started.");

    for (int i = 0; i < 3; ++i)
    {
        logger.log(
            LogLevel::Info,
            "Worker " + std::to_string(id) +
            " processing item " + std::to_string(i));

        std::this_thread::sleep_for(
            std::chrono::milliseconds(20));
    }

    logger.log(LogLevel::Info, "Worker finished.");
}

int main()
{
    Logger logger;

    logger.log(LogLevel::Info, "Application started.");

    std::vector<std::thread> workers;

    for (int i = 0; i < 4; ++i)
    {
        workers.emplace_back(worker, std::ref(logger), i);
    }

    for (auto& thread : workers)
    {
        thread.join();
    }

    logger.log(LogLevel::Info, "Application finished.");

    return 0;
}
