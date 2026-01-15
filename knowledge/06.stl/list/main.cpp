//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::list声明
//      2. std::list方法
//      3. std::list常用算法配合
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <list>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <numeric>
#include <vector>

template<typename T>
bool comparsion(T first, T second) {
    return first<second;
}

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
    std::list<int> vcon_0{2, 3, 3, 5, 7, 7, 1};
    std::list<int> vcon_1{4, 6};
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

    val = std::search(vcon_0.begin(), vcon_0.end(), std::list<int>{3, 5}.begin(), std::list<int>{3, 5}.end());
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

    std::reverse(vcon_0.begin(), vcon_0.end());
    show_container(vcon_0, "reverse");

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
    
    // // 生成算法
    std::cout<<"======= generate ======="<<std::endl;
    std::fill(vcon_0.begin(), vcon_0.end(), 5);
    show_container(vcon_0, "fill");

    std::generate(vcon_0.begin(), vcon_0.end(), [](){
        return rand()%100;
    });
    show_container(vcon_0, "generate");

    std::list<int> vcon_2;
    vcon_2.resize(vcon_0.size());
    std::copy_if(vcon_0.begin(), vcon_0.end(), vcon_2.begin(), [](const auto &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy");

    // 移除算法
    std::cout<<"======= remove ======="<<std::endl;
    std::list<int> vcon_r = {1, 3, 3, 6, 5, 6, 7};
    auto end = std::remove(vcon_r.begin(), vcon_r.end(), 3);
    vcon_r.erase(end, vcon_r.end());
    show_container(vcon_r, "remove");

    // 分割算法
    std::cout<<"======= partition ======="<<std::endl;
    std::list<int> vcon_3 = {1, 2, 3, 4, 5, 6, 7};
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
    std::list<int> vcon_4 = {1, 3, 5};
    std::list<int> vcon_5 = {2, 4, 6, 8};
    std::list<int> vcon_6;
    vcon_6.resize(vcon_4.size() + vcon_5.size());
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

// 自定义内存分配器
template <typename T>
class CustomAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    CustomAllocator() = default;
    
    template <typename U>
    CustomAllocator(const CustomAllocator<U>&) noexcept {}
    
    // 内存分配
    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    
    // 内存释放
    void deallocate(T* p, std::size_t n) noexcept {
        ::operator delete(p);
    }
    
    // 构造对象
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ::new((void*)p) U(std::forward<Args>(args)...);
    }
    
    // 析构对象
    template <typename U>
    void destroy(U* p) {
        p->~U();
    }
};

// 使用自定义分配器的deque
using CustomList = std::list<int, CustomAllocator<int>>;

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::list<int> intv(array, array+5);
    std::list<int> usr_list;
    std::list<std::string> str_list{"0x00", "0x01", "0x02"}; //list initialize
    
    show_container(str_list, "str_list");
    //clear, size, max_size
    str_list.clear();
    std::cout<<"size:"<<str_list.size()<<std::endl;
    std::cout<<"max_size:"<<str_list.max_size()<<std::endl;

    show_container(usr_list, "usr_list");

    //assign
    usr_list.assign(intv.begin(), intv.end());
    show_container(usr_list, "assign");

    //back
    int &end = usr_list.back();
    end = 5;
    show_container(usr_list, "back");

    //resize, empty, max_size
    usr_list.resize(3);
    show_container(usr_list, "resize");
    std::cout << "empty:"<<usr_list.empty() << std::endl;
    std::cout << "max_size:"<<usr_list.max_size() << std::endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::list<int>::iterator iterbegin = usr_list.begin();
    std::list<int>::iterator iterend = usr_list.end();
    std::list<int>::const_iterator iterconstbegin = usr_list.cbegin();
    std::list<int>::const_iterator iterconstend = usr_list.cend();
    std::list<int>::reverse_iterator iterrbegin = usr_list.rbegin();
    std::list<int>::reverse_iterator iterrend = usr_list.rend();
    std::list<int>::const_reverse_iterator iterconstrbegin = usr_list.crbegin();
    std::list<int>::const_reverse_iterator iterconstrend = usr_list.crend();
    std::cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(--iterend)<<" ";
    std::cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(--iterconstend)<<" ";
    std::cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(--iterrend)<<" ";
    std::cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(--iterconstrend)<<" "<<std::endl;

    //emplace, emplace_back, emplace_front
    auto it = usr_list.emplace(usr_list.begin(), 8);
    show_container(usr_list, "emplace");
    std::cout << "empalce iterator:" << *it << std::endl;
    usr_list.emplace_back(9);
    show_container(usr_list, "emplace_back");
    usr_list.emplace_front(1);
    show_container(usr_list, "emplace_front");

    //erase
    auto iter=usr_list.begin();
    while(iter!=usr_list.end())
    {
        if(*iter == 3) {
            iter=usr_list.erase(iter);
        } else {
            iter++;
        }
    }
    show_container(usr_list, "erase");

    //front
    int &front = usr_list.front();
    front = 6;
    show_container(usr_list, "front");

    //insert
    usr_list.insert(usr_list.begin(), 2, 5);
    show_container(usr_list, "insert");

    //swap, operator=
    std::list<int> swaplist = std::list<int>();
    swaplist.swap(usr_list);
    show_container(swaplist, "swaplist");

    //pop_back, pop_front, push_back, push_front
    swaplist.push_back(10);
    show_container(swaplist, "push_back");
    swaplist.push_front(5);
    show_container(swaplist, "push_front");
    swaplist.pop_back();
    show_container(swaplist, "pop_back");
    swaplist.pop_front();
    show_container(swaplist, "pop_front");

    //sort
    swaplist.sort();
    show_container(swaplist, "sort");

    //unique
    swaplist.unique();
    show_container(swaplist, "unique");

    //megre
    std::list<int> megrelist = {4, 5, 3, 1, 2};
    megrelist.sort();
    swaplist.merge(megrelist, comparsion<int>);
    show_container(swaplist, "megre");
    std::cout << "empty: " << megrelist.empty() << std::endl;

    //remove, remove_if
    swaplist.remove(2);
    show_container(swaplist, "remove");
    swaplist.remove_if([](const int& value){
        return value>7;
    });
    show_container(swaplist, "remove_if");

    //reverse   
    swaplist.reverse();
    show_container(swaplist, "reverse");

    //splice
    megrelist.assign(intv.begin(), intv.end());
    swaplist.splice(++swaplist.begin(), megrelist);
    show_container(swaplist, "splice");

    algorithm_process();

    {
        CustomList custom_list;
        custom_list.push_back(1);
        custom_list.push_back(2);
        custom_list.push_back(3);
        show_container(custom_list, "custom_list");
    }
    return 0;
} 