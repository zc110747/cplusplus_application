<<<<<<< HEAD
﻿//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::string_view声明
//      2. std::string_view方法
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string_view>

namespace CONSTRUCTOR {
    void test() {
        std::cout << "========================= CONSTRUCTOR =========================" << std::endl;
        std::string_view sv1;
        std::string_view sv2("Hello, World!");
        std::string_view sv3(sv2);
        std::string_view sv4(sv2.data(), sv2.size());
        std::string s1 = "Hello, World!";
        std::string_view sv5 = {"Hello, World!"};
        std::string_view sv6{"hello, world!"};
        
        sv1 = s1;

        std::cout << "sv1: " << sv1 << std::endl;
        std::cout << "sv2: " << sv2 << std::endl;
        std::cout << "sv3: " << sv3 << std::endl;
        std::cout << "sv4: " << sv4 << std::endl;
        std::cout << "sv5: " << sv5 << std::endl;
        std::cout << "sv6: " << sv6 << std::endl;
    }
}

namespace MEMBER_FUNCTION {
    
    void print_stringview(const std::string_view& sv) {
        std::cout << "length: " << sv.length() << ", content: " << sv << std::endl;
    }

    void test() {
        std::cout << "========================= MEMBER_FUNCTION =========================" << std::endl;

        // 从 C 风格字符串创建 std::string_view
        const char* cstr = "Hello, World!";
        std::string_view sv1(cstr);
        print_stringview(sv1);

        // 从 std::string 创建 std::string_view
        std::string str = "Hello, C++!";
        std::string_view sv2(str);
        print_stringview(sv2);

        //length，size, substr,
        std::cout << "length: " << sv2.length() << std::endl;
        std::cout << "size: " << sv2.size() << std::endl;
        std::cout << "substr: " << sv2.substr(0, 5) << std::endl;
        std::cout << "substr: " << sv2.substr(7, 3) << std::endl;

        //starts_with, ends_with, find, rfind, compare, at, operator[]
        std::cout << "starts_with: " << sv2.starts_with("Hello") << std::endl;
        std::cout << "ends_with: " << sv2.ends_with("C++!") << std::endl;
        std::cout << "find: " << sv2.find("C++") << std::endl;
        std::cout << "rfind: " << sv2.rfind("C++") << std::endl;
        std::cout << "compare: " << sv2.compare("Hello, C++!") << std::endl;
        std::cout << "at: " << sv2.at(0) << std::endl;
        std::cout << "operator[]: " << sv2[0] << std::endl;

        if (sv1 == sv2) {
            std::cout << "sv1 and sv2 are equal." << std::endl;
        } else {
            std::cout << "sv1 and sv2 are not equal." << std::endl;
        }

        if (sv1 != sv2) {
            std::cout << "sv1 and sv2 are not equal." << std::endl;
        } else {
            std::cout << "sv1 and sv2 are equal." << std::endl;
        }

        // copy
        std::string_view sv3{"test hello!"};
        std::string s2;
        s2.resize(sv3.size());
        sv3.copy(s2.data(), s2.size());
        std::cout << "s2: " << s2 << std::endl;

        // data
        const char* data = sv3.data();
        std::cout << "sv3 data: " << data << std::endl;

        // remove_prefix, remove_suffix
        std::string_view sv4 = {" this is a test.\n"};
        sv4.remove_prefix(1);
        sv4.remove_suffix(1);
        std::cout << "sv4: " << sv4 << std::endl;
    }
}

int main(int argc, char* argv[])
{
    CONSTRUCTOR::test();

    MEMBER_FUNCTION::test();

    return 0;
} 
=======
﻿//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::string_view声明
//      2. std::string_view方法
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string_view>

namespace CONSTRUCTOR {
    void test() {
        std::cout << "========================= CONSTRUCTOR =========================" << std::endl;
        std::string_view sv1;
        std::string_view sv2("Hello, World!");
        std::string_view sv3(sv2);
        std::string_view sv4(sv2.data(), sv2.size());
        std::string s1 = "Hello, World!";
        std::string_view sv5 = {"Hello, World!"};
        std::string_view sv6{"hello, world!"};
        
        sv1 = s1;

        std::cout << "sv1: " << sv1 << std::endl;
        std::cout << "sv2: " << sv2 << std::endl;
        std::cout << "sv3: " << sv3 << std::endl;
        std::cout << "sv4: " << sv4 << std::endl;
        std::cout << "sv5: " << sv5 << std::endl;
        std::cout << "sv6: " << sv6 << std::endl;
    }
}

namespace MEMBER_FUNCTION {
    
    void print_stringview(const std::string_view& sv) {
        std::cout << "length: " << sv.length() << ", content: " << sv << std::endl;
    }

    void test() {
        std::cout << "========================= MEMBER_FUNCTION =========================" << std::endl;

        // 从 C 风格字符串创建 std::string_view
        const char* cstr = "Hello, World!";
        std::string_view sv1(cstr);
        print_stringview(sv1);

        // 从 std::string 创建 std::string_view
        std::string str = "Hello, C++!";
        std::string_view sv2(str);
        print_stringview(sv2);

        //length，size, substr,
        std::cout << "length: " << sv2.length() << std::endl;
        std::cout << "size: " << sv2.size() << std::endl;
        std::cout << "substr: " << sv2.substr(0, 5) << std::endl;
        std::cout << "substr: " << sv2.substr(7, 3) << std::endl;

        //starts_with, ends_with, find, rfind, compare, at, operator[]
        std::cout << "starts_with: " << sv2.starts_with("Hello") << std::endl;
        std::cout << "ends_with: " << sv2.ends_with("C++!") << std::endl;
        std::cout << "find: " << sv2.find("C++") << std::endl;
        std::cout << "rfind: " << sv2.rfind("C++") << std::endl;
        std::cout << "compare: " << sv2.compare("Hello, C++!") << std::endl;
        std::cout << "at: " << sv2.at(0) << std::endl;
        std::cout << "operator[]: " << sv2[0] << std::endl;

        if (sv1 == sv2) {
            std::cout << "sv1 and sv2 are equal." << std::endl;
        } else {
            std::cout << "sv1 and sv2 are not equal." << std::endl;
        }

        if (sv1 != sv2) {
            std::cout << "sv1 and sv2 are not equal." << std::endl;
        } else {
            std::cout << "sv1 and sv2 are equal." << std::endl;
        }

        // copy
        std::string_view sv3{"test hello!"};
        std::string s2;
        s2.resize(sv3.size());
        sv3.copy(s2.data(), s2.size());
        std::cout << "s2: " << s2 << std::endl;

        // data
        const char* data = sv3.data();
        std::cout << "sv3 data: " << data << std::endl;

        // remove_prefix, remove_suffix
        std::string_view sv4 = {" this is a test.\n"};
        sv4.remove_prefix(1);
        sv4.remove_suffix(1);
        std::cout << "sv4: " << sv4 << std::endl;
    }
}

int main(int argc, char* argv[])
{
    CONSTRUCTOR::test();

    MEMBER_FUNCTION::test();

    return 0;
} 
>>>>>>> 171775b3443edbb693d4aa0be5b9466005143b33
