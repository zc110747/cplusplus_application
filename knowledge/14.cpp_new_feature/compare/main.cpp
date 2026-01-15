//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <compare>
#include <string>

struct point
{
    int x;
    int y;

    auto operator<=>(const point&) const = default;
};

struct float_point
{
    float x;
    float y;

    auto operator<=>(const float_point&) const = default;
};

class usr_string
{
public:
    usr_string(const char* str) : str_(str) {}
    std::weak_ordering operator<=>(const usr_string&) const = default;
private:
    std::string str_;
};

int main(int argc, char const *argv[])
{
    point p1{1, 2};
    point p2{3, 4};

    std::strong_ordering r = p1 <=> p2;

    if (r == std::strong_ordering::equivalent) {
        std::cout << "p1 and p2 are equivalent" << std::endl;
    } else if (r == std::strong_ordering::less) {
        std::cout << "p1 is less than p2" << std::endl;
    } else if (r == std::strong_ordering::greater) {
        std::cout << "p1 is greater than p2" << std::endl;
    } else {
        std::cout << "p1 and p2 are not comparable" << std::endl;
    }   
    
    float_point fp1{1.1f, 2.2f};
    float_point fp2{3.3f, 4.4f};

    std::partial_ordering r1 = fp1 <=> fp2;

    if (r1 == std::partial_ordering::equivalent) {
        std::cout << "fp1 and fp2 are equivalent" << std::endl;
    } else if (r1 == std::partial_ordering::less) {
        std::cout << "fp1 is less than fp2" << std::endl;
    } else if (r1 == std::partial_ordering::greater) {
        std::cout << "fp1 is greater than fp2" << std::endl;
    } else {
        std::cout << "fp1 and fp2 are not comparable" << std::endl;
    }   

    auto r2 = (5 <=> 7 == std::strong_ordering::equivalent);
    std::cout << "r2: " << r2 << std::endl;

    r2 = (5 <=> 7 < 0);
    std::cout << "r2: " << r2 << std::endl;

    // strong_ordering
    std::cout << "name: " << typeid(decltype(5 <=> 7)).name() << std::endl;

    // partial_ordering
    std::cout << "name: " << typeid(decltype(1.5 <=> 7)).name() << std::endl;

    // weak_ordering
    usr_string s1{"hello"};
    usr_string s2{"world"};
    std::cout << "name: " << typeid(decltype(s1 <=> s2)).name() << std::endl;
 
    return 0;
}