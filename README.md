# cplusplus_application  
基于C++标准库的基础应用实践，用于深入熟悉和学习C++语言  
参考学习书籍《STL源码剖析》和《泛型编程和STL》  
内部需要用到类，模板的知识，且涉及少量的lambda，建议实践前掌握  

## STL相关应用  
```cpp
//序列式容器
/*数据集合的向量*/
#include<vector>
/*数据集合的列表*/
#include<list>
#include<deque>
/*后进先出的堆栈容器*/
#include<stack>
/*先进先出的队列容器*/
#include<queue>
#include<priority_queue>
#include<tuple>
//关联式容器
/*不重复的自动排序的元素集合*/
#include<set>
/*带映射关系的表集合*/
#include<map>
#include<multiset>
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
```

## C++标准库应用
```cpp
#include<string>
#include<iostream>
#include<thread>
```

