/************************************************************************************
 * std::multiset
 * reference:http://www.cplusplus.com/reference/set/multiset/
 * multisets are containers that store unique elements following a specific order.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlmultiset
 * 3.More reference shows in function show_multiset_help or command "studymultiset -h"
***********************************************************************************/
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

void show_multiset_help(void);
template<class T>
void show_multiset(std::multiset<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void multiset_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::multiset<int> intv(array, array+5);
    std::multiset<int> usermultiset(intv);
    std::multiset<string> listmultiset;
    
    //operator=
    listmultiset = std::multiset<string>({"0x00", "0x01", "0x02"});
    
    show_multiset(listmultiset, "listmultiset");
    //count, clear, size
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
    std::pair<std::multiset<int>::const_iterator, std::multiset<int>::const_iterator> ret;
    ret = usermultiset.equal_range(5);
    cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    cout<<"equal_range upperbound:"<<*(ret.second)<<endl;
    std::multiset<int>::const_iterator iterlower = usermultiset.lower_bound(5);
    std::multiset<int>::const_iterator iterupper = usermultiset.upper_bound(5);
    cout<<"lowerbound:"<<*(ret.first)<<endl;
    cout<<"upperbound:"<<*(ret.second)<<endl;

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

    //get_allocator
    int *point = usermultiset.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        usermultiset.get_allocator().construct(&point[i], array[i]);
        cout<<point[i]<<" ";
        usermultiset.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

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
}

template<class T>
void show_multiset(std::multiset<T> value, string qstring, SHOW_MODE_ENUM mode)
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

void show_multiset_help(void)
{
    string helpstring;

    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element(c++11).\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element(c++11). \n");
    helpstring.append("clear            Removes all element from the multiset, and container size is zero. \n");
    helpstring.append("count            Searches the container for elements equivalent to val and returns the number of matches. \n");   
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element(c++11).\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element(c++11).\n");
    helpstring.append("emplace          Insert a new element at position, return newly iterator.(c++).\n");
    helpstring.append("emplace_hint     Inserts a new element in the multiset, if unique, with a hint on the insertion position.\n");
    helpstring.append("empty            Returns whehter the multiset is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the multiset container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container that are equivalent to val.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the multiset.\n");
    helpstring.append("find             Searches the container for an element equivalent to val and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the multiset.\n");
    helpstring.append("insert           insert new elements before the element at the multiset position.\n");
    helpstring.append("key_comp         Returns a copy of the comparison object used by the container to compare keys.\n");
    helpstring.append("lower_bound      Returns an iterator pointing to the first element in the container whose key is not considered to go before k.\n");   
    helpstring.append("max_size         Returns the maximum number of elements that the multiset can hold.\n");
    helpstring.append("operator=        Assgins new container, replacing current contents and modifying size.\n");
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
        show_multiset_help();
    else
        multiset_study_handle();

    return 0;
} 