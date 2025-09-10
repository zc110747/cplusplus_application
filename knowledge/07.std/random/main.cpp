/*

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