﻿/************************************************************************************
 * std::unordered_multimap
 * https://www.cplusplus.com/reference/unordered_map/unordered_multimap/
 * Unordered maps are associative containers that store elements formed by the combination 
 * of a key value and a mapped value, and which allows for fast retrieval of individual 
 * elements based on their keys.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studyunordered_multimap
 * 3.More reference shows in function show_unordered_multimap_help or command "studyunordered_multimap -h"
***********************************************************************************/
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

void show_unordered_multimap_help(void);
template<class T, class N>
void show_unordered_multimap(std::unordered_multimap<T, N>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void unordered_multimap_study_handle(void)
{
    std::unordered_multimap<string, int> userunordered_multimap = {
        {"alpha", 0},
        {"beta", 0}, 
        {"gamma", 1},   
    };
    auto copyunordered_multimap(userunordered_multimap);

    //size, clear, max_size, empty
    //load_factor, max_bucket_cout, max_load_factor
    show_unordered_multimap(copyunordered_multimap, "copyunordered_multimap");
    cout<<"size:"<<copyunordered_multimap.size()<<endl;
    copyunordered_multimap.clear();
    cout<<"clear size:"<<copyunordered_multimap.size()<<endl;
    cout<<"max_size:"<<copyunordered_multimap.max_size()<<endl;
    cout<<"empty:"<<copyunordered_multimap.empty()<<endl;
    cout<<"load_factor"<<copyunordered_multimap.load_factor()<<endl;
    cout<<"max_bucket_count"<<copyunordered_multimap.max_bucket_count()<<endl;
    cout<<"max_load_factor"<<copyunordered_multimap.max_load_factor()<<endl;

    show_unordered_multimap(userunordered_multimap, "userunordered_multimap");

    //bucket, bucket_count, bucket_size
    cout<<"bucket:"<<userunordered_multimap.bucket("beta")<<endl;
    auto bucket_count = userunordered_multimap.bucket_count();
    cout<<"bucket count:"<<bucket_count<<endl;
    for(auto i=0; i<bucket_count; i++)
    {
        cout<<"bucket #"<<i<<" has "<<userunordered_multimap.bucket_size(i)<<" elements."<<endl;
    }

    //begin, end, cbegin, cend
    std::unordered_multimap<string, int>::iterator iterbegin = userunordered_multimap.begin();
    std::unordered_multimap<string, int>::iterator iterend = userunordered_multimap.end();
    std::unordered_multimap<string, int>::const_iterator iterconstbegin = userunordered_multimap.cbegin();
    std::unordered_multimap<string, int>::const_iterator iterconstend = userunordered_multimap.cend();
    cout<<"begin: "<<iterbegin->first<<"=>"<<iterbegin->second<<",";
    cout<<"cbegin: "<<iterconstbegin->first<<"=>"<<iterconstbegin->second<<",";

    //count
    cout<<"count:"<<userunordered_multimap.count("beta")<<endl;

    //emplace, emplace_hint
    userunordered_multimap.emplace("delta", 6);
    show_unordered_multimap(userunordered_multimap, "emplace", SHOW_MODE_ENUM_TYPE_AUTO);
    userunordered_multimap.emplace_hint(userunordered_multimap.begin(), "epsilon", 10);
    show_unordered_multimap(userunordered_multimap, "emplace", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //equal_range
    std::pair<std::unordered_multimap<string, int>::iterator, std::unordered_multimap<string, int>::iterator> ret;
    ret = userunordered_multimap.equal_range("beta");
    if(ret.first != userunordered_multimap.end())
        cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<endl;
    else
        cout<<"no vaild lower bound"<<endl;
    if(ret.second != userunordered_multimap.end())
        cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<endl;
    else
        cout<<"no vaild upper bound"<<endl;

    //find, erase
    std::unordered_multimap<string, int>::iterator it = userunordered_multimap.find("gamma");
    cout<<"find:"<<it->first<<"=>"<<it->second<<endl;
    userunordered_multimap.erase(it);
    show_unordered_multimap(userunordered_multimap, "erase");

    //get_alloctator
    std::pair<const string, int>* p;
    p = userunordered_multimap.get_allocator().allocate(5);
    cout<<"size:"<<sizeof(p)<<","<<sizeof(std::unordered_multimap<string, int>::value_type)<<endl;
    userunordered_multimap.get_allocator().deallocate(p, 5);

    //insert
    userunordered_multimap.insert(userunordered_multimap.begin(), std::pair<string, int>("gamma", 5));
    userunordered_multimap.insert(userunordered_multimap.find("delta"), std::pair<string, int>("zeta", 9));
    show_unordered_multimap(userunordered_multimap, "insert");
    userunordered_multimap.insert({{"delta", 6}, {"epsilon", 9}});
    show_unordered_multimap(userunordered_multimap, "insert");

    //hash_function
    typedef std::unordered_multimap<string, int> stringintMap;
    stringintMap::hasher hash_fn = userunordered_multimap.hash_function();
    cout<<"file hash value:"<<hash_fn("file")<<endl;

    //key_eq
    bool equal = userunordered_multimap.key_eq()("this", "This");
    cout<<"key_eq:"<<equal<<endl;

    std::unordered_multimap<string, int> swapunordered_multimap;

    //swap
    swapunordered_multimap.swap(userunordered_multimap);
    show_unordered_multimap(swapunordered_multimap, "swap");

    //rehash, reserve
    swapunordered_multimap.rehash(15);
    cout<<"rehash:"<<swapunordered_multimap.bucket_count()<<endl;
    swapunordered_multimap.reserve(5);
    cout<<"reserve:"<<swapunordered_multimap.bucket_count()<<endl;
}

template<class T, class N>
void show_unordered_multimap(std::unordered_multimap<T, N> value, string qstring, SHOW_MODE_ENUM mode)
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

void show_unordered_multimap_help(void)
{
    string helpstring;

    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("bucket           Returns the bucket number where the element with key k is located.\n");
    helpstring.append("bucket_count     Returns the number of buckets in the unordered_multimap container..\n");
    helpstring.append("bucket_size      Returns the number of elements in bucket n.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the unordered_multimap, and container size is zero. \n");
    helpstring.append("count            Searches the container for elements with a key equivalent to k and returns the number of matches.\n");
    helpstring.append("emplace          Inserts a new element in the unordered_multimap if its key is unique.\n");
    helpstring.append("emplace_hint     Inserts a new element in the unordered_multimap if its key is unique, with a hint on the insertion position.\n");
    helpstring.append("empty            Returns whehter the unordered_multimap is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the unordered_multimap container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container which have a key equivalent to k.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the unordered_multimap.\n");
    helpstring.append("find             Searches the container for an element with a key equivalent to k and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the unordered_multimap.\n");
    helpstring.append("hash function    Returns the hash function object used by the unordered_multimap container.\n");
    helpstring.append("insert           Extends the container by inserting new elements, effectively increasing the container size by the number of elements inserted.\n");
    helpstring.append("key_eq           Returns the key equivalence comparison predicate used by the unordered_multimap container.\n");
    helpstring.append("load_factor      Returns the current load factor in the unordered_multimap container.\n");
    helpstring.append("max_bucket_count Returns the maximum number of buckets that the unordered_multimap container can have.\n");
    helpstring.append("max_load_factor  Get or set maximum load factor.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the unordered_multimap container can hold.\n");
    helpstring.append("operator=        Assigns new contents to the container, replacing its current content.\n");
    helpstring.append("rehash            Sets the number of buckets in the container to n or more.\n");
    helpstring.append("reserve          Sets the number of buckets in the container (bucket_count) to the most appropriate to contain at least n elements.\n");
    helpstring.append("size             Returns number of elements in the container.\n");    
    helpstring.append("swap             Exchange the content of the containder.\n");

    cout<<helpstring;
}

int main(int argc, char* argv[])
{
    int opt, mode(0);

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
        {
        case 'h':
            mode = 1;
            break;
        
        default:

            break;
        }
    }

    if(mode == 1)
        show_unordered_multimap_help();
    else
        unordered_multimap_study_handle();

    return 0;
} 
