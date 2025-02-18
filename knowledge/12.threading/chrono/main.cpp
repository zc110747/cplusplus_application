/*
std::chrono
chrono是一个非常重要的库，它提供了一套时间和日期的处理工具。chrono依赖于系统时钟，修改系统时钟可能会影响chrono的行为
std::chrono::second定义时间
std::duration用于保存时间点差值
std::duration_cast用于时间转换
*/
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>    // std::chrono::seconds

using std::cout;
using std::endl;

void clock_basic(void)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto sleep_time = std::chrono::seconds(1);

    std::this_thread::sleep_for(sleep_time);
 
    auto end = std::chrono::high_resolution_clock::now();
 
    std::chrono::duration<double> elapsed = end - start;
 
    std::cout<<"start timer: "<<start.time_since_epoch().count()<<std::endl;
    std::cout<<"sleep timer: "<<sleep_time.count()<<std::endl;
    std::cout<<"执行时间: "<<elapsed.count()<<" 秒"<<std::endl;

    auto now_time = std::chrono::system_clock::now();
    std::cout<<"now timer: "<<now_time.time_since_epoch().count()<<std::endl;

    auto steady_time = std::chrono::steady_clock::now();
    std::cout<<"steady time: "<<steady_time.time_since_epoch().count()<<std::endl;
}

void clock_convert(void)
{
    std::chrono::milliseconds ms(1000);

    auto sec = std::chrono::duration_cast<std::chrono::seconds>(ms);
    std::cout<<"1000毫秒等于"<<sec.count()<<"秒"<<std::endl;

    auto ms_from_sec = std::chrono::duration_cast<std::chrono::milliseconds>(sec);
    std::cout <<"1秒等于"<<ms_from_sec.count()<<"毫秒"<<std::endl;

    auto microsec = std::chrono::duration_cast<std::chrono::microseconds>(sec);
    std::cout <<"1秒等于"<<microsec.count()<<"微秒"<<std::endl;

    auto nano_sec = std::chrono::duration_cast<std::chrono::nanoseconds>(sec);
    std::cout <<"1秒等于"<<nano_sec.count()<<"纳秒"<<std::endl;
}

int main(int argc, char* argv[])
{
    clock_basic();

    clock_convert();

    return 0;
}
