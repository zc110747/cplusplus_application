/************************************************************************************
 * std::unordered_set
 * reference:http://www.cplusplus.com/reference/unordered_set/unordered_set/
 * unordered_sets are containers that store unique elements following a specific order.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlunordered_set
 * 3.More reference shows in function show_unordered_set_help or command "studyunordered_set -h"
***********************************************************************************/
#include <unordered_set>
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

void show_unordered_set_help(void);
template<class T>
void show_unordered_set(std::unordered_set<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void unordered_set_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::unordered_set<int> intv(array, array+5);
    std::unordered_set<int> userunordered_set(intv);
    std::unordered_set<string> listunordered_set;
    
    //operator=
    listunordered_set = std::unordered_set<string>({"0x00", "0x01", "0x02"});
    
    show_unordered_set(listunordered_set, "listunordered_set");
    //count, clear, size
    cout<<"count"<<listunordered_set.count("0x01");
    listunordered_set.clear();
    cout<<"size:"<<listunordered_set.size()<<endl;
    cout<<"empty:"<<listunordered_set.empty()<<endl;

    show_unordered_set(userunordered_set, "userunordered_set");

    //max_size
    cout<<"max_size:"<<userunordered_set.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::unordered_set<int>::iterator iterbegin = userunordered_set.begin();
    std::unordered_set<int>::iterator iterend = userunordered_set.end();
    std::unordered_set<int>::const_iterator iterconstbegin = userunordered_set.cbegin();
    std::unordered_set<int>::const_iterator iterconstend = userunordered_set.cend();
    cout<<"begin:"<<*iterbegin<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<endl;

    //emplace, find, emplace_hint
    auto it = userunordered_set.emplace(15);
    show_unordered_set(userunordered_set, "emplace");
    cout<<"val:"<<*(it.first)<<","<<"status:"<<it.second<<endl;
    auto find = userunordered_set.find(8);
    userunordered_set.emplace_hint(find, 9);
    show_unordered_set(userunordered_set, "emplace_hint");

    //equal_range
    std::pair<std::unordered_set<int>::const_iterator, std::unordered_set<int>::const_iterator> ret;
    ret = userunordered_set.equal_range(5);
    cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    cout<<"equal_range upperbound:"<<*(ret.second)<<endl;

    //erase
    auto iter=userunordered_set.begin();
    while(iter!=userunordered_set.end())
    {
        if(*iter == 3)
        {
            iter=userunordered_set.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_unordered_set(userunordered_set, "erase");

    //get_allocator
    int *point = userunordered_set.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        userunordered_set.get_allocator().construct(&point[i], array[i]);
        cout<<point[i]<<" ";
        userunordered_set.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //insert
    userunordered_set.insert(15);
    show_unordered_set(userunordered_set, "insert");

    //swap
    std::unordered_set<int> swapunordered_set;
    swapunordered_set.swap(userunordered_set);
    show_unordered_set(swapunordered_set, "swapunordered_set");
}

template<class T>
void show_unordered_set(std::unordered_set<T> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::unordered_set<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

void show_unordered_set_help(void)
{
    string helpstring;

    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("bucket           Returns the bucket number where the element with value k is located..\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the unordered_set, and container size is zero. \n");
    helpstring.append("count            Searches the container for elements equivalent to val and returns the number of matches. \n");   
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("emplace          Insert a new element at position, return newly iterator.\n");
    helpstring.append("emplace_hint     Inserts a new element in the unordered_set, if unique, with a hint on the insertion position.\n");
    helpstring.append("empty            Returns whehter the unordered_set is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the unordered_set container.\n");
    helpstring.append("equal_range      Returns the bounds of a range that includes all the elements in the container that are equivalent to val.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the unordered_set.\n");
    helpstring.append("find             Searches the container for an element equivalent to val and returns an iterator to it if found.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the unordered_set.\n");
    helpstring.append("insert           insert new elements before the element at the unordered_set position.\n");
    helpstring.append("key_comp         Returns a copy of the comparison object used by the container to compare keys.\n");
    helpstring.append("lower_bound      Returns an iterator pointing to the first element in the container whose key is not considered to go before k.\n");   
    helpstring.append("max_size         Returns the maximum number of elements that the unordered_set can hold.\n");
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
        show_unordered_set_help();
    else
        unordered_set_study_handle();

    return 0;
} 