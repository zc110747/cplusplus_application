/*

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
