/**********************************************************
 * 说明: constexpr
 * const 支持运行时和编译期的只读声明
 * constexpr 仅支持编译器的常量
************************************************************/
#include <iostream>
#include <unistd.h>
#include <memory>
#include <array>
#include <vector>

std::string command = "curl 'https://devapi.qweather.com/v7/weather/now?location=101210301&key=***' --compressed";

int func_add(int a, int b) {
    return a+b;
}
int main(int argc, char *argv[])
{
    std::array<char, 256> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.data(), "r"), pclose);
    std::string result;
    if (!pipe) {
        std::cerr << "无法打开管道" << std::endl;
        return 1;
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    std::cout<<result<<std::endl;

    int a = ({
        int b = 3;
        func_add(b, 2);
    });
    std::cout<<a<<std::endl;
    return 0;
}


