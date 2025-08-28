//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2025-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::multiset声明
//      2. std::multiset方法
//      3. std::multiset常用算法配合
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
void show_multiset(std::multiset<T> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        for(typename std::multiset<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::multiset<int> intv(array, array+5);
    std::multiset<int> usermultiset(intv);
    std::multiset<string> listmultiset;
    
    //operator=
    listmultiset = std::multiset<string>({"0x00", "0x01", "0x02"});
    
    show_multiset(listmultiset, "listmultiset");
    //count, clear, size,empty
    cout<<"count"<<listmultiset.count("0x01");
    listmultiset.clear();
    cout<<"size:"<<listmultiset.size()<<endl;
    cout<<"empty:"<<listmultiset.empty()<<endl;

    show_multiset(usermultiset, "usermultiset");

    //max_size
    cout<<"max_size:"<<usermultiset.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::multiset<int>::iterator iterbegin = usermultiset.begin();
    std::multiset<int>::iterator iterend = usermultiset.end();
    std::multiset<int>::const_iterator iterconstbegin = usermultiset.cbegin();
    std::multiset<int>::const_iterator iterconstend = usermultiset.cend();
    std::multiset<int>::reverse_iterator iterrbegin = usermultiset.rbegin();
    std::multiset<int>::reverse_iterator iterrend = usermultiset.rend();
    std::multiset<int>::const_reverse_iterator iterconstrbegin = usermultiset.crbegin();
    std::multiset<int>::const_reverse_iterator iterconstrend = usermultiset.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(--iterend)<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(--iterconstend)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(--iterrend)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(--iterconstrend)<<" "<<endl;

    //emplace, find, emplace_hint
    auto it = usermultiset.emplace(15);
    show_multiset(usermultiset, "emplace");
    cout<<"val:"<<*it<<endl;
    auto find = usermultiset.find(8);
    usermultiset.emplace_hint(find, 9);
    show_multiset(usermultiset, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    //equal_range, lower_bound, upper_bound
    std::pair<std::set<int>::const_iterator, std::set<int>::const_iterator> ret;
    ret = usermultiset.equal_range(5);
    if(ret.first != usermultiset.end())
        cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    if(ret.second != usermultiset.end())
        cout<<"equal_range upperbound:"<<*(ret.second)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    std::set<int>::const_iterator iterlower = usermultiset.lower_bound(5);
    std::set<int>::const_iterator iterupper = usermultiset.upper_bound(5);
    if(iterlower != usermultiset.end())
        cout<<"lowerbound:"<<*iterlower<<endl;
    else
        cout<<"invalid lowerbound"<<endl;
    if(iterupper != usermultiset.end())
        cout<<"upperbound:"<<*iterupper<<endl;
    else
        cout<<"invalid upperbound"<<endl;

    //erase
    auto iter=usermultiset.begin();
    while(iter!=usermultiset.end())
    {
        if(*iter == 3)
        {
            iter=usermultiset.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_multiset(usermultiset, "erase");

    //insert
    usermultiset.insert(15);
    show_multiset(usermultiset, "insert");

    //key_comp
    cout<<"key_comp:";
    for(auto &x:usermultiset)
    {
        if(usermultiset.key_comp()(x, 7))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //value_comp
    cout<<"value comp:";
    for(auto &x:usermultiset)
    {
        if(usermultiset.value_comp()(x, 9))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //swap
    std::multiset<int> swapmultiset;
    swapmultiset.swap(usermultiset);
    show_multiset(swapmultiset, "swapmultiset");
    
    return 0;
}
