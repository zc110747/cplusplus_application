/************************************************************************************
 * std::vector
 * reference:http://www.cplusplus.com/reference/vector/vector/
 * Vetors are sequence containers representing arrays that can change in size.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studyvector
 * 3.More reference shows in function show_vector_help or command "studyvector -h"
***********************************************************************************/
#include <vector>
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

void show_vector_help(void);
template<class T>
void show_vector(std::vector<T>, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void vector_study_handle(void)
{
    int array[5] = {8, 2, 1, 3, 5};
    std::vector<int> intv(array, array+5);
    std::vector<int> uservector(intv);
    std::vector<string> listvector;
    
    //operator=
    listvector = std::vector<string>({"0x00", "0x01", "0x02"}); //list initialize
    
    show_vector(listvector, "listvector");
    //clear, capacity, size, max_size
    listvector.clear();
    cout<<"capacity:"<<listvector.capacity()<<" ";
    cout<<"size:"<<listvector.size()<<endl;
    cout<<"max_size:"<<listvector.max_size()<<endl;

    show_vector(uservector, "uservector");

    //assign
    uservector.assign(uservector.begin(), uservector.end()-1);
    show_vector(uservector, "assign", SHOW_MODE_ENUM_TYPE_AUTO);

    //at
    int &p = uservector.at(2);
    p = 10;
    show_vector(uservector, "at", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //back
    int &end = uservector.back();
    end = 5;
    show_vector(uservector, "back", SHOW_MODE_ENUM_TYPE_OPERATOR);

    //capacity, reserve, resize, max_size
    cout<<"capactity:"<<uservector.capacity()<<endl;
    uservector.reserve(7);
    cout<<"reserve capactity:"<<uservector.capacity()<<endl;
    uservector.resize(3);
    uservector.shrink_to_fit();
    cout<<"fit capactity:"<<uservector.capacity()<<endl;
    cout<<"max_size:"<<uservector.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::vector<int>::iterator iterbegin = uservector.begin();
    std::vector<int>::iterator iterend = uservector.end();
    std::vector<int>::const_iterator iterconstbegin = uservector.cbegin();
    std::vector<int>::const_iterator iterconstend = uservector.cend();
    std::vector<int>::reverse_iterator iterrbegin = uservector.rbegin();
    std::vector<int>::reverse_iterator iterrend = uservector.rend();
    std::vector<int>::const_reverse_iterator iterconstrbegin = uservector.crbegin();
    std::vector<int>::const_reverse_iterator iterconstrend = uservector.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(iterend-1)<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(iterconstend-1)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(iterrend-1)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(iterconstrend-1)<<" "<<endl;

    //data
    int *ptr = uservector.data();
    ptr[0] = 4;
    ptr[2] = 3;
    show_vector(uservector, "data");

    //emplace, emplace_back
    auto it = uservector.emplace(uservector.begin()+1, 8);
    show_vector(uservector, "emplace");
    cout<<"empalce iterator:"<<*it<<endl;
    uservector.emplace_back(9);
    show_vector(uservector, "emplace_back");

    //erase
    auto iter=uservector.begin();
    while(iter!=uservector.end())
    {
        if(*iter == 3)
        {
            iter=uservector.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_vector(uservector, "erase");

    //front
    int &front = uservector.front();
    front = 6;
    show_vector(uservector, "front");

    //get_allocator
    int *point = uservector.get_allocator().allocate(5);
    cout<<"get_allocator:";
    for(int i=0; i<5; i++)
    {
        uservector.get_allocator().construct(&point[i], uservector[i]);
        cout<<point[i]<<" ";
        uservector.get_allocator().destroy(&point[i]);
    }
    cout<<endl;

    //insert
    uservector.insert(uservector.begin(), 2, 5);
    show_vector(uservector, "insert");

    //swap, operator=
    std::vector<int> swapvector = std::vector<int>();
    swapvector.swap(uservector);
    show_vector(swapvector, "swapvector");

    //pop_back, push_back
    swapvector.push_back(10);
    show_vector(swapvector, "push_back");
    swapvector.pop_back();
    show_vector(swapvector, "pop_back");
}

template<class T>
void show_vector(std::vector<T> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::vector<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
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

void show_vector_help(void)
{
    string helpstring;

    helpstring.append("assign           Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("at               Returns a reference to the element at position n.\n");
    helpstring.append("back             Returns a reference to the last element.\n");
    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("capacity         Returns the size of storage space currently allocted for the vector.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("clear            Removes all element from the vector, and container size is zero. \n");
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("data             Returns a direct point to the memory array used internally by vector.\n");
    helpstring.append("emplace          Insert a new element at position, return newly iterator.\n");
    helpstring.append("emplace_back     Insert new element at the end of the vector, no return.\n");
    helpstring.append("empty            Returns whehter the vector is empty.\n");
    helpstring.append("end              Returns an iterator referring to the past-the-end element in the vector container.\n");
    helpstring.append("erase            Removes a single element or a range of elements in the vector.\n");
    helpstring.append("front            Returns a refrence to the first element in the vector.\n");
    helpstring.append("get_allocator    Returns a copy of the allocator object associated with the vector.\n");
    helpstring.append("insert           insert new elements before the element at the set position.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the vector can hold.\n");
    helpstring.append("operator=        Assgins new container, replacing current contents and modifying size.\n");
    helpstring.append("operator[]       Returns a reference to the element at position n in the vector.\n");
    helpstring.append("pop_back         Remvoes the last element, not return.\n");
    helpstring.append("push_back        Adds a new element(copied or moved) at the end of the vector.\n");
    helpstring.append("rbegin           Returns a reverse_iterator pointing to the last element.\n");
    helpstring.append("rend             Returns a reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("reserve          Request the vector capacity be at least enough to contain n elements(smaller will not effect).\n");    
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
        show_vector_help();
    else
        vector_study_handle();

    return 0;
} 