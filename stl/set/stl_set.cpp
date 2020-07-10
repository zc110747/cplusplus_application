/**********************************************************
 * 说明: std::set
 * 自动排序的容器，不允许有相同的值
 * size         获取容器的当前内数据数目
 * max_size     获取容器指针的最大数据数目
 * empty        判断容器是否为空
 * swap         交换两个容器内的数据，被交换对象被清空
 * erase clear  删除容器内数据/清空容器
 * insert       向容器插入数据
 * emplace      向队列插入数据(避免不必要的临时对象产生)
 * find         查询数据在容器中的位置，并返回迭代器地址
 * lower_bound  返回非递减序列中的第一个大于等于值val的迭代器(上限闭合)
 * upper_bound  返回非递减序列中第一个大于值val的位置(下限闭合)
************************************************************/
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <vector>

template<class T>
void show_set(std::set<T> &set_val)
{
    std::for_each(set_val.begin(), set_val.end(), [](const T &set_ref){
        std::cout<<set_ref<<" ";
    });
     std::cout<<std::endl;
}

int main(int argc, char* argv[])
{
    std::set<std::string> set_str0;

    //insert emplace
    set_str0.insert("hello ");
    set_str0.insert("world ");
    set_str0.emplace("every one");

    //size max_size
    std::cout<<"set_str0 size: "<<set_str0.size()<<std::endl;
    std::cout<<"std::set Max size"<<set_str0.max_size()<<std::endl;

    //begin
    std::cout<<"set_str0 first val:"<<*set_str0.begin()<<std::endl;

    //clear
    set_str0.clear();
    std::cout<<"is empty:"<<set_str0.empty()<<std::endl;

    //count, auto
    std::set<int> set_int1 = {2, 3, 2, 1, 2, 4, 5, 5, 7};
    std::cout<<"set_int1 2 exist:"<<set_int1.count(2)<<std::endl;
    std::cout<<"set_int1 1 exist:"<<set_int1.count(0)<<std::endl;
    std::cout<<"set_int1 auto val:";
    for(auto &val:set_int1)
    {
        std::cout<<val<<" ";
    }
    std::cout<<std::endl;

    //iterator
    std::set<int>::iterator s_iterator;
    std::cout<<"set_int1 iterator val:";
    for(s_iterator=set_int1.begin(); s_iterator!=set_int1.end(); s_iterator++)
    {
        std::cout<<*s_iterator<<" ";
    }
    std::cout<<std::endl;
    
    //erase
    std::cout<<"set_int1 erase val:";
    set_int1.erase(0);
    show_set(set_int1);

    //swap
    auto set_int2 = set_int1;
    std::cout<<"set_int2 copy val:";
    show_set(set_int2);
    std::set<int> set_int3;
    set_int3.swap(set_int2);
    std::cout<<"set_int3 swap val:";
    show_set(set_int3);
    std::cout<<"set_int2 empty:"<<set_int2.empty()<<std::endl;

    //operator
    std::cout<<"operator ==:";
    if(set_int1 == set_int3){
        std::cout<<"set_int1 equal"<<std::endl;
    }

    //find
    s_iterator = find(set_int1.begin(), set_int1.end(), 2);
    if(s_iterator != set_int1.end())
    {
        std::cout<<"2 found"<<std::endl;
    }
    auto s_iterator1 = set_int1.find(2);
    if(s_iterator == s_iterator1)
    {
        std::cout<<"iterator equal"<<std::endl;
    }
    //迭代器不允许改变set元素，因为set内元素和位置是挂钩的
    //*s_iterator = 9; 

    //lower_bound, upper_bound
    auto s_iterator2 = set_int1.lower_bound(3);
    std::cout<<"lower bound:"<<*s_iterator2<<std::endl;
    s_iterator2 = set_int1.upper_bound(4);
    std::cout<<"upper bound:"<<*s_iterator2<<std::endl;

    return 0;
}
