<<<<<<< HEAD
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. assert
//      2. static_assert
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstring>
#include <type_traits>

template <typename T>
void check_size() {
    static_assert(sizeof(T) <= 4, "Type size exceeds 4 bytes");
    std::cout << "Type size is within the limit." << std::endl;
}

template <typename T, typename U>
int bit_copy(T& a, U& b) {
    static_assert(sizeof(T) == sizeof(U), "Type sizes are different");
    memcpy(&a, &b, sizeof(T));
    return 0; 
}

class MyClass {
public:
    MyClass() : data(1) {
        // 判断MyClass是否为 trivial 类型
        static_assert(!std::is_trivial_v<MyClass>, "MyClass is a trivial type");

        // 检测MyClass是否为 standard layout 类型
        static_assert(std::is_standard_layout_v<MyClass>, "MyClass is not a standard layout type");
    }
    int data{0};
};

int main(int argc, char const *argv[]) 
{
    check_size<int>();    // 编译通过，int类型大小通常为4字节
    // check_size<double>();  // 编译失败，double类型大小通常为8字节

    int a = 10;
    double b = 3.14;
    float c = 2.718;

    bit_copy(a, c);  // 编译通过，int和float类型大小相同
    std::cout << a << std::endl;
    // bit_copy(a, b);  // 编译不通过，int和double类型大小不同

    MyClass myObj;
    std::cout << myObj.data << std::endl;

    return 0;
}
=======
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. assert
//      2. static_assert
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstring>
#include <type_traits>

template <typename T>
void check_size() {
    static_assert(sizeof(T) <= 4, "Type size exceeds 4 bytes");
    std::cout << "Type size is within the limit." << std::endl;
}

template <typename T, typename U>
int bit_copy(T& a, U& b) {
    static_assert(sizeof(T) == sizeof(U), "Type sizes are different");
    memcpy(&a, &b, sizeof(T));
    return 0; 
}

class MyClass {
public:
    MyClass() : data(1) {
        // 判断MyClass是否为 trivial 类型
        static_assert(!std::is_trivial_v<MyClass>, "MyClass is a trivial type");

        // 检测MyClass是否为 standard layout 类型
        static_assert(std::is_standard_layout_v<MyClass>, "MyClass is not a standard layout type");
    }
    int data{0};
};

int main(int argc, char const *argv[]) 
{
    check_size<int>();    // 编译通过，int类型大小通常为4字节
    // check_size<double>();  // 编译失败，double类型大小通常为8字节

    int a = 10;
    double b = 3.14;
    float c = 2.718;

    bit_copy(a, c);  // 编译通过，int和float类型大小相同
    std::cout << a << std::endl;
    // bit_copy(a, b);  // 编译不通过，int和double类型大小不同

    MyClass myObj;
    std::cout << myObj.data << std::endl;

    return 0;
}
>>>>>>> 6d4de4e420b65a5489b112a5738b854b7c121dfb
