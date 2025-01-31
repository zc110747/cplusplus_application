/*
std::multimap
std::multimap 是 C++ 标准库中的一个关联容器，它允许存储多个具有相同键的键值对

构造函数：
std::multimap()：默认构造函数，创建一个空的 std::multimap。
std::multimap(const std::multimap& other)：复制构造函数，创建一个与 other 相同的 std::multimap。
std::multimap(std::multimap&& other)：移动构造函数，创建一个与 other 相同的 std::multimap，并将 other 置为空。
std::multimap(std::initializer_list<std::pair<const Key, T>> ilist)：使用初始化列表构造函数，创建一个包含 ilist 中元素的 std::multimap。
std::multimap(const std::multimap& other, const Allocator& alloc)：使用 alloc 作为分配器创建一个与 other 相同的 std::multimap。

成员函数：
begin：返回指向 multimap 中第一个元素的迭代器。
cbegin：返回指向 multimap 中第一个元素的常量迭代器。
cend：返回指向 multimap 中最后一个元素之后的位置的常量迭代器。
clear: 清空 multimap 中的所有元素。
contains(c++20): 判断 multimap 是否包含指定的键。
count：返回 multimap 中指定键的元素数量。
crbegin：返回指向 multimap 中第一个元素的常量反向迭代器。
crend：返回指向 multimap 中最后一个元素之后的位置的常量反向迭代器。
emplace：在 multimap 中插入一个新元素。
emplace_hint：在 multimap 中插入一个新元素，使用提示位置。
empty：判断 multimap 是否为空。
end：返回指向 multimap 中最后一个元素之后的位置的迭代器。
equal_range：返回 multimap 中指定键的元素范围。
erase：从 multimap 中删除指定位置的元素。
find：在 multimap 中查找指定键的元素。
get_allocator：返回 multimap 的分配器。
insert：在 multimap 中插入一个新元素。
key_comp：返回 multimap 中键的比较函数。
lower_bound：返回指向 multimap 中第一个不小于指定键的元素的迭代器。
max_size：返回 multimap 可容纳的最大元素数量。
rbegin：返回指向 multimap 中第一个元素的反向迭代器。
rend：返回指向 multimap 中最后一个元素之后的位置的反向迭代器。
size：返回 multimap 中的元素数量。
swap：交换两个 multimap 的内容。
upper_bound：返回指向 multimap 中第一个大于指定键的元素的迭代器。
value_comp：返回 multimap 中值的比较函数。
*/
#include <map>
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

template<typename T, class N>
void show_multimap(std::multimap<T, N> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        std::for_each(value.begin(), value.end(), [](const typename std::pair<T, N> &ref_value){
            cout<<ref_value.first<<"=>"<<ref_value.second<<" ";
        });
    }
    //auto
    else if(mode == SHOW_MODE_ENUM_TYPE_AUTO)
    {
        for(auto &ref:value)
            cout<<ref.first<<"=>"<<ref.second<<" ";
    }
    //iterator
    else if(mode == SHOW_MODE_ENUM_TYPE_ITERATOR)
    {
        for(typename std::multimap<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<iter->first<<"=>"<<iter->second<<" ";
    }
    cout<<endl;
}

int main(int argc, char* argv[])
{
    std::multimap<string, int> usermultimap = {
        {"alpha", 0},
        {"beta", 0}, 
        {"beta", 5},
        {"beta", 2},
        {"gamma", 1},   
    };
    auto copymultimap(usermultimap);

    //size, clear, max_size, empty
    show_multimap(copymultimap, "copymultimap");
    cout<<"size:"<<copymultimap.size()<<endl;
    copymultimap.clear();
    cout<<"clear size:"<<copymultimap.size()<<endl;
    cout<<"max_size:"<<copymultimap.max_size()<<endl;
    cout<<"empty:"<<copymultimap.empty()<<endl;

    show_multimap(usermultimap, "usermultimap");

    //begin, end, cbegin, cend
    //rbegin, rend, rcbegin, rcend
    std::multimap<string, int>::iterator iterbegin = usermultimap.begin();
    std::multimap<string, int>::iterator iterend = usermultimap.end();
    std::multimap<string, int>::const_iterator iterconstbegin = usermultimap.cbegin();
    std::multimap<string, int>::const_iterator iterconstend = usermultimap.cend();
    std::multimap<string, int>::reverse_iterator iterrbegin = usermultimap.rbegin();
    std::multimap<string, int>::reverse_iterator iterrend = usermultimap.rend();
    std::multimap<string, int>::const_reverse_iterator iterconstrbegin = usermultimap.crbegin();
    std::multimap<string, int>::const_reverse_iterator iterconstrend = usermultimap.crend();
    cout<<"begin: "<<iterbegin->first<<"=>"<<iterbegin->second<<",";
    --iterend;
    cout<<"end: "<<iterend->first<<"=>"<<iterend->second<<",";
    cout<<"cbegin: "<<iterconstbegin->first<<"=>"<<iterconstbegin->second<<",";
    --iterconstend;
    cout<<"cend: "<<iterconstend->first<<"=>"<<iterconstend->second<<",";
    cout<<"rbegin: "<<iterrbegin->first<<"=>"<<iterrbegin->second<<",";
    --iterrend;
    cout<<"rend: "<<iterrend->first<<"=>"<<iterrend->second<<",";
    cout<<"crbegin: "<<iterconstrbegin->first<<"=>"<<iterconstrbegin->second<<",";
    --iterconstrend;
    cout<<"crend: "<<iterconstrend->first<<"=>"<<iterconstrend->second<<endl;

    //count
    cout<<"count:"<<usermultimap.count("beta")<<endl;

    //emplace, emplace_hint
    usermultimap.emplace("delta", 6);
    show_multimap(usermultimap, "emplace", SHOW_MODE_ENUM_TYPE_AUTO);
    usermultimap.emplace_hint(usermultimap.begin(), "epsilon", 10);
    show_multimap(usermultimap, "emplace", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //equal_range, lower_bound, upper_bound
    std::pair<std::multimap<string, int>::iterator, std::multimap<string, int>::iterator> ret;
    ret = usermultimap.equal_range("beta");
    if(ret.first != usermultimap.end())
        cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<endl;
    else
        cout<<"invaild equal_range lower_bound"<<endl;
    if(ret.second != usermultimap.end())   
        cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<endl;
    else
        cout<<"invaild equal_range upper_bound"<<endl;
    std::multimap<string, int>::iterator lower = usermultimap.lower_bound("beta");
    std::multimap<string, int>::iterator upper = usermultimap.upper_bound("beta");
    if(lower != usermultimap.end())
        cout<<"lower_bound:"<<lower->first<<"=>"<<lower->second<<endl;
    else
        cout<<"invaild lower_bound"<<endl;
    if(upper != usermultimap.end())
        cout<<"upper_bound:"<<upper->first<<"=>"<<upper->second<<endl;
    else   
        cout<<"invaild upper_bound"<<endl;

    //find, erase
    std::multimap<string, int>::iterator it = usermultimap.find("gamma");
    cout<<"find:"<<it->first<<"=>"<<it->second<<endl;
    usermultimap.erase(it);
    show_multimap(usermultimap, "erase");

    //get_alloctator
    std::pair<const string, int>* p;
    p = usermultimap.get_allocator().allocate(5);
    cout<<"size:"<<sizeof(p)<<","<<sizeof(std::multimap<string, int>::value_type)<<endl;
    usermultimap.get_allocator().deallocate(p, 5);

    //insert
    usermultimap.insert(usermultimap.begin(), std::pair<string, int>("gamma", 5));
    usermultimap.insert(usermultimap.find("delta"), std::pair<string, int>("zeta", 9));
    show_multimap(usermultimap, "insert");

    //key_comp => key compare, use key value
    cout<<"key_comp: ";
    for(auto &x:usermultimap)
    {
        if(usermultimap.key_comp()(x.first, "c"))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    //value_comp => key compare, use object
    cout<<"value_comp: ";
    for(auto &x:usermultimap)
    {
        if(usermultimap.value_comp()(x, std::pair<string, int>("c", 1)))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    std::multimap<string, int> swapmultimap;

    //swap
    swapmultimap.swap(usermultimap);
    show_multimap(swapmultimap, "swap");

    return 0;
} 
