/*
std::unordered_multimap 是 C++ 标准库中的一个关联容器，它允许存储多个具有相同键的键值对。

主要特点：
1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于键的顺序。
2. 快速查找：由于使用哈希表实现，查找操作非常高效。
3. 键可重复：每个键可以在容器中出现多次。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

成员函数：
begin：返回指向容器中第一个元素的迭代器。
bucket：返回给定键所在的桶的索引。
bucket_count：返回容器中桶的数量。
bucket_size：返回给定桶中的元素数量。
cbegin：返回指向容器中第一个元素的常量迭代器。
cend：返回指向容器中最后一个元素的常量迭代器。
clear：移除容器中的所有元素。
contains(c++20)：检查容器是否包含具有给定键的元素。
count：返回容器中具有给定键的元素数量。
emplace：在容器中直接构造元素。
emplace_hint：在给定位置构造元素。
empty：检查容器是否为空。
end：返回指向容器中最后一个元素的迭代器。
equal_range：返回一个包含容器中具有给定键的所有元素的迭代器范围。
erase：移除容器中具有给定键的元素。
find：查找具有给定键的元素。
get_allocator：返回用于分配内存的分配器对象。
hash_function：返回用于计算哈希值的函数对象。
insert：插入元素到容器中。
key_eq：返回用于比较键的函数对象。
load_factor：返回当前容器的负载因子。
max_bucket_count：返回容器中桶的最大数量。
max_load_factor：返回容器的最大负载因子。
max_size：返回容器中可容纳的最大元素数量。
rehash：重新哈希容器。
size：返回容器中元素的数量。
swap：交换两个容器的内容。
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
void show_unordered_multimap(std::unordered_multimap<T, N> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        for(typename std::unordered_multimap<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<iter->first<<"=>"<<iter->second<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    std::unordered_multimap<string, int> usr_um = {
        {"alpha", 0},
        {"beta", 0}, 
        {"gamma", 1},   
    };
    auto cpy_um(usr_um);

    //size, clear, max_size, empty
    //load_factor, max_bucket_cout, max_load_factor
    show_unordered_multimap(cpy_um, "cpy_um");
    cout<<"size:"<<cpy_um.size()<<endl;
    cpy_um.clear();
    cout<<"clear size:"<<cpy_um.size()<<endl;
    cout<<"max_size:"<<cpy_um.max_size()<<endl;
    cout<<"empty:"<<cpy_um.empty()<<endl;
    cout<<"load_factor"<<cpy_um.load_factor()<<endl;
    cout<<"max_bucket_count"<<cpy_um.max_bucket_count()<<endl;
    cout<<"max_load_factor"<<cpy_um.max_load_factor()<<endl;

    show_unordered_multimap(usr_um, "usr_um");

    //bucket, bucket_count, bucket_size
    cout<<"bucket:"<<usr_um.bucket("beta")<<endl;
    auto bucket_count = usr_um.bucket_count();
    cout<<"bucket count:"<<bucket_count<<endl;
    for(auto i=0; i<bucket_count; i++)
    {
        cout<<"bucket #"<<i<<" has "<<usr_um.bucket_size(i)<<" elements."<<endl;
    }

    //begin, end, cbegin, cend
    std::unordered_multimap<string, int>::iterator iterbegin = usr_um.begin();
    std::unordered_multimap<string, int>::iterator iterend = usr_um.end();
    std::unordered_multimap<string, int>::const_iterator iterconstbegin = usr_um.cbegin();
    std::unordered_multimap<string, int>::const_iterator iterconstend = usr_um.cend();
    cout<<"begin: "<<iterbegin->first<<"=>"<<iterbegin->second<<",";
    cout<<"cbegin: "<<iterconstbegin->first<<"=>"<<iterconstbegin->second<<",";

    //count
    cout<<"count:"<<usr_um.count("beta")<<endl;

    //emplace, emplace_hint
    usr_um.emplace("delta", 6);
    show_unordered_multimap(usr_um, "emplace", SHOW_MODE_ENUM_TYPE_AUTO);
    usr_um.emplace_hint(usr_um.begin(), "epsilon", 10);
    show_unordered_multimap(usr_um, "emplace", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //equal_range
    std::pair<std::unordered_multimap<string, int>::iterator, std::unordered_multimap<string, int>::iterator> ret;
    ret = usr_um.equal_range("beta");
    if(ret.first != usr_um.end())
        cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<endl;
    else
        cout<<"no vaild lower bound"<<endl;
    if(ret.second != usr_um.end())
        cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<endl;
    else
        cout<<"no vaild upper bound"<<endl;

    //find, erase
    std::unordered_multimap<string, int>::iterator it = usr_um.find("gamma");
    cout<<"find:"<<it->first<<"=>"<<it->second<<endl;
    usr_um.erase(it);
    show_unordered_multimap(usr_um, "erase");

    //get_alloctator
    std::pair<const string, int>* p;
    p = usr_um.get_allocator().allocate(5);
    cout<<"size:"<<sizeof(p)<<","<<sizeof(std::unordered_multimap<string, int>::value_type)<<endl;
    usr_um.get_allocator().deallocate(p, 5);

    //insert
    usr_um.insert(usr_um.begin(), std::pair<string, int>("gamma", 5));
    usr_um.insert(usr_um.find("delta"), std::pair<string, int>("zeta", 9));
    show_unordered_multimap(usr_um, "insert");
    usr_um.insert({{"delta", 6}, {"epsilon", 9}});
    show_unordered_multimap(usr_um, "insert");

    //hash_function
    typedef std::unordered_multimap<string, int> stringintMap;
    stringintMap::hasher hash_fn = usr_um.hash_function();
    cout<<"file hash value:"<<hash_fn("file")<<endl;

    //key_eq
    bool equal = usr_um.key_eq()("this", "This");
    cout<<"key_eq:"<<equal<<endl;

    std::unordered_multimap<string, int> swapunordered_multimap;

    //swap
    swapunordered_multimap.swap(usr_um);
    show_unordered_multimap(swapunordered_multimap, "swap");

    //rehash, reserve
    swapunordered_multimap.rehash(15);
    cout<<"rehash:"<<swapunordered_multimap.bucket_count()<<endl;
    swapunordered_multimap.reserve(5);
    cout<<"reserve:"<<swapunordered_multimap.bucket_count()<<endl;

    return 0;
} 
