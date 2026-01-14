//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::multimap声明
//      2. std::multimap方法
//      3. std::multimap常用算法配合
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <map>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

// 辅助模板,检测是否为std::multimap
template<typename T, typename = std::enable_if_t<std::is_same<T, std::multimap<typename T::key_type, typename T::mapped_type>>::value>>
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

    for (const auto &pair : container_val) {
        std::cout<<"["<<pair.first<<": "<<pair.second<<"] ";
    }

    std::cout<<std::endl;
}

void algorithm_process(void)
{
    std::cout<<"==================== algorithm_process ===================="<<std::endl;

    // 查找算法
    std::cout<<"======= search ======="<<std::endl;
    std::multimap<int, int> vcon_0 = {
        {1, 1},
        {3, 3},
        {3, 4},
        {5, 5},
        {2, 2},
    };
    std::multimap<int, int> vcon_1 = {
        {5, 5},
        {4, 4},
    };
    show_container(vcon_0, "vcon_0");
    show_container(vcon_1, "vcon_1");
    std::cout<<"all_of:"<<std::all_of(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value.first >= 1;
    })<<std::endl;
    std::cout<<"any_of:"<<std::any_of(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value.first > 5;
    })<<std::endl;

    auto val = std::find(vcon_0.begin(), vcon_0.end(), std::pair<const int, int>(3, 3));
    if(val != vcon_0.end()) {
        std::cout<<"find:"<<val->first<<std::endl;
    }
    val = std::find_if(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value.first == 2;
    });
    if(val != vcon_0.end()) {
        std::cout<<"find_if:"<<val->first<<std::endl;
    }
    val = std::search(vcon_0.begin(), vcon_0.end(), vcon_1.begin(), vcon_1.end());
    if(val != vcon_0.end()) {
        std::cout<<"search:"<<val->first<<std::endl;
    }

    // 排序算法(std::multimap内部按照红黑树实现,默认按照键值排序的)

    // 变换算法
    std::multimap<int, int> vcon_new;
    std::cout<<"======= transform ======="<<std::endl;
    std::transform(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_new, vcon_new.end()), [](std::pair<const int, int> &pair){
        return std::make_pair(pair.first, pair.second+2);
    }); //map的键值是const类型,不能直接通过transform修改,创建新的容器
    show_container(vcon_new, "transform");

    // 计数算法
    std::cout<<"======= count ======="<<std::endl;
    std::cout<<"count:"<<std::count(vcon_0.begin(), vcon_0.end(), std::pair<const int, int>(3, 3))<<std::endl;
    std::cout<<"count if:"<<std::count_if(vcon_0.begin(), vcon_0.end(), [](const auto &ref_value){
        return ref_value.first > 2;
    })<<std::endl; //满足元素的个数

    // 比较算法
    std::cout<<"======= compare ======="<<std::endl;
    std::cout<<"equal:"<<std::equal(vcon_0.begin(), vcon_0.end(), vcon_0.begin())<<std::endl;
    auto val_1 = std::mismatch(vcon_1.begin(), vcon_1.end(), vcon_0.begin());
    if (val_1.first != vcon_1.end()) {
        std::cout<<"mismatch:"<<val_1.first->first<<" "<<val_1.second->first<<std::endl;
    }
    auto val_2 = std::mismatch(vcon_0.begin(), vcon_0.end(), vcon_1.begin(), [](const auto &ref_value_0, const auto &ref_value_1){
        return ref_value_0.first == ref_value_1.first;
    });
    if (val_2.first != vcon_0.end()) {
        std::cout<<"mismatch:"<<val_2.first->first<<" "<<val_2.second->first<<std::endl;
    }
    
    // 生成算法
    std::multimap<int, int> vcon_2;
    std::copy_if(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_2, vcon_2.begin()), [](const auto &ref_value){
        return ref_value.first < 10;
    });
    show_container(vcon_2, "copy_if");

    // 移除算法


    // 分割算法

    // // 归并算法
    std::cout<<"======= merge ======="<<std::endl;
    std::multimap<int, int> vcon_4 = {
        {3, 1},
        {3, 3}, 
        {5, 5},
    };
    std::multimap<int, int> vcon_5 = {
        {2, 2},
        {2, 4},
        {6, 6},
        {8, 8},
    };
    std::multimap<int, int> vcon_6;
    std::merge(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), std::inserter(vcon_6, vcon_6.begin()));
    show_container(vcon_6, "merge");
    vcon_6.clear();
    std::set_difference(vcon_4.begin(), vcon_4.end(), vcon_5.begin(), vcon_5.end(), std::inserter(vcon_6, vcon_6.begin()));
    show_container(vcon_6, "set_union");

    // 堆算法(forward_list不支持operator--,因此不支持堆算法)
    // std::greater<>() 执行大于比较操作
    // std::less<>() 执行小于比较操作
    // std::less_equal<>() 执行小于等于比较操作
    // std::greater_equal<>() 执行大于等于比较操作
    // std::equal_to<>() 执行等于比较操作
    // std::not_equal_to<>() 执行不等于比较操作

    // // 其它算法
    // // std::plus<>() 执行加法操作
    // // std::minus<>() 执行减法操作
    // // std::multiplies<>() 执行乘法操作
    // // std::divides<>() 执行除法操作
    // // std::modulus<>() 执行取模操作
    // // std::negate<>() 执行取反操作
}

int main(int argc, char* argv[])
{
    std::multimap<std::string, int> usermultimap = {
        {"alpha", 0},
        {"beta", 0}, 
        {"beta", 5},
        {"beta", 2},
        {"gamma", 1},   
    };
    auto copymultimap(usermultimap);

    //size, clear, max_size, empty
    show_container(copymultimap, "copymultimap");
    std::cout<<"size:"<<copymultimap.size()<<std::endl;
    copymultimap.clear();
    std::cout<<"clear size:"<<copymultimap.size()<<std::endl;
    std::cout<<"max_size:"<<copymultimap.max_size()<<std::endl;
    std::cout<<"empty:"<<copymultimap.empty()<<std::endl;

    show_container(usermultimap, "usermultimap");

    //begin, end, cbegin, cend
    //rbegin, rend, rcbegin, rcend
    std::multimap<std::string, int>::iterator iterbegin = usermultimap.begin();
    std::multimap<std::string, int>::iterator iterend = usermultimap.end();
    std::multimap<std::string, int>::const_iterator iterconstbegin = usermultimap.cbegin();
    std::multimap<std::string, int>::const_iterator iterconstend = usermultimap.cend();
    std::multimap<std::string, int>::reverse_iterator iterrbegin = usermultimap.rbegin();
    std::multimap<std::string, int>::reverse_iterator iterrend = usermultimap.rend();
    std::multimap<std::string, int>::const_reverse_iterator iterconstrbegin = usermultimap.crbegin();
    std::multimap<std::string, int>::const_reverse_iterator iterconstrend = usermultimap.crend();
    std::cout<<"begin: "<<iterbegin->first<<"=>"<<iterbegin->second<<",";
    --iterend;
    std::cout<<"end: "<<iterend->first<<"=>"<<iterend->second<<",";
    std::cout<<"cbegin: "<<iterconstbegin->first<<"=>"<<iterconstbegin->second<<",";
    --iterconstend;
    std::cout<<"cend: "<<iterconstend->first<<"=>"<<iterconstend->second<<",";
    std::cout<<"rbegin: "<<iterrbegin->first<<"=>"<<iterrbegin->second<<",";
    --iterrend;
    std::cout<<"rend: "<<iterrend->first<<"=>"<<iterrend->second<<",";
    std::cout<<"crbegin: "<<iterconstrbegin->first<<"=>"<<iterconstrbegin->second<<",";
    --iterconstrend;
    std::cout<<"crend: "<<iterconstrend->first<<"=>"<<iterconstrend->second<<std::endl;

    //count
    std::cout<<"count:"<<usermultimap.count("beta")<<std::endl;

    //emplace, emplace_hint
    usermultimap.emplace("delta", 6);
    show_container(usermultimap, "emplace");
    usermultimap.emplace_hint(usermultimap.begin(), "epsilon", 10);
    show_container(usermultimap, "emplace");

    //equal_range, lower_bound, upper_bound
    std::pair<std::multimap<std::string, int>::iterator, std::multimap<std::string, int>::iterator> ret;
    ret = usermultimap.equal_range("beta");
    if (ret.first != usermultimap.end())  {
        std::cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<std::endl;
    } else {
        std::cout<<"invaild equal_range lower_bound"<<std::endl;
    }
    if (ret.second != usermultimap.end())  {
        std::cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<std::endl;
    } else {
        std::cout<<"invaild equal_range upper_bound"<<std::endl;
    }

    std::multimap<std::string, int>::iterator lower = usermultimap.lower_bound("beta");
    std::multimap<std::string, int>::iterator upper = usermultimap.upper_bound("beta");
    if (lower != usermultimap.end())  {
        std::cout<<"lower_bound:"<<lower->first<<"=>"<<lower->second<<std::endl;
    } else {
        std::cout<<"invaild lower_bound"<<std::endl;
    }
    if (upper != usermultimap.end())  {
        std::cout<<"upper_bound:"<<upper->first<<"=>"<<upper->second<<std::endl;
    } else {
        std::cout<<"invaild upper_bound"<<std::endl;
    }
    
    //find, erase
    std::multimap<std::string, int>::iterator it = usermultimap.find("gamma");
    std::cout<<"find:"<<it->first<<"=>"<<it->second<<std::endl;
    usermultimap.erase(it);
    show_container(usermultimap, "erase");

    //get_alloctator
    std::pair<const std::string, int>* p;
    p = usermultimap.get_allocator().allocate(5);
    std::cout<<"size:"<<sizeof(p)<<","<<sizeof(std::multimap<std::string, int>::value_type)<<std::endl;
    usermultimap.get_allocator().deallocate(p, 5);

    //insert
    usermultimap.insert(usermultimap.begin(), std::pair<std::string, int>("gamma", 5));
    usermultimap.insert(usermultimap.find("delta"), std::pair<std::string, int>("zeta", 9));
    show_container(usermultimap, "insert");

    //key_comp => key compare, use key value
    std::cout<<"key_comp: ";
    for(auto &x:usermultimap)
    {
        if(usermultimap.key_comp()(x.first, "c"))
        {
            std::cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    std::cout<<std::endl;

    //value_comp => key compare, use object
    std::cout<<"value_comp: ";
    for(auto &x:usermultimap)
    {
        if(usermultimap.value_comp()(x, std::pair<std::string, int>("c", 1)))
        {
            std::cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    std::cout<<std::endl;

    std::multimap<std::string, int> swapmultimap;

    //swap
    swapmultimap.swap(usermultimap);
    show_container(swapmultimap, "swap");

    algorithm_process();
    return 0;
} 
