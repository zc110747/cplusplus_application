//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 原生字符串字面量
//      2. 用户自定义字面量
//      3. 单引号作为整数分隔符
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <string>

namespace NATIVE_LITERAL
{
    int test(void)
    {
        std::cout << "================= NATIVE_LITERAL =================" << std::endl;

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
        return 0;
    }
}

namespace USER_LITERAL
{
    // 自定义字符串类
    class user_string {
    public:
        user_string(const std::string& str) : data(str) {}

        void print() const {
            std::cout << "user_string: " << data << std::endl;
        }

        void push_back(char c) {
            data.push_back(c);
        }

    private:
        std::string data;
    };

    // 定义自定义字面量后缀
    user_string operator"" _my(const char* str, size_t len) {
        return user_string(std::string(str, len));
    }
    
    template<char ...c>
    user_string operator"" _my() {
        user_string str("");

        //折叠表达式
        using unused = int[];
        unused{(str.push_back(c), 0)...};
        return str;
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

    struct RGBA {
        int r;
        int g;
        int b;
        int a;
        RGBA(int R, int G, int B, int A =- 0):r(R), g(G), b(B){
        }
    };

    std::ostream & operator<<(std::ostream& out, RGBA& col) {
        out<<"r: "<<col.r<<std::endl;
        out<<"g: "<<col.g<<std::endl;
        out<<"b: "<<col.b<<std::endl;
        return out;
    }

    RGBA operator "" _C(const char *col, size_t n) {
        std::string s(col, n);
        std::vector<int> result;
        std::istringstream ss(s);
        std::string token;

        while (ss >> token) {
            result.push_back(std::stoi(token.substr(1)));
        }

        return RGBA(result[0], result[1], result[2]);
    }

    
    struct volt{
        unsigned int d;
    };
    volt operator "" _v(unsigned long long d) {
        return {(unsigned int)d};
    }

    std::ostream &operator<<(std::ostream &out, volt &vol){
        return out<<vol.d;
    }

    size_t operator "" _len(char const *ch, size_t l)
    {
        return l;
    }

    std::string operator "" _add(const char *ch, size_t l)
    {
        return "happy " + std::string(ch);
    }

    int test(void)
    {
        std::cout << "================= USER_LITERAL =================" << std::endl;

        "Hello, World!"_my.print();
        
        auto val1 = 123_my;
        val1.print();
        
        auto val2 = 12.3_w;
        auto val3 = "123"_string;
        std::cout << val2 << std::endl;
        std::cout << val3 << std::endl;

        auto val4 = "r121 g234 b123"_C;
        std::cout << val4;

        /*
            1.字面量为整型，只接受unsigned long long 或者const char*
            2.字面量为浮点型数，只接受long double和const char *
            3.字面量为字符串，只接受const char *
            4.字面量为字符，只接受一个char 参数
            5.operator, "",和后缀之间都需要空格
        */
        volt power = 5_v;
        std::cout << power << std::endl;
        std::cout<< "str size:" << "happy"_len << std::endl;
        std::cout<< "day"_add << std::endl;
        return 0;
    }
}

namespace SEPARTOR_LITERAL
{
    int test(void)
    {
        std::cout << "================= SEPARTOR_LITERAL =================" << std::endl;
        
        // 单引号作为整数分隔符
        int billion = 1'000'000'000;
        int largeNumber = 123'456'789;

        std::cout << "Billion: " << billion << std::endl;
        std::cout << "Large number: " << largeNumber << std::endl;
        
        return 0;
    }
}

int main() 
{
    // 使用原生字符串字面量
    NATIVE_LITERAL::test();

    // 使用用户自定义字面量
    USER_LITERAL::test();

    // 使用分隔符字面量
    SEPARTOR_LITERAL::test();
    return 0;
}