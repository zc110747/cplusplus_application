//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::chrono::system_clock
//      2. std::chrono::steady_clock
//      3. std::chrono::high_resolution_clock
//      4. std::chrono::duration_cast
//      5. std::chrono::time_point_cast
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>

namespace SYSTEM_CLOCK
{ 
    int test(void)
    {
        std::cout << "================= SYSTEM_CLOCK =================" << std::endl;

        // 类型std::chrono::system_clock::time_point
        auto now_time = std::chrono::system_clock::now();
        std::cout << "now timer: " << now_time.time_since_epoch().count() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto end_time = std::chrono::system_clock::now();

        std::chrono::duration<double> diff = end_time - now_time;
        std::cout << "diff: " << diff.count() << std::endl;

        std::time_t time = std::chrono::system_clock::to_time_t(now_time);
        std::cout << "time: " << std::ctime(&time) << std::endl;
 
        // 从time_point转换为time_t
        std::chrono::duration<int, std::ratio<60*60*24> > one_day(1);    //声明一个1天的时间间隔
        auto tomorrow = now_time + one_day;
        std::cout << "tomorrow timer: " << tomorrow.time_since_epoch().count() << std::endl;
        time = std::chrono::system_clock::to_time_t(tomorrow);
        std::cout << "time: " << std::ctime(&time) << std::endl;

        // 从time_t转换为time_point
        now_time = std::chrono::system_clock::from_time_t(time);
        std::cout << "now timer: " << now_time.time_since_epoch().count() << std::endl;

        return 0;
    }
};

namespace HIGH_RESOLUTION_CLOCK
{
    int test(void)
    {
        std::cout << "================= HIGH_RESOLUTION_CLOCK =================" << std::endl;

        // 类型std::chrono::high_resolution_clock::time_point
        auto now_time = std::chrono::high_resolution_clock::now();
        std::cout << "now timer: " << now_time.time_since_epoch().count() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> diff = end_time - now_time;
        std::cout << "diff: " << diff.count() << std::endl;

        std::time_t time = std::chrono::high_resolution_clock::to_time_t(now_time);
        std::cout << "time: " << std::ctime(&time) << std::endl;
 
        // 从time_point转换为time_t
        std::chrono::duration<int, std::ratio<60*60*24> > one_day(1);    //声明一个1天的时间间隔
        auto tomorrow = now_time + one_day;
        std::cout << "tomorrow timer: " << tomorrow.time_since_epoch().count() << std::endl;
        time = std::chrono::high_resolution_clock::to_time_t(tomorrow);
        std::cout << "time: " << std::ctime(&time) << std::endl;

        // 从time_t转换为time_point
        now_time = std::chrono::high_resolution_clock::from_time_t(time);
        std::cout << "now timer: " << now_time.time_since_epoch().count() << std::endl;

        return 0;
    }
}

namespace STEADY_CLOCK
{
    int test(void)
    {
        std::cout << "================= STEADY_CLOCK =================" << std::endl;

        // 类型std::chrono::steady_clock::time_point
        auto now_time = std::chrono::steady_clock::now();
        std::cout << "now timer: " << now_time.time_since_epoch().count() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        auto end_time = std::chrono::steady_clock::now();

        std::chrono::duration<double> diff = end_time - now_time;
        std::cout << "diff: " << diff.count() << std::endl;

        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - now_time);
        std::cout << "time_span: " << time_span.count() << std::endl;

        return 0;
    }
}

namespace DURATION_CAST
{
    int test(void) 
    {
        std::cout << "================= DURATION_CAST =================" << std::endl;

        std::chrono::milliseconds ms(1000);

        auto sec = std::chrono::duration_cast<std::chrono::seconds>(ms);
        std::cout << "1000毫秒等于" << sec.count() << "秒" << std::endl;

        auto ms_from_sec = std::chrono::duration_cast<std::chrono::milliseconds>(sec);
        std::cout << "1秒等于" << ms_from_sec.count() << "毫秒" << std::endl;

        auto microsec = std::chrono::duration_cast<std::chrono::microseconds>(sec);
        std::cout << "1秒等于" << microsec.count() << "微秒" << std::endl;

        auto nano_sec = std::chrono::duration_cast<std::chrono::nanoseconds>(sec);
        std::cout << "1秒等于" << nano_sec.count() << "纳秒" << std::endl;

        auto abs_ms = std::chrono::abs(ms);
        std::cout << "绝对值1000毫秒等于" << abs_ms.count() << "毫秒" << std::endl;

        auto abs_sec = std::chrono::round<std::chrono::seconds>(ms);
        std::cout << "1000毫秒四舍五入等于" << abs_sec.count() << "秒" << std::endl;

        auto abs_sec_ceil = std::chrono::ceil<std::chrono::seconds>(ms);
        std::cout << "1000毫秒向上取整等于" << abs_sec_ceil.count() << "秒" << std::endl;

        auto abs_sec_floor = std::chrono::floor<std::chrono::seconds>(ms);
        std::cout << "1000毫秒向下取整等于" << abs_sec_floor.count() << "秒" << std::endl;

        // 将类型转换为time_point，基于起始时间
        auto today = std::chrono::time_point_cast<std::chrono::duration<int, std::ratio<60*60*24>>>(std::chrono::system_clock::now());
        std::cout << "today:" << today.time_since_epoch().count() << " days since epoch" << std::endl;
        
        return 0;
    } 
}

int main(int argc, char* argv[])
{
    SYSTEM_CLOCK::test();

    HIGH_RESOLUTION_CLOCK::test();

    STEADY_CLOCK::test();

    DURATION_CAST::test();
    return 0;
}
