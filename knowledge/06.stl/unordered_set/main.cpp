//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::unordered_set声明
//      2. std::unordered_set方法
//      3. std::unordered_set常用算法配合
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
void show_unordered_set(std::unordered_set<T> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        for(typename std::unordered_set<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::unordered_set<int> intv(array, array+5);
    std::unordered_set<int> userunordered_set(intv);
    std::unordered_set<string> listunordered_set;
    
    //operator=
    listunordered_set = std::unordered_set<string>({"0x00", "0x01", "0x02"});
    
    show_unordered_set(listunordered_set, "listunordered_set");
    //count, clear, size, empty
    //load_factor,max_bucket_count,max_load_factor
    cout<<"count"<<listunordered_set.count("0x01");
    listunordered_set.clear();
    cout<<"size:"<<listunordered_set.size()<<endl;
    cout<<"empty:"<<listunordered_set.empty()<<endl;
    cout<<"load_factor"<<listunordered_set.load_factor()<<endl;
    cout<<"max_bucket_count"<<listunordered_set.max_bucket_count()<<endl;
    cout<<"max_load_factor"<<listunordered_set.max_load_factor()<<endl;

    show_unordered_set(userunordered_set, "userunordered_set");

    //max_size
    cout<<"max_size:"<<userunordered_set.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    std::unordered_set<int>::iterator iterbegin = userunordered_set.begin();
    std::unordered_set<int>::iterator iterend = userunordered_set.end();
    std::unordered_set<int>::const_iterator iterconstbegin = userunordered_set.cbegin();
    std::unordered_set<int>::const_iterator iterconstend = userunordered_set.cend();
    cout<<"begin:"<<*iterbegin<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<endl;

    //bucket, bucket_count, bucket_size
    cout<<"bucket:"<<userunordered_set.bucket(5)<<endl;
    auto bucket_count = userunordered_set.bucket_count();
    cout<<"bucket count:"<<bucket_count<<endl;
    for(auto i=0; i<bucket_count; i++)
    {
        cout<<"bucket #"<<i<<" has "<<userunordered_set.bucket_size(i)<<" elements."<<endl;
    }

    //emplace, find, emplace_hint
    auto it = userunordered_set.emplace(15);
    show_unordered_set(userunordered_set, "emplace");
    cout<<"val:"<<*(it.first)<<","<<"status:"<<it.second<<endl;
    auto find = userunordered_set.find(8);
    userunordered_set.emplace_hint(find, 9);
    show_unordered_set(userunordered_set, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::unordered_set<int>::const_iterator, std::unordered_set<int>::const_iterator> ret;
    ret = userunordered_set.equal_range(5);
    cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    cout<<"equal_range upperbound:"<<*(ret.second)<<endl;

    //erase
    auto iter=userunordered_set.begin();
    while(iter!=userunordered_set.end())
    {
        if(*iter == 3)
        {
            iter=userunordered_set.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_unordered_set(userunordered_set, "erase");

    //hash_function
    typedef std::unordered_set<int> stringintMap;
    stringintMap::hasher hash_fn = userunordered_set.hash_function();
    cout<<"file hash value:"<<hash_fn(5)<<endl;

    //insert
    userunordered_set.insert(15);
    show_unordered_set(userunordered_set, "insert");

    //key_eq
    bool equal = userunordered_set.key_eq()(4, 6);
    cout<<"key_eq:"<<equal<<endl;

    //swap
    std::unordered_set<int> swapunordered_set;
    swapunordered_set.swap(userunordered_set);
    show_unordered_set(swapunordered_set, "swapunordered_set");

    //rehash, reserve
    swapunordered_set.rehash(15);
    cout<<"rehash:"<<swapunordered_set.bucket_count()<<endl;
    swapunordered_set.reserve(5);
    cout<<"reserve:"<<swapunordered_set.bucket_count()<<endl;

    return 0;
} 
