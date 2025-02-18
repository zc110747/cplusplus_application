/*
std::format
是 C++20 引入的标准库函数，位于 <format> 头文件，用于实现类型安全且灵活的字符串格式化，替代传统的printf系列函数和 std::stringstream。
*/

#include <iostream>
// #include <format>
#include <bit>
#include <cstdint>

int main(int argc, char* argv[])
{
    // // 简单占位符
    // std::string s1 = std::format("{} + {} = {}", 1, 2, 3);
    // std::cout << s1 << std::endl;

    // // 指定索引
    // std::string s2 = std::format("{1} {0}", "World", "Hello");
    // std::cout << s2 << std::endl;

    // // 格式说明符
    // double pi = 3.14159;
    // std::string s3 = std::format("Pi: {:.2f}", pi);
    // std::cout << s3 << std::endl;

    float f = 3.14f;
    // 将 float 类型的 f 转换为 uint32_t 类型
    std::uint32_t u = std::bit_cast<std::uint32_t>(f);

    std::cout << "Float value: " << f << std::endl;
    std::cout << "Bit-casted uint32_t value: " << u << std::endl;

    // 将 uint32_t 类型的 u 转换回 float 类型
    float f_back = std::bit_cast<float>(u);
    std::cout << "Back to float value: " << f_back << std::endl;
    return 0;

}