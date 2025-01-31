/*
std::random 是 C++ 标准库中的一个组件，用于生成伪随机数。它提供了多种随机数生成器和分布，以满足不同的需求

随机数生成器
std::default_random_engine：默认的随机数生成器，通常基于 Mersenne Twister 算法。
std::mt19937：32位 Mersenne Twister 随机数生成器。
std::mt19937_64：64位 Mersenne Twister 随机数生成器。

分布
std::uniform_int_distribution：生成均匀分布的整数。
std::uniform_real_distribution：生成均匀分布的浮点数。
std::normal_distribution：生成正态分布的浮点数。
std::bernoulli_distribution：生成伯努利分布的布尔值。
*/
#include <iostream>
#include <random>

int main(int argc, char *argv[]) 
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // 创建一个均匀分布的整数分布
    std::uniform_int_distribution<> dis(1, 6);

    // 生成随机数
    for (int i = 0; i < 10; ++i) {
        std::cout << dis(gen) << ' ';
    }
    std::cout << '\n';

    // 创建一个正态分布的浮点数分布
    std::default_random_engine gen2(rd());
    std::normal_distribution<> floatdis(0, 1);

    for (int i = 0; i < 10; ++i) {
        std::cout << static_cast<int>(floatdis(gen2)*100) << ' ';
    }
    std::cout << '\n';

    return 0;
}