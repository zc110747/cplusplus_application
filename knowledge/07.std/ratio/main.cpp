/*

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


