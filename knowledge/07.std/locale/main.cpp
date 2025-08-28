/*
std::locale 是 C++ 标准库中的一个类，用于表示特定的本地化环境

locale -a查看支持的指令集

主要特点：
1. 本地化支持：std::locale 提供了对不同地区和语言的本地化支持，使得程序能够适应不同的用户群体。
2. 可定制性：可以通过创建自定义的 std::locale 对象来定制特定的本地化设置。
3. 全局和局部设置：可以在全局范围内设置默认的 std::locale，也可以在局部范围内使用特定的 std::locale。
4. 与标准库组件集成：std::locale 与许多标准库组件（如 std::cout、std::cin、std::string 等）集成，使得这些组件能够自动适应本地化设置


*/

#include <iostream>
#include <locale>

int main(int argc, char* argv[]) 
{
    // 设置全局的默认 locale 为用户的默认 locale
    std::locale::global(std::locale(""));

    // 使用全局的默认 locale 输出数字
    std::cout.imbue(std::locale());
    std::cout << "Number in default locale: " << 1234.56 << std::endl;

    // 使用特定的 locale 输出数字
    try
    {
        std::locale german("C.utf8");
        std::cout.imbue(german);
        std::cout << "Number in German locale: " << 1234.56 << std::endl;

        std::locale loc1("C.utf8");
        std::locale loc2 = std::locale::global( loc1 );
        std::cout << "The name of the previous locale is: " << loc2.name( )
            << "." << std::endl;
        std::cout << "The name of the current locale is: " << loc1.name( )
            << "." << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}