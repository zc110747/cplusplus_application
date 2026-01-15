//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::valarray声明
//      2. std::valarray方法
//      3. 算法操作std::valarray
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <valarray>

template<typename T>
void print_container(T container_val, std::string qstring = "")
{
    //empty, size
    if (!qstring.empty()) {
        if (qstring.size() < 10) {
            for (auto index = qstring.size(); index < 10; index++) {
                qstring.push_back(' ');
            }
        }

        qstring += ":";
        std::cout << qstring;
    }

    for (const auto &ref : container_val) {
        std::cout << ref << " ";
    }
    std::cout <<std::endl;
}

int main(int argc, char* argv[])
{
    std::valarray<int> v1{1};
    v1 = -1;
    print_container(v1, "v1");

    v1 = {4, 1, 3, 5, 7, 6};
    print_container(v1, "v1");

    // operator[]
    std::cout << "v1[0]: " << v1[0] << std::endl;

    // std::slice(start, size, stride)，从start开始，取size个元素，每个元素间隔stride个元素
    std::valarray<int> v2 = v1[std::slice(0, 3, 2)];
    print_container(v2, "v2");

    std::valarray<int> v3 = v1[v1 % 2 == 0]; // 偶数元素
    print_container(v3, "v3");

    std::valarray<int> v4 = v1[v1 > 3]; //大于3的元素
    print_container(v4, "v4");

    // operator+、-、*、/
    // 每一项进行相应处理
    std::valarray<int> v5 = v1 + 1;
    print_container(v5, "v5");

    std::valarray<int> v6 = v1 - 1;
    print_container(v6, "v6");

    std::valarray<int> v7 = v1 * 2;
    print_container(v7, "v7");

    std::valarray<int> v8 = v1 / 2;
    print_container(v8, "v8");

    // swap
    v3.swap(v4);
    print_container(v3, "v3");
    print_container(v4, "v4");

    // size
    std::cout << "v3.size(): " << v3.size() << std::endl;
    v3.resize(8);
    print_container(v3, "v3");

    // sum
    std::cout << "v1.sum(): " << v1.sum() << std::endl;

    // min、max
    std::cout << "v1.min(): " << v1.min() << std::endl;
    std::cout << "v1.max(): " << v1.max() << std::endl;

    // shift(左移，空出的位置用0填充)
    print_container(v1, "v1");
    auto v9 = v1.shift(2);
    print_container(v9, "v9");

    // cshift(左移，空出的位置用移出的元素填充)
    auto v10 = v1.cshift(2);
    print_container(v10, "v10");

    // apply(对valarray对象中的元素进行操作)
    std::valarray<int> v11 = v1.apply([](int i) { return i * i; });
    print_container(v11, "v11");

    // 数学函数支持
    // std::abs 取绝对值
    std::valarray<double> v_7{-1, -2, -3, -4, -5};
    std::valarray<double> v_8 = std::abs(v_7);
    print_container(v_8, "v_8");

    // std::sqrt 开方
    std::valarray<double> v_9 = std::sqrt(v_8);
    print_container(v_9, "v_9");

    // std::sin
    std::valarray<double> v_1 = {0, 0.25, 0.5, 0.75, 1};
    std::valarray<double> v_2 = std::sin(v_1 * std::numbers::pi);
    print_container(v_2, "v_2");

    // std::log
    std::valarray<double> v_3 = std::log(v_2);
    print_container(v_3, "v_3");

    // std::pow
    std::valarray<double> v_4{1, 2, 3, 4, 5};
    std::valarray<double> v_5 = std::pow(v_4, 2);  // 1^2, 2^2, 3^2, 4^2, 5^2
    print_container(v_5, "v_5");

    std::valarray<double> v_6 = std::pow(v_4, v_4); // 1^1, 2^2, 3^3, 4^4, 5^5
    print_container(v_6, "v_6");

    return 0;
} 
