/**********************************************************
* 说明: multimap 
 * 关联容器，提供映射的处理关系
 * size         获取容器的当前大小
 * max_size     获取容器的最大允许容量
 * empty        判断容器是否为空
 * swap         交换两个map内的信息，另外一个map将会被清空
 * erase clear  容器删除/清空数据
 * insert       向容器内插入数据
 * find         查找存在指定值，返回迭代器
 * emplace      向队列插入数据(避免不必要的临时对象产生)
 * lower_bound  返回非递减序列中的第一个大于等于值val的迭代器(上限闭合)
 * upper_bound  返回非递减序列中第一个大于值val的位置(下限闭合)
************************************************************/
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <algorithm>
#include <vector>

template<typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, std::multimap<T1, T2> &multimap_val)
{
    os<<"{";
    for(auto val:multimap_val)
    {
        os<<" {"<<val.first<<" => "<<val.second<<"} ";
    }
    os<<"}";
    return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> &vector_val)
{
    os<<"{";
    for(auto val:vector_val)
    {
        os<<" "<<val<<" ";
    }
    os<<"}";
    return os;
}

int main(int argc, char* argv[])
{
    std::multimap<int, std::string> multimap_val0 = {
        {1, "val1"},
        {2, "val2"}
    };
    std::cout<<multimap_val0<<std::endl;

    multimap_val0.insert(std::make_pair(3, "val3"));
    std::cout<<multimap_val0<<std::endl;

    multimap_val0.insert(std::make_pair(3, "val4"));
    multimap_val0.emplace(std::pair<int, std::string>(4, "val4"));
    std::cout<<multimap_val0<<std::endl;

    //begin, end, rbegin, rend
    auto multimap_val1 = multimap_val0;
    std::cout<<"{";
    std::for_each(multimap_val1.begin(), multimap_val1.end(), [](const std::pair<int, std::string> &pair_val){
        std::cout<<" {"<<pair_val.first<<" => "<<pair_val.second<<"} ";
    });
    std::cout<<"}"<<std::endl;

    //size， max_size
    std::cout<<"size:"<<multimap_val1.size()<<std::endl;
    std::cout<<"max_size:0x"<<std::hex<<multimap_val1.max_size()<<std::endl;
    
    //swap
    std::multimap<int, std::string> multimap_val2;
    multimap_val2.swap(multimap_val1);
    std::cout<<multimap_val2<<std::endl;
    std::cout<<"empty:"<<multimap_val1.empty()<<std::endl;

    //erase clear
    for(auto iter=multimap_val2.begin(); iter!=multimap_val2.end();)
    {
        if((*iter).first == 3)
        {
            iter = multimap_val2.erase(iter);
        }
        else
        {
            iter++;
        }   
    }
    std::cout<<multimap_val2<<std::endl;
    multimap_val2.clear();
    std::cout<<"empty:"<<multimap_val2.empty()<<std::endl;

    //find
    std::multimap<std::string, std::vector<int>> multimap_val3 = {
        {"first", {1, 2, 3}},
        {"second", {4, 5, 6}},
    };
    std::cout<<multimap_val3<<std::endl;
    std::cout<<(*(multimap_val3.find("first"))).second<<std::endl;
    auto multimap_val4 = multimap_val3;
    
    //key_comp
    std::cout<<multimap_val3.key_comp()((*multimap_val3.begin()).first, "g")<<std::endl;
    std::cout<<multimap_val3<<std::endl;
}
