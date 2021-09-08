/************************************************************************************
 * std::list
 * reference:http://www.cplusplus.com/reference/list/list/
 * Lists are sequence containers that allow constant time insert 
 * and erase operations anywhere within the sequence, 
 * and iteration in both directions.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studylist
 * 3.More reference shows in function show_list_help or command "studylist -h"
***********************************************************************************/
#include <list>
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

void show_list_help(void);
template<class T>
void show_list(std::list<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

template<class T>
bool comparsion(T first, T second){
    return first<second;
}

void list_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::list<int> intv(array, array+5);
    std::list<int> userlist;
    std::list<string> listlist{"0x00", "0x01", "0x02"}; //list initialize
    
    show_list(listlist, "listlist");
    //clear, size, max_size
    listlist.clear();
    cout<<"size:"<<listlist.size()<<endl;
    cout<<"max_size:"<<listlist.max_size()<<endl;

    show_list(userlist, "userlist");

    //assign
    userlist.assign(intv.begin(), intv.end());
    show_list(userlist, "assign", SHOW_MODE_ENUM_TYPE_AUTO);

    //back
    int &end = userlist.back();
    end = 5;
    show_list(userlist, "back");

    //resize, empty, max_size
    userlist.resize(3);
    show_list(userlist, "resize");
    cout<<"empty:"<<userlist.empty()<<endl;
    cout<<"max_size:"<<userlist.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::list<int>::iterator iterbegin = userlist.begin();
    std::list<int>::iterator iterend = userlist.end();
    std::list<int>::const_iterator iterconstbegin = userlist.cbegin();
    std::list<int>::const_iterator iterconstend = userlist.cend();
    std::list<int>::reverse_iterator iterrbegin = userlist.rbegin();
    std::list<int>::reverse_iterator iterrend = userlist.rend();
    std::list<int>::const_reverse_iterator iterconstrbegin = userlist.crbegin();
    std::list<int>::const_reverse_iterator iterconstrend = userlist.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(--iterend)<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(--iterconstend)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(--iterrend)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(--iterconstrend)<<" "<<endl;

    //emplace, emplace_back, emplace_front
    auto it = userlist.emplace(userlist.begin(), 8);
    show_list(userlist, "emplace");
    cout<<"empalce iterator:"<<*it<<endl;
    userlist.emplace_back(9);
    show_list(userlist, "emplace_back");
    userlist.emplace_front(1);
    show_list(userlist, "emplace_front");

    //erase
    auto iter=userlist.begin();
    while(iter!=userlist.end())
    {
        if(*iter == 3)
        {
            iter=userlist.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_list(userlist, "erase");

    //front
    int &front = userlist.front();
    front = 6;
    show_list(userlist, "front");

    //get_allocator
    int *point = userlist.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        userlist.get_allocator().construct(&point[i], array[i]);
        cout<<point[i]<<" ";
        userlist.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //insert
    userlist.insert(userlist.begin(), 2, 5);
    show_list(userlist, "insert");

    //swap, operator=
    std::list<int> swaplist = std::list<int>();
    swaplist.swap(userlist);
    show_list(swaplist, "swaplist");

    //pop_back, pop_front, push_back, push_front
    swaplist.push_back(10);
    show_list(swaplist, "push_back");
    swaplist.push_front(5);
    show_list(swaplist, "push_front");
    swaplist.pop_back();
    show_list(swaplist, "pop_back");
    swaplist.pop_front();
    show_list(swaplist, "pop_front");

    //sort
    swaplist.sort();
    show_list(swaplist, "sort");

    //unique
    swaplist.unique();
    show_list(swaplist, "unique");

    //megre
    std::list<int> megrelist = {4, 5, 3, 1, 2};
    megrelist.sort();
    swaplist.merge(megrelist, comparsion<int>);
    show_list(swaplist, "megre");
    cout<<"empty: "<<megrelist.empty()<<endl;

    //remove, remove_if
    swaplist.remove(2);
    show_list(swaplist, "remove");
    swaplist.remove_if([](const int& value){
        return value>7;
    });
    show_list(swaplist, "remove_if");

    //reverse   
    swaplist.reverse();
    show_list(swaplist, "reverse");

    //splice
    megrelist.assign(intv.begin(), intv.end());
    swaplist.splice(++swaplist.begin(), megrelist);
    show_list(swaplist, "splice");
}

template<class T>
void show_list(std::list<T> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::list<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

void show_list_help(void)
{
    string helpstring;

    helpstring.append("assign           Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("back             Returns a reference to the last element.\n");
    helpstring.append("begin            Returns a iterator pointing to the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element.\n");
    helpstring.append("clear            Removes all element from the list, and container size is zero.\n");
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("emplace          Insert a new element at position, return newly iterator.\n");
    helpstring.append("emplace_back     Insert new element at the end of the list, no return.\n");
    helpstring.append("emplace_front    Insert new element at the end of the list, no return.\n");
    helpstring.append("empty            Returns whehter the list is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the list container.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the list.\n");
    helpstring.append("front            Returns a refrence to the first element in the list.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the list.\n");
    helpstring.append("insert           insert new elements before the element at the set position.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the list can hold.\n");
    helpstring.append("merge            Merges x into the list by transferring all of its elements at their respective ordered positions into the container.\n");
    helpstring.append("operator=        Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("pop_back         Remvoes the last element, not return.\n");
    helpstring.append("pop_front        Remvoes the first element, not return.\n");
    helpstring.append("push_back        Adds a new element(copied or moved) at the end of the deque.\n");
    helpstring.append("push_front       Adds a new element(copied or moved) at the beginning of the deque.\n");
    helpstring.append("rbegin           Returns a reverse_iterator pointing to the last element.\n");
    helpstring.append("remove           Removes from the container all the elements that compare equal to val.\n");
    helpstring.append("remove_if        Removes from the container all the elements for which Predicate pred returns true.\n");
    helpstring.append("rend             Returns a reverse_iterator pointing to the theoretical element preceding the first element.\n");  
    helpstring.append("resize           Resizes the container so that it contains n elements(smaller will reduce element).\n");
    helpstring.append("reverse          Reverses the order of the elements in the list container.\n");  
    helpstring.append("size             Returns number of elements in the container.\n");    
    helpstring.append("sort             Sorts the elements in the list, altering their position within the container.\n");       
    helpstring.append("splice           Transfers elements from x into the container, inserting them at position.\n");       
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
        show_list_help();
    else
        list_study_handle();

    return 0;
} 