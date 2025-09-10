//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::unordered_multiset声明
//      2. std::unordered_multiset方法
//      3. std::unordered_multiset常用算法配合
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <numeric>

template<typename T>
void show_container(T container_val, std::string qstring)
{
    //empty, size
    if(!qstring.empty())
    {
        for(auto index=qstring.size(); index<13; index++)
            qstring.push_back(' ');
        qstring += ":";
        std::cout << qstring;
    }

    for(const auto &ref : container_val) {
        std::cout << ref << " ";
    }

    std::cout << std::endl;
}

void algorithm_process(void)
{
    std::cout << "==================== algorithm_process ====================" <<std::endl;

    // 查找算法
    std::cout << "======= search =======" <<std::endl;
    std::unordered_multiset<int> vcon_0{2, 3, 3, 5, 7, 7, 1};
    std::unordered_multiset<int> vcon_1{4, 6};

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

    auto search_seq = std::unordered_multiset<int>{3, 5};
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
    std::unordered_multiset<int> vcon_new;
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
    std::unordered_multiset<int> vcon_2;
    std::copy_if(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_2, vcon_2.end()), [](const auto &ref_value){
        return ref_value < 50;
    });
    show_container(vcon_2, "copy_if");

    // 移除算法

    // 分割算法(multiset默认排序)

    // 归并算法
    std::cout << "======= merge =======" <<std::endl;
    std::unordered_multiset<int> vcon_4 = {1, 3, 5};
    std::unordered_multiset<int> vcon_5 = {2, 3, 4, 6, 8};
    std::unordered_multiset<int> vcon_6;
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
    int array[] = {8, 2, 1, 3, 5, 4, 10};
    std::unordered_multiset<int> umcon_0(array, array + sizeof(array) / sizeof(int));
    std::unordered_multiset<int> umcon_1(umcon_0);
    std::unordered_multiset<std::string> umcon_2;
    
    //operator=
    umcon_2 = std::unordered_multiset<std::string>({"0x00", "0x01", "0x02"});
    
    show_container(umcon_2, "umcon_2");
    //count, clear, size, empty
    //load_factor,max_bucket_count,max_load_factor
    std::cout << "count" << umcon_2.count("0x01");
    umcon_2.clear();
    std::cout << "size:" << umcon_2.size() << std::endl;
    std::cout << "empty:" << umcon_2.empty() << std::endl;
    std::cout << "load_factor" << umcon_2.load_factor() << std::endl;
    std::cout << "max_bucket_count" << umcon_2.max_bucket_count() << std::endl;
    std::cout << "max_load_factor" << umcon_2.max_load_factor() << std::endl;

    show_container(umcon_1, "umcon_1");

    //max_size
    std::cout << "max_size:" << umcon_1.max_size() << std::endl;

    //begin,  end,  cbegin,  cend
    std::unordered_multiset<int>::iterator iterbegin = umcon_1.begin();
    std::unordered_multiset<int>::iterator iterend = umcon_1.end();
    std::unordered_multiset<int>::const_iterator iterconstbegin = umcon_1.cbegin();
    std::unordered_multiset<int>::const_iterator iterconstend = umcon_1.cend();
    std::cout << "begin:" << *iterbegin<<" ";
    std::cout << "cbegin:" << *iterconstbegin << std::endl;

    //bucket, bucket_count, bucket_size
    std::cout << "bucket:" << umcon_1.bucket(5) << std::endl;
    auto bucket_count = umcon_1.bucket_count();
    std::cout << "bucket count:" << bucket_count << std::endl;
    for(auto i=0; i<bucket_count; i++) {
        std::cout << "bucket #" << i<<" has " << umcon_1.bucket_size(i)<<" elements." << std::endl;
    }

    //emplace, find, emplace_hint
    auto it = umcon_1.emplace(15);
    show_container(umcon_1, "emplace");
    std::cout << "emplace:" << *it << std::endl;
    auto find = umcon_1.find(8);
    umcon_1.emplace_hint(find, 9);
    show_container(umcon_1, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::unordered_multiset<int>::const_iterator, std::unordered_multiset<int>::const_iterator> ret;
    ret = umcon_1.equal_range(5);
    std::cout << "equal_range lowerbound:" << *(ret.first) << std::endl;
    std::cout << "equal_range upperbound:" << *(ret.second) << std::endl;

    //erase
    auto iter=umcon_1.begin();
    while (iter!=umcon_1.end()) {
        if (*iter == 3) {
            iter = umcon_1.erase(iter);
        } else {
            iter++;
        }
    }
    show_container(umcon_1, "erase");

    //hash_function
    typedef std::unordered_multiset<int> umcon_3;
    umcon_3::hasher hash_fn = umcon_1.hash_function();
    std::cout << "file hash value: " << hash_fn(5) << std::endl;

    //insert
    umcon_1.insert(15);
    show_container(umcon_1, "insert");

    //key_eq
    bool equal = umcon_1.key_eq()(4, 6);
    std::cout << "key_eq: " << equal << std::endl;

    //swap
    std::unordered_multiset<int> umcon_4;
    umcon_4.swap(umcon_1);
    show_container(umcon_4, "umcon_4");

    //rehash, reserve
    umcon_4.rehash(15);
    std::cout << "rehash: " << umcon_4.bucket_count() << std::endl;
    umcon_4.reserve(5);
    std::cout << "reserve: " << umcon_4.bucket_count() << std::endl;

    algorithm_process();
    return 0;
} 
