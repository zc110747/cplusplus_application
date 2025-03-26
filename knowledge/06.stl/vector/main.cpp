/*
std::vector
https://www.cplusplus.com/reference/vector/vector/

std::vector 是 C++ 标准模板库（STL）中的一个容器，它表示一个动态数组，能够在运行时改变其大小。std::vector 是一个模板类，因此可以存储任何类型的元素。以下是 std::vector 的一些关键特性和常用操作：

特性:
1. 动态大小：std::vector 的大小可以在运行时动态增长或缩小。
2. 连续存储：元素在内存中是连续存储的，这使得随机访问非常高效。
3. 自动内存管理：std::vector 负责管理其内部的内存，包括分配和释放。
4. 高效的插入和删除：在末尾插入和删除元素的操作通常是高效的（摊销常数时间复杂度），但在中间或开头插入/删除元素可能会导致元素的移动，效率较低。

常用操作:
插入元素：使用 push_back 在末尾插入元素，使用 insert 在指定位置插入元素。
访问元素：使用 operator[] 或 at 方法访问元素。
删除元素：使用 pop_back 删除末尾元素，使用 erase 删除指定位置的元素。
获取大小和容量：使用 size 获取当前元素个数，使用 capacity 获取当前分配的内存空间大小。
调整大小：使用 resize 改变容器的大小，使用 reserve 预分配内存空间

接口说明:
assign      清除矢量并将指定的元素复制到该空矢量。
at          返回对矢量中指定位置的元素的引用。
back        返回对向量中最后一个元素的引用。
begin       对该向量中第一个元素返回随机访问迭代器。
capacity    返回在不分配更多的存储的情况下向量可以包含的元素数。
cbegin      返回指向向量中第一个元素的随机访问常量迭代器。
cend        返回一个随机访问常量迭代器，它指向刚超过矢量末尾的位置。
crbegin     返回一个指向反向矢量中第一个元素的常量迭代器。
crend       返回一个指向反向矢量末尾的常量迭代器。
clear       清除向量的元素。
data        返回指向向量中第一个元素的指针。
emplace     将就地构造的元素插入到指定位置的向量中。
emplace_back 将一个就地构造的元素添加到向量末尾。
empty       测试矢量容器是否为空。
end         返回指向矢量末尾的随机访问迭代器。
erase       从指定位置删除向量中的一个元素或一系列元素。
front       返回对向量中第一个元素的引用。
get_allocator 将对象返回到矢量使用的 allocator 类。
insert      将一个元素或多个元素插入到指定位置的向量中。
max_size    返回向量的最大长度。
pop_back    删除矢量末尾处的元素。
push_back   在矢量末尾处添加一个元素。
rbegin      返回指向反向向量中第一个元素的迭代器。
rend        返回一个指向反向矢量末尾的迭代器。
reserve     保留向量对象的最小存储长度。
resize      为矢量指定新的大小。
shrink_to_fit 放弃额外容量。
size        返回向量中的元素数量。
swap        交换两个向量的元素
operator[]  返回对指定位置的矢量元素的引用。
operator=   用另一个向量的副本替换该向量中的元素

总结:
push_back: 当传入的是对象实例时，它会调用对象的拷贝构造函数或者移动构造函数，把传入的对象复制或者移动到容器里。如果是临时对象，则会调用对象的移动构造函数。
emplace_back: 它会在容器的末尾直接构造对象，也就是就地构造，不需要进行对象的拷贝或者移动操作，或者直接使用传入的参数调用对象的构造函数，性能更优。
*/
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl;

typedef enum
{
    SHOW_MODE_ENUM_LAMBDA = 0,
    SHOW_MODE_ENUM_TYPE_AUTO,
    SHOW_MODE_ENUM_TYPE_ITERATOR,
    SHOW_MODE_ENUM_TYPE_OPERATOR
}SHOW_MODE_ENUM;

template<typename T>
void show_vector(std::vector<T> value, string qlabel, SHOW_MODE_ENUM mode = SHOW_MODE_ENUM_LAMBDA)
{
    //empty, size
    if(!qlabel.empty())
    {
        for(auto index=qlabel.size(); index<13; index++)
            qlabel.push_back(' ');
        qlabel += ": ";
        cout<<qlabel;
    }

    //begin, end
    if(mode == SHOW_MODE_ENUM_LAMBDA)
    {
        std::for_each(value.begin(), value.end(), [](const T &ref_value){
            cout<<ref_value<<" ";
        });
    }

    //autoreference
    else if(mode == SHOW_MODE_ENUM_TYPE_AUTO)
    {
        for(auto &ref : value)
            cout<<ref<<" ";
    }

    //iterator
    else if(mode == SHOW_MODE_ENUM_TYPE_ITERATOR)
    {
        for(typename std::vector<T>::iterator iter=value.begin(); iter!=value.end(); iter++)
            cout<<*iter<<" ";
    }
    
    //operator[], size
    else if(mode == SHOW_MODE_ENUM_TYPE_OPERATOR)
    {
        for(int index=0; index<value.size(); index++)
            cout<<value[index]<<" ";
    }
    cout<<endl;
}

struct MyStruct
{
    int a;
    int b;
    MyStruct(int a, int b):a(a), b(b){}
};

std::ostream& operator<<(std::ostream& os, const MyStruct& obj)
{
    os << obj.a << " " << obj.b;
    return os;
}

int main(int argc, char* argv[])
{
    int array[5] = {8, 2, 1, 3, 5};
    std::vector<int> intv(array, array+5);
    std::vector<int> uservector(intv);
    std::vector<string> listvector;
    
    //operator=
    listvector = std::vector<string>({"0x00", "0x01", "0x02"}); //list initialize
    show_vector(listvector, "listvector");

    //clear, capacity, size, max_size
    listvector.clear();
    cout<<"capacity:"<<listvector.capacity()<<" | ";
    cout<<"size:"<<listvector.size()<<endl;
    cout<<"max_size:"<<listvector.max_size()<<endl;
    show_vector(uservector, "uservector");

    //assign
    uservector.assign(uservector.begin(), uservector.end()-1);
    show_vector(uservector, "assign", SHOW_MODE_ENUM_TYPE_AUTO);

    //at
    int &p = uservector.at(2);
    p = 10;
    show_vector(uservector, "at", SHOW_MODE_ENUM_TYPE_ITERATOR);

    //back
    int &end = uservector.back();
    end = 5;
    show_vector(uservector, "back", SHOW_MODE_ENUM_TYPE_OPERATOR);

    //capacity, reserve, resize, max_size
    cout<<"capactity:"<<uservector.capacity()<<endl;
    uservector.reserve(7);
    cout<<"reserve capactity:"<<uservector.capacity()<<endl;
    uservector.resize(3);
    uservector.shrink_to_fit();
    cout<<"fit capactity:"<<uservector.capacity()<<endl;
    cout<<"max_size:"<<uservector.max_size()<<endl;

    //begin,  end,  cbegin,  cend
    //rbegin, rend, rcbegin, rcend
    std::vector<int>::iterator iterbegin = uservector.begin();
    std::vector<int>::iterator iterend = uservector.end();
    std::vector<int>::const_iterator iterconstbegin = uservector.cbegin();
    std::vector<int>::const_iterator iterconstend = uservector.cend();
    std::vector<int>::reverse_iterator iterrbegin = uservector.rbegin();
    std::vector<int>::reverse_iterator iterrend = uservector.rend();
    std::vector<int>::const_reverse_iterator iterconstrbegin = uservector.crbegin();
    std::vector<int>::const_reverse_iterator iterconstrend = uservector.crend();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(iterend-1)<<" ";
    cout<<"cbegin:"<<*iterconstbegin<<" "<<"cend:"<<*(iterconstend-1)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(iterrend-1)<<" ";
    cout<<"crbegin:"<<*iterconstrbegin<<" "<<"crend:"<<*(iterconstrend-1)<<" "<<endl;

    //data
    int *ptr = uservector.data();
    ptr[0] = 4;
    ptr[2] = 3;
    show_vector(uservector, "data");

    //emplace, emplace_back
    auto it = uservector.emplace(uservector.begin()+1, 8);
    show_vector(uservector, "emplace");
    cout<<"empalce iterator:"<<*it<<endl;
    uservector.emplace_back(9);
    show_vector(uservector, "emplace_back");

    std::vector<MyStruct> myvector;
    myvector.emplace_back(MyStruct(1, 2));
    myvector.emplace_back(3, 4);
    for (const auto& element : myvector)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    //erase
    auto iter=uservector.begin();
    while(iter!=uservector.end())
    {
        if(*iter == 3)
        {
            iter=uservector.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    show_vector(uservector, "erase");

    //front
    int &front = uservector.front();
    front = 6;
    show_vector(uservector, "front");

    //insert
    uservector.insert(uservector.begin(), 2, 5);
    show_vector(uservector, "insert");

    //swap, operator=
    std::vector<int> swapvector = std::vector<int>();
    swapvector.swap(uservector);
    show_vector(swapvector, "swapvector");

    //pop_back, push_back
    swapvector.push_back(10);
    show_vector(swapvector, "push_back");
    swapvector.pop_back();
    show_vector(swapvector, "pop_back");

    return 0;
} 