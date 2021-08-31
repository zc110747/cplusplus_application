/************************************************************************************
 * std::unordered_multimap
 * reference:http://www.cplusplus.com/reference/unordered_map/unordered_multimap/
 * Unordered maps are associative containers that store elements formed by the combination 
 * of a key value and a mapped value, and which allows for fast retrieval of individual 
 * elements based on their keys.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlunordered_multimap
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







































// /*********************************************************************
//  * 说明: unordered_multimap 
//  * 关联容器，提供映射的处理关系
//  * size         获取容器的当前大小
//  * max_size     获取容器的最大允许容量
//  * empty        判断容器是否为空
//  * swap         交换两个unordered_multimap内的信息，另外一个unordered_multimap将会被清空
//  * erase clear  容器删除/清空数据
//  * insert       向容器内插入数据
//  * find         查找存在指定值，返回迭代器
//  * emplace      向队列插入数据(避免不必要的临时对象产生)
//  * lower_bound  返回非递减序列中的第一个大于等于值val的迭代器(上限闭合)
//  * upper_bound  返回非递减序列中第一个大于值val的位置(下限闭合)
// *********************************************************************/
// #include <unordered_multimap>
// #include <string>
// #include <iostream>
// #include <algorithm>
// #include <typeinfo>
// #include <unordered_multimap>

// template<class T0, class T1>
// void show_unordered_multimap(std::unordered_multimap<T0, T1> &unordered_multimap_val)
// {
//     std::for_each(unordered_multimap_val.begin(), unordered_multimap_val.end(), [](const std::pair<T0, T1> &unordered_multimap_ref){
//         std::cout<<unordered_multimap_ref.first<<":"<<unordered_multimap_ref.second<<" ";
//     });
//     std::cout<<std::endl;
// }

// int main(int argc, char* argv[])
// {
//     std::unordered_multimap<std::string, int> unordered_multimap_si0 = {
//         {"first", 1},
//         {"second", 2},
//         {"third", 3}
//     };
//     std::unordered_multimap<std::string, std::string> unordered_multimap_ss0 = {
//         {"val0", "0"},
//         {"val1", "1"},
//     };
//     std::unordered_multimap<int, int> unordered_multimap_ii0 = {
//         {10, 500},
//         {20, 600},
//         {30, 900}
//     };
//     std::cout<<"show unordered_multimap:";
//     show_unordered_multimap<std::string, int>(unordered_multimap_si0);

//     //size, max_size, empty
//     std::cout<<"unordered_multimap_ss0 size:"<<unordered_multimap_ss0.size()<<std::endl;
//     unordered_multimap_ss0.clear();
//     std::cout<<"unordered_multimap_ss0 size:"<<unordered_multimap_ss0.size()<<std::endl;
//     std::cout<<"unordered_multimap_ss0 max size:"<<unordered_multimap_ss0.max_size()<<std::endl;
//     std::cout<<"unordered_multimap_ss0 is empty:"<<unordered_multimap_ss0.empty()<<std::endl;
//     unordered_multimap_ss0["first"] = "0";
//     unordered_multimap_ss0["second"] = "1";
//     unordered_multimap_ss0["third"] = "1";
//     for(auto iter=unordered_multimap_ss0.begin();iter!=unordered_multimap_ss0.end();)
//     {
//         if(iter->second == "1")
//         {
//             iter = unordered_multimap_ss0.erase(iter);
//         }
//         else
//         {
//             iter++;
//         }
//     }
//     show_unordered_multimap<std::string, std::string>(unordered_multimap_ss0);
    
//     //lower_bound, upper_bound, swap
//     auto unordered_multimap_iterator = unordered_multimap_ii0.lower_bound(9);
//     std::cout<<"lower bound val:"<<unordered_multimap_iterator->first<<":"<<unordered_multimap_iterator->second<<std::endl;
//     unordered_multimap_iterator = unordered_multimap_ii0.upper_bound(21);
//     std::cout<<"upper bound val:"<<unordered_multimap_iterator->first<<":"<<unordered_multimap_iterator->second<<std::endl;
//     std::cout<<"show unordered_multimap_ii00:";
//     show_unordered_multimap<int, int>(unordered_multimap_ii0);

//     //swap
//     std::unordered_multimap<int, int> unordered_multimap_ii1;
//     unordered_multimap_ii1.swap(unordered_multimap_ii0);
//     std::cout<<"show unordered_multimap_ii1:";
//     show_unordered_multimap<int, int>(unordered_multimap_ii1);
//     std::cout<<"unordered_multimap_ii0 size:"<<unordered_multimap_ii0.size()<<std::endl;

//     //insert, emplace, []
//     unordered_multimap_si0.insert(std::pair<std::string, int>("four", 4));
//     unordered_multimap_si0.insert(std::make_pair<std::string, int>("five", 5));
//     unordered_multimap_si0.insert(std::unordered_multimap<std::string, int>::value_type("six", 6));
//     unordered_multimap_si0.emplace(std::pair<std::string, int>("ten", 10));
//     unordered_multimap_si0.emplace(std::pair<std::string, int>("ten", 11)); //相同的值添加无效
//     unordered_multimap_si0["Seven"] = 7;
//     std::cout<<"unordered_multimap_si0 val:";
//     show_unordered_multimap<std::string, int>(unordered_multimap_si0);
//     std::cout<<"unordered_multimap_si0 val:";
//     for(auto val:unordered_multimap_si0)
//     {
//         std::cout<<val.first<<":"<<val.second<<" ";
//     }
//     std::cout<<std::endl;
//     std::cout<<"unordered_multimap_si0 iterator val:";
//     for(auto iteartor_val=unordered_multimap_si0.begin(); iteartor_val!=unordered_multimap_si0.end(); iteartor_val++)
//     {
//         std::cout<<iteartor_val->first<<":"<<iteartor_val->second<<" ";
//     }
//     std::cout<<std::endl;

//     //find
//     std::unordered_multimap<std::string, int>::iterator iter = unordered_multimap_si0.find("second");
//     if(iter != unordered_multimap_si0.end())
//         std::cout<<iter->first<<":"<<iter->second;
//     std::cout<<std::endl;

//     //sort
//     std::cout<<"show sort:";
//     std::unordered_multimap<std::pair<std::string, int>>  vec_pair;
//     for(auto val:unordered_multimap_si0)
//     {
//         vec_pair.push_back(std::pair<std::string, int>(val.first, val.second));
//     }
//     std::sort(vec_pair.begin(), vec_pair.end(), [](std::pair<std::string, int> &unordered_multimap_ref0, std::pair<std::string, int> &unordered_multimap_ref1)->bool{
//         return unordered_multimap_ref0.second < unordered_multimap_ref1.second;
//     });
//     for(auto val:vec_pair)
//     {
//         std::cout<<val.first<<":"<<val.second<<" ";
//     }
//     std::cout<<std::endl;
//     return 0;
// }
