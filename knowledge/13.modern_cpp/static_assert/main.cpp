/*
静态断言（static_assert）是C++11引入的一个编译时断言机制，用于在编译阶段检查某个条件是否为真。
如果条件为假，编译器会产生一个错误信息，并且编译过程会失败。静
态断言通常用于在编译时验证模板参数、常量表达式或其他编译时可确定的条件。
*/
#include <iostream>
#include <cstring>

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

int main() {
    check_size<int>();    // 编译通过，int类型大小通常为4字节
    // check_size<double>();  // 编译失败，double类型大小通常为8字节

    int a = 10;
    double b = 3.14;
    float c = 2.718;

    bit_copy(a, c);  // 编译通过，int和float类型大小相同
    std::cout << a << std::endl;
    // bit_copy(a, b);  // 编译不通过，int和double类型大小不同

    return 0;
}
