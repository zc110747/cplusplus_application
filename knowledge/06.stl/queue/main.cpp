/*
std::queue 是 C++ 标准库中的一个容器适配器，它提供了一种先进先出（FIFO）的数据结构。

主要特点：
1. 先进先出：元素按照插入的顺序进行处理，先插入的元素先被取出。
2. 基于其他容器实现：std::queue 通常基于 std::deque 或 std::list 实现，但也可以使用其他容器。
3. 高效操作：插入和删除操作的时间复杂度为常数级别，即 O(1)。

构造函数：
queue();
explicit queue(const container_type& right);

成员函数：
back: 返回队列中最后一个元素的引用。
empty: 如果队列为空，则返回 true，否则返回 false。
front: 返回队列中第一个元素的引用。
pop: 移除队列中第一个元素。
push: 将元素添加到队列的末尾。
size: 返回队列中元素的数量。
swap: 交换两个队列的内容。
*/
#include <queue>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    //queue
    std::queue<string> usr_queue;

    //emplace, push, size, empty
    usr_queue.push("first");
    usr_queue.push("second");
    usr_queue.push("third");
    usr_queue.emplace("fourth");
    usr_queue.push("fifth");
    cout<<"size:"<<usr_queue.size()<<endl;
    cout<<"empty:"<<usr_queue.empty()<<endl;

    //front
    string &strfront = usr_queue.front();
    cout<<"front: "<<strfront<<endl;

    //back
    string &strback = usr_queue.back();
    cout<<"back: "<<strback<<endl;

    //swap
    std::queue<string> swapqueue;
    swapqueue.swap(usr_queue);

    //pop
    cout<<"swapqueue:";
    int size = swapqueue.size();
    for(int index=0; index<size; index++)
    {
        cout<<swapqueue.front()<<" ";
        swapqueue.pop();
    }
    cout<<endl;

    //priority
    std::priority_queue<int> userpriority_queue;

    //emplace, push, size, empty
    userpriority_queue.push(7);
    userpriority_queue.push(5);
    userpriority_queue.push(6);
    userpriority_queue.emplace(1);
    userpriority_queue.push(3);
    cout<<"pr_size:"<<userpriority_queue.size()<<endl;
    cout<<"pr_empty:"<<userpriority_queue.empty()<<endl;

    //swap
    std::priority_queue<int> swappriority_queue;
    swappriority_queue.swap(userpriority_queue);

    //pop
    cout<<"swappriority_queue:";
    int pr_size = swappriority_queue.size();
    for(int index=0; index<pr_size; index++)
    {
        cout<<swappriority_queue.top()<<" ";
        swappriority_queue.pop();
    }
    cout<<endl;

    return 0;
}
