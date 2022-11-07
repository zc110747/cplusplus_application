/************************************************************************************
 * std::set
 * https://www.cplusplus.com/reference/set/set/
 * Sets are containers that store unique elements following a specific order.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studyset
 * 3.More reference shows in function show_set_help or command "studyset -h"
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

void show_set_help(void);
template<typename T>
void show_set(std::set<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void set_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::set<int> intv(array, array+5);
    std::set<int> userset(intv);
    std::set<string> listset;
    
    //operator=
    listset = std::set<string>({"0x00", "0x01", "0x02"});
    
    show_set(listset, "listset");
    //count, clear, size, empty
    cout<<"count"<<listset.count("0x01");
    listset.clear();
    cout<<"size:"<<listset.size()<<endl;
    cout<<"empty:"<<listset.empty()<<endl;

    show_set(userset, "userset");

    //max_size
    cout<<"max_size:"<<userset.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::set<int>::iterator iterbegin = userset.begin();
    std::set<int>::iterator iterend = userset.end();
    std::set<int>::const_iterator iterconstbegin = userset.cbegin();
    std::set<int>::const_iterator iterconstend = userset.cend();
    std::set<int>::reverse_iterator iterrbegin = userset.rbegin();
    std::set<int>::reverse_iterator iterrend = userset.rend();
    std::set<int>::const_reverse_iterator iterconstrbegin = userset.crbegin();
    std::set<int>::const_reverse_iterator iterconstrend = userset.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(--iterend)<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(--iterconstend)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(--iterrend)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(--iterconstrend)<<" "<<endl;

    //emplace, find, emplace_hint
    auto it = userset.emplace(15);
    show_set(userset, "emplace");
    cout<<"val:"<<*(it.first)<<","<<"status:"<<it.second<<endl;
    auto find = userset.find(8);
    userset.emplace_hint(find, 9);
    show_set(userset, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::set<int>::const_iterator, std::set<int>::const_iterator> ret;
    ret = userset.equal_range(5);
    if(ret.first != userset.end())
        cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    if(ret.second != userset.end())
        cout<<"equal_range upperbound:"<<*(ret.second)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    std::set<int>::const_iterator iterlower = userset.lower_bound(5);
    std::set<int>::const_iterator iterupper = userset.upper_bound(5);
    if(iterlower != userset.end())
        cout<<"lowerbound:"<<*iterlower<<endl;
    else
        cout<<"invalid lowerbound"<<endl;
    if(iterupper != userset.end())
        cout<<"upperbound:"<<*iterupper<<endl;
    else
        cout<<"invalid upperbound"<<endl;

    //erase
    auto iter=userset.begin();
    while(iter!=userset.end())
    {
        if(*iter == 3)
        {
            iter=userset.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_set(userset, "erase");

    //get_allocator
    int *point = userset.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        userset.get_allocator().construct(&point[i], array[i]);
        cout<<point[i]<<" ";
        userset.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //insert
    userset.insert(15);
    show_set(userset, "insert");

    //key_comp
    cout<<"key_comp:";
    for(auto &x:userset)
    {
        if(userset.key_comp()(x, 7))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //value_comp
    cout<<"value comp:";
    for(auto &x:userset)
    {
        if(userset.value_comp()(x, 9))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //swap
    std::set<int> swapset;
    swapset.swap(userset);
    show_set(swapset, "swapset");
}

template<typename T>
void show_set(std::set<T> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::set<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

void show_set_help(void)
{
    string helpstring;

    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the set, and container size is zero. \n");
    helpstring.append("count            Searches the container for elements equivalent to val and returns the number of matches. \n");   
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("emplace          Insert a new element at position, return newly iterator.\n");
    helpstring.append("emplace_hint     Inserts a new element in the set, if unique, with a hint on the insertion position.\n");
    helpstring.append("empty            Returns whehter the set is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the set container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container that are equivalent to val.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the set.\n");
    helpstring.append("find             Searches the container for an element equivalent to val and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the set.\n");
    helpstring.append("insert           insert new elements before the element at the set position.\n");
    helpstring.append("key_comp         Returns a copy of the comparison object used by the container to compare keys.\n");
    helpstring.append("lower_bound      Returns an iterator pointing to the first element in the container whose key is not considered to go before k.\n");   
    helpstring.append("max_size         Returns the maximum number of elements that the set can hold.\n");
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
        show_set_help();
    else
        set_study_handle();

    return 0;
} 