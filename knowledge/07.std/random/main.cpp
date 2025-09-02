/*
std::random 是 C++ 标准库中的一个组件，用于生成伪随机数。它提供了多种随机数生成器和分布，以满足不同的需求
随机数生成器
std::minstd_rand：线性同余生成器，速度快，但随机性较差
std::default_random_engine：默认的随机数生成器，通常基于Mersenne Twister算法。
std::mt19937：32位Mersenne Twister随机数生成器, 周期长，随机性好。
std::mt19937_64：64位Mersenne Twister随机数生成器。
分布
std::uniform_int_distribution：生成均匀分布的整数。
std::uniform_real_distribution：生成均匀分布的浮点数。
std::normal_distribution：生成正态分布的浮点数。
std::bernoulli_distribution：生成伯努利分布的布尔值。

std::random_device：是 C++ 标准库中的一个类，用于生成非确定性的随机数。它通常用于初始化随机数生成器的种子，以确保每次程序运行时生成的随机数序列都是不同的。
*/

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

    // 创建一个伯克利分布的浮点数分布
    std::minstd_rand gen3(rd());
    std::bernoulli_distribution bool_dis(0.5);  

    for (int i = 0; i < 10; ++i) {
        std::cout << bool_dis(gen3) << ' ';
    }
    std::cout << '\n';

    // 生成随机字符串
    auto &&str = generate_random_string(gen, 10);
    std::cout << str << '\n';
    return 0;
}