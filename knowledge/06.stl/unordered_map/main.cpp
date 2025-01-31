/*
std::unordered_map 是 C++ 标准库中的一个关联容器，它提供了一种键值对的存储方式，其中键是唯一的，而值可以重复

主要特点：
1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于键的顺序。
2. 快速查找：由于使用哈希表实现，查找操作非常高效。
3. 键唯一性：每个键在容器中只能出现一次。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存

成员函数：
at: 返回与给定键相关联的值。
begin: 返回指向容器中第一个元素的迭代器。
bucket: 返回给定键所在的桶的索引。
bucket_count: 返回桶的数量。
bucket_size: 返回给定桶中的元素数量。
cbegin: 返回指向容器中第一个元素的常量迭代器。
cend: 返回指向容器中最后一个元素的下一个位置的常量迭代器。
clear: 移除容器中的所有元素。
count: 返回与给定键相关联的值的数量。
contains(c++20): 检查容器中是否存在给定键。
emplace: 构造并插入元素。
emplace_hint: 构造并插入元素，使用给定的提示位置。
empty: 检查容器是否为空。
end: 返回指向容器中最后一个元素的下一个位置的迭代器。
equal_range: 返回与给定键相关联的元素范围。
erase: 移除容器中指定位置的元素或指定键的元素。
find: 查找与给定键相关联的元素。
hash_function: 返回用于计算哈希值的函数对象。
insert: 插入元素。
key_eq: 返回用于比较键的函数对象。
load_factor: 返回当前负载因子。
max_bucket_count: 返回桶的最大数量。
max_load_factor: 返回最大负载因子。
max_size: 返回容器可以容纳的最大元素数量。
rehash: 重新哈希容器。
size: 返回容器中元素的数量。
swap: 交换两个容器的内容。
unordered_map: 构造并返回一个新的 unordered_map 对象。
*/
#include <unordered_map>
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
}SHOW_MODE_ENUM;

template<typename T, typename N>
void show_unordered_map(std::unordered_map<T, N> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        std::for_each(value.begin(), value.end(), [](const typename std::pair<T, N> &ref_value){
            cout<<ref_value.first<<"=>"<<ref_value.second<<" ";
        });
    }
    //auto
    else if(mode == SHOW_MODE_ENUM_TYPE_AUTO)
    {
        for(auto &ref:value)
            cout<<ref.first<<"=>"<<ref.second<<" ";
    }
    //iterator
    else if(mode == SHOW_MODE_ENUM_TYPE_ITERATOR)
    {
        for(typename std::unordered_map<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<iter->first<<"=>"<<iter->second<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    std::unordered_map<string, int> usr_umap = {
        {"alpha", 0},
        {"beta", 0}, 
        {"gamma", 1},   
    };
    auto cpy_umap(usr_umap);

    //size, clear, max_size, empty
    //load_factor, max_bucket_cout, max_load_factor
    show_unordered_map(cpy_umap, "cpy_umap");
    cout<<"size:"<<cpy_umap.size()<<endl;
    cpy_umap.clear();
    cout<<"clear size:"<<cpy_umap.size()<<endl;
    cout<<"max_size:"<<cpy_umap.max_size()<<endl;
    cout<<"empty:"<<cpy_umap.empty()<<endl;
    cout<<"load_factor"<<cpy_umap.load_factor()<<endl;
    cout<<"max_bucket_count"<<cpy_umap.max_bucket_count()<<endl;
    cout<<"max_load_factor"<<cpy_umap.max_load_factor()<<endl;
    cout<<"operator[]:"<<usr_umap["alpha"]<<endl;
    
    show_unordered_map(usr_umap, "usr_umap");

    //at, operator[]
    usr_umap.at("beta") = 5;
    show_unordered_map(usr_umap, "usr_umap", SHOW_MODE_ENUM_LAMBDA);
    usr_umap["beta"] = 4;
    show_unordered_map(usr_umap, "operator[]");

    //bucket, bucket_count, bucket_size
    cout<<"bucket:"<<usr_umap.bucket("beta")<<endl;
    auto bucket_count = usr_umap.bucket_count();
    cout<<"bucket count:"<<bucket_count<<endl;
    for(auto i=0; i<bucket_count; i++)
    {
        cout<<"bucket #"<<i<<" has "<<usr_umap.bucket_size(i)<<" elements."<<endl;
    }

    //begin, end, cbegin, cend
    std::unordered_map<string, int>::iterator iterbegin = usr_umap.begin();
    std::unordered_map<string, int>::iterator iterend = usr_umap.end();
    std::unordered_map<string, int>::const_iterator iterconstbegin = usr_umap.cbegin();
    std::unordered_map<string, int>::const_iterator iterconstend = usr_umap.cend();
    cout<<"begin: "<<iterbegin->first<<"=>"<<iterbegin->second<<",";
    cout<<"cbegin: "<<iterconstbegin->first<<"=>"<<iterconstbegin->second<<",";

    //count
    cout<<"count:"<<usr_umap.count("beta")<<endl;

    //emplace, emplace_hint
    usr_umap.emplace("delta", 6);
    show_unordered_map(usr_umap, "emplace", SHOW_MODE_ENUM_TYPE_AUTO);
    usr_umap.emplace_hint(usr_umap.begin(), "epsilon", 10);
    show_unordered_map(usr_umap, "emplace", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //equal_range
    std::pair<std::unordered_map<string, int>::iterator, std::unordered_map<string, int>::iterator> ret;
    ret = usr_umap.equal_range("beta");
    if(ret.first != usr_umap.end())
        cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<endl;
    else
        cout<<"no vaild lower bound"<<endl;
    if(ret.second != usr_umap.end())
        cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<endl;
    else
        cout<<"no vaild upper bound"<<endl;

    //find, erase
    std::unordered_map<string, int>::iterator it = usr_umap.find("gamma");
    cout<<"find:"<<it->first<<"=>"<<it->second<<endl;
    usr_umap.erase(it);
    show_unordered_map(usr_umap, "erase");

    //get_alloctator
    std::pair<const string, int>* p;
    p = usr_umap.get_allocator().allocate(5);
    cout<<"size:"<<sizeof(p)<<","<<sizeof(std::unordered_map<string, int>::value_type)<<endl;
    usr_umap.get_allocator().deallocate(p, 5);

    //insert
    usr_umap.insert(usr_umap.begin(), std::pair<string, int>("gamma", 5));
    usr_umap.insert(usr_umap.find("delta"), std::pair<string, int>("zeta", 9));
    show_unordered_map(usr_umap, "insert");
    usr_umap.insert({{"delta", 6}, {"epsilon", 9}});
    show_unordered_map(usr_umap, "insert");

    //hash_function
    typedef std::unordered_map<string, int> stringintMap;
    stringintMap::hasher hash_fn = usr_umap.hash_function();
    cout<<"file hash value:"<<hash_fn("file")<<endl;

    //key_eq
    bool equal = usr_umap.key_eq()("this", "This");
    cout<<"key_eq:"<<equal<<endl;

    std::unordered_map<string, int> swapunordered_map;

    //swap
    swapunordered_map.swap(usr_umap);
    show_unordered_map(swapunordered_map, "swap");

    //rehash, reserve
    swapunordered_map.rehash(15);
    cout<<"rehash:"<<swapunordered_map.bucket_count()<<endl;
    swapunordered_map.reserve(5);
    cout<<"reserve:"<<swapunordered_map.bucket_count()<<endl;

    return 0;
} 
