/*
std::priority_queue 是 C++ 标准库中的一个容器适配器，它提供了一种优先队列的实现。

主要特点：
1. 自动排序：元素在插入时会自动按照优先级进行排序。默认情况下，使用 < 运算符进行比较，即最大元素总是位于队首。
2. 高效操作：插入和删除操作的时间复杂度为对数级别，即 O(log n)。
3. 基于堆实现：std::priority_queue 通常基于堆数据结构实现，这使得它在处理大量数据时非常高效。

构造函数:
priority_queue();
explicit priority_queue(const Traits& _comp);
priority_queue(const Traits& _comp, const container_type& _Cont);
priority_queue(const priority_queue& right);
template <class InputIterator>
priority_queue(InputIterator first, InputIterator last);
template <class InputIterator>
priority_queue(InputIterator first, InputIterator last, const Traits& _comp);
template <class InputIterator>
priority_queue(InputIterator first, InputIterator last, const Traits& _comp, const container_type& _Cont)

成员函数：
empty：判断队列是否为空。
pop：移除队首元素。
push：插入元素到队列。
size：返回队列中元素的个数。
top：返回队首元素的引用。
*/

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main(int argc, char** argv) 
{
    std::priority_queue<int> p_queue;

    // size, top, push, pop
    p_queue.push(1);
    p_queue.push(2);
    p_queue.push(3);
    std::cout<<"pp_queueq.size() = "<<p_queue.size()<<std::endl;
    std::cout<<"p_queue.top() = "<<p_queue.top()<<std::endl;

    while(!p_queue.empty()) {
        cout<<p_queue.top()<<", ";
        p_queue.pop();
    }
    cout<<endl;

    // 构造函数
    std::vector<int> vec = {1, 6, 4, 2, 3};
    std::priority_queue<int> p_queue2(vec.begin(), vec.begin()+3);
    while(!p_queue2.empty()) {
        cout<<p_queue2.top()<<", ";
        p_queue2.pop();
    }
    cout<<endl;

    std::priority_queue<int, std::vector<int>, std::greater<int>> p_queue3(vec.begin(), vec.begin()+3);
    while(!p_queue3.empty()) {
        cout<<p_queue3.top()<<", ";
        p_queue3.pop();
    }
    cout<<endl;
    
    return 0;
}
