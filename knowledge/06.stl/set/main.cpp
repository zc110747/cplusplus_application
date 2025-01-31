/*
std::set 是 C++ 标准库中的一个关联容器，它存储唯一的元素，并按照特定的顺序进行排序。

std::set 具有以下特性：
1. 元素唯一性：std::set 中的每个元素都是唯一的，不允许重复。
2. 自动排序：元素在插入时会自动按照排序准则进行排序。
3. 高效查找：由于元素是有序的，查找操作的时间复杂度为对数级别，即 O(log n)

构造函数：
std::set<T> set; // 创建一个空的 set。
std::set<T> set(initializer_list<T> list); // 使用初始化列表创建 set。
std::set<T> set(const std::set<T>& other); // 复制另一个 set。
std::set<T> set(std::set<T>&& other); // 移动另一个 set。

成员函数：

begin: 返回指向容器中第一个元素的迭代器。
cbegin: 返回指向容器中第一个元素的常量迭代器。
cend: 返回指向容器中最后一个元素之后的位置的常量迭代器。
clear: 清空容器中的所有元素。
contains(c++20): 检查容器中是否包含指定元素。
count: 返回容器中等于给定值的元素数量。
crbegin: 返回指向容器中最后一个元素的常量反向迭代器。
crend: 返回指向容器中第一个元素之前的位置的常量反向迭代器。
emplace: 在容器中就地构造元素。
emplace_hint: 在容器中使用提示位置就地构造元素。
empty: 检查容器是否为空。
end: 返回指向容器中最后一个元素之后的位置的迭代器。
equal_range: 返回一对迭代器，分别表示等于给定值的元素范围。
erase: 从容器中删除指定元素或范围内的元素。
find: 在容器中查找等于给定值的元素。
get_allocator: 获取容器的分配器。
insert: 将元素插入容器中。
key_comp: 返回用于比较键的函数对象。
lower_bound: 返回指向容器中第一个不小于给定值的元素的迭代器。
max_size: 返回容器可容纳的最大元素数量。
rbegin: 返回指向容器中最后一个元素的反向迭代器。
rend: 返回指向容器中第一个元素之前的位置的反向迭代器。
size: 返回容器中元素的数量。
swap: 交换两个容器的内容。
upper_bound: 返回指向容器中第一个大于给定值的元素的迭代器。
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
void show_set(std::set<T> value, string qlabel, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
{
    //empty, size
    if(!qlabel.empty())
    {
        for(auto index=qlabel.size(); index<13; index++)
            qlabel.push_back(' ');
        qlabel += ":";
        cout<<qlabel;
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
        for(typename std::set<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    cout<<endl;
}


int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::set<int> arr_set(array, array+5);
    std::set<int> usr_set(arr_set);
    std::set<string> str_set;
    
    //operator=
    str_set = std::set<string>({"0x00", "0x01", "0x02"});
    show_set(str_set, "str_set");

    //count, clear, size, empty
    cout<<"count"<<str_set.count("0x01")<<" | ";
    str_set.clear();
    cout<<"size:"<<str_set.size()<<endl;
    cout<<"empty:"<<str_set.empty()<<endl;

    show_set(usr_set, "usr_set");

    //max_size
    cout<<"max_size:"<<usr_set.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::set<int>::iterator iterbegin = usr_set.begin();
    std::set<int>::iterator iterend = usr_set.end();
    std::set<int>::const_iterator iterconstbegin = usr_set.cbegin();
    std::set<int>::const_iterator iterconstend = usr_set.cend();
    std::set<int>::reverse_iterator iterrbegin = usr_set.rbegin();
    std::set<int>::reverse_iterator iterrend = usr_set.rend();
    std::set<int>::const_reverse_iterator iterconstrbegin = usr_set.crbegin();
    std::set<int>::const_reverse_iterator iterconstrend = usr_set.crend();
    cout<<"begin:"<<*iterbegin<<" | "<<"end:"<<*(--iterend)<<" | ";
    cout<<"cbegin:"<<*iterconstbegin<<" | "<<"cend:"<<*(--iterconstend)<<" | ";
    cout<<"rbegin:"<<*iterrbegin<<" | "<<"rend:"<<*(--iterrend)<<" | ";
    cout<<"crbegin:"<<*iterconstrbegin<<" | "<<"crend:"<<*(--iterconstrend)<<" | "<<endl;

    //emplace, find, emplace_hint
    auto it = usr_set.emplace(15);
    show_set(usr_set, "emplace");
    cout<<"val:"<<*(it.first)<<","<<"status:"<<it.second<<endl;
    auto find = usr_set.find(8);
    usr_set.emplace_hint(find, 9);
    show_set(usr_set, "emplace_hint");

    //equal_range, lower_bound, upper_bound
    std::pair<std::set<int>::const_iterator, std::set<int>::const_iterator> ret;
    ret = usr_set.equal_range(5);
    if(ret.first != usr_set.end())
        cout<<"equal_range lowerbound:"<<*(ret.first)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;
    if(ret.second != usr_set.end())
        cout<<"equal_range upperbound:"<<*(ret.second)<<endl;
    else
        cout<<"invalid equal_range lowerbound"<<endl;

    std::set<int>::const_iterator iterlower = usr_set.lower_bound(5);
    std::set<int>::const_iterator iterupper = usr_set.upper_bound(5);
    if(iterlower != usr_set.end())
        cout<<"lowerbound:"<<*iterlower<<endl;
    else
        cout<<"invalid lowerbound"<<endl;
    if(iterupper != usr_set.end())
        cout<<"upperbound:"<<*iterupper<<endl;
    else
        cout<<"invalid upperbound"<<endl;

    //erase
    auto iter=usr_set.begin();
    while(iter!=usr_set.end())
    {
        if(*iter == 3)
        {
            iter=usr_set.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_set(usr_set, "erase");

    //insert
    usr_set.insert(15);
    show_set(usr_set, "insert");

    //key_comp
    cout<<"key_comp:";
    for(auto &x:usr_set)
    {
        if(usr_set.key_comp()(x, 7))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //value_comp
    cout<<"value comp:";
    for(auto &x:usr_set)
    {
        if(usr_set.value_comp()(x, 9))
        {
            cout<<x<<" ";
        }
    }
    cout<<endl;

    //swap
    std::set<int> swapset;
    swapset.swap(usr_set);
    show_set(swapset, "swapset");

    return 0;
}
