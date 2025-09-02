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
