/*
std::multiset std::multiset 是 C++ 标准库中的一个关联容器，它类似于 std::set，但允许存储多个具有相同键的元素

主要特点:
自动排序：元素在插入时会自动按照排序准则进行排序。
允许重复元素：与 std::set 不同，std::multiset 允许存储多个具有相同键的元素。
高效查找：由于元素是有序的，查找操作的时间复杂度为对数级别。

构造函数：
std::multiset<T>：创建一个空的 std::multiset。
std::multiset<T>(const std::multiset<T>& other)：创建一个与另一个 std::multiset 相同的 std::multiset。
std::multiset<T>(std::multiset<T>&& other)：创建一个从另一个 std::multiset 移动的 std::multiset。
std::multiset<T>(std::initializer_list<T> ilist)：创建一个包含初始值列表中元素的 std::multiset。

成员函数：
begin: 返回指向容器中第一个元素的迭代器。
cbegin: 返回指向容器中第一个元素的常量迭代器。
cend: 返回指向容器中最后一个元素之后位置的常量迭代器。
clear: 清除容器中的所有元素。
contains(c++20): 检查容器中是否包含特定元素。
count: 返回容器中具有特定键的元素数量。
crbegin: 返回指向容器中第一个元素的常量反向迭代器。
crend: 返回指向容器中最后一个元素之后位置的常量反向迭代器。
emplace: 在容器中直接构造元素。
emplace_hint: 在容器中的指定位置直接构造元素。
empty: 检查容器是否为空。
end: 返回指向容器中最后一个元素之后位置的迭代器。
equal_range: 返回一个范围，其中包含具有特定键的所有元素。
erase: 从容器中删除一个或多个元素。
find: 在容器中查找具有特定键的元素。
get_allocator: 返回用于构造容器的分配器对象的副本。
insert: 在容器中插入一个或多个元素。
key_comp: 返回用于比较键的函数对象。
lower_bound: 返回指向容器中第一个不小于给定键的元素的迭代器。
max_size: 返回容器可容纳的最大元素数量。
rbegin: 返回指向容器中第一个元素的反向迭代器。
rend: 返回指向容器中最后一个元素之后位置的反向迭代器。
size: 返回容器中的元素数量。
swap: 交换两个 std::multiset 的内容。
upper_bound: 返回指向容器中第一个大于给定键的元素的迭代器。
value_comp: 返回用于比较值的函数对象。
*/
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

template<typename T>
void show_multiset(std::multiset<T> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        for(typename std::multiset<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::multiset<int> intv(array, array+5);
    std::multiset<int> usermultiset(intv);
    std::multiset<string> listmultiset;
    
    //operator=
    listmultiset = std::multiset<string>({"0x00", "0x01", "0x02"});
    
    show_multiset(listmultiset, "listmultiset");
    //count, clear, size,empty
    cout<<"count"<<listmultiset.count("0x01");
    listmultiset.clear();
    cout<<"size:"<<listmultiset.size()<<endl;
    cout<<"empty:"<<listmultiset.empty()<<endl;

    show_multiset(usermultiset, "usermultiset");

    //max_size
    cout<<"max_size:"<<usermultiset.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::multiset<int>::iterator iterbegin = usermultiset.begin();
    std::multiset<int>::iterator iterend = usermultiset.end();
    std::multiset<int>::const_iterator iterconstbegin = usermultiset.cbegin();
    std::multiset<int>::const_iterator iterconstend = usermultiset.cend();
    std::multiset<int>::reverse_iterator iterrbegin = usermultiset.rbegin();
    std::multiset<int>::reverse_iterator iterrend = usermultiset.rend();
    std::multiset<int>::const_reverse_iterator iterconstrbegin = usermultiset.crbegin();
    std::multiset<int>::const_reverse_iterator iterconstrend = usermultiset.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(--iterend)<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(--iterconstend)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(--iterrend)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(--iterconstrend)<<" "<<endl;

    //emplace, find, emplace_hint
    auto it = usermultiset.emplace(15);
    show_multiset(usermultiset, "emplace");
    cout<<"val:"<<*it<<endl;
    auto find = usermultiset.find(8);
    usermultiset.emplace_hint(find, 9);
    show_multiset(usermultiset, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    //equal_range, lower_bound, upper_bound
    std::pair<std::set<int>::const_iterator, std::set<int>::const_iterator> ret;
    ret = usermultiset.equal_range(5);
    if(ret.first != usermultiset.end())
        cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    if(ret.second != usermultiset.end())
        cout<<"equal_range upperbound:"<<*(ret.second)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    std::set<int>::const_iterator iterlower = usermultiset.lower_bound(5);
    std::set<int>::const_iterator iterupper = usermultiset.upper_bound(5);
    if(iterlower != usermultiset.end())
        cout<<"lowerbound:"<<*iterlower<<endl;
    else
        cout<<"invalid lowerbound"<<endl;
    if(iterupper != usermultiset.end())
        cout<<"upperbound:"<<*iterupper<<endl;
    else
        cout<<"invalid upperbound"<<endl;

    //erase
    auto iter=usermultiset.begin();
    while(iter!=usermultiset.end())
    {
        if(*iter == 3)
        {
            iter=usermultiset.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_multiset(usermultiset, "erase");

    //insert
    usermultiset.insert(15);
    show_multiset(usermultiset, "insert");

    //key_comp
    cout<<"key_comp:";
    for(auto &x:usermultiset)
    {
        if(usermultiset.key_comp()(x, 7))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //value_comp
    cout<<"value comp:";
    for(auto &x:usermultiset)
    {
        if(usermultiset.value_comp()(x, 9))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //swap
    std::multiset<int> swapmultiset;
    swapmultiset.swap(usermultiset);
    show_multiset(swapmultiset, "swapmultiset");
    
    return 0;
}
