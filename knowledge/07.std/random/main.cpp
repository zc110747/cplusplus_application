//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. random_device - 随机数发生器
//      2. pseudo random - 伪随机数发生器
//      3. distribution - 随机数分布处理
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <random>

#if __cplusplus > 202002L
#include <concepts>

template <typename T>
concept VariadicTemplate = std::same_as<T, std::minstd_rand>
                        || std::same_as<T, std::default_random_engine>
                        || std::same_as<T, std::mt19937_64>
                        || std::same_as<T, std::mt19937>;

template <VariadicTemplate T>
std::string generate_random_string(T &seed, size_t length) {
    const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<size_t> dist(0, charset.size() - 1);

    std::string result;
    for (size_t i = 0; i < length; ++i) {
        result += charset[dist(seed)];
    }
    return result;
}
#else
template <typename T, typename = void>
struct is_random_engine : std::false_type {};

template <typename T>
struct is_random_engine<T, std::enable_if_t<std::is_same_v<T, std::minstd_rand> ||
                                            std::is_same_v<T, std::default_random_engine> ||
                                            std::is_same_v<T, std::mt19937_64> ||
                                            std::is_same_v<T, std::mt19937>>> : std::true_type {};

template <typename T>
constexpr bool is_random_engine_v = is_random_engine<T>::value;

template <typename T, typename = std::enable_if_t<is_random_engine_v<T>>>
std::string generate_random_string(T &seed, size_t length) {
    const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<size_t> dist(0, charset.size() - 1);

    std::string result;
    for (size_t i = 0; i < length; ++i) {
        result += charset[dist(seed)];
    }
    return result;
}
#endif

namespace RANDOM_DEVICE
{
    int test(void)
    {
        std::cout << "==================== RANDOM_DEVICE ====================" << std::endl;

        std::random_device rd;
        std::cout << "rd() = " << rd() << std::endl;                    // 输出一个随机数
        std::cout << "rd.min() = " << rd.min() << std::endl;            // 输出随机数范围最小值
        std::cout << "rd.max() = " << rd.max() << std::endl;            // 输出随机数范围最大值
        std::cout << "rd.entropy() = " << rd.entropy() << std::endl;    // entropy 是随机数生成器的熵，用于衡量随机数的质量和随机性
        return 0;
    }
}

namespace PSEUDO_RANDOM
{
    int test(void)
    {
        std::cout << "==================== PSEUDO_RANDOM ====================" << std::endl;

        std::random_device rd;

        std::minstd_rand gen0(rd());    // 最小标准minstd_rand随机数生成器
        std::cout << "minstd_rand() = " << gen0() << std::endl;             // 输出一个随机数
        std::cout << "minstd_rand.min() = " << gen0.min() << std::endl;     // 输出随机数范围最小值
        std::cout << "minstd_rand.max() = " << gen0.max() << std::endl;     // 输出随机数范围最大值

        std::minstd_rand0 gen1(rd());   // 最小标准minstd_rand0随机数生成器
        std::cout << "minstd_rand0() = " << gen1() << std::endl;
        std::cout << "minstd_rand0.min() = " << gen1.min() << std::endl;
        std::cout << "minstd_rand0.max() = " << gen1.max() << std::endl;

        std::mt19937 gen2(rd());        // 梅森旋转算法mt19937随机数生成器
        std::cout << "mt19937() = " << gen2() << std::endl;
        std::cout << "mt19937.min() = " << gen2.min() << std::endl;
        std::cout << "mt19937.max() = " << gen2.max() << std::endl;

        std::mt19937_64 gen3(rd());     // 梅森旋转算法mt19937_64随机数生成器
        std::cout << "mt19937_64() = " << gen3() << std::endl;
        std::cout << "mt19937_64.min() = " << gen3.min() << std::endl;
        std::cout << "mt19937_64.max() = " << gen3.max() << std::endl;

        std::ranlux24_base gen4(rd());    // 随机数生成器ranlux24_base
        std::cout << "ranlux24_base() = " << gen4() << std::endl;
        std::cout << "ranlux24_base.min() = " << gen4.min() << std::endl;
        std::cout << "ranlux24_base.max() = " << gen4.max() << std::endl;

        std::ranlux48_base gen5(rd());    // 随机数生成器ranlux48_base
        std::cout << "ranlux48_base() = " << gen5() << std::endl;
        std::cout << "ranlux48_base.min() = " << gen5.min() << std::endl;
        std::cout << "ranlux48_base.max() = " << gen5.max() << std::endl;

        std::ranlux24 gen6(rd());      // 随机数生成器ranlux24
        std::cout << "ranlux24() = " << gen4() << std::endl;
        std::cout << "ranlux24.min() = " << gen4.min() << std::endl;
        std::cout << "ranlux24.max() = " << gen4.max() << std::endl;

        std::ranlux48 gen7(rd());      // 随机数生成器ranlux48
        std::cout << "ranlux48() = " << gen5() << std::endl;
        std::cout << "ranlux48.min() = " << gen5.min() << std::endl;
        std::cout << "ranlux48.max() = " << gen5.max() << std::endl;

        std::knuth_b gen8(rd());      // 随机数生成器knuth_b
        std::cout << "knuth_b() = " << gen8() << std::endl;
        std::cout << "knuth_b.min() = " << gen8.min() << std::endl;
        std::cout << "knuth_b.max() = " << gen8.max() << std::endl;

        std::default_random_engine gen9(rd());
        std::cout << "default_random_engine() = " << gen9() << std::endl;
        std::cout << "default_random_engine.min() = " << gen9.min() << std::endl;
        std::cout << "default_random_engine.max() = " << gen9.max() << std::endl;
        return 0;
    }
}

namespace DISTRIBUTION
{
    int test(void)
    {
        std::cout << "==================== DISTRIBUTION ====================" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());

        // 创建一个均匀分布的整数分布
        std::uniform_int_distribution<> dis(1, 100);
        std::cout << "uniform_int_distribution() = ";

        for (size_t i = 0; i < 10; i++) {
            std::cout << dis(gen) << ' ';
        }
        std::cout << std::endl;

        // 创建一个均匀分布的浮点数分布
        std::uniform_real_distribution<> dis1(1, 100);
        std::cout << "uniform_real_distribution() = ";
        for (size_t i = 0; i < 10; i++) {
            std::cout << dis1(gen) << ' ';
        }
        std::cout << std::endl;

        // 创建一个正态分布的浮点数分布
        std::normal_distribution<> floatdis1(0, 1);
        std::cout << "normal_distribution() = ";
        for (size_t i = 0; i < 10; i++) {
            std::cout << floatdis1(gen)*100 << ' ';
        }
        std::cout << std::endl;

        // 创建一个伯克利分布的浮点数分布
        std::bernoulli_distribution bool_dis(0.5);
        std::cout << "bernoulli_distribution() = ";
        for (size_t i = 0; i < 10; i++) {
            std::cout << bool_dis(gen) << ' ';
        }
        std::cout << std::endl;

        // 创建一个二项分布的整数分布
        std::binomial_distribution<> bin_dis(10, 0.5);
        std::cout << "binomial_distribution() = ";
        for (size_t i = 0; i < 10; i++) {
            std::cout << bin_dis(gen) << ' ';
        }
        std::cout << std::endl;

        // 生成随机字符串
        auto &&str = generate_random_string(gen, 10);
        std::cout << str << '\n';

        return 0;
    }
}
int main(int argc, char *argv[]) 
{
    RANDOM_DEVICE::test();

    PSEUDO_RANDOM::test();

    DISTRIBUTION::test();

    return 0;
}