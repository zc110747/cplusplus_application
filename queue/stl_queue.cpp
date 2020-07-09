/**********************************************************
 * 说明: queue 
 * 先进先出的数据结构
 * push，pop
 * size
 * empty
 * front, back
 * emplace
************************************************************/
#include <queue>
#include <list>
#include <iostream>
#include <algorithm>
#include <string>

int main(int argc, char* argv[])
{
    std::queue<int> q_val0;
    q_val0.push(0);
    q_val0.push(5);
    q_val0.push(4);
    while(!q_val0.empty())
    {
        std::cout<<q_val0.front()<<" ";
        q_val0.pop();
    }
    std::cout<<std::endl;
    std::cout<<q_val0.size()<<std::endl;

    q_val0.push(2);
    q_val0.push(6);
    std::queue<int> q_val1;
    q_val1.swap(q_val0);

    std::cout<<"swap\n"<<"q_val0 size:"<<q_val0.size()<<std::endl;
    std::cout<<"q_val0 size:"<<q_val1.size()<<std::endl;
    std::cout<<q_val1.front()<<" "<<q_val1.back()<<std::endl;

    std::list<std::string> list_str{"hello", "test", "vector", "and", "queue"};
    std::queue<std::string, std::list<std::string>> qv_str0(list_str);
    while(!qv_str0.empty())
    {
        std::cout<<qv_str0.front()<<" ";
        qv_str0.pop();
    }
    std::cout<<std::endl;

    qv_str0.push("copy");
    qv_str0.push("and");
    qv_str0.push("move");
    qv_str0.push("test");
    auto qv_str1 = qv_str0;
    if(qv_str1 == qv_str1)
    {
        std::cout<<"equal val\n"<<std::endl;
    }

    //右值引用
    auto qv_str2 = std::move(qv_str1);
    std::cout<<"q_str1 size:"<<qv_str1.size()<<std::endl;
    std::cout<<"q_str2 size:"<<qv_str2.size()<<std::endl;

    auto &&qv_str3 = std::move(qv_str2);
    std::cout<<"q_str2 size:"<<qv_str2.size()<<std::endl;
    std::cout<<"q_str3 size:"<<qv_str3.size()<<std::endl;

    //功能上等同于push, 可以减少资源的销毁和复制动作
    qv_str2.emplace("OK");
    std::cout<<"q_str2 size:"<<qv_str2.size()<<std::endl;
    std::cout<<"q_str3 size:"<<qv_str3.size()<<std::endl;

}
