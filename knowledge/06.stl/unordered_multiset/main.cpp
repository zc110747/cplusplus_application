/*
std::unordered_multiset 是 C++ 标准库中的一个容器，它类似于
std::unordered_set，但允许存储多个具有相同值的元素

主要特点：
1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于其值。
2. 允许重复元素：与 std::unordered_set 不同，std::unordered_multiset 允许存储多个具有相同值的元素。
3. 快速查找：由于使用哈希表实现，查找操作的平均时间复杂度为常数时间 O(1)。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

成员函数：
begin: 返回指向容器中第一个元素的迭代器。
bucket: 返回给定值所在的桶的索引。
bucket_count: 返回桶的数量。
bucket_size: 返回给定桶中的元素数量。
cbegin: 返回指向容器中第一个元素的常量迭代器。
cend: 返回指向容器中最后一个元素的下一个位置的常量迭代器。
clear: 移除容器中的所有元素。
count: 返回容器中具有给定值的元素数量。
contains: 检查容器中是否存在具有给定值的元素。
emplace: 在容器中构造并插入一个新元素。
emplace_hint: 在容器中构造并插入一个新元素，使用给定的提示位置。
empty: 检查容器是否为空。
end: 返回指向容器中最后一个元素的下一个位置的迭代器。
equal_range: 返回一个范围，其中包含具有给定值的所有元素。
erase: 移除容器中具有给定值的所有元素。
find: 查找容器中具有给定值的元素。
get_allocator: 返回用于分配内存的分配器对象。
hash_function: 返回用于计算哈希值的哈希函数对象。
insert: 插入元素到容器中。
key_eq: 返回用于比较键的相等性谓词。
load_factor: 返回当前的负载因子。
max_bucket_count: 返回最大桶数。
max_load_factor: 返回最大负载因子。
max_size: 返回容器可容纳的最大元素数量。
rehash: 重新哈希容器，以减少冲突。
size: 返回容器中的元素数量。
swap: 交换两个容器的内容。
*/
#include <unordered_set>
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

template<typename T>
void show_unordered_multiset(std::unordered_multiset<T> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        std::for_each(value.begin(), value.end(), [](const T &ref_value){
            cout<<ref_value<<" ";
        });
    }
    //auto
    else if(mode == SHOW_MODE_ENUM_TYPE_AUTO)
    {
        for(auto &ref : value)
            cout<<ref<<" ";
    }
    //iterator
    else if(mode == SHOW_MODE_ENUM_TYPE_ITERATOR)
    {
        for(typename std::unordered_multiset<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::unordered_multiset<int> arr_ums(array, array+5);
    std::unordered_multiset<int> usr_ums(arr_ums);
    std::unordered_multiset<string> str_ums;
    
    //operator=
    str_ums = std::unordered_multiset<string>({"0x00", "0x01", "0x02"});
    
    show_unordered_multiset(str_ums, "str_ums");
    //count, clear, size, empty
    //load_factor,max_bucket_count,max_load_factor
    cout<<"count"<<str_ums.count("0x01");
    str_ums.clear();
    cout<<"size:"<<str_ums.size()<<endl;
    cout<<"empty:"<<str_ums.empty()<<endl;
    cout<<"load_factor"<<str_ums.load_factor()<<endl;
    cout<<"max_bucket_count"<<str_ums.max_bucket_count()<<endl;
    cout<<"max_load_factor"<<str_ums.max_load_factor()<<endl;

    show_unordered_multiset(usr_ums, "usr_ums");

    //max_size
    cout<<"max_size:"<<usr_ums.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    std::unordered_multiset<int>::iterator iterbegin = usr_ums.begin();
    std::unordered_multiset<int>::iterator iterend = usr_ums.end();
    std::unordered_multiset<int>::const_iterator iterconstbegin = usr_ums.cbegin();
    std::unordered_multiset<int>::const_iterator iterconstend = usr_ums.cend();
    cout<<"begin:"<<*iterbegin<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<endl;

    //bucket, bucket_count, bucket_size
    cout<<"bucket:"<<usr_ums.bucket(5)<<endl;
    auto bucket_count = usr_ums.bucket_count();
    cout<<"bucket count:"<<bucket_count<<endl;
    for(auto i=0; i<bucket_count; i++)
    {
        cout<<"bucket #"<<i<<" has "<<usr_ums.bucket_size(i)<<" elements."<<endl;
    }

    //emplace, find, emplace_hint
    auto it = usr_ums.emplace(15);
    show_unordered_multiset(usr_ums, "emplace");
    cout<<"emplace:"<<*it<<endl;
    auto find = usr_ums.find(8);
    usr_ums.emplace_hint(find, 9);
    show_unordered_multiset(usr_ums, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::unordered_multiset<int>::const_iterator, std::unordered_multiset<int>::const_iterator> ret;
    ret = usr_ums.equal_range(5);
    cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    cout<<"equal_range upperbound:"<<*(ret.second)<<endl;

    //erase
    auto iter=usr_ums.begin();
    while(iter!=usr_ums.end())
    {
        if(*iter == 3)
        {
            iter=usr_ums.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_unordered_multiset(usr_ums, "erase");

    //hash_function
    typedef std::unordered_multiset<int> stringintMap;
    stringintMap::hasher hash_fn = usr_ums.hash_function();
    cout<<"file hash value:"<<hash_fn(5)<<endl;

    //insert
    usr_ums.insert(15);
    show_unordered_multiset(usr_ums, "insert");

    //key_eq
    bool equal = usr_ums.key_eq()(4, 6);
    cout<<"key_eq:"<<equal<<endl;

    //swap
    std::unordered_multiset<int> swapunordered_multiset;
    swapunordered_multiset.swap(usr_ums);
    show_unordered_multiset(swapunordered_multiset, "swapunordered_multiset");

    //rehash, reserve
    swapunordered_multiset.rehash(15);
    cout<<"rehash:"<<swapunordered_multiset.bucket_count()<<endl;
    swapunordered_multiset.reserve(5);
    cout<<"reserve:"<<swapunordered_multiset.bucket_count()<<endl;

    return 0;
} 
