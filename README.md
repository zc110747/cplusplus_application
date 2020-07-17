# cplusplus_application  
基于C++标准库的基础应用实践，用于深入熟悉和学习C++语言  
参考学习书籍《STL源码剖析》和《泛型编程和STL》  
内部需要用到类，模板的知识，且涉及少量的lambda，建议实践前掌握  
因为代码部分涉及到C++11及之后版本的实现，目前测试版本为g++9.3.0, 早期版本测试可能不通过

## stl/ 标准模板库, STL相关应用  
```cpp
//序列式容器
#include<vector>            /*数据集合的向量*/
#include<list>              /*数据集合的列表*/    
#include<deque>
#include<stack>             /*后进先出的堆栈容器*/
#include<queue>             /*先进先出的队列容器*/
#include<tuple>
//关联式容器
#include<set>               /*不重复的自动排序的元素集合*/
#include<map>               /*带映射关系的表集合*/
#include<multiset>          /*支撑重复的自动排序的元素集合*/
#include<multimap>
#include<hash_set>
#include<hash_map>
#include<hash_multiset>
#include<hash_multimap>
//算法
//支持的算法(常用)
//遍历for_each
//排序sort
//查找find, find_if, find_end, find_first_of
//复制copy
//重复元素折叠unique
//交换swap
#include<algorithm>
//
```

## std/ C++标准库应用
```cpp
#include<string>
#include<iostream>
#include<thread>
```

