/************************************************************************************
 * std::multimap
 * reference:http://www.cplusplus.com/reference/map/multimap/
 * Multimaps are associative containers that store elements formed by a combination of 
 * a key value and a mapped value, following a specific order, and where multiple elements 
 * can have equivalent keys
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlmultimap
 * 3.More reference shows in function show_multimap_help or command "studymultimap -h"
***********************************************************************************/
#include <map>
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

void show_multimap_help(void);
template<class T, class N>
void show_multimap(std::multimap<T, N>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void multimap_study_handle(void)
{
    std::multimap<string, int> usermultimap = {
        {"alpha", 0},
        {"beta", 0}, 
        {"beta", 5},
        {"beta", 2},
        {"gamma", 1},   
    };
    auto copymultimap(usermultimap);

    //size, clear, max_size, empty
    show_multimap(copymultimap, "copymultimap");
    cout<<"size:"<<copymultimap.size()<<endl;
    copymultimap.clear();
    cout<<"clear size:"<<copymultimap.size()<<endl;
    cout<<"max_size:"<<copymultimap.max_size()<<endl;
    cout<<"empty:"<<copymultimap.empty()<<endl;

    show_multimap(usermultimap, "usermultimap");

    //begin, end, cbegin, cend
    //rbegin, rend, rcbegin, rcend
    std::multimap<string, int>::iterator iterbegin = usermultimap.begin();
    std::multimap<string, int>::iterator iterend = usermultimap.end();
    std::multimap<string, int>::const_iterator iterconstbegin = usermultimap.cbegin();
    std::multimap<string, int>::const_iterator iterconstend = usermultimap.cend();
    std::multimap<string, int>::reverse_iterator iterrbegin = usermultimap.rbegin();
    std::multimap<string, int>::reverse_iterator iterrend = usermultimap.rend();
    std::multimap<string, int>::const_reverse_iterator iterconstrbegin = usermultimap.crbegin();
    std::multimap<string, int>::const_reverse_iterator iterconstrend = usermultimap.crend();
    cout<<"begin: "<<iterbegin->first<<"=>"<<iterbegin->second<<",";
    --iterend;
    cout<<"end: "<<iterend->first<<"=>"<<iterend->second<<",";
    cout<<"cbegin: "<<iterconstbegin->first<<"=>"<<iterconstbegin->second<<",";
    --iterconstend;
    cout<<"cend: "<<iterconstend->first<<"=>"<<iterconstend->second<<",";
    cout<<"rbegin: "<<iterrbegin->first<<"=>"<<iterrbegin->second<<",";
    --iterrend;
    cout<<"rend: "<<iterrend->first<<"=>"<<iterrend->second<<",";
    cout<<"crbegin: "<<iterconstrbegin->first<<"=>"<<iterconstrbegin->second<<",";
    --iterconstrend;
    cout<<"crend: "<<iterconstrend->first<<"=>"<<iterconstrend->second<<endl;

    //count
    cout<<"count:"<<usermultimap.count("beta")<<endl;

    //emplace, emplace_hint
    usermultimap.emplace("delta", 6);
    show_multimap(usermultimap, "emplace", SHOW_MODE_ENUM_TYPE_AUTO);
    usermultimap.emplace_hint(usermultimap.begin(), "epsilon", 10);
    show_multimap(usermultimap, "emplace", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //equal_range, lower_bound, upper_bound
    std::pair<std::multimap<string, int>::iterator, std::multimap<string, int>::iterator> ret;
    ret = usermultimap.equal_range("beta");
    cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<endl;
    cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<endl;
    std::multimap<string, int>::iterator lower = usermultimap.lower_bound("beta");
    std::multimap<string, int>::iterator upper = usermultimap.upper_bound("beta");
    cout<<"lower_bound:"<<lower->first<<"=>"<<lower->second<<endl;
    cout<<"upper_bound:"<<upper->first<<"=>"<<upper->second<<endl;

    //find, erase
    std::multimap<string, int>::iterator it = usermultimap.find("gamma");
    cout<<"find:"<<it->first<<"=>"<<it->second<<endl;
    usermultimap.erase(it);
    show_multimap(usermultimap, "erase");

    //get_alloctator
    std::pair<const string, int>* p;
    p = usermultimap.get_allocator().allocate(5);
    cout<<"size:"<<sizeof(p)<<","<<sizeof(std::multimap<string, int>::value_type)<<endl;
    usermultimap.get_allocator().deallocate(p, 5);

    //insert
    usermultimap.insert(usermultimap.begin(), std::pair<string, int>("gamma", 5));
    usermultimap.insert(usermultimap.find("delta"), std::pair<string, int>("zeta", 9));
    show_multimap(usermultimap, "insert");

    //key_comp => key compare, use key value
    cout<<"key_comp: ";
    for(auto &x:usermultimap)
    {
        if(usermultimap.key_comp()(x.first, "c"))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    //value_comp => key compare, use object
    cout<<"value_comp: ";
    for(auto &x:usermultimap)
    {
        if(usermultimap.value_comp()(x, std::pair<string, int>("c", 1)))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    std::multimap<string, int> swapmultimap;

    //swap
    swapmultimap.swap(usermultimap);
    show_multimap(swapmultimap, "swap");

}

template<class T, class N>
void show_multimap(std::multimap<T, N> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::multimap<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<iter->first<<"=>"<<iter->second<<" ";
    }
    cout<<endl;
}

void show_multimap_help(void)
{
    string helpstring;

    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element(c++11).\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the multimap, and container size is zero. \n");
    helpstring.append("count            Searches the container for elements with a key equivalent to k and returns the number of matches.\n");
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element(c++11).\n");
    helpstring.append("emplace          Inserts a new element in the multimap if its key is unique.\n");
    helpstring.append("emplace_hint     Inserts a new element in the multimap if its key is unique, with a hint on the insertion position..\n");
    helpstring.append("empty            Returns whehter the multimap is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the multimap container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container which have a key equivalent to k.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the multimap.\n");
    helpstring.append("find             Searches the container for an element with a key equivalent to k and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the multimap.\n");
    helpstring.append("insert           Extends the container by inserting new elements, effectively increasing the container size by the number of elements inserted.\n");
    helpstring.append("key_comp         Returns a copy of the comparison object used by the container to compare keys.\n");
    helpstring.append("lower_bound      Returns an iterator pointing to the first element in the container whose key is not considered to go before k.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the multimap can hold.\n");
    helpstring.append("operator=        Assigns new contents to the container, replacing its current content.\n");
    helpstring.append("operator[]       If k matches the key of an element in the container, the function returns a reference to its multimapped value.\n");
    helpstring.append("rbegin           Returns a reverse_iterator pointing to the last element.\n");
    helpstring.append("rend             Returns a reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("size             Returns number of elements in the container.\n");    
    helpstring.append("swap             Exchange the content of the containder.\n");
    helpstring.append("upper_bound      Returns an iterator pointing to the first element in the container whose key is considered to go after k.\n");
    helpstring.append("value_comp       Returns a comparison object that can be used to compare two elements to get whether the key of the first one goes before the second.\n");

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
        show_multimap_help();
    else
        multimap_study_handle();

    return 0;
} 







































// /*********************************************************************
//  * 说明: multimap 
//  * 关联容器，提供映射的处理关系
//  * size         获取容器的当前大小
//  * max_size     获取容器的最大允许容量
//  * empty        判断容器是否为空
//  * swap         交换两个multimap内的信息，另外一个multimap将会被清空
//  * erase clear  容器删除/清空数据
//  * insert       向容器内插入数据
//  * find         查找存在指定值，返回迭代器
//  * emplace      向队列插入数据(避免不必要的临时对象产生)
//  * lower_bound  返回非递减序列中的第一个大于等于值val的迭代器(上限闭合)
//  * upper_bound  返回非递减序列中第一个大于值val的位置(下限闭合)
// *********************************************************************/
// #include <multimap>
// #include <string>
// #include <iostream>
// #include <algorithm>
// #include <typeinfo>
// #include <multimap>

// template<class T0, class T1>
// void show_multimap(std::multimap<T0, T1> &multimap_val)
// {
//     std::for_each(multimap_val.begin(), multimap_val.end(), [](const std::pair<T0, T1> &multimap_ref){
//         std::cout<<multimap_ref.first<<":"<<multimap_ref.second<<" ";
//     });
//     std::cout<<std::endl;
// }

// int main(int argc, char* argv[])
// {
//     std::multimap<std::string, int> multimap_si0 = {
//         {"first", 1},
//         {"second", 2},
//         {"third", 3}
//     };
//     std::multimap<std::string, std::string> multimap_ss0 = {
//         {"val0", "0"},
//         {"val1", "1"},
//     };
//     std::multimap<int, int> multimap_ii0 = {
//         {10, 500},
//         {20, 600},
//         {30, 900}
//     };
//     std::cout<<"show multimap:";
//     show_multimap<std::string, int>(multimap_si0);

//     //size, max_size, empty
//     std::cout<<"multimap_ss0 size:"<<multimap_ss0.size()<<std::endl;
//     multimap_ss0.clear();
//     std::cout<<"multimap_ss0 size:"<<multimap_ss0.size()<<std::endl;
//     std::cout<<"multimap_ss0 max size:"<<multimap_ss0.max_size()<<std::endl;
//     std::cout<<"multimap_ss0 is empty:"<<multimap_ss0.empty()<<std::endl;
//     multimap_ss0["first"] = "0";
//     multimap_ss0["second"] = "1";
//     multimap_ss0["third"] = "1";
//     for(auto iter=multimap_ss0.begin();iter!=multimap_ss0.end();)
//     {
//         if(iter->second == "1")
//         {
//             iter = multimap_ss0.erase(iter);
//         }
//         else
//         {
//             iter++;
//         }
//     }
//     show_multimap<std::string, std::string>(multimap_ss0);
    
//     //lower_bound, upper_bound, swap
//     auto multimap_iterator = multimap_ii0.lower_bound(9);
//     std::cout<<"lower bound val:"<<multimap_iterator->first<<":"<<multimap_iterator->second<<std::endl;
//     multimap_iterator = multimap_ii0.upper_bound(21);
//     std::cout<<"upper bound val:"<<multimap_iterator->first<<":"<<multimap_iterator->second<<std::endl;
//     std::cout<<"show multimap_ii00:";
//     show_multimap<int, int>(multimap_ii0);

//     //swap
//     std::multimap<int, int> multimap_ii1;
//     multimap_ii1.swap(multimap_ii0);
//     std::cout<<"show multimap_ii1:";
//     show_multimap<int, int>(multimap_ii1);
//     std::cout<<"multimap_ii0 size:"<<multimap_ii0.size()<<std::endl;

//     //insert, emplace, []
//     multimap_si0.insert(std::pair<std::string, int>("four", 4));
//     multimap_si0.insert(std::make_pair<std::string, int>("five", 5));
//     multimap_si0.insert(std::multimap<std::string, int>::value_type("six", 6));
//     multimap_si0.emplace(std::pair<std::string, int>("ten", 10));
//     multimap_si0.emplace(std::pair<std::string, int>("ten", 11)); //相同的值添加无效
//     multimap_si0["Seven"] = 7;
//     std::cout<<"multimap_si0 val:";
//     show_multimap<std::string, int>(multimap_si0);
//     std::cout<<"multimap_si0 val:";
//     for(auto val:multimap_si0)
//     {
//         std::cout<<val.first<<":"<<val.second<<" ";
//     }
//     std::cout<<std::endl;
//     std::cout<<"multimap_si0 iterator val:";
//     for(auto iteartor_val=multimap_si0.begin(); iteartor_val!=multimap_si0.end(); iteartor_val++)
//     {
//         std::cout<<iteartor_val->first<<":"<<iteartor_val->second<<" ";
//     }
//     std::cout<<std::endl;

//     //find
//     std::multimap<std::string, int>::iterator iter = multimap_si0.find("second");
//     if(iter != multimap_si0.end())
//         std::cout<<iter->first<<":"<<iter->second;
//     std::cout<<std::endl;

//     //sort
//     std::cout<<"show sort:";
//     std::multimap<std::pair<std::string, int>>  vec_pair;
//     for(auto val:multimap_si0)
//     {
//         vec_pair.push_back(std::pair<std::string, int>(val.first, val.second));
//     }
//     std::sort(vec_pair.begin(), vec_pair.end(), [](std::pair<std::string, int> &multimap_ref0, std::pair<std::string, int> &multimap_ref1)->bool{
//         return multimap_ref0.second < multimap_ref1.second;
//     });
//     for(auto val:vec_pair)
//     {
//         std::cout<<val.first<<":"<<val.second<<" ";
//     }
//     std::cout<<std::endl;
//     return 0;
// }
