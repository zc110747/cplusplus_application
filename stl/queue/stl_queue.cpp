/**********************************************************
 * 说明: queue 
 * 先进先出的队列结构
 * push，pop    队列的进出处理
 * size         队列内数据的长度
 * empty        判断队列是否为空
 * front, back  输出队列的起始值和末尾值
 * emplace      向队列插入数据(避免不必要的临时对象产生)
************************************************************/
#include <queue>
#include <list>
#include <iostream>
#include <algorithm>
#include <string>

int main(int argc, char* argv[])
{
    std::queue<int> queue_int0;
    queue_int0.push(0);
    queue_int0.push(5);
    queue_int0.push(4);
    while(!queue_int0.empty())
    {
        std::cout<<queue_int0.front()<<" ";
        queue_int0.pop();
    }
    std::cout<<std::endl;
    std::cout<<queue_int0.size()<<std::endl;

    queue_int0.push(2);
    queue_int0.push(6);
    std::queue<int> queue_int1;
    queue_int1.swap(queue_int0);
    std::cout<<"queue_int0 size:"<<queue_int0.size()<<std::endl;
    std::cout<<"queue_int0 size:"<<queue_int1.size()<<std::endl;
    std::cout<<queue_int1.front()<<" "<<queue_int1.back()<<std::endl;

    std::list<std::string> list_str{"hello", "test", "vector", "and", "queue"};
    std::queue<std::string, std::list<std::string>> queue_str0(list_str);
    while(!queue_str0.empty())
    {
        std::cout<<queue_str0.front()<<" ";
        queue_str0.pop();
    }
    std::cout<<std::endl;

    queue_str0.push("copy");
    queue_str0.push("and");
    queue_str0.push("move");
    queue_str0.push("test");
    auto queue_str1 = queue_str0;
    if(queue_str1 == queue_str1)
    {
        std::cout<<"equal val"<<std::endl;
    }

    //右值引用
    auto queue_str2 = std::move(queue_str1);
    std::cout<<"q_str1 size:"<<queue_str1.size()<<std::endl;
    std::cout<<"q_str2 size:"<<queue_str2.size()<<std::endl;

    auto &&qv_str3 = std::move(queue_str2);
    std::cout<<"q_str2 size:"<<queue_str2.size()<<std::endl;
    std::cout<<"q_str3 size:"<<qv_str3.size()<<std::endl;

    //功能上等同于push, 可以减少资源的销毁和复制动作
    queue_str2.emplace("OK");
    std::cout<<"q_str2 size:"<<queue_str2.size()<<std::endl;
    std::cout<<"q_str3 size:"<<qv_str3.size()<<std::endl;
}
