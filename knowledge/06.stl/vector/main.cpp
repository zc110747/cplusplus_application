//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::vector声明
//      2. std::vector方法
//      3. std::vector常用算法配合
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <numeric>
#include <random>

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
    std::vector<int> vcon_0{2, 3, 3, 5, 7, 7, 1};
    std::vector<int> vcon_1{4, 6};
    std::cout<<"all_of:"<<std::all_of(vcon_0.begin(), vcon_0.end(), [](const int &ref_value){
        return ref_value >= 1;
    })<<std::endl;
    std::cout<<"any_of:"<<std::any_of(vcon_0.begin(), vcon_0.end(), [](const int &ref_value){
        return ref_value > 5;
    })<<std::endl;

    auto val = std::find(vcon_0.begin(), vcon_0.end(), 3);
    if(val != vcon_0.end()) {
        std::cout<<"find:"<<*val<<std::endl;
    }
    val = std::find_if(vcon_0.begin(), vcon_0.end(), [](const int &ref_value){
        return ref_value > 5;
    });
    if(val != vcon_0.end()) {
        std::cout<<"find_if:"<<*val<<std::endl;
    }

    val = std::search(vcon_0.begin(), vcon_0.end(), std::vector<int>{3, 5}.begin(), std::vector<int>{3, 5}.end());
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
    std::shuffle(vcon_0.begin(), vcon_0.end(), std::default_random_engine());
    show_container(vcon_0, "shuffle");

    // 变换算法
    std::cout<<"======= transform ======="<<std::endl;
    std::transform(vcon_0.begin(), vcon_0.end(), vcon_0.begin(), [](const int &ref_value){
        return ref_value*ref_value;
    });
    show_container(vcon_0, "transform");

    std::replace(vcon_0.begin(), vcon_0.end(), 9, 100);
    show_container(vcon_0, "replace");

    std::replace_if(vcon_0.begin(), vcon_0.end(), [](const int &ref_value){
        return ref_value == 100;
    }, 10);
    show_container(vcon_0, "replace_if");

    // 计数算法
    std::cout<<"======= count ======="<<std::endl;
    std::cout<<"count:"<<std::count(vcon_0.begin(), vcon_0.end(), 9)<<std::endl;
    std::cout<<"count of:"<<std::count_if(vcon_0.begin(), vcon_0.end(), [](const int &ref_value){
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
    auto val_2 = std::mismatch(vcon_0.begin(), vcon_0.end(), vcon_1.begin(), [](const int &ref_value_0, const int &ref_value_1){
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

    std::vector<int> vcon_2;
    vcon_2.resize(vcon_0.size());
    std::copy_if(vcon_0.begin(), vcon_0.end(), vcon_2.begin(), [](const int &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy_if");
    std::copy_if(vcon_0.begin(), vcon_0.end(), std::back_inserter(vcon_2), [](const int &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy_if");

    // 移除算法
    std::cout<<"======= remove ======="<<std::endl;
    std::vector<int> vcon_r = {1, 3, 3, 6, 5, 6, 7};
    auto end = std::remove(vcon_r.begin(), vcon_r.end(), 3);
    vcon_r.erase(end, vcon_r.end());
    show_container(vcon_r, "remove");
    std::sort(vcon_r.begin(), vcon_r.end());
    auto unique_end = std::unique(vcon_r.begin(), vcon_r.end());
    vcon_r.erase(unique_end, vcon_r.end());
    show_container(vcon_r, "unique");

    // 分割算法
    std::cout<<"======= partition ======="<<std::endl;
    std::vector<int> vcon_3 = {1, 2, 3, 4, 5, 6, 7};
    std::stable_partition(vcon_3.begin(), vcon_3.end(), [](const int &ref_value){
        return ref_value % 2 == 0;
    });
    show_container(vcon_3, "stable_partition");
    std::partition(vcon_3.begin(), vcon_3.end(), [](const int &ref_value){
        return ref_value % 2 == 0;
    });
    show_container(vcon_3, "partition");

    // 归并算法
    std::cout<<"======= merge ======="<<std::endl;
    std::vector<int> vcon_4 = {1, 3, 5};
    std::vector<int> vcon_5 = {2, 4, 6, 8};
    std::vector<int> vcon_6;
    vcon_6.resize(vcon_4.size() + vcon_5.size());
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

struct MyStruct
{
    int a;
    int b;
    MyStruct(int a, int b):a(a), b(b){}
};

std::ostream& operator<<(std::ostream& os, const MyStruct& obj)
{
    os << obj.a << " " << obj.b;
    return os;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::vector<int> intv(array, array+5);
    std::vector<int> uservector(intv);
    std::vector<std::string> listvector;

    // 创建一个包含5个元素的vector，每个元素初始值为0
    std::vector<int> vec_init(5);
    show_container(vec_init, "vec_init");
    
    //operator =
    listvector = std::vector<std::string>({"0x00", "0x01", "0x02"}); //list initialize
    show_container(listvector, "listvector");

    //clear, capacity, size, max_size
    listvector.clear();
    std::cout<<"capacity:"<<listvector.capacity()<<" | ";
    std::cout<<"size:"<<listvector.size()<<std::endl;
    std::cout<<"max_size:"<<listvector.max_size()<<std::endl;
    show_container(uservector, "uservector");

    //assign
    uservector.assign(uservector.begin(), uservector.end()-1);
    show_container(uservector, "assign");

    //at
    int &p = uservector.at(2);
    p = 10;
    show_container(uservector, "at");

    //back
    int &end = uservector.back();
    end = 5;
    show_container(uservector, "back");

    //capacity, reserve, resize, max_size
    std::cout<<"capactity:"<<uservector.capacity()<<std::endl;
    uservector.reserve(7);
    std::cout<<"reserve capactity:"<<uservector.capacity()<<std::endl;
    uservector.resize(3);
    uservector.shrink_to_fit();
    std::cout<<"fit capactity:"<<uservector.capacity()<<std::endl;
    std::cout<<"max_size:"<<uservector.max_size()<<std::endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::vector<int>::iterator iterbegin = uservector.begin();
    std::vector<int>::iterator iterend = uservector.end();
    std::vector<int>::const_iterator iterconstbegin = uservector.cbegin();
    std::vector<int>::const_iterator iterconstend = uservector.cend();
    std::vector<int>::reverse_iterator iterrbegin = uservector.rbegin();
    std::vector<int>::reverse_iterator iterrend = uservector.rend();
    std::vector<int>::const_reverse_iterator iterconstrbegin = uservector.crbegin();
    std::vector<int>::const_reverse_iterator iterconstrend = uservector.crend();
    std::cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(iterend-1)<<" ";
    std::cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(iterconstend-1)<<" ";
    std::cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(iterrend-1)<<" ";
    std::cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(iterconstrend-1)<<" "<<std::endl;

    //data
    int *ptr = uservector.data();
    ptr[0] = 4;
    ptr[2] = 3;
    show_container(uservector, "data");

    //emplace, emplace_back
    auto it = uservector.emplace(uservector.begin()+1, 8);
    show_container(uservector, "emplace");
    std::cout<<"empalce iterator:"<<*it<<std::endl;
    uservector.emplace_back(9);
    show_container(uservector, "emplace_back");

    std::vector<MyStruct> myvector;
    myvector.emplace_back(MyStruct(1, 2));
    myvector.emplace_back(3, 4);
    show_container(myvector, "emplace_back");

    //erase
    auto iter=uservector.begin();
    while(iter!=uservector.end()) {
        if(*iter == 3) {
            iter=uservector.erase(iter);
        } else {
            iter++;
        }
    }
    show_container(uservector, "erase");

    //front
    int &front = uservector.front();
    front = 6;
    show_container(uservector, "front");

    //insert
    uservector.insert(uservector.begin(), 2, 5);
    show_container(uservector, "insert");

    //swap, operator=
    std::vector<int> swapvector = std::vector<int>();
    swapvector.swap(uservector);
    show_container(swapvector, "swapvector");

    //pop_back, push_back
    swapvector.push_back(10);
    show_container(swapvector, "push_back");
    swapvector.pop_back();
    show_container(swapvector, "pop_back");

    algorithm_process();
    return 0;
} 