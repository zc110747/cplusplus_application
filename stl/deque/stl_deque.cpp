/**********************************************************
 * 说明: deque 
 * 双向开口的连续线性空间容器
 * size             获取容器的当前大小
 * max_size         获取容器的最大允许容量
 * empty            判断容器是否为空
 * push_back        在容器的末尾插入数据
 * push_front       在容器的前端插入数据
 * pop_back         在容器的末尾删除数据
 * pop_front        在容器的前端删除数据
 * swap             交换两个容器内的信息，另外一个将会被清空
 * erase clear      容器删除/清空数据
 * insert           向容器内插入数据
 * emplace          向容器插入数据(避免不必要的临时对象产生,可选电阻
 * emplace_back     在容器尾部插入数据(右值引用)
 * emplace_front    在容器前端插入数据(右值引用)
 * begin, rbegin    容器的首地址和翻转序列的首地址
 * end, rend        容器的末尾地址和翻转序列的末尾地址
 * shrink_to_fit    收缩容量以减少内存占用
************************************************************/
#include <deque>
#include <list>
#include <iostream>
#include <algorithm>
#include <string>

template<typename T>
void show_deque(std::string str, T &deque_val)
{
    std::cout<<str;
    for(auto val:deque_val)
    {
        std::cout<<val<<" ";
    }
    std::cout<<std::endl;
}

//重载运算符，支持结构体的直接输出
template<typename T>
std::ostream &operator<<(std::ostream &os, const std::deque<T> &deque_val) {
    for(auto val:deque_val)
    {
        os<<val<<" ";
    }
    return os;
}


int main(int argc, char* argv[])
{
    //push_front, push_back, pop_back, pop_front
    //size, max_size, 
    std::deque<int> deque_int0;
    deque_int0.push_front(2);
    deque_int0.push_front(1);
    deque_int0.push_back(3);
    deque_int0.push_back(4);
    deque_int0.push_back(5);
    deque_int0.push_front(0);
    show_deque("deque_int0 val:", deque_int0);
    std::cout<<"deque_int0 size:"<<deque_int0.size()<<std::endl;
    std::cout<<"deque max_size:0x"<<std::hex<<deque_int0.max_size()<<std::endl;
    deque_int0.pop_back();
    deque_int0.pop_front();
    std::cout<<"deque_int0 val:"<<deque_int0<<std::endl;

    //begin, rbegin, end, rend
    std::cout<<"begin val:"<<*deque_int0.begin()<<" "<<"rbegin val:"<<*deque_int0.rbegin()<<std::endl;

    //swap, empty
    std::deque<int> deque_int1;
    deque_int1.swap(deque_int0);
    std::cout<<"deque_int1 val:"<<deque_int1<<std::endl;
    std::cout<<"deque_int0 empty:"<<deque_int0.empty()<<std::endl;
    deque_int1.clear();
    std::cout<<"deque_int1empty:"<<deque_int1.empty()<<std::endl;

    //emplace, emplace_front, emplace_back, emplace, insert
    std::deque<std::string> deque_str0 = {"hello,", "this", "is", "deque", "test"};
    std::cout<<"deque_str0 val:"<<deque_str0<<std::endl;
    std::cout<<"deque_str0 val:";
    deque_str0.emplace_front("now,");
    deque_str0.emplace_back("that is ok!");
    std::for_each(deque_str0.begin(),  deque_str0.end(), [](std::string &val){
        std::cout<<val<<" ";
    });
    std::cout<<std::endl;
    deque_str0.emplace(deque_str0.begin()+2, "Lucy");
    std::cout<<deque_str0<<std::endl;
    deque_str0.insert(deque_str0.begin()+2, "My");
    std::cout<<deque_str0<<std::endl;

    //erase
    for(auto iter=deque_str0.begin(); iter!=deque_str0.end();)
    {
        if(*iter == "hello,")
        {
            iter = deque_str0.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    std::cout<<deque_str0<<std::endl;

    deque_str0.shrink_to_fit();
    std::cout<<deque_str0.size()<<std::endl;
}
