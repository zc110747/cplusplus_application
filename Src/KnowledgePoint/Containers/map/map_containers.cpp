/************************************************************************************
 * std::map
 * https://www.cplusplus.com/reference/map/map/
 * Maps are associative containers that store elements formed by a combination 
 * of a key value and a mapped value, following a specific order
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studymap
 * 3.More reference shows in function show_map_help or command "studymap -h"
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

void show_map_help(void);
template<typename T, typename N>
void show_map(std::map<T, N>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void map_study_handle(void)
{
    std::map<string, int> usermap = {
        {"alpha", 0},
        {"beta", 0}, 
        {"gamma", 1},   
    };
    auto copymap(usermap);

    //size, clear, max_size, empty
    show_map(copymap, "copymap");
    cout<<"size:"<<copymap.size()<<endl;
    copymap.clear();
    cout<<"clear size:"<<copymap.size()<<endl;
    cout<<"max_size:"<<copymap.max_size()<<endl;
    cout<<"empty:"<<copymap.empty()<<endl;

    show_map(usermap, "usermap");

    //at, operator[]
    usermap.at("beta") = 5;
    show_map(usermap, "usermap", SHOW_MODE_ENUM_LAMBDA);
    usermap["beta"] = 4;
    show_map(usermap, "operator[]");

    //begin, end, cbegin, cend
    //rbegin, rend, rcbegin, rcend
    std::map<string, int>::iterator iterbegin = usermap.begin();
    std::map<string, int>::iterator iterend = usermap.end();
    std::map<string, int>::const_iterator iterconstbegin = usermap.cbegin();
    std::map<string, int>::const_iterator iterconstend = usermap.cend();
    std::map<string, int>::reverse_iterator iterrbegin = usermap.rbegin();
    std::map<string, int>::reverse_iterator iterrend = usermap.rend();
    std::map<string, int>::const_reverse_iterator iterconstrbegin = usermap.crbegin();
    std::map<string, int>::const_reverse_iterator iterconstrend = usermap.crend();
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
    cout<<"count:"<<usermap.count("beta")<<endl;

    //emplace, emplace_hint
    usermap.emplace("delta", 6);
    show_map(usermap, "emplace", SHOW_MODE_ENUM_TYPE_AUTO);
    usermap.emplace_hint(usermap.begin(), "epsilon", 10);
    show_map(usermap, "emplace", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //equal_range, lower_bound, upper_bound
    std::pair<std::map<string, int>::iterator, std::map<string, int>::iterator> ret;
    ret = usermap.equal_range("beta");
    if(ret.first != usermap.end())
        cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<endl;
    else
        cout<<"invaild equal_range lower_bound"<<endl;
    if(ret.second != usermap.end())   
        cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<endl;
    else
        cout<<"invaild equal_range upper_bound"<<endl;
    std::map<string, int>::iterator lower = usermap.lower_bound("beta");
    std::map<string, int>::iterator upper = usermap.upper_bound("beta");
    if(lower != usermap.end())
        cout<<"lower_bound:"<<lower->first<<"=>"<<lower->second<<endl;
    else
        cout<<"invaild lower_bound"<<endl;
    if(upper != usermap.end())
        cout<<"upper_bound:"<<upper->first<<"=>"<<upper->second<<endl;
    else   
        cout<<"invaild upper_bound"<<endl;

    //find, erase
    std::map<string, int>::iterator it = usermap.find("gamma");
    cout<<"find:"<<it->first<<"=>"<<it->second<<endl;
    usermap.erase(it);
    show_map(usermap, "erase");

    //get_alloctator
    std::pair<const string, int>* p;
    p = usermap.get_allocator().allocate(5);
    cout<<"size:"<<sizeof(p)<<","<<sizeof(std::map<string, int>::value_type)<<endl;
    usermap.get_allocator().deallocate(p, 5);

    //insert
    usermap.insert(usermap.begin(), std::pair<string, int>("gamma", 5));
    usermap.insert(usermap.find("delta"), std::pair<string, int>("zeta", 9));
    show_map(usermap, "insert");

    //key_comp => key compare, use key value
    cout<<"key_comp: ";
    for(auto &x:usermap)
    {
        if(usermap.key_comp()(x.first, "c"))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    //value_comp => key compare, use object
    cout<<"value_comp: ";
    for(auto &x:usermap)
    {
        if(usermap.value_comp()(x, std::pair<string, int>("c", 1)))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    std::map<string, int> swapmap;

    //swap
    swapmap.swap(usermap);
    show_map(swapmap, "swap");

}

template<typename T, typename N>
void show_map(std::map<T, N> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::map<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<iter->first<<"=>"<<iter->second<<" ";
    }
    cout<<endl;
}

void show_map_help(void)
{
    string helpstring;

    helpstring.append("at               Returns a reference to the element at position n.\n");
    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element.\n");
    helpstring.append("clear            Removes all element from the map, and container size is zero.\n");
    helpstring.append("count            Searches the container for elements with a key equivalent to k and returns the number of matches.\n");
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("emplace          Inserts a new element in the map if its key is unique.\n");
    helpstring.append("emplace_hint     Inserts a new element in the map if its key is unique, with a hint on the insertion position..\n");
    helpstring.append("empty            Returns whehter the map is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the map container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container which have a key equivalent to k.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the map.\n");
    helpstring.append("find             Searches the container for an element with a key equivalent to k and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the map.\n");
    helpstring.append("insert           Extends the container by inserting new elements, effectively increasing the container size by the number of elements inserted.\n");
    helpstring.append("key_comp         Returns a copy of the comparison object used by the container to compare keys.\n");
    helpstring.append("lower_bound      Returns an iterator pointing to the first element in the container whose key is not considered to go before k.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the map can hold.\n");
    helpstring.append("operator=        Assigns new contents to the container, replacing its current content.\n");
    helpstring.append("operator[]       If k matches the key of an element in the container, the function returns a reference to its mapped value.\n");
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
        show_map_help();
    else
        map_study_handle();

    return 0;
} 
