/**********************************************************
 * 说明:stack
 * 先入先出的栈操作
 * stack常用方法
 * push, pop
 * size
 * empty 判断为空
 * swap 交换占用
************************************************************/
#include <stack>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[])
{    
    std::stack<int> st_int;
    st_int.push(1);
    st_int.push(3);
    st_int.push(5);
    int size = st_int.size();
    std::cout<<"stack size:"<<size<<std::endl;
    for(int index=0; index<size; index++)
    {
        std::cout<<st_int.top()<<" ";
        st_int.pop();
    }
    std::cout<<std::endl;
    std::cout<<"st_int is empty:"<<st_int.empty()<<std::endl;

    
    std::vector<int>  v_int{1, 2, 3, 4, 5};
    std::stack<int, std::vector<int>> s_vint(v_int);
    while(!s_vint.empty())
    {
        std::cout<<s_vint.top()<<" ";
        s_vint.pop();
    }
    std::cout<<std::endl;

    std::list<std::string> list_str = {"11", "22", "33"};
    std::stack<std::string, std::list<std::string>> sl_str(list_str);
    while(!sl_str.empty())
    {
        std::cout<<sl_str.top()<<" ";
        sl_str.pop();
    }
    std::cout<<std::endl;
    sl_str.push("world!");
    sl_str.push("hello ");
    std::stack<std::string, std::list<std::string>> sl_str1;
    sl_str1.swap(sl_str);
    std::cout<<"sl_str is empty:"<<sl_str.empty()<<std::endl;

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
