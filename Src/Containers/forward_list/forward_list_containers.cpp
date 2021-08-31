/************************************************************************************
 * std::forward_list
 * reference:http://www.cplusplus.com/reference/forward_list/forward_list/
 * Forward lists are sequence containers that allow constant time insert 
 * and erase operations anywhere within the sequence.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlforward_list
 * 3.More reference shows in function show_forward_list_help or command "studyforward_list -h"
***********************************************************************************/
#include <forward_list>
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

void show_forward_list_help(void);
template<class T>
void show_forward_list(std::forward_list<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

template<class T>
bool comparsion(T first, T second){
    return first<second;
}

void forward_list_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::forward_list<int> intv(array, array+5);
    std::forward_list<int> userforward_list;
    std::forward_list<string> forward_listforward_list{"0x00", "0x01", "0x02"}; //forward_list initialize
    
    show_forward_list(forward_listforward_list, "forward_listforward_list");
    //clear, max_size
    forward_listforward_list.clear();
    cout<<"max_size:"<<forward_listforward_list.max_size()<<endl;

    show_forward_list(userforward_list, "userforward_list");

    //assign
    userforward_list.assign(intv.begin(), intv.end());
    show_forward_list(userforward_list, "assign", SHOW_MODE_ENUM_TYPE_AUTO);

    //resize, empty, max_size
    userforward_list.resize(3);
    show_forward_list(userforward_list, "resize");
    cout<<"empty:"<<userforward_list.empty()<<endl;
    cout<<"max_size:"<<userforward_list.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    std::forward_list<int>::iterator iterbegin = userforward_list.begin();
    std::forward_list<int>::iterator iterend = userforward_list.end();
    std::forward_list<int>::const_iterator iterconstbegin = userforward_list.cbegin();
    std::forward_list<int>::const_iterator iterconstend = userforward_list.cend();
    cout<<"begin:"<<*iterbegin<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<endl;

    //emplace_after, emplace_back, emplace_front
    auto it = userforward_list.emplace_after(userforward_list.begin(), 6);
    show_forward_list(userforward_list, "emplace_after");
    cout<<"empalce iterator:"<<*it<<endl;
    userforward_list.emplace_front(1);
    show_forward_list(userforward_list, "emplace_front");

    //erase_after
    userforward_list.erase_after(++userforward_list.begin());
    show_forward_list(userforward_list, "erase_after");

    //front
    int &front = userforward_list.front();
    front = 6;
    show_forward_list(userforward_list, "front");

    //get_allocator
    int *point = userforward_list.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        userforward_list.get_allocator().construct(&point[i], array[i]);
        cout<<point[i]<<" ";
        userforward_list.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //insert, before_begin, cbefore_begin
    userforward_list.insert_after(userforward_list.before_begin(), 2, 5);
    show_forward_list(userforward_list, "insert_after");
    userforward_list.insert_after(userforward_list.cbefore_begin(), 2, 1);
    show_forward_list(userforward_list, "insert_after");

    //swap, operator=
    std::forward_list<int> swapforward_list = std::forward_list<int>();
    swapforward_list.swap(userforward_list);
    show_forward_list(swapforward_list, "swapforward_list");

    //pop_front, push_front
    swapforward_list.push_front(5);
    show_forward_list(swapforward_list, "push_front");
    swapforward_list.pop_front();
    show_forward_list(swapforward_list, "pop_front");

    //sort
    swapforward_list.sort();
    show_forward_list(swapforward_list, "sort");

    //unique
    swapforward_list.unique();
    show_forward_list(swapforward_list, "unique");

    //megre
    std::forward_list<int> megreforward_list = {4, 5, 3, 1, 2};
    megreforward_list.sort();
    swapforward_list.merge(megreforward_list, comparsion<int>);
    show_forward_list(swapforward_list, "megre");
    cout<<"empty: "<<megreforward_list.empty()<<endl;

    //remove, remove_if
    swapforward_list.remove(2);
    show_forward_list(swapforward_list, "remove");
    swapforward_list.remove_if([](const int& value){
        return value>7;
    });
    show_forward_list(swapforward_list, "remove_if");

    //reverse   
    swapforward_list.reverse();
    show_forward_list(swapforward_list, "reverse");

    //splice
    megreforward_list.assign(intv.begin(), intv.end());
    swapforward_list.splice_after(++swapforward_list.begin(), megreforward_list);
    show_forward_list(swapforward_list, "splice");
}

template<class T>
void show_forward_list(std::forward_list<T> value, string qstring, SHOW_MODE_ENUM mode)
{
    //empty, size
    if(!qstring.empty())
    {
        for(auto index=qstring.size(); index<18; index++)
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
        for(typename std::forward_list<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

void show_forward_list_help(void)
{
    string helpstring;

    helpstring.append("assign           Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("before_begin     Returns an iterator pointing to the position before the first element.\n");
    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("cbefore_begin    Returns a const_iterator pointing to the position before the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the forward_list, and container size is zero. \n");
    helpstring.append("emplace_after    The container is extended by inserting a new element after the element at position.\n");
    helpstring.append("emplace_front    Inserts a new element at the beginning of the forward_list, right before its current first element.\n");
    helpstring.append("empty            Returns whehter the forward_list is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the forward_list container.\n");
    helpstring.append("erase_after      Removes from the forward_list container either a single element (the one after position) or a range of elements ((position,last)).\n");
    helpstring.append("front            Returns a refrence to the first element in the forward_list.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the forward_list.\n");
    helpstring.append("insert_after     The container is extended by inserting new elements after the element at position.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the forward_list can hold.\n");
    helpstring.append("merge            Merges x into the forward_list by transferring all of its elements at their respective ordered positions into the container.\n");
    helpstring.append("operator=        Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("pop_front        Remvoes the first element, not return.\n");
    helpstring.append("push_front       Adds a new element(copied or moved) at the beginning of the deque.\n");
    helpstring.append("remove           Removes from the container all the elements that compare equal to val.\n");
    helpstring.append("remove_if        Removes from the container all the elements for which Predicate pred returns true.\n");
    helpstring.append("resize           Resizes the container so that it contains n elements(smaller will reduce element).\n");
    helpstring.append("reverse          Reverses the order of the elements in the forward_list container.\n");  
    helpstring.append("sort             Sorts the elements in the forward_list, altering their position within the container.\n");       
    helpstring.append("splice_after     Transfers elements from fwdlst into the container inserting them after the element pointed by position.\n");       
    helpstring.append("swap             Exchange the content of the containder.\n");
    helpstring.append("unique           The version with no parameters (1), removes all but the first element from every consecutive group of equal elements in the container.\n");
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
        show_forward_list_help();
    else
        forward_list_study_handle();

    return 0;
} 