//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::bitset声明
//      2. std::bitset成员函数
//      3. std::bit_cast转换
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <bitset>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <bit>

int main(int argc, char* argv[])
{
    std::bitset<8> bits_0("11111101");
    std::bitset<4> bits_1(std::string("0110"));

    //count, any, none, all
    std::cout << "count: " << bits_0.count() << std::endl ;
    std::cout << std::boolalpha;
    std::cout << "all: " << bits_0.all() << std::endl ;
    std::cout << "any: " << bits_0.any() << std::endl ;
    std::cout << "none: " << bits_0.none() << std::endl ;

    //to_string
    std::cout << "to_string: " << bits_1.to_string() << std::endl ;

    //flip
    std::cout << "flip: " << bits_1.flip(2) << std::endl ;
    std::cout << "flip: " << bits_1.flip() << std::endl ;

    //operator [], &, |, ^, <<, >>
    std::cout << std::noboolalpha;
    std::cout << "operator [] :" << bits_1[1] << std::endl ;
    {
        std::bitset<8> val1("01010000");
        std::bitset<8> val2("11110000");

        auto val = val1&val2;
        std::cout << "operator & :" << val << std::endl ;

        val = val1 | val2;
        std::cout << "operator | :" << val << std::endl ;

        val = val1 ^ val2;
        std::cout << "operator ^ :" << val << std::endl ;

        auto val3 = val1>>1;
        auto val4 = val2<<1;
        std::cout << "operator <<1 >>1:" << val3<<" " << val4 << std::endl ;
    }
    
    //size, test
    std::cout << "size:" << bits_1.size() << std::endl ;
    for(int index=0; index<bits_1.size(); index++)
    {
        std::cout << "index:" << index<<" value:" << bits_1.test(index) << std::endl ;
    }
    
    //reset
    bits_1.reset();
    std::cout << "reset:" << bits_1 << std::endl ;

    //set
    bits_1.set(2, true);
    std::cout << "set:" << bits_1 << std::endl ;

    //to_ullong, to_ulong
    std::cout << "ullong:" << bits_1.to_ullong() << std::endl ;
    std::cout << "ulong:" << bits_1.to_ulong() << std::endl ;
    
    auto val = bits_1.to_ulong();
    std::bitset<8> bitsetval{bits_1.to_ulong()};
    std::cout << "bitset:" << bitsetval << std::endl ;

#if __cplusplus >= 202002L
    // bit_cast
    float f = 3.14f;
    // 将 float 类型的 f 转换为 unsigned int 类型
    unsigned int u = std::bit_cast<unsigned int>(f);

    std::cout << "Float value: " << f << std::endl;
    std::cout << "Bit-casted unsigned int value: " << u << std::endl;

    // 将 unsigned int 类型的 u 转换回 float 类型
    float f_back = std::bit_cast<float>(u);
    std::cout << "Back to float value: " << f_back << std::endl;
#endif

    return 0;
} 
