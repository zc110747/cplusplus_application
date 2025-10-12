//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::set声明
//      2. std::set方法
//      3. std::set常用算法配合
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <set>
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
        for (auto index=qstring.size(); index<13; index++) {
            qstring.push_back(' ');
        }
        qstring += ":";
        std::cout << qstring;
    }

    for (const auto &ref : container_val) {
        std::cout << ref << " ";
    }

    std::cout << std::endl;
}

void algorithm_process(void)
{
    std::cout << "==================== algorithm_process ====================" <<std::endl;

    // 查找算法
    std::cout << "======= search =======" <<std::endl;
    std::set<int> vcon_0{2, 3, 3, 5, 7, 7, 1};
    std::set<int> vcon_1{4, 6};

    show_container(vcon_0, "vcon_0");
    show_container(vcon_1, "vcon_1");

    std::cout << "all_of:" <<std::all_of(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value >= 1;
    }) <<std::endl;
    std::cout << "any_of:" <<std::any_of(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value > 5;
    }) <<std::endl;

    auto val = std::find(vcon_0.begin(), vcon_0.end(), 3);
    if(val != vcon_0.end()) {
        std::cout << "find:" << *val <<std::endl;
    }
    val = std::find_if(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value > 5;
    });
    if(val != vcon_0.end()) {
        std::cout << "find_if:" << *val <<std::endl;
    }

    auto search_seq = std::set<int>{3, 5};
    val = std::search(vcon_0.begin(), vcon_0.end(), search_seq.begin(), search_seq.end());
    if (val != vcon_0.end()) {
        std::cout << "search:" << *val <<std::endl;
    } else {
        std::cout << "search:" << "not found" <<std::endl;
    }

    // // 排序算法(multiset默认排序)
    std::cout << "======= sort =======" <<std::endl;
    std::cout << "is sort:" <<std::is_sorted(vcon_0.begin(), vcon_0.end()) <<std::endl;
    
    val = std::is_sorted_until(vcon_0.begin(), vcon_0.end());
    if(val != vcon_0.end()) {
        std::cout << "is_sorted_until:" << *val <<std::endl;
    }

    // 变换算法
    std::set<int> vcon_new;
    std::cout << " ======= transform =======" << std::endl;
    std::transform(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_new, vcon_new.end()), [](const auto &ref_value){
        return ref_value*ref_value;
    }); //set的键值是const类型,不能直接通过transform修改,创建新的容器
    show_container(vcon_new, "transform");

    // 计数算法
    std::cout << "======= count =======" <<std::endl;
    std::cout << "count:" <<std::count(vcon_0.begin(), vcon_0.end(), 9) <<std::endl;
    std::cout << "count if:" <<std::count_if(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value > 5;
    }) <<std::endl; //满足元素的个数
    std::vector<int> destination;
    std::partial_sum(vcon_0.begin(), vcon_0.end(), std::back_inserter(destination)); //元素累加和
    show_container(destination, "partial_sum");

    destination.clear();
    std::adjacent_difference(vcon_0.begin(), vcon_0.end(), std::back_inserter(destination)); //相邻元素的差
    show_container(destination, "adjacent_difference");

    // 比较算法
    std::cout << "======= compare =======" <<std::endl;
    std::cout << "equal:" <<std::equal(vcon_0.begin(), vcon_0.end(), destination.begin()) <<std::endl;
    auto val_1 = std::mismatch(vcon_1.begin(), vcon_1.end(), vcon_0.begin());
    if (val_1.first != vcon_1.end()) {
        std::cout << "mismatch:" << *val_1.first << " " << *val_1.second <<std::endl;
    }
    auto val_2 = std::mismatch(vcon_0.begin(), vcon_0.end(), vcon_1.begin(), [](const auto &ref_value_0, const auto &ref_value_1){
        return ref_value_0%2 == ref_value_1%2;
    });
    if (val_2.first != vcon_0.end()) {
        std::cout << "mismatch:" << *val_2.first << " " << *val_2.second <<std::endl;
    }
    
    // 生成算法
    std::set<int> vcon_2;
    std::copy_if(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_2, vcon_2.end()), [](const auto &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy_if");

    // 移除算法

    // 分割算法(multiset默认排序)

    // 归并算法
    std::cout << "======= merge =======" <<std::endl;
    std::set<int> vcon_4 = {1, 3, 5};
    std::set<int> vcon_5 = {2, 3, 4, 6, 8};
    std::set<int> vcon_6;
    std::merge(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), std::inserter(vcon_6, vcon_6.end()));
    show_container(vcon_6, "merge");
    vcon_6.clear();
    std::set_difference(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), std::inserter(vcon_6, vcon_6.end()));
    show_container(vcon_6, "set_difference");
    vcon_6.clear();
    std::set_union(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), std::inserter(vcon_6, vcon_6.end()));
    show_container(vcon_6, "set_union");

    // 堆算法
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
    std::cout << "======= other =======" <<std::endl;
    auto result = std::reduce(vcon_4.begin(), vcon_4.end(), 0, std::plus<>());
    std::cout << "reduce:" <<result <<std::endl;
    result = std::reduce(vcon_4.begin(), vcon_4.end(), 1, std::multiplies<>());
    std::cout << "reduce:" <<result <<std::endl;
    result = std::transform_reduce(vcon_4.begin(), vcon_4.end(), vcon_4.begin(), 0, std::plus<>(), std::multiplies<>());     //先转换后归约
    std::cout << "transform_reduce:" <<result <<std::endl;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::set<int> arr_set(array, array+5);
    std::set<int> usr_set(arr_set);
    std::set<std::string> str_set;
    
    //operator=
    str_set = std::set<std::string>({"0x00", "0x01", "0x02"});
    show_container(str_set, "str_set");

    //count, clear, size, empty
    std::cout << " count"<<str_set.count("0x01") << " | ";
    str_set.clear();
    std::cout << " size:"<<str_set.size() << std::endl;
    std::cout << " empty:"<<str_set.empty() << std::endl;

    show_container(usr_set, "usr_set");

    //max_size
    std::cout << " max_size:"<<usr_set.max_size() << std::endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::set<int>::iterator iterbegin = usr_set.begin();
    std::set<int>::iterator iterend = usr_set.end();
    std::set<int>::const_iterator iterconstbegin = usr_set.cbegin();
    std::set<int>::const_iterator iterconstend = usr_set.cend();
    std::set<int>::reverse_iterator iterrbegin = usr_set.rbegin();
    std::set<int>::reverse_iterator iterrend = usr_set.rend();
    std::set<int>::const_reverse_iterator iterconstrbegin = usr_set.crbegin();
    std::set<int>::const_reverse_iterator iterconstrend = usr_set.crend();
    std::cout << " begin:" << *iterbegin<<" | " << "end:" << *(--iterend) << " | ";
    std::cout << " cbegin:" << *iterconstbegin<<" | " << "cend:" << *(--iterconstend) << " | ";
    std::cout << " rbegin:" << *iterrbegin<<" | " << "rend:" << *(--iterrend) << " | ";
    std::cout << " crbegin:" << *iterconstrbegin<<" | " << "crend:" << *(--iterconstrend) << " | " << std::endl;

    //emplace, find, emplace_hint
    auto it = usr_set.emplace(15);
    show_container(usr_set, "emplace");
    std::cout << " val:" << *(it.first) << "," << "status:"<<it.second << std::endl;
    auto find = usr_set.find(8);
    usr_set.emplace_hint(find, 9);
    show_container(usr_set, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::set<int>::const_iterator, std::set<int>::const_iterator> ret;
    ret = usr_set.equal_range(5);
    if(ret.first != usr_set.end()) {
        std::cout << " equal_range lowerbound:" << *(ret.first) << std::endl;       
    } else {
        std::cout << " invalid equal_range lowerbound" << std::endl;
    }
    if(ret.second != usr_set.end()) {
        std::cout << " equal_range upperbound:" << *(ret.second) << std::endl;
    } else {
        std::cout << " invalid equal_range upperbound" << std::endl;
    }

    std::set<int>::const_iterator iterlower = usr_set.lower_bound(5);
    std::set<int>::const_iterator iterupper = usr_set.upper_bound(5);
    if(iterlower != usr_set.end()) {
        std::cout << " lowerbound:" << *iterlower << std::endl;
    } else {
        std::cout << " invalid lowerbound" << std::endl;
    }
    if(iterupper != usr_set.end()) {
        std::cout << " upperbound:" << *iterupper << std::endl;
    } else {
        std::cout << " invalid upperbound" << std::endl;
    }

    //erase
    auto iter=usr_set.begin();
    while (iter!=usr_set.end()) {
        if (*iter == 3) {
            iter=usr_set.erase(iter);
        } else {
            iter++;
        }
    }
    show_container(usr_set, "erase");

    //insert
    usr_set.insert(15);
    show_container(usr_set, "insert");

    //key_comp
    std::cout << " key_comp:";
    for (auto &x:usr_set) {
        if(usr_set.key_comp()(x, 7)) {
            std::cout <<x<<" ";
        }
    }
    std::cout << std::endl;

    //value_comp
    std::cout << " value comp:";
    for (auto &x:usr_set) {
        if(usr_set.value_comp()(x, 9)) {
            std::cout <<x<<" ";
        }
    }
    std::cout << std::endl;

    //swap
    std::set<int> swapset;
    swapset.swap(usr_set);
    show_container(swapset, "swapset");

    algorithm_process();
    
    return 0;
}
