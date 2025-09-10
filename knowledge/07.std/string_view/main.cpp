/*

*/
#include <iostream>
#include <string_view>

using std::cout;
using std::endl;

void printStringView(std::string_view sv) {
    std::cout << "Length: " << sv.length() << ", Content: " << sv << std::endl;
}

int main(int argc, char* argv[])
{
    // 从 C 风格字符串创建 std::string_view
    const char* cstr = "Hello, World!";
    std::string_view sv1(cstr);
    printStringView(sv1);

    // 从 std::string 创建 std::string_view
    std::string str = "Hello, C++!";
    std::string_view sv2(str);
    printStringView(sv2); 

    //length，size, substr,
    std::cout << sv2.length() << std::endl;
    std::cout << sv2.size() << std::endl;
    std::cout << sv2.substr(0, 5) << std::endl;
    std::cout << sv2.substr(7, 3) << std::endl;

    //starts_with, ends_with, find, rfind, compare, at, operator[]
    std::cout << sv2.starts_with("Hello") << std::endl;
    std::cout << sv2.ends_with("C++!") << std::endl;
    std::cout << sv2.find("C++") << std::endl;
    std::cout << sv2.rfind("C++") << std::endl;
    std::cout << sv2.compare("Hello, C++!") << std::endl;
    std::cout << sv2.at(0) << std::endl;
    std::cout << sv2[0] << std::endl;
    return 0;
} 
