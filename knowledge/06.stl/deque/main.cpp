/*
std::deque 它代表双端队列（double-ended queue）。双端队列是一种序列容器，允许在两端（前端和后端）快速插入和删除元素

构造函数：
std::deque<T> deque;：创建一个空的双端队列。
std::deque<T> deque(size);：创建一个指定大小的双端队列。
std::deque<T> deque(size, value);：创建一个指定大小并初始化所有元素的双端队列。

成员函数：
at：返回指定位置的元素引用。
assign：将指定范围内的元素赋值给双端队列。
operator[]：返回指定位置的元素引用。
front：返回第一个元素的引用。
back：返回最后一个元素的引用。
begin：返回指向第一个元素的迭代器。
end：返回指向最后一个元素的下一个位置的迭代器。
empty：检查双端队列是否为空。
size：返回双端队列中元素的个数。
max_size：返回双端队列可以容纳的最大元素个数。
clear：清空双端队列中的所有元素。
insert：在指定位置插入一个元素。
erase：删除指定位置的元素。
push_back：在双端队列的末尾添加一个元素。
pop_back：删除双端队列末尾的元素。
push_front：在双端队列的开头添加一个元素。
pop_front：删除双端队列开头的元素。
resize：调整双端队列的大小。
swap：交换两个双端队列的内容。
*/
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

template<typename T>
void show_deque(std::deque<T> value, string qstring, SHOW_MODE_ENUM mode=SHOW_MODE_ENUM_LAMBDA)
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

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::deque<int> intv(array, array+5);
    std::deque<int> userdeque(intv);
    std::deque<string> listdeque{"0x00", "0x01", "0x02"}; //list initialize
    
    show_deque(listdeque, "listdeque");
    
    // clear, size, max_size
    listdeque.clear();
    cout<<"size:"<<listdeque.size()<<endl;
    cout<<"max_size:"<<listdeque.max_size()<<endl;
    cout<<"empty:"<<listdeque.empty()<<endl;

    show_deque(userdeque, "userdeque");

    // assign，operator[]
    userdeque.assign(userdeque.begin(), userdeque.end()-1);
    show_deque(userdeque, "assign", SHOW_MODE_ENUM_TYPE_AUTO);
    std::cout << "operator[]:"<<userdeque[0]<<endl;

    // at
    int &p = userdeque.at(2);
    p = 10;
    show_deque(userdeque, "at", SHOW_MODE_ENUM_TYPE_ITERATOR);

    // back
    int &end = userdeque.back();
    end = 5;
    show_deque(userdeque, "back", SHOW_MODE_ENUM_TYPE_OPERATOR);

    // capacity, reserve, resize, max_size
    userdeque.resize(3);
    userdeque.shrink_to_fit();
    cout<<"max_size:"<<userdeque.max_size()<<endl;

    // begin,  end,  cbegin,  cend
    // rbegin, rend, rcbegin, rcend
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

    // emplace, emplace_back, emplace_front
    auto it = userdeque.emplace(userdeque.begin()+1, 8);
    show_deque(userdeque, "emplace");
    cout<<"empalce iterator:"<<*it<<endl;
    userdeque.emplace_back(9);
    show_deque(userdeque, "emplace_back");
    userdeque.emplace_front(5);
    show_deque(userdeque, "emplace_front");

    // erase
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

    // front
    int &front = userdeque.front();
    front = 6;
    show_deque(userdeque, "front");

    // insert
    userdeque.insert(userdeque.begin(), 2, 5);
    show_deque(userdeque, "insert");

    // swap, operator=
    std::deque<int> swapdeque = std::deque<int>();
    swapdeque.swap(userdeque);
    show_deque(swapdeque, "swapdeque");

    // pop_back, pop_front, push_back, push_front
    swapdeque.push_back(6);
    show_deque(swapdeque, "push_back");
    swapdeque.pop_back();
    show_deque(swapdeque, "pop_back");
    swapdeque.push_front(6);
    show_deque(swapdeque, "push_front");
    swapdeque.pop_front();
    show_deque(swapdeque, "pop_front");

    // shrink_to_fit
    swapdeque.shrink_to_fit();
    
    return 0;
} 
