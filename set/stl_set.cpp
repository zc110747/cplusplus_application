/**********************************************************
 * 说明: set
 * 自动排序的容器，不允许有相同的值
 * size 获取set的当前大小
 * max_size 容器的最大大小
 * empty 判断容器是否为空
 * swap 交换两个set内的信息，另外一个set将会被清空
 * erase clear 容器删除数据
 * insert 容器插入数据
 * find 查询数据在set中的位置，并返回迭代器地址
 * lower_bound 返回非递减序列中的第一个大于等于值val的迭代器(上限闭合)
 * upper_bound 返回非递减序列中第一个大于值val的位置(下限闭合)
************************************************************/
#include <string>
#include <set>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <vector>

using std::for_each;
using std::set;

template<class T>
void show_set(std::set<T> &set_val)
{
    for_each(set_val.begin(), set_val.end(), [](const T &set_ref){
        std::cout<<set_ref<<" ";
    });
     std::cout<<std::endl;
}

int main(int argc, char* argv[])
{
    std::set<std::string> s_str;
    s_str.insert("hello ");
    s_str.insert("world ");
    std::cout<<"set size: "<<s_str.size()<<std::endl;
    std::cout<<"set first val:"<<*s_str.begin()<<std::endl;
    std::cout<<"set rend val:"<<*s_str.rend()<<std::endl;
    s_str.clear();
    std::cout<<"is empty:"<<s_str.empty()<<std::endl;
    std::cout<<"set Max size"<<s_str.max_size()<<std::endl;

    std::set<int> s_int = {2, 3, 2, 1, 2, 4, 5, 5, 7};
    std::cout<<"2 is exist:"<<s_int.count(2)<<std::endl;
    std::cout<<"0 is exist:"<<s_int.count(0)<<std::endl;
    std::cout<<"auto test:";
    for(auto &val:s_int)
    {
        std::cout<<val<<" ";
    }
    std::cout<<std::endl;
    std::set<int>::iterator s_iterator;
    std::cout<<"iterator test:";
    for(s_iterator=s_int.begin(); s_iterator!=s_int.end(); s_iterator++)
    {
        std::cout<<*s_iterator<<" ";
    }
    std::cout<<std::endl;
    
    //erase删除某个元素
    std::cout<<"erase test:";
    s_int.erase(0);
    show_set<int>(s_int);
    s_int.erase(2);
    show_set<int>(s_int);

    //swap set内容器交换
    std::cout<<"swap test:";
    auto s_int1 = s_int;
    show_set<int>(s_int1);
    std::set<int> s_int2;
    s_int2.swap(s_int1);
    show_set<int>(s_int2);
    std::cout<<"s_int1 is empty:"<<s_int1.empty()<<std::endl;

    //运算符重载<, >, ==, 
    std::cout<<"operator ==:";
    if(s_int == s_int2){
        std::cout<<"s_int equal"<<std::endl;
    }

    //find算法
    s_iterator = find(s_int.begin(), s_int.end(), 2);
    if(s_iterator != s_int.end())
    {
        std::cout<<"2 found"<<std::endl;
    }
    auto s_iterator1 = s_int.find(2);
    if(s_iterator == s_iterator1)
    {
        std::cout<<"iterator equal"<<std::endl;
    }
    //迭代器不允许改变set元素，因为set内元素和位置是挂钩的
    //*s_iterator = 9; 

    //lower_bound, upper_bound
    auto s_iterator2 = s_int.lower_bound(3);
    std::cout<<"lower bound:"<<*s_iterator2<<std::endl;
    s_iterator2 = s_int.upper_bound(4);
    std::cout<<"upper bound:"<<*s_iterator2<<std::endl;

    return 0;
}
