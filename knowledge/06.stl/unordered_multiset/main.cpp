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
