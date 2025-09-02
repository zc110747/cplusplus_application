/*
std::byte 
std::byte 是 C++17 引入的标准库类型，定义在 <cstddef> 头文件中，用于表示最小的可寻址内存单元，也就是字节。它是一个枚举类型，用于更安全、更明确地处理字节数据，避免使用 char 或 unsigned char 时可能出现的混淆。

类型安全：std::byte 不能隐式转换为整数类型，必须显式转换，这有助于避免意外的类型转换错误。
位操作：支持常见的位操作符，如 &、|、^、~、<< 和 >>。
无符号性：std::byte 是无符号的，这意味着它的取值范围是从 0 到 255
*/
#include <iostream>
#include <cstddef>
#include <utility>

int main(int argc, char* argv[])
{
    std::byte b1{0x55};
    std::byte b2 = static_cast<std::byte>(10);

    auto b3 = b1 & b2;
    auto b4 = b1 | b2;
    auto b5 = b1 ^ b2;
    auto b6 = ~b1;
    auto b7 = b1 << 2;

    std::cout << "b1: " << std::to_integer<int>(b1) << std::endl;
    std::cout << "b2: " << std::to_integer<int>(b2) << std::endl;
    std::cout << "b3: " << std::to_integer<int>(b3) << std::endl;
    std::cout << "b4: " << std::to_integer<int>(b4) << std::endl;
    std::cout << "b5: " << std::to_integer<int>(b5) << std::endl;
    std::cout << "b6: " << std::to_integer<int>(b6) << std::endl;
    std::cout << "b7: " << std::to_integer<int>(b7) << std::endl;
    return 0;
} 
