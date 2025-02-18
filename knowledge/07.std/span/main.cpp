/*
std::span
std::span 是 C++20 引入的一个容器视图类，它提供了一种轻量级的、非拥有的方式来访问连续的对象序列

主要特点：
1. 非拥有性：std::span 不拥有它所指向的数据，它只是提供了对数据的访问接口。这意味着 std::span 不会分配或释放内存，它只是引用了已经存在的内存。
2. 连续内存访问：std::span 要求它所指向的数据是连续存储的，这使得它可以高效地进行随机访问和迭代。
3. 大小和范围：std::span 可以在编译时或运行时指定其大小，这使得它可以灵活地适应不同的使用场景。
4. 与标准库的集成：std::span 可以与标准库中的其他容器和算法无缝集成，使得代码更加简洁和高效。

注意:
span是对其它内存的视图引用，因此使用时不要修改原始的数据，如引用已经释放的内存或者改变原数组的大小
*/
#if __cplusplus < 202002
#error "span is support by cpp20"
#endif

#include <span>
#include <iostream>
#include <vector>
#include <algorithm>

template <class T>
void print_span(const std::span<T> &arr)
{
    for (auto x : arr) {
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;
}

int main(int argc, char* argv[])
{
    int arr[] = {1, 2, 3, 4};

    //静态span，编译时固定，不能修改
    std::span<int, 4> s(arr);
    print_span<int>(s);
    std::cout<<s.size()<<std::endl;

    //动态span
    std::span<int> s1(arr);
    
    //获取子视图
    auto s2 = s1.subspan(0, 2);
    print_span(s2);

    auto s3 = s1.subspan<0, 2>();
    print_span<int>(s3);

    //span是对其它内存的视图引用，不要修改原始的数据
    //vec修改，无论是push还是pop都影响span的应用
    std::vector vec = {1, 4, 2, 3, 5};
    std::span<int> s4(vec);
    print_span(s4);

    std::sort(s4.begin(), s4.end());
    print_span(s4);
    return 0;
} 