/*
std::array 它提供了固定大小的数组功能

构造函数
std::array<T, N>()：默认构造函数，创建一个包含 N 个默认初始化元素的数组
std::array<T, N>(const T& value)：创建一个包含 N 个值为 value 的元素的数组
std::array<T, N>(std::initializer_list<T> ilist)：使用初始化列表创建数组

成员函数
at(size_type pos)：返回指定位置的元素的引用，如果越界则抛出 std::out_of_range 异常
operator[](size_type pos)：返回指定位置的元素的引用，不进行越界检查
front()：返回第一个元素的引用
back()：返回最后一个元素的引用
data()：返回指向数组第一个元素的指针
empty()：检查数组是否为空，返回 bool 类型
size()：返回数组中元素的数量
max_size()：返回数组可以容纳的最大元素数量，通常与 size() 相同
fill(const T& value)：将数组中的所有元素设置为指定的值
swap(std::array<T, N>& other)：交换两个数组的内容

迭代器
begin()：返回指向数组第一个元素的迭代器
end()：返回指向数组最后一个元素的后一个位置的迭代器
rbegin()：返回指向数组最后一个元素的反向迭代器
rend()：返回指向数组第一个元素的前一个位置的反向迭代器
cbegin()：返回指向数组第一个元素的常量迭代器
cend()：返回指向数组最后一个元素的后一个位置的常量迭代器
crbegin()：返回指向数组最后一个元素的常量反向迭代器
crend()：返回指向数组第一个元素的前一个位置的常量反向迭代器
*/
#include <array>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl;

typedef enum
{
    SHOW_MODE_ENUM_LAMBDA = 0,
    SHOW_MODE_ENUM_TYPE_AUTO,
    SHOW_MODE_ENUM_TYPE_ITERATOR,
    SHOW_MODE_ENUM_TYPE_OPERATOR
}SHOW_MODE_ENUM;

template<typename T, size_t N>
void show_array(std::array<T, N> value, string qstring, SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA)
{
    //empty, size
    if(!qstring.empty())
    {
        for(auto index=qstring.size(); index<13; index++)
            qstring.push_back(' ');
        qstring += ":";
        cout<<qstring;
    }

    //begin, end
    if(mode == SHOW_MODE_ENUM_LAMBDA)
    {
        std::for_each(value.begin(), value.end(), [](const T &ref_value){
            cout<<ref_value<<" ";
        });
    }
    //auto
    else if(mode == SHOW_MODE_ENUM_TYPE_AUTO)
    {
        for(auto &ref : value)
            cout<<ref<<" ";
    }
    //iterator
    else if(mode == SHOW_MODE_ENUM_TYPE_ITERATOR)
    {
        for(typename std::array<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    //operator[], size
    else if(mode == SHOW_MODE_ENUM_TYPE_OPERATOR)
    {
        for(int index=0; index<value.size(); index++)
            cout<<value[index]<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    std::array<int, 5> u_arr = {1, 2, 3, 4, 5};

    show_array(u_arr, "u_arr:");
    
    // at
    u_arr.at(0) = 6;
    show_array(u_arr, "u_arr:");
    
    // begin, end, rbegin, rend, cbegin, cend, crbegin, crend
    auto itbegin = u_arr.begin();
    std::array<int, 5>::iterator itend = u_arr.end();
    std::array<int, 5>::reverse_iterator itrbegin = u_arr.rbegin();
    std::array<int, 5>::reverse_iterator itrend = u_arr.rend();
    std::array<int, 5>::const_iterator itconstbegin = u_arr.cbegin();
    std::array<int, 5>::const_iterator itconstend = u_arr.cend();
    std::array<int, 5>::const_reverse_iterator itconstrbegin = u_arr.crbegin();
    std::array<int, 5>::const_reverse_iterator itconstrend = u_arr.crend();
    cout<<"begin:"<<*itbegin<<" "<<"end:"<<*(itend-1)<<endl;
    cout<<"rbegin:"<<*itrbegin<<" "<<"rend:"<<*(itrend-1)<<endl;
    cout<<"cbegin:"<<*itconstbegin<<" "<<"cend:"<<*(itconstend-1)<<endl;
    cout<<"crbegin:"<<*itconstrbegin<<" "<<"crend:"<<*(itconstrend-1)<<endl;

    // operator[]
    cout<<"operator[]:"<<u_arr[0]<<endl;

    // empty，size，max_size
    cout<<"empty"<<u_arr.empty()<<endl;
    cout<<"size"<<u_arr.size()<<endl;
    cout<<"max_size"<<u_arr.max_size()<<endl;

    // fill
    u_arr.fill(10);
    show_array(u_arr, "fill");

    // data
    std::array<string, 2> u_str = {"0x01", "0x02"};
    string *point = u_str.data();
    *(point + 1) = "0x03";
    show_array(u_str, "u_str");

    // back, front
    string &front = u_str.front();
    front = "0x2A";
    show_array(u_str, "front");
    string &back = u_str.back();
    back = "0x15";
    show_array(u_str, "back");

    // swap
    std::array<string, 2> userswapstring;
    userswapstring.swap(u_str);
    show_array(userswapstring, "swap");

    return 0;
} 