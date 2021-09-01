/************************************************************************************
 * std::deque
 * reference:http://www.cplusplus.com/reference/deque/deque/
 * deque is an irregular acronym of double-ended queue.
 * 1. Need compile with command -std=c++11
 * 2. Generate excutable file studystldeque
 * 3. More reference shows in function show_deque_help or command "studydeque -h"
***********************************************************************************/
#include <deque>
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
    SHOW_MODE_ENUM_TYPE_OPERATOR
}SHOW_MODE_ENUM;

void show_deque_help(void);
template<class T>
void show_deque(std::deque<T> value, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void deque_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::deque<int> intv(array, array+5);
    std::deque<int> userdeque(intv);
    std::deque<string> listdeque{"0x00", "0x01", "0x02"}; //list initialize
    
    show_deque(listdeque, "listdeque");
    
    //clear, size, max_size
    listdeque.clear();
    cout<<"size:"<<listdeque.size()<<endl;
    cout<<"max_size:"<<listdeque.max_size()<<endl;
    cout<<"empty:"<<listdeque.empty()<<endl;

    show_deque(userdeque, "userdeque");

    //assign
    userdeque.assign(userdeque.begin(), userdeque.end()-1);
    show_deque(userdeque, "assign", SHOW_MODE_ENUM_TYPE_AUTO);

    //at
    int &p = userdeque.at(2);
    p = 10;
    show_deque(userdeque, "at", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //back
    int &end = userdeque.back();
    end = 5;
    show_deque(userdeque, "back", SHOW_MODE_ENUM_TYPE_OPERATOR);

    //capacity, reserve, resize, max_size
    userdeque.resize(3);
    userdeque.shrink_to_fit();
    cout<<"max_size:"<<userdeque.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::deque<int>::iterator iterbegin = userdeque.begin();
    std::deque<int>::iterator iterend = userdeque.end();
    std::deque<int>::const_iterator iterconstbegin = userdeque.cbegin();
    std::deque<int>::const_iterator iterconstend = userdeque.cend();
    std::deque<int>::reverse_iterator iterrbegin = userdeque.rbegin();
    std::deque<int>::reverse_iterator iterrend = userdeque.rend();
    std::deque<int>::const_reverse_iterator iterconstrbegin = userdeque.crbegin();
    std::deque<int>::const_reverse_iterator iterconstrend = userdeque.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*iterend<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(iterconstend-1)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(iterrend-1)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(iterconstrend-1)<<" "<<endl;


    //emplace, emplace_back, emplace_front
    auto it = userdeque.emplace(userdeque.begin()+1, 8);
    show_deque(userdeque, "emplace");
    cout<<"empalce iterator:"<<*it<<endl;
    userdeque.emplace_back(9);
    show_deque(userdeque, "emplace_back");
    userdeque.emplace_front(5);
    show_deque(userdeque, "emplace_front");

    //erase
    auto iter=userdeque.begin();
    while(iter!=userdeque.end())
    {
        if(*iter == 3)
        {
            iter=userdeque.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_deque(userdeque, "erase");

    //front
    int &front = userdeque.front();
    front = 6;
    show_deque(userdeque, "front");

    //get_allocator
    int *point = userdeque.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        userdeque.get_allocator().construct(&point[i], userdeque[i]);
        cout<<point[i]<<" ";
        userdeque.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //insert
    userdeque.insert(userdeque.begin(), 2, 5);
    show_deque(userdeque, "insert");

    //swap, operator=
    std::deque<int> swapdeque = std::deque<int>();
    swapdeque.swap(userdeque);
    show_deque(swapdeque, "swapdeque");

    //pop_back, pop_front, push_back, push_front
    swapdeque.push_back(6);
    show_deque(swapdeque, "push_back");
    swapdeque.pop_back();
    show_deque(swapdeque, "pop_back");
    swapdeque.push_front(6);
    show_deque(swapdeque, "push_front");
    swapdeque.pop_front();
    show_deque(swapdeque, "pop_front");
}

template<class T>
void show_deque(std::deque<T> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::deque<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    //operator[], size
    else if(mode == SHOW_MODE_ENUM_TYPE_OPERATOR)
    {
        for(int index=0; index<value.size(); index++)
            cout<<value[index]<<" ";
    }
    cout<<endl;
}

void show_deque_help(void)
{
    string helpstring;

    helpstring.append("assign           Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("at               Returns a reference to the element at position n.\n");
    helpstring.append("back             Returns a reference to the last element.\n");
    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element.\n");
    helpstring.append("clear            Removes all element from the deque, and container size is zero. \n");
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("emplace          Insert a new element at position, return newly iterator.\n");
    helpstring.append("emplace_back     Insert new element at the end of the deque, no return.\n");
    helpstring.append("emplace_front    Insert new element at the beginning of the deque, no return.\n");
    helpstring.append("empty            Returns whehter the deque is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the deque container.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the deque.\n");
    helpstring.append("front            Returns a refrence to the first element in the deque.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the vector.\n");
    helpstring.append("insert           insert new elements before the element at the set position.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the deque can hold.\n");
    helpstring.append("operator=        Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("operator[]       Returns a reference to the element at position n in the deque.\n");
    helpstring.append("pop_back         Remvoes the last element, not return.\n");
    helpstring.append("pop_front        Remvoes the first element, not return.\n");
    helpstring.append("push_back        Adds a new element(copied or moved) at the end of the deque.\n");
    helpstring.append("push_front       Adds a new element(copied or moved) at the beginning of the deque.\n");
    helpstring.append("rbegin           Returns a reverse_iterator pointing to the last element.\n");
    helpstring.append("rend             Returns a reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("resize           Resizes the container so that it contains n elements(smaller will reduce element).\n");
    helpstring.append("shrink_to_fit    Request the container to reduce its capacity to fit its size\n");
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
        show_deque_help();
    else
        deque_study_handle();

    return 0;
} 
