//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      logger_user.cpp
//
//  Purpose:
//      独立翻译单元里使用 LOG_*, 用于验证 logger_message 模板定义在头文件中
//      (只放在 main.cpp 中时, 这里会链接失败: undefined reference)
//
/////////////////////////////////////////////////////////////////////////////
#define LOG_MODULE_ON   1

#include "logger.hpp"

void logger_user_demo(void)
{
    LOG_INFO("log from another translation unit: {}", 42);
    LOG_DEBUG("debug from another translation unit");
}
