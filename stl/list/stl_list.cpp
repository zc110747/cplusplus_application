/**********************************************************
 * 说明: list 
 * 列表容器, 提供一类数据集合的双向链表容器
 * list常用方法
 * size, resize             获取容量和容器大小调整
 * push_back, push_front    在列表的首尾插入数据
 * pop_front, pop_back      删除列表的首尾数据
 * erase, remove, clear     删除某个迭代器，值或全部元素
 * unique                   去除容器内相邻的重复元素
 * splice                   剪切后一个List，插入到前List中, 被剪切对象释放
 * merge                    两个列表的合并, 并合并列表释放
 * sort                     列表排序
************************************************************/
#include <list>
#include <iostream>
#include <algorithm>
#include <string>

template<class T>
int show_list(std::list<T> &list_val)
{
    std::for_each(list_val.begin(), list_val.end(), [](T &value){
        std::cout<<value<<" ";
    });
    std::cout<<std::endl;
}

int main(int argc, char* argv[])
{
    //init
    int array[] = {8, 2, 1, 4, 3, 4, 5};
    std::list<int> list_int0(array, array+7);
    std::list<int> list_int1(list_int0);

    //size, clear, resize
    std::cout<<"list_int0 size: "<<list_int0.size()<<std::endl;
    list_int0.clear(); //和vector不同,clear会释放所有空间
    list_int0.resize(10, 3);
    std::cout<<"list_int0 size: "<<list_int0.size()<<std::endl;

    //push_back, push_front, pop_back, pop_front
    list_int0 = list_int1;
    list_int0.push_back(6);
    list_int0.push_front(4);
    std::cout<<"list_int0 size: "<<list_int0.size()<<std::endl;
    std::cout<<"list_int0 val:";
    show_list<int>(list_int0);
    list_int0.pop_back();
    list_int0.pop_front();
    std::cout<<"list_int0 val:";
    show_list<int>(list_int0);

    //remove, erase
    for(auto list_iterator = list_int0.begin(); list_iterator != list_int0.end();)
    {
        if(*list_iterator == 4){
            list_iterator = list_int0.erase(list_iterator); //列表内迭代器删除,同时将后一个迭代器赋值用于后续的遍历
        }
        else{
            list_iterator++;
        }  
    }
    std::cout<<"list_int0 val:";
    show_list(list_int0); 
    list_int0.remove(1);
    std::cout<<"list_int0 val:";
    show_list(list_int0); 

    //iterator
    std::cout<<"list_int1 val for iterator: ";
    for(std::list<int>::iterator list_iter = list_int1.begin(); list_iter != list_int1.end(); list_iter++)
    {
        std::cout<<*list_iter<<" ";
    }
    std::cout<<std::endl;

    //merge
    std::cout<<"list_int1 val:";
    show_list(list_int1);
    list_int1.merge(list_int0);
    std::cout<<"list_int1 val:";
    show_list(list_int1);
    std::cout<<"list_int0 size:"<<list_int0.size()<<std::endl;

    //sort, unique
    std::list<std::string> list_str0{"11", "21", "14", "21", "23", "11", "15"}; //列表初始化 C++11支持
    //list_str0.sort();                //从小到大排序
    list_str0.sort(std::greater<std::string>());  //从大到小排序
    std::cout<<"list_str2 val:";
    std::for_each(list_str0.begin(), list_str0.end(), [](std::string &value){
        std::cout<<value<<" ";
    });
    std::cout<<std::endl;
    list_str0.unique();
    std::cout<<"list_str0 unique val:";
    show_list(list_str0);

    //splice
    std::list<std::string> list_str1{"12", "34", "56", "78"};
    std::list<std::string> list_str2{"11", "22", "33"};
    auto ListIterator = list_str1.begin();
    ListIterator++;
    list_str1.splice(ListIterator, list_str2);
    std::cout<<"splice string0: ";
    show_list<std::string>(list_str1);
    std::cout<<"list_str1 size:"<<list_str2.size()<<std::endl;
}
