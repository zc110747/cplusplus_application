//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::unordered_map声明
//      2. std::unordered_map方法
//      3. std::unordered_map常用算法配合
//
// Author:
//      @公众号 <嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>

// 辅助模板,检测是否为std::unordered_map
template<typename T, typename = std::enable_if_t<std::is_same<T, std::unordered_map<typename T::key_type, typename T::mapped_type>>::value>>
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

    for(const auto &pair : container_val) {
        std::cout << "[" << pair.first<<": " << pair.second<<"] ";
    }

    std::cout<<std::endl;
}

void algorithm_process(void)
{
    std::cout<<"==================== algorithm_process ===================="<<std::endl;

    // 查找算法
    std::cout<<"======= search ======="<<std::endl;
    std::unordered_map<int, int> vcon_0 = {
        {1, 1},
        {3, 3},
        {4, 4},
        {5, 5},
        {2, 2},
    };
    std::unordered_map<int, int> vcon_1 = {
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

    // 排序算法(std::map内部按照红黑树实现,默认按照键值排序的)
    std::cout<<"======= sort ======="<<std::endl;
    std::cout<<"is sort:"<<std::is_sorted(vcon_0.begin(), vcon_0.end())<<std::endl;

    val = std::is_sorted_until(vcon_0.begin(), vcon_0.end());
    if(val != vcon_0.end()) {
        std::cout<<"is_sorted_until:"<<val->first<<std::endl;
    }
    std::vector<std::pair<int, int>> vec_0(vcon_0.begin(), vcon_0.end());

    // 根据键值排序(也可以根据值排序)
    std::sort(vec_0.begin(), vec_0.end(), [](const auto &ref_value_0, const auto &ref_value_1){
        return ref_value_0.first < ref_value_1.first;
    });
    for (const auto &pair : vec_0) {
        std::cout<<"["<<pair.first<<": "<<pair.second<<"] ";
    }
    std::cout<<std::endl;

    // 变换算法
    std::unordered_map<int, int> vcon_new;
    std::cout<<"======= transform ======="<<std::endl;
    std::transform(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_new, vcon_new.end()), [](std::pair<const int, int> &pair){
        return std::make_pair(pair.first, pair.second+2);
    }); //map的键值是const类型,不能直接通过transform修改,创建新的容器
    show_container(vcon_new, "transform");

    // // 计数算法
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
    std::unordered_map<int, int> vcon_2;
    std::copy_if(vcon_0.begin(), vcon_0.end(), std::inserter(vcon_2, vcon_2.begin()), [](const auto &ref_value){
        return ref_value.first < 10;
    });
    show_container(vcon_2, "copy");

    // 移除算法


    // 分割算法

    // 归并算法
    std::cout<<"======= merge ======="<<std::endl;
    std::unordered_map<int, int> vcon_4 = {
        {1, 1},
        {3, 3}, 
        {5, 5},
    };
    std::unordered_map<int, int> vcon_5 = {
        {2, 2},
        {4, 4},
        {6, 6},
        {8, 8},
    };
    std::unordered_map<int, int> vcon_6;
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
    std::unordered_map<std::string, int> mcon_0 = {
        {"alpha", 0},
        {"beta", 0}, 
        {"gamma", 1},   
    };
    auto mcon_1(mcon_0);

    //size, clear, max_size, empty
    //load_factor, max_bucket_cout, max_load_factor
    show_container(mcon_1, "mcon_1");
    std::cout << "size:" << mcon_1.size() << std::endl;
    mcon_1.clear();
    std::cout << "clear size:" << mcon_1.size() << std::endl;
    std::cout << "max_size:" << mcon_1.max_size() << std::endl;
    std::cout << "empty:" << mcon_1.empty() << std::endl;
    std::cout << "load_factor" << mcon_1.load_factor() << std::endl;
    std::cout << "max_bucket_count" << mcon_1.max_bucket_count() << std::endl;
    std::cout << "max_load_factor" << mcon_1.max_load_factor() << std::endl;
    std::cout << "operator[]:" << mcon_0["alpha"] << std::endl;
    
    show_container(mcon_0, "mcon_0");

    //at, operator[]
    mcon_0.at("beta") = 5;
    show_container(mcon_0, "mcon_0");
    mcon_0["beta"] = 4;
    show_container(mcon_0, "operator[]");

    //bucket, bucket_count, bucket_size
    std::cout << "bucket:" << mcon_0.bucket("beta") << std::endl;
    auto bucket_count = mcon_0.bucket_count();
    std::cout << "bucket count:" << bucket_count << std::endl;
    for(auto i=0; i<bucket_count; i++)
    {
        std::cout << "bucket #" << i<<" has " << mcon_0.bucket_size(i)<<" elements." << std::endl;
    }

    //begin, end, cbegin, cend
    std::unordered_map<std::string, int>::iterator iterbegin = mcon_0.begin();
    std::unordered_map<std::string, int>::iterator iterend = mcon_0.end();
    std::unordered_map<std::string, int>::const_iterator iterconstbegin = mcon_0.cbegin();
    std::unordered_map<std::string, int>::const_iterator iterconstend = mcon_0.cend();
    std::cout << "begin: " << iterbegin->first<<"=>" << iterbegin->second<<",";
    std::cout << "cbegin: " << iterconstbegin->first<<"=>" << iterconstbegin->second<<",";

    //count
    std::cout << "count:" << mcon_0.count("beta") << std::endl;

    //emplace, emplace_hint
    mcon_0.emplace("delta", 6);
    show_container(mcon_0, "emplace");
    mcon_0.emplace_hint(mcon_0.begin(), "epsilon", 10);
    show_container(mcon_0, "emplace");

    //equal_range
    std::pair<std::unordered_map<std::string, int>::iterator, std::unordered_map<std::string, int>::iterator> ret;
    ret = mcon_0.equal_range("beta");
    if (ret.first != mcon_0.end()) {
        std::cout << "equal_range lower_bound:" << ret.first->first<<"=>" << ret.first->second << std::endl;
    } else {
        std::cout << "no vaild lower bound" << std::endl;
    }
    if (ret.second != mcon_0.end()) {
        std::cout << "equal_range upper_bound:" << ret.second->first<<"=>" << ret.second->second << std::endl;
    } else {
        std::cout << "no vaild upper bound" << std::endl;
    }

    //find, erase
    std::unordered_map<std::string, int>::iterator it = mcon_0.find("gamma");
    std::cout << "find:" << it->first<<"=>" << it->second << std::endl;
    mcon_0.erase(it);
    show_container(mcon_0, "erase");

    //get_alloctator
    std::pair<const std::string, int>* p;
    p = mcon_0.get_allocator().allocate(5);
    std::cout << "size:" << sizeof(p)<<"," << sizeof(std::unordered_map<std::string, int>::value_type) << std::endl;
    mcon_0.get_allocator().deallocate(p, 5);

    //insert
    mcon_0.insert(mcon_0.begin(), std::pair<std::string, int>("gamma", 5));
    mcon_0.insert(mcon_0.find("delta"), std::pair<std::string, int>("zeta", 9));
    show_container(mcon_0, "insert");
    mcon_0.insert({{"delta", 6}, {"epsilon", 9}});
    show_container(mcon_0, "insert");

    //hash_function
    typedef std::unordered_map<std::string, int> mcon_2;
    mcon_2::hasher hash_fn = mcon_0.hash_function();
    std::cout << "file hash value:" << hash_fn("file") << std::endl;

    //key_eq
    bool equal = mcon_0.key_eq()("this", "This");
    std::cout << "key_eq:" << equal << std::endl;

    std::unordered_map<std::string, int> mcon_3;

    //swap
    mcon_3.swap(mcon_0);
    show_container(mcon_3, "swap");

    //rehash, reserve
    mcon_3.rehash(15);
    std::cout << "rehash:" << mcon_3.bucket_count() << std::endl;
    mcon_3.reserve(5);
    std::cout << "reserve:" << mcon_3.bucket_count() << std::endl;

    algorithm_process();
    return 0;
} 
