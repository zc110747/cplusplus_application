/*
std::list
std::list是C++标准库中的一个容器类，它提供了一种双向链表的数据结构。
与std::forward_list不同，std::list允许在链表的任何位置进行插入和删除操作，并且提供了更多的成员函数和算法。

构造函数
std::list<T> list;：创建一个空的std::list对象。
std::list<T> list(size);：创建一个包含size个元素的std::list对象，每个元素都被默认构造。
std::list<T> list(size, value);：创建一个包含size个元素的std::list对象，每个元素都被初始化为value。
std::list<T> list(initializer_list);：使用初始化列表创建一个std::list对象。
std::list<T> list(first, last);：使用迭代器范围[first, last)创建一个std::list对象。

成员函数
assign：将新元素分配给链表。
back：返回链表的最后一个元素的引用。
begin：返回指向链表第一个元素的迭代器。
before_begin：返回指向链表第一个元素之前的位置的迭代器。
cbegin：返回指向链表第一个元素的常量迭代器。
cbefore_begin：返回指向链表第一个元素之前的位置的常量迭代器。
cend：返回指向链表最后一个元素之后的位置的常量迭代器。
clear：移除链表中的所有元素。
emplace：在链表的指定位置插入一个新元素。
emplace_back：在链表的末尾插入一个新元素。
emplace_front：在链表的开头插入一个新元素。
empty：检查链表是否为空。
end：返回指向链表最后一个元素之后的位置的迭代器。
erase：移除链表中的一个或多个元素。
front：返回链表的第一个元素的引用。
get_allocator：返回链表的分配器对象。
insert：在链表的指定位置插入一个或多个元素。
max_size：返回链表的最大可能大小。
merge：合并两个有序链表。
pop_back：移除链表的最后一个元素。
pop_front：移除链表的第一个元素。
push_back：在链表的末尾插入一个新元素。
push_front：在链表的开头插入一个新元素。
rbegin：返回指向链表最后一个元素的反向迭代器。
remove：移除链表中所有等于value的元素。
remove_if：移除链表中满足特定条件的元素。
rend: 返回指向链表第一个元素之前的位置的反向迭代器。
resize：改变链表的大小。
reverse：反转链表的元素顺序。
size: 返回链表的元素个数。
sort：对链表进行排序。
splice：将一个链表的元素转移到另一个链表的指定位置。
swap：交换两个链表的内容。
unique：移除链表中连续的重复元素。
*/
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

template<typename T>
bool comparsion(T first, T second){
    return first<second;
}

template<typename T>
void show_list(std::list<T> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::list<int> intv(array, array+5);
    std::list<int> usr_list;
    std::list<string> str_list{"0x00", "0x01", "0x02"}; //list initialize
    
    show_list(str_list, "str_list");
    //clear, size, max_size
    str_list.clear();
    cout<<"size:"<<str_list.size()<<endl;
    cout<<"max_size:"<<str_list.max_size()<<endl;

    show_list(usr_list, "usr_list");

    //assign
    usr_list.assign(intv.begin(), intv.end());
    show_list(usr_list, "assign", SHOW_MODE_ENUM_TYPE_AUTO);

    //back
    int &end = usr_list.back();
    end = 5;
    show_list(usr_list, "back");

    //resize, empty, max_size
    usr_list.resize(3);
    show_list(usr_list, "resize");
    cout<<"empty:"<<usr_list.empty()<<endl;
    cout<<"max_size:"<<usr_list.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::list<int>::iterator iterbegin = usr_list.begin();
    std::list<int>::iterator iterend = usr_list.end();
    std::list<int>::const_iterator iterconstbegin = usr_list.cbegin();
    std::list<int>::const_iterator iterconstend = usr_list.cend();
    std::list<int>::reverse_iterator iterrbegin = usr_list.rbegin();
    std::list<int>::reverse_iterator iterrend = usr_list.rend();
    std::list<int>::const_reverse_iterator iterconstrbegin = usr_list.crbegin();
    std::list<int>::const_reverse_iterator iterconstrend = usr_list.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(--iterend)<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(--iterconstend)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(--iterrend)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(--iterconstrend)<<" "<<endl;

    //emplace, emplace_back, emplace_front
    auto it = usr_list.emplace(usr_list.begin(), 8);
    show_list(usr_list, "emplace");
    cout<<"empalce iterator:"<<*it<<endl;
    usr_list.emplace_back(9);
    show_list(usr_list, "emplace_back");
    usr_list.emplace_front(1);
    show_list(usr_list, "emplace_front");

    //erase
    auto iter=usr_list.begin();
    while(iter!=usr_list.end())
    {
        if(*iter == 3)
        {
            iter=usr_list.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_list(usr_list, "erase");

    //front
    int &front = usr_list.front();
    front = 6;
    show_list(usr_list, "front");

    //insert
    usr_list.insert(usr_list.begin(), 2, 5);
    show_list(usr_list, "insert");

    //swap, operator=
    std::list<int> swaplist = std::list<int>();
    swaplist.swap(usr_list);
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

    return 0;
} 