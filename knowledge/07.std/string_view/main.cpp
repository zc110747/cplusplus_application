/*
std::string_view 是 C++17 引入的一个轻量级的非拥有性字符串视图类，它位于 <string_view> 头文件中。std::string_view 提供了一种高效的方式来引用字符串数据，而不需要复制字符串内容，这使得它在处理字符串时非常节省内存和时间

成员函数：
compare: 比较字符串视图与另一个字符串视图或 C 风格字符串
length：返回字符串视图的长度
size: 返回字符串视图的长度
substr: 返回字符串视图的子字符串视图
starts_with: 检查字符串视图是否以指定的前缀开头
ends_with: 检查字符串视图是否以指定的后缀结尾
find: 在字符串视图中查找子字符串
rfind: 在字符串视图中反向查找子字符串
at: 返回指定位置的字符
operator[]: 返回指定位置的字符
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
