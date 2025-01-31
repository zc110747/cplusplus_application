/*
1. 原生字符串字面量
原生字符串字面量（Raw String Literals）是C++11引入的一个特性，它允许你在字符串中包含特殊字符（如反斜杠和引号）而不需要进行转义。
原生字符串字面量以 R"( 开始，以 )" 结束。在这两个符号之间的所有字符都被视为字符串的一部分，包括换行符和引号

2. 用户自定义字面量
用户可以自定义字面量（User-Defined Literals），这允许你为基本类型（如整数、浮点数、字符串等）定义新的后缀，从而创建自定义的字面量。
对于字符串字面量，你可以定义一个自定义后缀来创建一个特定类型的对象。

3. 单引号作为整数分隔符
在C++14及以后的版本中，单引号（'）可以用作整数和浮点数字面量的分隔符，以提高代码的可读性。
这些分隔符不会影响数值本身，它们只是为了让数字更易于阅读。
*/
#include <iostream>

// 自定义字符串类
class MyString {
public:
    MyString(const std::string& str) : data(str) {}

    void print() const {
        std::cout << "MyString: " << data << std::endl;
    }

private:
    std::string data;
};

// 定义自定义字面量后缀
MyString operator"" _my(const char* str, size_t len) {
    return MyString(std::string(str, len));
}


template <char...c>
std::string operator "" _w()
{
    std::string str;

    //折叠表达式
    using unused = int[];
    unused{(str.push_back(c), 0)...};
    return str;
}

//字符串转换成string对象
std::string operator "" _string(const char* str, size_t len)
{
    return std::string(str, len);
}

int main() {
    // 使用原生字符串字面量
    const char* str = R"(This is a raw string literal.
It can contain "quotes" and \backslashes without escaping.)";
    std::cout << str << std::endl;

    //C++可使用R"cpp(字符串)cpp支持原生字符串字面量
    //外部添加cpp可以支持内部嵌套
    //另外支持u8-char8_t, u-char16_t, U-char32_t, L-wchar-t
    //格式用于表示字面量存储格式
    char hello_HTML[] = R"cpp(<!DOCTYPE HTML
        <html lang="en">
        <head>
            <title>Hello World!</title>
        </head>
        <body>
            "(Hello World!)"
        </body>
        </html>
    )cpp";
    std::cout << hello_HTML << std::endl;

    // 使用自定义字面量后缀创建 MyString 对象
    "Hello, World!"_my.print();
    
    auto x = 12.3_w;
    auto y = "123"_string;
    std::cout << x << std::endl;
    std::cout << y << std::endl;

    // 单引号作为整数分隔符
    int billion = 1'000'000'000;
    int largeNumber = 123'456'789;

    std::cout << "Billion: " << billion << std::endl;
    std::cout << "Large number: " << largeNumber << std::endl;
    return 0;
}