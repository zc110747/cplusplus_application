//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::array声明
//      2. std::array方法
//      3. std::array常用算法配合
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <array>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>
#include <numeric>
#include <vector>

template<typename T>
void show_container(T container_val, std::string qstring)
{
    //empty, size
    if (!qstring.empty()) {
        for (auto index=qstring.size(); index<13; index++) {
            qstring.push_back(' ');
        }

        qstring += ":";
        std::cout<<qstring;
    }

    for (const auto &ref : container_val) {
        std::cout<<ref<<" ";
    }

    std::cout<<std::endl;
}

void algorithm_process(void)
{
    std::cout<<"==================== algorithm_process ===================="<<std::endl;

    // 查找算法
    std::cout<<"======= search ======="<<std::endl;
    std::array<int, 7> vcon_0{2, 3, 3, 5, 7, 7, 1};
    std::array<int, 2> vcon_1{4, 6};
    std::cout<<"all_of:"<<std::all_of(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value >= 1;
    })<<std::endl;
    std::cout<<"any_of:"<<std::any_of(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value > 5;
    })<<std::endl;

    auto val = std::find(vcon_0.begin(), vcon_0.end(), 3);
    if(val != vcon_0.end()) {
        std::cout<<"find:"<<*val<<std::endl;
    }
    val = std::find_if(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value > 5;
    });
    if(val != vcon_0.end()) {
        std::cout<<"find_if:"<<*val<<std::endl;
    }

    val = std::search(vcon_0.begin(), vcon_0.end(), std::array<int, 2>{3, 5}.begin(), std::array<int, 2>{3, 5}.end());
    if(val != vcon_0.end()) {
        std::cout<<"search:"<<*val<<std::endl;
    }

    // 排序算法
    std::cout<<"======= sort ======="<<std::endl;
    std::cout<<"is sort:"<<std::is_sorted(vcon_0.begin(), vcon_0.end())<<std::endl;
    
    val = std::is_sorted_until(vcon_0.begin(), vcon_0.end());
    if(val != vcon_0.end()) {
        std::cout<<"is_sorted_until:"<<*val<<std::endl;
    }
    std::sort(vcon_0.begin(), vcon_0.end());
    std::cout<<"is sort:"<<std::is_sorted(vcon_0.begin(), vcon_0.end())<<std::endl;

    std::reverse(vcon_0.begin(), vcon_0.end());
    show_container(vcon_0, "reverse");
    std::rotate(vcon_0.begin(), vcon_0.begin()+1, vcon_0.end());
    show_container(vcon_0, "rotate");
    std::random_shuffle(vcon_0.begin(), vcon_0.end());
    show_container(vcon_0, "random_shuffle");

    // 变换算法
    std::cout<<"======= transform ======="<<std::endl;
    std::transform(vcon_0.begin(), vcon_0.end(), vcon_0.begin(), [](const auto &ref_value){
        return ref_value*ref_value;
    });
    show_container(vcon_0, "transform");

    std::replace(vcon_0.begin(), vcon_0.end(), 9, 100);
    show_container(vcon_0, "replace");

    std::replace_if(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value == 100;
    }, 10);
    show_container(vcon_0, "replace_if");

    // 计数算法
    std::cout<<"======= count ======="<<std::endl;
    std::cout<<"count:"<<std::count(vcon_0.begin(), vcon_0.end(), 9)<<std::endl;
    std::cout<<"count if:"<<std::count_if(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value > 5;
    })<<std::endl; //满足元素的个数
    std::vector<int> destination;
    std::partial_sum(vcon_0.begin(), vcon_0.end(), std::back_inserter(destination)); //元素累加和
    show_container(destination, "partial_sum");

    destination.clear();
    std::adjacent_difference(vcon_0.begin(), vcon_0.end(), std::back_inserter(destination)); //相邻元素的差
    show_container(destination, "adjacent_difference");

    // 比较算法
    std::cout<<"======= compare ======="<<std::endl;
    std::cout<<"equal:"<<std::equal(vcon_0.begin(), vcon_0.end(), destination.begin())<<std::endl;
    auto val_1 = std::mismatch(vcon_1.begin(), vcon_1.end(), vcon_0.begin());
    if (val_1.first != vcon_1.end()) {
        std::cout<<"mismatch:"<<*val_1.first<<" "<<*val_1.second<<std::endl;
    }
    auto val_2 = std::mismatch(vcon_0.begin(), vcon_0.end(), vcon_1.begin(), [](const auto &ref_value_0, const auto &ref_value_1){
        return ref_value_0%2 == ref_value_1%2;
    });
    if (val_2.first != vcon_0.end()) {
        std::cout<<"mismatch:"<<*val_2.first<<" "<<*val_2.second<<std::endl;
    }
    
    // 生成算法
    std::cout<<"======= generate ======="<<std::endl;
    std::fill(vcon_0.begin(), vcon_0.end(), 5);
    show_container(vcon_0, "fill");

    std::generate(vcon_0.begin(), vcon_0.end(), [](){
        return rand()%100;
    });
    show_container(vcon_0, "generate");

    std::array<int, 7> vcon_2;
    std::fill(vcon_2.begin(), vcon_2.end(), 0);
    std::copy_if(vcon_0.begin(), vcon_0.end(), vcon_2.begin(), [](const auto &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy");

    // 移除算法
    // std::array是固定长度格式，移除算法不适用

    // 分割算法
    std::cout<<"======= partition ======="<<std::endl;
    std::array<int, 7> vcon_3 = {1, 2, 3, 4, 5, 6, 7};
    std::stable_partition(vcon_3.begin(), vcon_3.end(), [](const auto &ref_value){
        return ref_value % 2 == 0;
    });
    show_container(vcon_3, "stable_partition");
    std::partition(vcon_3.begin(), vcon_3.end(), [](const auto &ref_value){
        return ref_value % 2 == 0;
    });
    show_container(vcon_3, "partition");

    // 归并算法
    std::cout<<"======= merge ======="<<std::endl;
    std::array<int, 3> vcon_4 = {1, 3, 5};
    std::array<int, 4> vcon_5 = {2, 4, 6, 8};
    std::array<int, 7> vcon_6;
    std::merge(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), vcon_6.begin());
    show_container(vcon_6, "merge");
    std::fill(vcon_6.begin(), vcon_6.end(), 0);
    std::set_difference(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), vcon_6.begin());
    show_container(vcon_6, "set_union");

    // 堆算法
    // std::greater<>() 执行大于比较操作
    // std::less<>() 执行小于比较操作
    // std::less_equal<>() 执行小于等于比较操作
    // std::greater_equal<>() 执行大于等于比较操作
    // std::equal_to<>() 执行等于比较操作
    // std::not_equal_to<>() 执行不等于比较操作
    std::cout<<"======= heap ======="<<std::endl;
    std::make_heap(vcon_4.begin(), vcon_4.end());
    std::push_heap(vcon_4.begin(), vcon_4.end());
    std::sort_heap(vcon_4.begin(), vcon_4.end(), std::less<>());
    show_container(vcon_4, "make_heap");

    // 其它算法
    // std::plus<>() 执行加法操作
    // std::minus<>() 执行减法操作
    // std::multiplies<>() 执行乘法操作
    // std::divides<>() 执行除法操作
    // std::modulus<>() 执行取模操作
    // std::negate<>() 执行取反操作
    std::cout<<"======= other ======="<<std::endl;
    auto result = std::reduce(vcon_4.begin(), vcon_4.end(), 0, std::plus<>());
    std::cout<<"reduce:"<<result<<std::endl;
    result = std::reduce(vcon_4.begin(), vcon_4.end(), 1, std::multiplies<>());
    std::cout<<"reduce:"<<result<<std::endl;
    result = std::transform_reduce(vcon_4.begin(), vcon_4.end(), vcon_4.begin(), 0, std::plus<>(), std::multiplies<>());     //先转换后归约
    std::cout<<"transform_reduce:"<<result<<std::endl;
}

int main(int argc, char* argv[])
{
    std::array<int, 5> vcon_0 = {1, 2, 3, 4, 5};

    show_container(vcon_0, "vcon_0:");
    
    // at
    vcon_0.at(0) = 6;
    show_container(vcon_0, "vcon_0:");
    
    // begin, end, rbegin, rend, cbegin, cend, crbegin, crend
    auto itbegin = vcon_0.begin();
    std::array<int, 5>::iterator itend = vcon_0.end();
    std::array<int, 5>::reverse_iterator itrbegin = vcon_0.rbegin();
    std::array<int, 5>::reverse_iterator itrend = vcon_0.rend();
    std::array<int, 5>::const_iterator itconstbegin = vcon_0.cbegin();
    std::array<int, 5>::const_iterator itconstend = vcon_0.cend();
    std::array<int, 5>::const_reverse_iterator itconstrbegin = vcon_0.crbegin();
    std::array<int, 5>::const_reverse_iterator itconstrend = vcon_0.crend();
    std::cout<<"begin:"<<*itbegin<<" "<<"end:"<<*(itend-1)<<std::endl;
    std::cout<<"rbegin:"<<*itrbegin<<" "<<"rend:"<<*(itrend-1)<<std::endl;
    std::cout<<"cbegin:"<<*itconstbegin<<" "<<"cend:"<<*(itconstend-1)<<std::endl;
    std::cout<<"crbegin:"<<*itconstrbegin<<" "<<"crend:"<<*(itconstrend-1)<<std::endl;

    // operator[]
    std::cout<<"operator[]:"<<vcon_0[0]<<std::endl;

    // empty，size，max_size
    std::cout<<"empty"<<vcon_0.empty()<<std::endl;
    std::cout<<"size"<<vcon_0.size()<<std::endl;
    std::cout<<"max_size"<<vcon_0.max_size()<<std::endl;

    // fill
    vcon_0.fill(10);
    show_container(vcon_0, "fill");

    // data
    std::array<std::string, 2> u_str = {"0x01", "0x02"};
    std::string *point = u_str.data();
    *(point + 1) = "0x03";
    show_container(u_str, "u_str");

    // back, front
    std::string &front = u_str.front();
    front = "0x2A";
    show_container(u_str, "front");
    std::string &back = u_str.back();
    back = "0x15";
    show_container(u_str, "back");

    // swap
    std::array<std::string, 2> userswapstring;
    userswapstring.swap(u_str);
    show_container(userswapstring, "swap");

    algorithm_process();

    return 0;
} 