/**********************************************************
 * 说明:stack
 * 后进先出的栈序列
 * push, pop    用于压栈和出栈的接口
 * size         堆栈内当前的数据量
 * empty        判断栈内数据是否为空
 * emplace      向栈内压入数据(避免不必要的临时对象产生)
 * swap         交换栈内数据，被交换方则被情空
************************************************************/
#include <stack>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[])
{    
    std::stack<int> stack_int0;

    //push pop emplace size empty
    stack_int0.push(1);
    stack_int0.push(3);
    stack_int0.push(5);
    stack_int0.emplace(6);
    int size = stack_int0.size();
    std::cout<<"stack_int0 size:"<<size<<std::endl;
    std::cout<<"stack_int0 val:";
    for(int index=0; index<size; index++)
    {
        std::cout<<stack_int0.top()<<" ";
        stack_int0.pop();
    }
    std::cout<<std::endl;
    std::cout<<"stack_int0 empty:"<<stack_int0.empty()<<std::endl;

    
    std::vector<int>  v_int{1, 2, 3, 4, 5};
    std::stack<int, std::vector<int>> stack_int1(v_int);
    std::cout<<"stack_int1 val:";
    while(!stack_int1.empty())
    {
        std::cout<<stack_int1.top()<<" ";
        stack_int1.pop();
    }
    std::cout<<std::endl;

    std::list<std::string> list_str = {"11", "22", "33"};
    std::stack<std::string, std::list<std::string>> stack_str0(list_str);
    std::cout<<"stack_str0 val:";
    while(!stack_str0.empty())
    {
        std::cout<<stack_str0.top()<<" ";
        stack_str0.pop();
    }
    std::cout<<std::endl;
    stack_str0.push("world!");
    stack_str0.push("hello");
    std::stack<std::string, std::list<std::string>> sl_str1;
    sl_str1.swap(stack_str0);
    std::cout<<"stack_str0 is empty:"<<stack_str0.empty()<<std::endl;

    auto sl_str2 = sl_str1;
    std::cout<<"sl_str1 is empty:"<<sl_str1.empty()<<std::endl;
    std::cout<<"sl_str2 is empty:"<<sl_str2.empty()<<std::endl;

    auto sl_str3 = std::move(sl_str2);
    std::cout<<"sl_str2 is empty:"<<sl_str2.empty()<<std::endl;
    std::cout<<"sl_str3 is empty:"<<sl_str3.empty()<<std::endl;  

    //右值引用(数据指向同一块内存)
    auto &&sl_str4 = std::move(sl_str3);
    std::cout<<"sl_str3 is empty:"<<sl_str3.empty()<<std::endl;
    std::cout<<"sl_str4 is empty:"<<sl_str4.empty()<<std::endl;  
    while(!sl_str3.empty())
    {
        std::cout<<sl_str3.top()<<" ";
        sl_str3.pop();
    }
    std::cout<<std::endl;
    std::cout<<"sl_str3 is empty:"<<sl_str3.empty()<<std::endl;
    std::cout<<"sl_str4 is empty:"<<sl_str4.empty()<<std::endl; 
}
