//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::unordered_multimap声明
//      2. std::unordered_multimap方法
//      3. std::unordered_multimap常用算法配合
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
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
