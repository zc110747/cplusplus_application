/*********************************************************************
 * 说明: map 
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
*********************************************************************/
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <vector>

template<class T0, class T1>
void show_map(std::map<T0, T1> &map_val)
{
    std::for_each(map_val.begin(), map_val.end(), [](const std::pair<T0, T1> &map_ref){
        std::cout<<map_ref.first<<":"<<map_ref.second<<" ";
    });
    std::cout<<std::endl;
}

int main(int argc, char* argv[])
{
    std::map<std::string, int> map_si0 = {
        {"first", 1},
        {"second", 2},
        {"third", 3}
    };
    std::map<std::string, std::string> map_ss0 = {
        {"val0", "0"},
        {"val1", "1"},
    };
    std::map<int, int> map_ii0 = {
        {10, 500},
        {20, 600},
        {30, 900}
    };
    std::cout<<"show map:";
    show_map<std::string, int>(map_si0);

    //size, max_size, empty
    std::cout<<"map_ss0 size:"<<map_ss0.size()<<std::endl;
    map_ss0.clear();
    std::cout<<"map_ss0 size:"<<map_ss0.size()<<std::endl;
    std::cout<<"map_ss0 max size:"<<map_ss0.max_size()<<std::endl;
    std::cout<<"map_ss0 is empty:"<<map_ss0.empty()<<std::endl;
    map_ss0["first"] = "0";
    map_ss0["second"] = "1";
    map_ss0["third"] = "1";
    for(auto iter=map_ss0.begin();iter!=map_ss0.end();)
    {
        if(iter->second == "1")
        {
            iter = map_ss0.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_map<std::string, std::string>(map_ss0);
    
    //lower_bound, upper_bound, swap
    auto map_iterator = map_ii0.lower_bound(9);
    std::cout<<"lower bound val:"<<map_iterator->first<<":"<<map_iterator->second<<std::endl;
    map_iterator = map_ii0.upper_bound(21);
    std::cout<<"upper bound val:"<<map_iterator->first<<":"<<map_iterator->second<<std::endl;
    std::cout<<"show map_ii00:";
    show_map<int, int>(map_ii0);

    //swap
    std::map<int, int> map_ii1;
    map_ii1.swap(map_ii0);
    std::cout<<"show map_ii1:";
    show_map<int, int>(map_ii1);
    std::cout<<"map_ii0 size:"<<map_ii0.size()<<std::endl;

    //insert, emplace, []
    map_si0.insert(std::pair<std::string, int>("four", 4));
    map_si0.insert(std::make_pair<std::string, int>("five", 5));
    map_si0.insert(std::map<std::string, int>::value_type("six", 6));
    map_si0.emplace(std::pair<std::string, int>("ten", 10));
    map_si0.emplace(std::pair<std::string, int>("ten", 11)); //相同的值添加无效
    map_si0["Seven"] = 7;
    std::cout<<"map_si0 val:";
    show_map<std::string, int>(map_si0);
    std::cout<<"map_si0 val:";
    for(auto val:map_si0)
    {
        std::cout<<val.first<<":"<<val.second<<" ";
    }
    std::cout<<std::endl;
    std::cout<<"map_si0 iterator val:";
    for(auto iteartor_val=map_si0.begin(); iteartor_val!=map_si0.end(); iteartor_val++)
    {
        std::cout<<iteartor_val->first<<":"<<iteartor_val->second<<" ";
    }
    std::cout<<std::endl;

    //find
    std::map<std::string, int>::iterator iter = map_si0.find("second");
    if(iter != map_si0.end())
        std::cout<<iter->first<<":"<<iter->second;
    std::cout<<std::endl;

    //sort
    std::cout<<"show sort:";
    std::vector<std::pair<std::string, int>>  vec_pair;
    for(auto val:map_si0)
    {
        vec_pair.push_back(std::pair<std::string, int>(val.first, val.second));
    }
    std::sort(vec_pair.begin(), vec_pair.end(), [](std::pair<std::string, int> &map_ref0, std::pair<std::string, int> &map_ref1)->bool{
        return map_ref0.second < map_ref1.second;
    });
    for(auto val:vec_pair)
    {
        std::cout<<val.first<<":"<<val.second<<" ";
    }
    std::cout<<std::endl;
    return 0;
}
