/*
std::ratio C++11 引入的一个模板类，定义在 <ratio> 头文件中，用于在编译时表示有理数。
它主要用于处理编译时的分数运算，比如时间单位的转换、频率的计算等，能在编译阶段完成分数的化简和计算，提高程序的性能和安全性。

std::ratio_add：计算两个比例的和。
std::ratio_subtract：计算两个比例的差。
std::ratio_multiply：计算两个比例的积。
std::ratio_divide：计算两个比例的商
*/
#include <iostream>
#include <ratio>


int main()
{
    // 定义一个 3/4 的比例
    using three_fourths = std::ratio<3, 4>;
    // 定义一个 1/2 的比例
    using one_half = std::ratio<1, 2>;

    // 计算两个比例的和
    using sum = std::ratio_add<three_fourths, one_half>;
    std::cout << "3/4 + 1/2 = " << sum::num << "/" << sum::den << std::endl;

    // 计算两个比例的差
    using sub = std::ratio_subtract<three_fourths, one_half>;
    std::cout << "3/4 - 1/2 = " << sub::num << "/" << sub::den << std::endl;

    // 计算两个比例的积
    using mul = std::ratio_multiply<three_fourths, one_half>;
    std::cout << "3/4 * 1/2 = " << mul::num << "/" << mul::den << std::endl;

    // 计算两个比例的商
    using div = std::ratio_divide<three_fourths, one_half>;
    std::cout << "3/4 / 1/2 = " << div::num << "/" << div::den << std::endl;
    
    return 0;
}


