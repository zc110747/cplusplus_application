//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::set声明
//      2. std::set方法
//      3. std::set常用算法配合
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <set>
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
void show_set(std::set<T> value, string qlabel, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
{
    //empty, size
    if(!qlabel.empty())
    {
        for(auto index=qlabel.size(); index<13; index++)
            qlabel.push_back(' ');
        qlabel += ":";
        cout<<qlabel;
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
        for(typename std::set<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}


int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::set<int> arr_set(array, array+5);
    std::set<int> usr_set(arr_set);
    std::set<string> str_set;
    
    //operator=
    str_set = std::set<string>({"0x00", "0x01", "0x02"});
    show_set(str_set, "str_set");

    //count, clear, size, empty
    cout<<"count"<<str_set.count("0x01")<<" | ";
    str_set.clear();
    cout<<"size:"<<str_set.size()<<endl;
    cout<<"empty:"<<str_set.empty()<<endl;

    show_set(usr_set, "usr_set");

    //max_size
    cout<<"max_size:"<<usr_set.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::set<int>::iterator iterbegin = usr_set.begin();
    std::set<int>::iterator iterend = usr_set.end();
    std::set<int>::const_iterator iterconstbegin = usr_set.cbegin();
    std::set<int>::const_iterator iterconstend = usr_set.cend();
    std::set<int>::reverse_iterator iterrbegin = usr_set.rbegin();
    std::set<int>::reverse_iterator iterrend = usr_set.rend();
    std::set<int>::const_reverse_iterator iterconstrbegin = usr_set.crbegin();
    std::set<int>::const_reverse_iterator iterconstrend = usr_set.crend();
    cout<<"begin:"<<*iterbegin<<" | "<<"end:"<<*(--iterend)<<" | ";
    cout<<"cbegin:"<<*iterconstbegin<<" | "<<"cend:"<<*(--iterconstend)<<" | ";
    cout<<"rbegin:"<<*iterrbegin<<" | "<<"rend:"<<*(--iterrend)<<" | ";
    cout<<"crbegin:"<<*iterconstrbegin<<" | "<<"crend:"<<*(--iterconstrend)<<" | "<<endl;

    //emplace, find, emplace_hint
    auto it = usr_set.emplace(15);
    show_set(usr_set, "emplace");
    cout<<"val:"<<*(it.first)<<","<<"status:"<<it.second<<endl;
    auto find = usr_set.find(8);
    usr_set.emplace_hint(find, 9);
    show_set(usr_set, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::set<int>::const_iterator, std::set<int>::const_iterator> ret;
    ret = usr_set.equal_range(5);
    if(ret.first != usr_set.end())
        cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    if(ret.second != usr_set.end())
        cout<<"equal_range upperbound:"<<*(ret.second)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;

    std::set<int>::const_iterator iterlower = usr_set.lower_bound(5);
    std::set<int>::const_iterator iterupper = usr_set.upper_bound(5);
    if(iterlower != usr_set.end())
        cout<<"lowerbound:"<<*iterlower<<endl;
    else
        cout<<"invalid lowerbound"<<endl;
    if(iterupper != usr_set.end())
        cout<<"upperbound:"<<*iterupper<<endl;
    else
        cout<<"invalid upperbound"<<endl;

    //erase
    auto iter=usr_set.begin();
    while(iter!=usr_set.end())
    {
        if(*iter == 3)
        {
            iter=usr_set.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_set(usr_set, "erase");

    //insert
    usr_set.insert(15);
    show_set(usr_set, "insert");

    //key_comp
    cout<<"key_comp:";
    for(auto &x:usr_set)
    {
        if(usr_set.key_comp()(x, 7))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //value_comp
    cout<<"value comp:";
    for(auto &x:usr_set)
    {
        if(usr_set.value_comp()(x, 9))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //swap
    std::set<int> swapset;
    swapset.swap(usr_set);
    show_set(swapset, "swapset");

    return 0;
}
