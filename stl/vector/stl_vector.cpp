/**********************************************************
 * std::vector
 * 包含一系列相同变量的向量容器
 * size                 容器内当前数据的数目
 * max_size             容器内当前数据的最大数目
 * capacity             容器当前的容量
 * resize               以指定值扩展容器空间, 小于当前数目则删除
 * push_back, pop_back  向容器内写入或者弹出数据
 * insert               向容器的指定地址插入指定数目的数据
 * erase, clear         删除数据或者内部全部数据
 * reserve              增加容器的容量(大于当前容量则扩容，否则不变)
 * emplace_back         功能等同于push_back, 向队列插入数据(避免不必要的临时对象产生)
************************************************************/
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

template<class T>
void show_vector(std::vector<T> vector_val)
{
    //begin end
    std::for_each(vector_val.begin(), vector_val.end(), [](const T &ref_value){
        std::cout<<ref_value<<" ";
    });
    std::cout<<std::endl;
}

int main(int argc, char* argv[])
{
    //init
    int array[5] = {8, 2, 1, 3, 5};
    std::vector<int> vector_int0(array, array+5);
    std::vector<int> vector_int1(vector_int0);
    std::vector<int> vector_int2{5, 2, 1, 3, 1}; //列表初始化

    //push_back, pop_back，resize, size, max_size
    std::cout<<"vector_int1 val:";
    show_vector(vector_int1);
    vector_int1.push_back(7);
    vector_int1.insert(vector_int1.begin(), 1, 2);
    vector_int1.emplace_back(19);
    std::cout<<"vector_int1 val:";
    show_vector(vector_int1);
    vector_int1.pop_back();
    std::cout<<"vector_int1 val:";
    show_vector(vector_int1);
    vector_int1.resize(2, 3);
    std::cout<<"vector_int1 val:";
    show_vector(vector_int1);
    vector_int1.resize(8, 6); //扩展空间为8位,并以6赋值
    std::cout<<"vector_int1 val:";
    show_vector(vector_int1);
    std::cout<<"vector_int1 size:"<<vector_int1.size()<<std::endl;
    vector_int1.reserve(9);
    std::cout<<"vector_int1 val:";
    show_vector(vector_int1);
    std::cout<<"vector_int1 size:"<<vector_int1.capacity()<<std::endl;
    std::cout<<"vector_int1 max_size:"<<vector_int1.max_size()<<std::endl;

    //capacity, empty, clear, reserve
    std::cout<<"vector_int2 capacity:"<<vector_int2.capacity()<<std::endl;
    std::cout<<"vector_int2 empty:"<<vector_int2.empty()<<std::endl;
    vector_int2.clear(); 
    vector_int2.reserve(1);
    std::cout<<"capacity vector_int2:"<<vector_int2.capacity()<<std::endl;
    std::cout<<"vector_int2 empty:"<<vector_int2.empty()<<std::endl;

    //front, back
    std::cout<<"vector_int1 front:"<<vector_int1.front()<<std::endl;
    std::cout<<"vector_int1 end:"<<vector_int1.back()<<std::endl;

    //iterator
    std::cout<<"vector_int1 iterator val:";
    for(std::vector<int>::iterator iter0=vector_int1.begin(); iter0 != vector_int1.end(); iter0++)
    {
        std::cout<<*iter0<<" ";
    }
    std::cout<<std::endl;

    // //auto
    std::cout<<"vector_int1 iterator auto:";
    for(auto iter1=vector_int1.begin(); iter1 != vector_int1.end(); iter1++)
    {
        std::cout<<*iter1<<" ";
    }
    std::cout<<std::endl;

    // //sort, foreach, lambda
    std::cout<<"vector_int1 sort val ";
    std::sort(vector_int1.begin(), vector_int1.end(), [](int &t1, int &t2)->bool{
        if(t1<t2)
             return true;
        return false;
    });
    show_vector(vector_int1);

    std::vector<std::string> vector_str0{"hello", "vector", "test"};
    std::vector<std::string> vector_str1;
    std::cout<<"vector_str0 val:";
    show_vector(vector_str0);
    vector_str1.swap(vector_str0);
    std::cout<<"vector_str1 val:";
    show_vector(vector_str1);
    std::cout<<"vector_str0 empty:"<<vector_str0.empty()<<std::endl;
}
