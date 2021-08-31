/************************************************************************************
 * std::array
 * http://www.cplusplus.com/reference/array/array/
 * Arrays are fixed-size sequence containers.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlarray
 * 3.More reference shows in function show_array_help or command "studyarray -h"
***********************************************************************************/
#include <array>
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

void show_array_help(void);
template<class T, size_t N>
void show_array(std::array<T, N> value, string qstring="", SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA);

void array_study_handle(void)
{
    std::array<int, 5> userarray = {1, 2, 3, 4, 5};

    show_array(userarray, "userarray:");
    
    //at
    userarray.at(0) = 6;
    show_array(userarray, "userarray:");
    
    //begin, end, rbegin, rend, cbegin, cend, crbegin, crend
    std::array<int, 5>::iterator itbegin = userarray.begin();
    std::array<int, 5>::iterator itend = userarray.end();
    std::array<int, 5>::reverse_iterator itrbegin = userarray.rbegin();
    std::array<int, 5>::reverse_iterator itrend = userarray.rend();
    std::array<int, 5>::const_iterator itconstbegin = userarray.cbegin();
    std::array<int, 5>::const_iterator itconstend = userarray.cend();
    std::array<int, 5>::const_reverse_iterator itconstrbegin = userarray.crbegin();
    std::array<int, 5>::const_reverse_iterator itconstrend = userarray.crend();
    cout<<"begin:"<<*itbegin<<" "<<"end:"<<*(itend-1)<<endl;
    cout<<"rbegin:"<<*itrbegin<<" "<<"rend:"<<*(itrend-1)<<endl;
    cout<<"cbegin:"<<*itconstbegin<<" "<<"cend:"<<*(itconstend-1)<<endl;
    cout<<"crbegin:"<<*itconstrbegin<<" "<<"crend:"<<*(itconstrend-1)<<endl;

    //empty, max_size
    cout<<"empty"<<userarray.empty()<<endl;
    cout<<"max_size"<<userarray.max_size()<<endl;

    //front
    userarray.fill(10);
    show_array(userarray, "fill");

    //data
    std::array<string, 2> userstring = {"0x01", "0x02"};
    string *point = userstring.data();
    *(point + 1) = "0x03";
    show_array(userstring, "userstring");

    //back, front
    string &front = userstring.front();
    front = "0x2A";
    show_array(userstring, "front");
    string &back = userstring.back();
    back = "0x15";
    show_array(userstring, "back");

    //swap
    std::array<string, 2> userswapstring;
    userswapstring.swap(userstring);
    show_array(userswapstring, "swap");
}

template<class T, size_t N>
void show_array(std::array<T, N> value, string qstring, SHOW_MODE_ENUM mode)
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
        for(typename std::array<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
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

void show_array_help(void)
{
    string helpstring;

    helpstring.append("at               Returns a reference to the element at position n.\n");
    helpstring.append("back             Returns a reference to the last element.\n");
    helpstring.append("begin            Returns an iterator pointing to the first element.\n");
    helpstring.append("cbegin           Returns a const_iterator pointing to the first element.\n");
    helpstring.append("cend             Returns a const_iterator pointing to the past-the-end element. \n");
    helpstring.append("crbegin          Returns a const_reverse_iterator pointing to the last element.\n");
    helpstring.append("crend            Returns a const_reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("data             Returns a pointer to the fisrt element in array.\n");
    helpstring.append("empty            Returns whehter the array is empty.\n");
    helpstring.append("end              Returns an iterator pointing to the past-the-end element.\n");
    helpstring.append("fill             Sets val as the value for all the elements in the array object.\n");
    helpstring.append("front            Returns a refrence to the first element in the array.\n");
    helpstring.append("max_size         Returns the maximum number of elements that the array can hold.\n");
    helpstring.append("operator[]       Returns a reference to the element at position n in the array.\n");
    helpstring.append("rbegin           Returns a reverse_iterator pointing to the last element.\n");
    helpstring.append("rend             Returns a reverse_iterator pointing to the theoretical element preceding the first element.\n");
    helpstring.append("resize           Resizes the container so that it contains n elements(smaller will reduce element).\n");
    helpstring.append("size             Returns the number of elements in the array container.\n");    
    helpstring.append("swap             Exchange the content of the array by the other content.\n");

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
        show_array_help();
    else
        array_study_handle();

    return 0;
} 