/*
std::map
https://www.cplusplus.com/reference/map/map/
std::map 是 C++ 标准库中的一个关联容器，它存储的元素是键值对（key-value pairs）。每个元素由一个键（key）和一个值（value）组成，键是唯一的，每个键对应一个值。std::map 中的元素按照键的顺序自动排序，默认情况下是按照升序排列，支持自定义比较函数进行键值的比较排序。

特性:
1. 有序性：std::map 中的元素按照键的顺序排列，这使得查找、插入和删除操作的时间复杂度为对数级别（O(log n)）
2. 唯一性：每个键在 std::map 中只能出现一次
3. 动态大小：std::map 的大小可以动态增长或缩小
4. 高效查找：由于元素是有序的，std::map支持快速查找操作

常用操作:
1. 插入元素：insert、emplace
2. 删除元素：erase、clear
3. 查找元素：find、count
4. 访问元素：at、operator[](operator访问不存在的key同时写入会添加对象)
4. 迭代器：begin、end、rbegin、rend、cbegin、cend、crbegin、crend
5. 大小和容量：size、empty、max_size

接口说明:
at          查找具有指定键值的元素。
begin       返回一个迭代器，此迭代器指向 map 中的第一个元素。
cbegin      返回一个常量迭代器，此迭代器指向 map 中的第一个元素。
cend        返回一个超过末尾常量迭代器。
clear       清除 map 的所有元素。
contains    检查 map 中是否包含具有指定键的元素。
count       返回映射中其键与参数中指定的键匹配的元素数量。
crbegin     返回一个常量迭代器，此迭代器指向反向 map 中的第一个元素。
crend       返回一个常量迭代器，此迭代器指向反向 map 中最后一个元素之后的位置。
emplace     将就地构造的元素插入到 map 。
emplace_hint 将就地构造的元素插入到 map ，附带位置提示。
empty       如果 map 为空，则返回 true 。
end         返回超过末尾迭代器。
equal_range 返回一对迭代器。 此迭代器对中的第一个迭代器指向 map 中其键大于指定键的第一个元素。此迭代器对中的第二个迭代器指向 map 中其键等于或大于指定键的第
一个元素。
erase       从指定位置移除映射中的元素或元素范围。
find        返回一个迭代器，此迭代器指向 map 中其键与指定键相等的元素的位置。
get_allocator 返回用于构造 allocator 的 map 对象的副本。
insert      将一个或一系列元素插入到 map 中的指定位置。
key_comp    返回用于对 map 中的键进行排序的比较对象副本。
lower_bound 返回一个迭代器，此迭代器指向 map 中其键值等于或大于指定键的键值的第一个元素。
max_size    返回 map 的最大长度。
rbegin      返回一个迭代器，此迭代器指向反向 map 中的第一个元素。
rend        返回一个迭代器，此迭代器指向反向 map 中最后一个元素之后的位置。
size        返回 map 中的元素数量。
swap        交换两个映射的元素。
upper_bound 返回一个迭代器，此迭代器指向 map 中其键值大于指定键的键值的第一个元素。
value_comp  检索用于对 map 中的元素值进行排序的比较对象副本
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

template<typename T, typename N>
void show_map(std::map<T, N> value, string qstring, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
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
        for(typename std::map<T, N>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<iter->first<<"=>"<<iter->second<<" ";
    }
    cout<<endl;
}

struct CompareByLength {
    bool operator()(const std::string& a, const std::string& b) const {
        if (a.length() == b.length())
        {
            return a < b;
        }
        return a.length() < b.length();
    }
};

int main(int argc, char* argv[])
{
    std::map<string, int> usermap = {
        {"alpha", 4},
        {"beta", 2}, 
        {"gamma", 3},   
    };
    auto copymap = usermap;

    //size, clear, max_size, empty
    show_map(copymap, "copymap");
    cout<<"size:"<<copymap.size()<<endl;
    copymap.clear();
    cout<<"clear size:"<<copymap.size()<<endl;
    cout<<"max_size:"<<copymap.max_size()<<endl;
    cout<<"empty:"<<copymap.empty()<<endl;

    show_map(usermap, "usermap");

    //at, operator[]
    usermap.at("beta") = 5;
    show_map(usermap, "usermap", SHOW_MODE_ENUM_LAMBDA);
    usermap["beta"] = 4;
    show_map(usermap, "operator[]");

    //begin, end, cbegin, cend
    //rbegin, rend, rcbegin, rcend
    std::map<string, int>::iterator iterbegin = usermap.begin();
    std::map<string, int>::iterator iterend = usermap.end();
    std::map<string, int>::const_iterator iterconstbegin = usermap.cbegin();
    std::map<string, int>::const_iterator iterconstend = usermap.cend();
    std::map<string, int>::reverse_iterator iterrbegin = usermap.rbegin();
    std::map<string, int>::reverse_iterator iterrend = usermap.rend();
    std::map<string, int>::const_reverse_iterator iterconstrbegin = usermap.crbegin();
    std::map<string, int>::const_reverse_iterator iterconstrend = usermap.crend();
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
    cout<<"count:"<<usermap.count("beta")<<endl;

    //emplace, emplace_hint
    usermap.emplace("delta", 6);
    show_map(usermap, "emplace", SHOW_MODE_ENUM_TYPE_AUTO);
    usermap.emplace_hint(usermap.begin(), "epsilon", 10);
    show_map(usermap, "emplace", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //equal_range, lower_bound, upper_bound
    std::pair<std::map<string, int>::iterator, std::map<string, int>::iterator> ret;
    ret = usermap.equal_range("beta");
    if(ret.first != usermap.end())
        cout<<"equal_range lower_bound:"<<ret.first->first<<"=>"<<ret.first->second<<endl;
    else
        cout<<"invaild equal_range lower_bound"<<endl;
    if(ret.second != usermap.end())   
        cout<<"equal_range upper_bound:"<<ret.second->first<<"=>"<<ret.second->second<<endl;
    else
        cout<<"invaild equal_range upper_bound"<<endl;
    std::map<string, int>::iterator lower = usermap.lower_bound("beta");
    std::map<string, int>::iterator upper = usermap.upper_bound("beta");
    if(lower != usermap.end())
        cout<<"lower_bound:"<<lower->first<<"=>"<<lower->second<<endl;
    else
        cout<<"invaild lower_bound"<<endl;
    if(upper != usermap.end())
        cout<<"upper_bound:"<<upper->first<<"=>"<<upper->second<<endl;
    else   
        cout<<"invaild upper_bound"<<endl;

    //find, erase
    std::map<string, int>::iterator it = usermap.find("gamma");
    cout<<"find:"<<it->first<<"=>"<<it->second<<endl;
    usermap.erase(it);
    show_map(usermap, "erase");

    //get_alloctator
    std::pair<const string, int>* p;
    p = usermap.get_allocator().allocate(5);
    cout<<"size:"<<sizeof(p)<<","<<sizeof(std::map<string, int>::value_type)<<endl;
    usermap.get_allocator().deallocate(p, 5);

    //insert
    usermap.insert(usermap.begin(), std::pair<string, int>("gamma", 5));
    usermap.insert(usermap.find("delta"), std::pair<string, int>("zeta", 9));
    show_map(usermap, "insert");

    //key_comp => key compare, use key value
    cout<<"key_comp: ";
    for(auto &x:usermap)
    {
        if(usermap.key_comp()(x.first, "c"))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    //value_comp => key compare, use object
    cout<<"value_comp: ";
    for(auto &x:usermap)
    {
        if(usermap.value_comp()(x, std::pair<string, int>("c", 1)))
        {
            cout<<x.first<<"=>"<<x.second<<" ";
        }
    }
    cout<<endl;

    std::map<string, int> swapmap;

    //swap
    swapmap.swap(usermap);
    show_map(swapmap, "swap");

    // key compare
    std::map<string, int, CompareByLength> compare_map = {
        {"alpha1", 3},
        {"beta2", 4},
        {"gamma3", 1},
    };
    std::cout<<"compare_map: ";
    for(auto &x:compare_map) {
        cout<<x.first<<"=>"<<x.second<<" ";
    }
    cout<<endl;

    return 0;
} 
