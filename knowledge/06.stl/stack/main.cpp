/*
std::stack 是 C++ 标准库中的一个容器适配器，它提供了一种后进先出（LIFO）的数据结构。

主要特点
1. 后进先出：元素按照插入的顺序的逆序进行处理，最后插入的元素最先被取出。
2. 基于其他容器实现：std::stack 通常基于 std::deque 或 std::vector 实现，但也可以使用其他容器。
3. 高效操作：插入和删除操作的时间复杂度为常数级别，即 O(1)

构造函数
stack();
explicit stack(const container_type& right);

成员函数
empty：检查栈是否为空。
pop：移除栈顶元素。
push：将元素压入栈顶。
size：返回栈中元素的个数。
top：返回栈顶元素的引用。
swap：交换两个栈的内容。
需要注意的是，std::stack 没有提供访问栈中元素的迭代器，因此无法像其他容器一样使用迭代器进行遍历。
*/
#include <stack>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl;


int main(int argc, char* argv[])
{
    //stack
    std::stack<string> usr_stack;

    //emplace, push, size, empty
    usr_stack.push("first");
    usr_stack.push("second");
    usr_stack.push("third");
    usr_stack.emplace("fourth");
    usr_stack.push("fifth");
    cout<<"size:"<<usr_stack.size()<<endl;
    cout<<"empty:"<<usr_stack.empty()<<endl;

    //emplace
    usr_stack.emplace("sixth");

    //top
    string &strfront = usr_stack.top();
    cout<<"top: "<<strfront<<endl;

    //swap
    std::stack<string> swapstack;
    swapstack.swap(usr_stack);

    //pop
    cout<<"swapstack:";
    int size = swapstack.size();
    for(int index=0; index<size; index++)
    {
        cout<<swapstack.top()<<" ";
        swapstack.pop();
    }
    cout<<endl;

    return 0;
}
 