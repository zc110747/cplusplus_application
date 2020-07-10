/**********************************************************
 * 说明: multiset
 * 允许键值重复的有序数据集合
 * size         获取容器当前内部数据的数量
 * max_size     当前容器支持的最大数据量
 * empty        判断容器是否为空
 * swap         交换容器内数据,原容器会被清空
 * erase        删除容器内等于某变量的值, 或者指定迭代器
 * clear        删除容器内数据
 * insert       容器插入数据
 * emplace      向队列插入数据(避免不必要的临时对象产生)
 * find         查询数据在multi中的位置，并返回迭代器地址
 * lower_bound  返回非递减序列中的第一个大于等于值val的迭代器(上限闭合)
 * upper_bound  返回非递减序列中第一个大于值val的位置(下限闭合)
************************************************************/
#include <string>
#include <set>
#include <list>
#include <iostream>
#include <algorithm>
#include <vector>

template<class Iter>
void show_set(std::multiset<Iter> &multiset_val, bool is_reverse = false)
{
    typedef typename std::multiset<Iter>::iterator MultsetTypeIter;
    typedef typename std::multiset<Iter>::reverse_iterator MultsetTypeReversedIter;
    
    MultsetTypeIter iter_begin, iter_end;
    MultsetTypeReversedIter iter_rbegin, iter_rend;

    //rbegin, rend, begin, end
    if(is_reverse)
    {
        iter_rbegin = multiset_val.rbegin();
        iter_rend = multiset_val.rend();
        std::for_each(iter_rbegin, iter_rend, [](const Iter &set_ref){
            std::cout<<set_ref<<" ";
        });
        std::cout<<std::endl;
    }
    else
    {
        iter_begin = multiset_val.begin();
        iter_end = multiset_val.end();
        std::for_each(iter_begin, iter_end, [](const Iter &set_ref){
            std::cout<<set_ref<<" ";
        });
        std::cout<<std::endl;
    }
}

int main(int argc, char* argv[])
{
    //size, max_size, empty
    std::vector<int> vct0 = {1, 2, 2, 6, 7, 6};
    std::multiset<int> mulset_int0(vct0.begin(), vct0.end());
    std::cout<<"mulset_int0 size:"<<mulset_int0.size()<<std::endl;
    std::cout<<"mulset_int0 MaxSize:"<<mulset_int0.max_size()<<std::endl;
    std::cout<<"mulset_int0 empty:"<<mulset_int0.empty()<<std::endl;
    std::cout<<"mulset_int0 val:";
    show_set(mulset_int0, false);
    std::cout<<"mulset_int0 reverse val:";
    show_set(mulset_int0, true);

    //insert, emplace, erase
    std::multiset<int> mulset_int1 = {1, 2, 2, 3, 5};
    std::cout<<"mult_set1 val:";
    show_set(mulset_int1, false);
    mulset_int1.insert(6);
    mulset_int1.insert(1);
    mulset_int1.emplace(3);
    std::cout<<"mulset_int1 val:";
    show_set(mulset_int1, false);
    mulset_int1.clear();
    std::cout<<"mulset_int1 empty:"<<mulset_int1.empty()<<std::endl;

    //erase lower_bound upper_bound
    std::list<int> list_val0 = {1, 3, 5, 7, 5, 6, 2, 3};
    std::multiset<int> mulset_int2(list_val0.begin(), list_val0.end());
    std::cout<<"mulset_int2 val:";
    show_set(mulset_int2);
    mulset_int2.erase(2);
    std::cout<<"mulset_int2 val:";
    show_set(mulset_int2);
    for(auto iter=mulset_int2.begin();iter!=mulset_int2.end();)
    {
        if(*iter == 3)
        {
            iter = mulset_int2.erase(iter);
        }
        else
        {
            iter++;
        }   
    }
    std::cout<<"mulset_int2 val:";
    show_set(mulset_int2);
    std::cout<<"lower bound:"<<*mulset_int2.lower_bound(5)<<std::endl;
    std::cout<<"upper bound:"<<*mulset_int2.upper_bound(5)<<std::endl;

    //swap 
    std::multiset<std::string> mulset_str0{"hello", "multiset", "string"};
    show_set(mulset_str0);
    auto mulset_str1 = mulset_str0;
    show_set(mulset_str1);
    std::multiset<std::string> mulset_str2;
    mulset_str2.swap(mulset_str1);
    std::cout<<"mulset str1 size:"<<mulset_str1.size()<<std::endl;
    std::cout<<"mulset str2 size:"<<mulset_str2.size()<<std::endl;
    for(auto &val:mulset_str2)
    {
        std::cout<<val<<" ";
    }
    std::cout<<std::endl;

    //find
    if(mulset_str2.find("multiset") != mulset_str2.end())
    {
        std::cout<<"exist string"<<std::endl;
    }   
}
