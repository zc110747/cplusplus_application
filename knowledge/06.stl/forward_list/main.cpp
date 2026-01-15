//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::forward_list声明
//      2. std::forward_list方法
//      3. std::forward_list常用算法配合
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <forward_list>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <numeric>
#include <vector>

template<typename T>
void show_container(T container_val, std::string qstring)
{
    //empty, size
    if (!qstring.empty()) {
        for(auto index=qstring.size(); index<13; index++) {
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
    std::forward_list<int> vcon_0{2, 3, 3, 5, 7, 7, 1};
    std::forward_list<int> vcon_1{4, 6};
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

    val = std::search(vcon_0.begin(), vcon_0.end(), std::forward_list<int>{3, 5}.begin(), std::forward_list<int>{3, 5}.end());
    if(val != vcon_0.end()) {
        std::cout<<"search:"<<*val<<std::endl;
    }

    // 排序算法(forward_list不存在operator--, 不支持sort，reserve)
    std::cout<<"======= sort ======="<<std::endl;
    std::cout<<"is sort:"<<std::is_sorted(vcon_0.begin(), vcon_0.end())<<std::endl;
    
    val = std::is_sorted_until(vcon_0.begin(), vcon_0.end());
    if(val != vcon_0.end()) {
        std::cout<<"is_sorted_until:"<<*val<<std::endl;
    }

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

    // 生成算法
    std::cout<<"======= generate ======="<<std::endl;
    std::fill(vcon_0.begin(), vcon_0.end(), 5);
    show_container(vcon_0, "fill");

    std::generate(vcon_0.begin(), vcon_0.end(), [](){
        return rand()%100;
    });
    show_container(vcon_0, "generate");

    std::forward_list<int> vcon_2;
    vcon_2.resize(std::distance(vcon_0.begin(), vcon_0.end()));
    std::copy_if(vcon_0.begin(), vcon_0.end(), vcon_2.begin(), [](const auto &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy");

    // 移除算法
    std::cout<<"======= remove ======="<<std::endl;
    std::forward_list<int> vcon_r = {1, 3, 3, 6, 5, 6, 7};
    auto new_end = std::remove(vcon_r.begin(), vcon_r.end(), 3);
    vcon_r.erase_after(new_end, vcon_r.end());
    show_container(vcon_r, "remove");

    // 分割算法
    std::cout<<"======= partition ======="<<std::endl;
    std::forward_list<int> vcon_3 = {1, 2, 3, 4, 5, 6, 7};
    std::stable_partition(vcon_3.begin(), vcon_3.end(), [](const auto &ref_value){
        return ref_value % 2 == 0;
    });
    show_container(vcon_3, "stable_partition");
    std::partition(vcon_3.begin(), vcon_3.end(), [](const auto &ref_value){
        return ref_value % 2 == 0;
    });
    show_container(vcon_3, "partition");

//     // 归并算法
    std::cout<<"======= merge ======="<<std::endl;
    std::forward_list<int> vcon_4 = {1, 3, 5};
    std::forward_list<int> vcon_5 = {2, 4, 6, 8};
    std::forward_list<int> vcon_6;
    vcon_6.resize(std::distance(vcon_4.begin(), vcon_4.end()) + std::distance(vcon_5.begin(), vcon_5.end()));
    std::merge(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), vcon_6.begin());
    show_container(vcon_6, "merge");
    std::fill(vcon_6.begin(), vcon_6.end(), 0);
    std::set_difference(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), vcon_6.begin());
    show_container(vcon_6, "set_union");

    // 堆算法(forward_list不支持operator--，因此不支持堆算法)
    // std::greater<>() 执行大于比较操作
    // std::less<>() 执行小于比较操作
    // std::less_equal<>() 执行小于等于比较操作
    // std::greater_equal<>() 执行大于等于比较操作
    // std::equal_to<>() 执行等于比较操作
    // std::not_equal_to<>() 执行不等于比较操作

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

// 定义比较函数
template<typename T>
bool comparsion(const T& a, const T& b) {
    return a < b;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::forward_list<int> intv(array, array+5);
    std::forward_list<int> userforward_list;
    std::forward_list<std::string> forward_listforward_list{"0x00", "0x01", "0x02"}; //forward_list initialize
    
    show_container(forward_listforward_list, "forward_listforward_list");
    
    //clear, max_size
    forward_listforward_list.clear();
    std::cout<<"max_size:"<<forward_listforward_list.max_size()<<std::endl;

    show_container(userforward_list, "userforward_list");

    //assign
    userforward_list.assign(intv.begin(), intv.end());
    show_container(userforward_list, "assign");

    //resize, empty, max_size
    userforward_list.resize(3);
    show_container(userforward_list, "resize");
    std::cout << "empty:"<<userforward_list.empty() <<std::endl;
    std::cout << "max_size:"<<userforward_list.max_size() <<std::endl;

    //begin,  end,  cbegin,  cend
    std::forward_list<int>::iterator iterbegin = userforward_list.begin();
    std::forward_list<int>::iterator iterend = userforward_list.end();
    std::forward_list<int>::const_iterator iterconstbegin = userforward_list.cbegin();
    std::forward_list<int>::const_iterator iterconstend = userforward_list.cend();
    std::cout<<"begin:"<<*iterbegin<<" ";
    std::cout<<"cbegin:"<<*iterconstbegin<<std::endl;

    //emplace_after, emplace_back, emplace_front
    auto it = userforward_list.emplace_after(userforward_list.begin(), 6);
    show_container(userforward_list, "emplace_after");
    std::cout<<"empalce iterator:"<<*it<<std::endl;
    userforward_list.emplace_front(1);
    show_container(userforward_list, "emplace_front");

    //erase_after
    userforward_list.erase_after(++userforward_list.begin());
    show_container(userforward_list, "erase_after");

    //front
    int &front = userforward_list.front();
    front = 6;
    show_container(userforward_list, "front");

    //insert, before_begin, cbefore_begin
    userforward_list.insert_after(userforward_list.before_begin(), 2, 5);
    show_container(userforward_list, "insert_after");
    userforward_list.insert_after(userforward_list.cbefore_begin(), 2, 1);
    show_container(userforward_list, "insert_after");

    //swap, operator=
    std::forward_list<int> swapforward_list = std::forward_list<int>();
    swapforward_list.swap(userforward_list);
    show_container(swapforward_list, "swapforward_list");

    //pop_front, push_front
    swapforward_list.push_front(5);
    show_container(swapforward_list, "push_front");
    swapforward_list.pop_front();
    show_container(swapforward_list, "pop_front");

    //sort
    swapforward_list.sort();
    show_container(swapforward_list, "sort");

    //unique
    swapforward_list.unique();
    show_container(swapforward_list, "unique");

    //megre
    std::forward_list<int> megreforward_list = {4, 5, 3, 1, 2};
    megreforward_list.sort();
    swapforward_list.merge(megreforward_list, comparsion<int>);
    show_container(swapforward_list, "megre");
    std::cout<<"empty: "<<megreforward_list.empty()<<std::endl;

    //remove, remove_if
    swapforward_list.remove(2);
    show_container(swapforward_list, "remove");
    swapforward_list.remove_if([](const int& value){
        return value>7;
    });
    show_container(swapforward_list, "remove_if");

    //reverse   
    swapforward_list.reverse();
    show_container(swapforward_list, "reverse");

    //splice
    megreforward_list.assign(intv.begin(), intv.end());
    swapforward_list.splice_after(++swapforward_list.begin(), megreforward_list);
    show_container(swapforward_list, "splice");

    algorithm_process();
    return 0;
} 