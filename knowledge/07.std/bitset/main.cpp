<<<<<<< HEAD
﻿/*
std::bitset是 C++ 标准库中的一个类模板，用于表示固定大小的位序列。
它提供了一种方便的方式来处理二进制数据，例如位操作、位计数、位测试等

主要特点：
1. 固定大小：std::bitset的大小在编译时确定，因此它适用于需要固定大小的位序列的场景。
2. 高效操作：std::bitset提供了高效的位操作，例如位设置、位清除、位翻转、位测试等。
3. 类型安全：std::bitset是类型安全的，它确保所有操作都是在位级别上进行的，避免了类型错误。
4. 易于使用：std::bitset提供了直观的接口，使得位操作变得简单易懂

成员函数：
all: 检查所有位是否都设置为1。
any: 检查是否至少有一位被设置为1。
count: 计算位序列中设置为1的位数。
flip: 翻转位序列中的所有位或指定位。
none: 检查是否没有位被设置为1。
operator[]: 访问位序列中的指定位。
reset: 将位序列中的所有位设置为0。
set: 将位序列中的所有位设置为1或指定位。
size: 返回位序列的大小。
test: 检查指定位是否被设置为1。
to_string: 将位序列转换为字符串。
to_ullong: 将位序列转换为无符号长整数。
to_ulong: 将位序列转换为无符号整数。

std::bitcast
std::bit_cast 是 C++20 引入的一个函数模板，定义在 <bit> 头文件中，用于在不同类型之间进行位级别的转换。它提供了一种类型安全的方式来重新解释对象的位模式，避免了传统的类型转换（如 reinterpret_cast）可能带来的未定义行为
*/
#include <bitset>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <bit>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    std::bitset<8> userbitset("11111101");
    std::bitset<4> lowbitset(std::string("0110"));

    //count, any, none, all
    cout<<"count: "<<userbitset.count()<<endl;
    cout<<std::boolalpha;
    cout<<"all: "<<userbitset.all()<<endl;
    cout<<"any: "<<userbitset.any()<<endl;
    cout<<"none: "<<userbitset.none()<<endl;

    //to_string
    cout<<"to_string: "<<lowbitset.to_string()<<endl;

    //flip
    cout<<"flip: "<<lowbitset.flip(2)<<endl;
    cout<<"flip: "<<lowbitset.flip()<<endl;

    //operator [], &, |, ^, <<, >>
    cout<<std::noboolalpha;
    cout<<"operator [] :"<<lowbitset[1]<<endl;
    {
        std::bitset<8> val1("01010000");
        std::bitset<8> val2("11110000");

        auto val = val1&val2;
        cout<<"operator & :"<<val<<endl;

        val = val1 | val2;
        cout<<"operator | :"<<val<<endl;

        val = val1 ^ val2;
        cout<<"operator ^ :"<<val<<endl;

        auto val3 = val1>>1;
        auto val4 = val2<<1;
        cout<<"operator <<1 >>1:"<<val3<<" "<<val4<<endl;
    }
    
    //size, test
    cout<<"size:"<<lowbitset.size()<<endl;
    for(int index=0; index<lowbitset.size(); index++)
    {
        cout<<"index:"<<index<<" value:"<<lowbitset.test(index)<<endl;
    }
    
    //reset
    lowbitset.reset();
    cout<<"reset:"<<lowbitset<<endl;

    //set
    lowbitset.set(2, true);
    cout<<"set:"<<lowbitset<<endl;

    //to_ullong, to_ulong
    cout<<"ullong:"<<lowbitset.to_ullong()<<endl;
    cout<<"ulong:"<<lowbitset.to_ulong()<<endl;
    
    auto val = lowbitset.to_ulong();
    std::bitset<8> bitsetval{lowbitset.to_ulong()};
    cout<<"bitset:"<<bitsetval<<endl;

#if __cplusplus >= 202002L
    // bit_cast
    float f = 3.14f;
    // 将 float 类型的 f 转换为 uint32_t 类型
    uint32_t u = std::bit_cast<uint32_t>(f);

    std::cout << "Float value: " << f << std::endl;
    std::cout << "Bit-casted uint32_t value: " << u << std::endl;

    // 将 uint32_t 类型的 u 转换回 float 类型
    float f_back = std::bit_cast<float>(u);
    std::cout << "Back to float value: " << f_back << std::endl;
#endif

    return 0;
} 
=======
﻿/*
std::bitset是 C++ 标准库中的一个类模板，用于表示固定大小的位序列。
它提供了一种方便的方式来处理二进制数据，例如位操作、位计数、位测试等

主要特点：
1. 固定大小：std::bitset的大小在编译时确定，因此它适用于需要固定大小的位序列的场景。
2. 高效操作：std::bitset提供了高效的位操作，例如位设置、位清除、位翻转、位测试等。
3. 类型安全：std::bitset是类型安全的，它确保所有操作都是在位级别上进行的，避免了类型错误。
4. 易于使用：std::bitset提供了直观的接口，使得位操作变得简单易懂

成员函数：
all: 检查所有位是否都设置为1。
any: 检查是否至少有一位被设置为1。
count: 计算位序列中设置为1的位数。
flip: 翻转位序列中的所有位或指定位。
none: 检查是否没有位被设置为1。
operator[]: 访问位序列中的指定位。
reset: 将位序列中的所有位设置为0。
set: 将位序列中的所有位设置为1或指定位。
size: 返回位序列的大小。
test: 检查指定位是否被设置为1。
to_string: 将位序列转换为字符串。
to_ullong: 将位序列转换为无符号长整数。
to_ulong: 将位序列转换为无符号整数。

std::bitcast
std::bit_cast 是 C++20 引入的一个函数模板，定义在 <bit> 头文件中，用于在不同类型之间进行位级别的转换。它提供了一种类型安全的方式来重新解释对象的位模式，避免了传统的类型转换（如 reinterpret_cast）可能带来的未定义行为
*/
#include <bitset>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <bit>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    std::bitset<8> userbitset("11111101");
    std::bitset<4> lowbitset(std::string("0110"));

    //count, any, none, all
    cout<<"count: "<<userbitset.count()<<endl;
    cout<<std::boolalpha;
    cout<<"all: "<<userbitset.all()<<endl;
    cout<<"any: "<<userbitset.any()<<endl;
    cout<<"none: "<<userbitset.none()<<endl;

    //to_string
    cout<<"to_string: "<<lowbitset.to_string()<<endl;

    //flip
    cout<<"flip: "<<lowbitset.flip(2)<<endl;
    cout<<"flip: "<<lowbitset.flip()<<endl;

    //operator [], &, |, ^, <<, >>
    cout<<std::noboolalpha;
    cout<<"operator [] :"<<lowbitset[1]<<endl;
    {
        std::bitset<8> val1("01010000");
        std::bitset<8> val2("11110000");

        auto val = val1&val2;
        cout<<"operator & :"<<val<<endl;

        val = val1 | val2;
        cout<<"operator | :"<<val<<endl;

        val = val1 ^ val2;
        cout<<"operator ^ :"<<val<<endl;

        auto val3 = val1>>1;
        auto val4 = val2<<1;
        cout<<"operator <<1 >>1:"<<val3<<" "<<val4<<endl;
    }
    
    //size, test
    cout<<"size:"<<lowbitset.size()<<endl;
    for(int index=0; index<lowbitset.size(); index++)
    {
        cout<<"index:"<<index<<" value:"<<lowbitset.test(index)<<endl;
    }
    
    //reset
    lowbitset.reset();
    cout<<"reset:"<<lowbitset<<endl;

    //set
    lowbitset.set(2, true);
    cout<<"set:"<<lowbitset<<endl;

    //to_ullong, to_ulong
    cout<<"ullong:"<<lowbitset.to_ullong()<<endl;
    cout<<"ulong:"<<lowbitset.to_ulong()<<endl;
    
    auto val = lowbitset.to_ulong();
    std::bitset<8> bitsetval{lowbitset.to_ulong()};
    cout<<"bitset:"<<bitsetval<<endl;

#if __cplusplus >= 202002L
    // bit_cast
    float f = 3.14f;
    // 将 float 类型的 f 转换为 uint32_t 类型
    uint32_t u = std::bit_cast<uint32_t>(f);

    std::cout << "Float value: " << f << std::endl;
    std::cout << "Bit-casted uint32_t value: " << u << std::endl;

    // 将 uint32_t 类型的 u 转换回 float 类型
    float f_back = std::bit_cast<float>(u);
    std::cout << "Back to float value: " << f_back << std::endl;
#endif

    return 0;
} 
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
