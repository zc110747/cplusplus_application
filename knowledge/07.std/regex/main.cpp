/*
std::regex 是 C++ 标准库中的一个类，用于处理正则表达式。
正则表达式是一种强大的文本处理工具，用于匹配、查找和替换字符串中的模式。
std::regex 提供了一种类型安全且高效的方式来处理正则表达式。


*/

#include <string>
#include <iostream>
#include <regex>

int main(int argc, char* argv[])
{
    // 创建一个正则表达式对象，用于匹配一个或多个数字
    std::regex pattern("\\d+");

    // 要匹配的字符串
    std::string text = "The answer is 42 and the next number is 123";

    // 检查字符串中是否存在匹配的模式
    if (std::regex_search(text, pattern)) {
        std::cout << "Match found!" << std::endl;
    }

    // 提取匹配的子串
    std::smatch matches;
    if (std::regex_search(text, matches, pattern)) {
        std::cout << "Match found: " << matches[0] << std::endl;
    }

    // 替换字符串中的匹配项
    std::string new_text = std::regex_replace(text, pattern, "forty-two");
    std::cout << "Replaced text: " << new_text << std::endl;

    // 使用正则表达式迭代器查找所有匹配项
    std::sregex_iterator it(text.begin(), text.end(), pattern);
    std::sregex_iterator end;
    while (it != end) {
        std::cout << "Match: " << it->str() << std::endl;
        ++it;
    }

    return 0;
} 