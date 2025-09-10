/*

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
