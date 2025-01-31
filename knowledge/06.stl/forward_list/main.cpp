/*
std::forward_list 它代表单向链表（singly linked list）。
单向链表是一种线性数据结构，其中每个元素（节点）包含一个值和指向下一个元素的指针

构造函数
std::forward_list<T>()：默认构造函数，创建一个空的单向链表。
std::forward_list<T>(size_type count, const T& value)：创建一个包含 count 个值为 value 的元素的单向链表。
std::forward_list<T>(std::initializer_list<T> ilist)：使用初始化列表创建单向链表

成员函数
assign: 将链表中的元素替换为 count 个值为 value 的元素
before_begin: 返回一个指向链表头部之前的位置的迭代器。
begin: 返回一个指向链表头部的迭代器。
cbefore_begin: 返回一个指向链表头部之前的位置的常量迭代器。
cbegin: 返回一个指向链表头部的常量迭代器。
cend: 返回一个指向链表尾部之后的位置的常量迭代器。
clear: 清空链表中的所有元素。
emplace_after: 在指定位置之后插入一个新元素。
emplace_front: 在链表头部插入一个新元素。
empty: 检查链表是否为空。
end: 返回一个指向链表尾部之后的位置的迭代器。
erase_after: 删除指定位置之后的元素。
front: 返回链表头部的元素的引用。
get_allocator: 返回用于分配内存的分配器对象。
insert_after: 在指定位置之后插入一个或多个元素。
max_size: 返回链表可以容纳的最大元素数量。
merge: 合并两个有序链表。
pop_front: 移除链表头部的元素。
push_front: 在链表头部插入一个元素。
remove: 移除链表中所有值为 value 的元素。
remove_if: 移除链表中满足特定条件的元素。
resize: 改变链表的大小。
reverse: 反转链表中的元素顺序。
sort: 对链表中的元素进行排序。
splice_after: 将一个链表中的元素插入到另一个链表的指定位置之后。
swap: 交换两个链表的内容。
unique: 移除链表中连续的重复元素。
*/
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

template<typename T>
bool comparsion(T first, T second){
    return first<second;
}

template<typename T>
void show_forward_list(std::forward_list<T> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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


int main(int argc, char* argv[])
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

    return 0;
} 