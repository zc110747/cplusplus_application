/*
C++ module语法支持不完善，目前保留后续说明
*/
#include <iostream>
#include <format>

// module语法支持需要高于201907版本的C++

int main(int argc, char* argv[])
{
    std::cout << __cpp_modules << std::endl;
    return 0;
}