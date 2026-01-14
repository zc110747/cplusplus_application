//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::multiset声明
//      2. std::multiset方法
//      3. std::multiset常用算法配合
//
// Author:
//      @公众号 <嵌入式技术总结>
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
        for(auto index=qstring.size(); index<13; index++) {
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
    std::multiset<int> vcon_0{2, 3, 3, 5, 7, 7, 1};
    std::multiset<int> vcon_1{4, 6};

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

    auto search_seq = std::multiset<int>{3, 5};
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
    std::multiset<int> vcon_new;
    std::cout<<"======= transform ======="<<std::endl;
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
    std::multiset<int> vcon_2;
    std::copy_if(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_2, vcon_2.end()), [](const auto &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy_if");

    // 移除算法

    // 分割算法(multiset默认排序)

    // 归并算法
    std::cout << "======= merge =======" <<std::endl;
    std::multiset<int> vcon_4 = {1, 3, 5};
    std::multiset<int> vcon_5 = {2, 3, 4, 6, 8};
    std::multiset<int> vcon_6;
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

// 使用自定义分配器的multiset
using CustomMultiset = std::multiset<int, std::less<int>, CustomAllocator<int>>;

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::multiset<int> intv(array, array+5);
    std::multiset<int> usermultiset(intv);
    std::multiset<std::string> listmultiset;
    
    //operator=
    listmultiset = std::multiset<std::string>({"0x00", "0x01", "0x02"});
    
    show_container(listmultiset, "listmultiset");
    //count, clear, size,empty
    std::cout << "count" << listmultiset.count("0x01");
    listmultiset.clear();
    std::cout << "size:" << listmultiset.size() <<std::endl;
    std::cout << "empty:" << listmultiset.empty() <<std::endl;

    show_container(usermultiset, "usermultiset");

    //max_size
    std::cout << "max_size:" <<usermultiset.max_size() <<std::endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::multiset<int>::iterator iterbegin = usermultiset.begin();
    std::multiset<int>::iterator iterend = usermultiset.end();
    std::multiset<int>::const_iterator iterconstbegin = usermultiset.cbegin();
    std::multiset<int>::const_iterator iterconstend = usermultiset.cend();
    std::multiset<int>::reverse_iterator iterrbegin = usermultiset.rbegin();
    std::multiset<int>::reverse_iterator iterrend = usermultiset.rend();
    std::multiset<int>::const_reverse_iterator iterconstrbegin = usermultiset.crbegin();
    std::multiset<int>::const_reverse_iterator iterconstrend = usermultiset.crend();
    std::cout << "begin:" << *iterbegin << " "  << "end:" << *(--iterend) << " ";
    std::cout << "cbegin:" << *iterconstbegin << " "  << "cend:" << *(--iterconstend) << " ";
    std::cout << "rbegin:" << *iterrbegin << " "  << "rend:" << *(--iterrend) << " ";
    std::cout << "crbegin:" << *iterconstrbegin << " "  << "crend:" << *(--iterconstrend) << " " <<std::endl;

    //emplace, find, emplace_hint
    auto it = usermultiset.emplace(15);
    show_container(usermultiset, "emplace");
    std::cout << "val:" << *it <<std::endl;
    auto find = usermultiset.find(8);
    usermultiset.emplace_hint(find, 9);
    show_container(usermultiset, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    //equal_range, lower_bound, upper_bound
    std::pair<std::multiset<int>::const_iterator, std::multiset<int>::const_iterator> ret;
    ret = usermultiset.equal_range(5);
    if(ret.first != usermultiset.end()) {
        std::cout << "equal_range lowerbound:" << *(ret.first) <<std::endl;
    } else {
        std::cout << "invalid equal_range lowerbound" <<std::endl;
    }
    if(ret.second != usermultiset.end()) {
        std::cout << "equal_range upperbound:" << *(ret.second) <<std::endl;
    } else {
        std::cout << "invalid equal_range upperbound" <<std::endl;
    }

    std::multiset<int>::const_iterator iterlower = usermultiset.lower_bound(5);
    std::multiset<int>::const_iterator iterupper = usermultiset.upper_bound(5);
    if(iterlower != usermultiset.end()) {
        std::cout << "lowerbound:" << *iterlower <<std::endl;
    } else {
        std::cout << "invalid lowerbound" <<std::endl;
    }
    if(iterupper != usermultiset.end()) {
        std::cout << "upperbound:" << *iterupper <<std::endl;
    } else {
        std::cout << "invalid upperbound" <<std::endl;
    }

    //erase
    auto iter=usermultiset.begin();
    while (iter!=usermultiset.end()) {
        if(*iter == 3) {
            iter=usermultiset.erase(iter);
        } else {
            iter++;
        }
    }
    show_container(usermultiset, "erase");

    //insert
    usermultiset.insert(15);
    show_container(usermultiset, "insert");

    //key_comp
    std::cout << "key_comp:";
    for(auto &x:usermultiset)
    {
        if(usermultiset.key_comp()(x, 7))
        {
            std::cout << x << " ";
        }
    }
    std::cout << std::endl;

    //value_comp
    std::cout << "value comp:";
    for(auto &x:usermultiset)
    {
        if(usermultiset.value_comp()(x, 9))
        {
            std::cout << x << " ";
        }
    }
    std::cout << std::endl;

    //swap
    std::multiset<int> swapmultiset;
    swapmultiset.swap(usermultiset);
    show_container(swapmultiset, "swapmultiset");
    
    {
        CustomMultiset custom_multiset;
        custom_multiset.insert(1);
        custom_multiset.insert(2);
        custom_multiset.insert(3);
        show_container(custom_multiset, "custom_multiset");
    }

    algorithm_process();
    return 0;
}
