# stl

STL（Standard Template Library）是C++标准库的一部分，它基于template提供了一组通用的容器、算法和函数，用于处理数据结构和算法的实现。

- [std::array](#array)
- [std::deque](#deque)
- [std::forward_list](#forward_list)
- [std::list](#list)
- [std::map](#map)
- [std::multimap](#multimap)
- [std::multiset](#multiset)
- [std::priority_queue](#priority_queue)
- [std::queue](#queue)
- [std::set](#set)
- [std::stack](#stack)
- [std::unordered_map](#unordered_map)
- [std::unordered_multimap](#unordered_multimap)
- [std::unordered_multiset](#unordered_multiset)
- [std::unordered_set](#unordered_set)
- [std::vector](#vector)
- [algorithm](#algorithm)
- [返回主页](../../README.md)

## array

std::array它提供了固定大小的数组功能，它们保存按严格线性序列排序的特定数量的元素。

- 格式

```cpp
template < class T, size_t N > class array;
```

相关网址: <https://www.cplusplus.com/reference/array/array>

- 常用构造方法

```cpp
// 默认构造函数
std::array<int, 5> arr_0;

// 列表初始化
std::array<int, 5> arr_1{1, 2, 3, 4, 5};

// 赋值构造函数
std::array<int, 5> arr_3(arr_1);
```

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin| 返回指向数组第一个元素的迭代器。|
| end | 返回指向数组最后一个元素的后一个位置的迭代器。|
| rbegin | 返回指向数组最后一个元素的反向迭代器。|
| rend | 返回指向数组第一个元素的前一个位置的反向迭代器。|
| cbegin | 返回指向数组第一个元素的常量迭代器。|
| cend | 返回指向数组最后一个元素的后一个位置的常量迭代器。|
| crbegin | 返回指向数组最后一个元素的常量反向迭代器。|
| crend | 返回指向数组第一个元素的前一个位置的常量反向迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| size | 返回数组中元素的数量。|
| max_size | 返回数组可以容纳的最大元素数量，通常与 size() 相同。|
| empty | 检查数组是否为空，返回 bool 类型。|
| operator[] | 返回指定位置的元素的引用，不进行越界检查。|
| at | 返回指定位置的元素的引用，如果越界则抛出 std::out_of_range 异常。|
| front | 返回第一个元素的引用。|
| back | 返回最后一个元素的引用。|
| data | 返回指向数组第一个元素的指针。|
| fill | 将数组中的所有元素设置为指定的值。|
| swap | 交换两个数组的内容。|

## deque

std::deque它代表双端队列（double-ended queue）。双端队列是一种序列容器，允许在两端（前端和后端）快速插入和删除元素。

- 格式

```cpp
template < class T, class Alloc = allocator<T> > class deque;
```

相关网址: <https://www.cplusplus.com/reference/deque/deque>

- 常用构造方法

```cpp
// 默认构造函数
std::deque<int> c0;

// 列表初始化
std::deque<int> c1{1, 2, 3, 4, 5};

// 赋值构造函数
std::deque<int> c2(c1);

// 范围构造函数
std::deque<int> c3(c1.begin(), c1.end());

// 长度2
std::deque<int> c4(2);

// 长度2，值3
std::deque<int> c5(2, 3);
```

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向数组第一个元素的迭代器。|
| end | 返回指向数组最后一个元素的后一个位置的迭代器。|
| rbegin | 返回指向数组最后一个元素的反向迭代器。|
| rend | 返回指向数组第一个元素的前一个位置的反向迭代器。|
| cbegin | 返回指向数组第一个元素的常量迭代器。|
| cend | 返回指向数组最后一个元素的后一个位置的常量迭代器。|
| crbegin | 返回指向数组最后一个元素的常量反向迭代器。|
| crend | 返回指向数组第一个元素的前一个位置的常量反向迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| size | 返回双端队列中元素的个数。|
| max_size | 返回双端队列可以容纳的最大元素个数。|
| resize | 调整双端队列的大小。|
| empty | 检查双端队列是否为空 |
| shrink_to_fit | 减少双端队列的容量，使其与元素数量相同。|
| operator[] | 返回指定位置的元素引用。|
| at | 返回指定位置的元素引用。|
| front | 返回第一个元素的引用。|
| back | 返回最后一个元素的引用。|
| assign | 将指定范围内的元素赋值给双端队列。|
| push_back | 在双端队列的末尾添加一个元素。|
| push_front | 在双端队列的开头添加一个元素。|
| pop_back | 删除双端队列末尾的元素。|
| pop_front | 删除双端队列开头的元素。|
| insert | 在指定位置插入一个元素。|
| erase | 删除指定位置的元素。|
| swap | 交换两个双端队列的内容。|
| clear | 清空双端队列中的所有元素。|
| emplace |在指定位置构造一个元素。|
| emplace_back | 在双端队列的末尾构造一个元素。|
| emplace_front |  在双端队列的开头构造一个元素。|
| get_allocator | 返回用于分配内存的分配器对象。|

## forward_list

std::forward_list 它代表单向链表（singly linked list）。单向链表是一种线性数据结构，其中每个元素（节点）包含一个值和指向下一个元素的指针。

- 格式

```cpp
template < class T, class Alloc = allocator<T> > class forward_list;
```

相关网址: <https://www.cplusplus.com/reference/forward_list/forward_list>

- 常用构造方法

```cpp
// 默认构造函数
std::forward_list<int> c0;

// 列表初始化
std::forward_list<int> c1{1, 2, 3, 4, 5};

// 赋值构造函数
std::forward_list<int> c2(c1);

// 范围构造函数
std::forward_list<int> c3(c1.begin(), c1.end());

// 长度2
std::forward_list<int> c4(2);

// 长度2，值3
std::forward_list<int> c5(2, 3);
```

- 迭代器

| 方法 | 说明 |
| --- | --- |
| before_begin | 返回一个指向链表头部之前的位置的迭代器。|
| begin | 返回指向数组第一个元素的迭代器。|
| end | 返回指向数组最后一个元素的后一个位置的迭代器。|
| cbegin | 返回指向数组第一个元素的常量迭代器。|
| cend | 返回指向数组最后一个元素的后一个位置的常量迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查链表是否为空。|
| max_size | 返回链表可以容纳的最大元素数量。|
| front | 返回链表头部的元素的引用。|
| assign | 将链表中的元素替换为 count 个值为 value 的元素。|
| pop_front | 移除链表头部的元素。|
| push_front | 在链表头部插入一个元素。|
| emplace_after | 在指定位置之后插入一个新元素。|
| insert_after | 在指定位置之后插入一个或多个元素。|
| erase_after | 删除指定位置之后的元素。|
| swap | 交换两个链表的内容。|
| resize | 改变链表的大小。|
| clear | 清空链表中的所有元素。|
| splice_after | 将一个链表中的元素插入到另一个链表的指定位置之后。|
| remove | 移除链表中所有值为 value 的元素。|
| remove_if | 移除链表中满足特定条件的元素。|
| unique | 移除链表中连续的重复元素。|
| merge | 合并两个有序链表。|
| sort | 对链表中的元素进行排序。|
| reverse | 反转链表中的元素顺序。|
| get_allocator | 返回用于分配内存的分配器对象。|

## list 

std::list是C++标准库中的一个容器类，它提供了一种双向链表的数据结构。与std::forward_list不同，std::list允许在链表的任何位置进行插入和删除操作，并且提供了更多的成员函数和算法。

- 格式

```cpp
template < class T, class Alloc = allocator<T> > class list;
```

相关网址: <https://www.cplusplus.com/reference/list/list>

- 常用构造方法

```cpp
// 默认构造函数
std::list<int> c0;

// 列表初始化
std::list<int> c1{1, 2, 3, 4, 5};

// 赋值构造函数
std::list<int> c2(c1);

// 范围构造函数
std::list<int> c3(c1.begin(), c1.end());

// 长度2
std::list<int> c4(2);

// 长度2，值3
std::list<int> c5(2, 3);
```

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向链表第一个元素的迭代器。|
| end | 返回指向链表最后一个元素之后的位置的迭代器。|
| rbegin | 返回指向链表最后一个元素的反向迭代器。|
| rend | 返回指向链表第一个元素的前一个位置的反向迭代器。|
| cbegin | 返回指向链表第一个元素的常量迭代器。|
| cend | 返回指向链表最后一个元素之后的位置的常量迭代器。|
| crbegin | 返回指向链表最后一个元素的常量反向迭代器。|
| crend | 返回指向链表第一个元素的前一个位置的常量反向迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查链表是否为空。|
| size | 返回链表的元素数量。|
| max_size | 返回链表可以容纳的最大元素数量。|
| front | 返回链表的第一个元素的引用。|
| back | 返回链表的最后一个元素的引用。|
| assign | 将新元素分配给链表。|
| emplace_front | 在链表的开头构造一个元素。|
| push_front | 在链表的开头添加一个元素。|
| pop_front | 移除链表的第一个元素。|
| emplace_back | 在链表的末尾构造一个元素。|
| pop_back | 移除链表的最后一个元素。|
| emplace | 在链表的指定位置构造一个元素。|
| insert | 在链表的指定位置插入一个或多个元素。|
| erase | 移除链表中的一个或多个元素。|
| swap | 交换两个链表的内容。|
| clear | 移除链表中的所有元素。|
| splice | 将一个链表中的元素插入到另一个链表的指定位置。|
| remove | 移除链表中所有等于value的元素。|
| remove_if | 移除链表中满足特定条件的元素。|
| unique | 移除链表中连续的重复元素。|
| merge | 合并两个有序链表。|
| sort | 对链表中的元素进行排序。|
| reserse | 反转链表的元素顺序。|
| get_allocator | 返回链表的分配器对象。|

## map

std::map是C++ 标准库中的一个关联容器，它存储的元素是键值对（key-value pairs）。每个元素由一个键（key）和一个值（value）组成，键是唯一的，每个键对应一个值。其中的元素按照键的顺序自动排序，默认情况下是按照升序排列，支持自定义比较函数进行键值的比较排序。

- 格式

```cpp
template < class Key,                                   // map::key_type           
        class T,                                        // map::mapped_type           
        class Compare = less<Key>,                      // map::key_compare           
        class Alloc = allocator<pair<const Key,T>> >    // map::allocator_type
class map;
```

相关网址: https://www.cplusplus.com/reference/map/map/

- 常用构造方法

```cpp
// 默认构造函数
std::map<int, int> c0;

// 列表初始化
std::map<int, int> c1 = {
    {1, 1},
    {2, 2},
};

// 赋值构造函数
std::map<int, int> c2(c1);

// 范围构造函数
std::map<int, int> c3(c1.begin(), c1.end());

// 结构体对象
struct classcomp{
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};
std::map<int, int, classcomp> c4;

// lambda对象初始化
constexpr auto funcomp = [](const char& lhs, const char& rhs) {return lhs<rhs;};
std::map<int, int, decltype(funcomp)> c5(funcomp);
```

- 主要特性

1. 有序性：map中的元素按照键的顺序排列，这使得查找、插入和删除操作的时间复杂度为对数级别（O(log n)）
2. 唯一性：每个键在map中只能出现一次
3. 动态大小：map的大小可以动态增长或缩小
4. 高效查找：由于元素是有序的，map支持快速查找操作

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回一个迭代器，此迭代器指向map中的第一个元素。|
| end | 返回一个超过末尾迭代器。|
| rbegin | 返回一个反向迭代器，此迭代器指向map中的最后一个元素。|
| rend | 返回一个超过末尾反向迭代器。|
| cbegin | 返回一个常量迭代器，此迭代器指向map中的第一个元素。|
| cend | 返回一个超过末尾常量迭代器。|
| crbegin | 返回一个常量反向迭代器，此迭代器指向反向map中的第一个元素。|
| crend | 返回一个常量反向迭代器，此迭代器指向反向map中最后一个元素之后的位置。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查map是否为空。|
| size | 返回map中的元素数量。|
| max_size | 返回map可以容纳的最大元素数量。|
| operator[] | 访问或插入具有指定键的元素。|
| operator= | 赋值运算符，将一个map赋值给另一个 map。|
| at | 查找具有指定键值的元素。|
| insert | 插入一个元素或元素范围。|
| erase | 从map中移除一个元素或元素范围。|
| swap | 交换两个map的内容。|
| clear | 清除map的所有元素。|
| emplace | 原位构造一个元素并将其插入到map中。|
| emplace_hint | 原位构造一个元素并将其插入到map中，附带位置提示。|
| key_comp | 返回用于对map中的键进行排序的比较对象副本。|
| value_comp | 返回用于对map中的值进行排序的比较对象副本。|
| find | 返回一个迭代器，此迭代器指向map中其键与指定键相等的元素的位置。|
| count | 返回映射中其键与参数中指定的键匹配的元素数量。|
| lower_bound | 返回一个迭代器，此迭代器指向map中其键值等于或大于指定键的键值的第一个元素。|
| upper_bound | 返回一个迭代器，此迭代器指向map中其键值大于指定键的键值的第一个元素。|
| equal_range | 返回一对迭代器, 此迭代器对中的第一个迭代器指向map中其键大于指定键的第一个元素。|
| get_allocator | 返回用于构造 allocator 的map对象的副本。|

## multimap

std::multimap 是C++标准库中的一个关联容器，它允许存储多个具有相同键的键值对。

- 格式

```cpp
template < class Key,                                  // multimap::key_type           
        class T,                                       // multimap::mapped_type           
        class Compare = less<Key>,                     // multimap::key_compare           
        class Alloc = allocator<pair<const Key,T> >    // multimap::allocator_type           
      > class multimap; 
```

相关网址: https://cplusplus.com/reference/map/multimap/

- 常用构造方法

```cpp
// 默认构造函数
std::multimap<int, int> c0;

// 列表初始化
std::multimap<int, int> c1 = {
    {2, 1},
    {2, 2},
};

// 赋值构造函数
std::multimap<int, int> c2(c1);

// 范围构造函数
std::multimap<int, int> c3(c1.begin(), c1.end());

// 结构体对象
struct classcomp{
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};
std::multimap<int, int, classcomp> c4;

// lambda对象初始化
constexpr auto funcomp = [](const char& lhs, const char& rhs) {return lhs<rhs;};
std::multimap<int, int, decltype(funcomp)> c5(funcomp);
```

- 主要特性

1. 一个键对应多个值：与 map 不同，multimap 允许用相同的键存储多个元素
2. 有序容器：元素会根据键自动排序（对于相同键的元素，还会根据值排序）
3. 关联容器：提供基于键的高效查找操作

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回一个迭代器，此迭代器指向map中的第一个元素。|
| end | 返回一个超过末尾迭代器。|
| rbegin | 返回一个反向迭代器，此迭代器指向map中的最后一个元素。|
| rend | 返回一个超过末尾反向迭代器。|
| cbegin | 返回一个常量迭代器，此迭代器指向map中的第一个元素。|
| cend | 返回一个超过末尾常量迭代器。|
| crbegin | 返回一个常量反向迭代器，此迭代器指向反向map中的第一个元素。|
| crend | 返回一个常量反向迭代器，此迭代器指向反向map中最后一个元素之后的位置。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查multimap是否为空。|
| size | 返回multimap中的元素数量。|
| max_size | 返回multimap可以容纳的最大元素数量。|
| operator= | 赋值运算符，将一个multimap赋值给另一个multimap。|
| insert | 插入一个元素或元素范围。|
| erase | 从multimap中移除一个元素或元素范围。|
| swap | 交换两个multimap的内容。|
| clear | 清除multimap的所有元素。|
| emplace | 原位构造一个元素并将其插入到multimap中。|
| emplace_hint | 原位构造一个元素并将其插入到multimap中，附带位置提示。|
| key_comp | 返回用于对multimap中的键进行排序的比较对象副本。|
| value_comp | 返回用于对multimap中的值进行排序的比较对象副本。|
| find | 返回一个迭代器，此迭代器指向multimap中其键与指定键相等的元素的位置。|
| count | 返回映射中其键与参数中指定的键匹配的元素数量。|
| lower_bound | 返回一个迭代器，此迭代器指向multimap中其键值等于或大于指定键的键值的第一个元素。|
| upper_bound | 返回一个迭代器，此迭代器指向multimap中其键值大于指定键的键值的第一个元素。|
| equal_range | 返回一对迭代器, 此迭代器对中的第一个迭代器指向map中其键大于指定键的第一个元素。|
| get_allocator | 返回用于构造 allocator 的multimap对象的副本。|

## multiset

std::multiset是按照特定顺序存储元素的容器，其中多个元素可以具有相同的值。

- 格式

```cpp
template < class T,                     // multiset::key_type/value_type           
        class Compare = less<T>,        // multiset::key_compare/value_compare           
        class Alloc = allocator<T> >    // multiset::allocator_type           
class multiset;
```

相关网址: https://cplusplus.com/reference/set/multiset/

- 常用构造方法

```cpp
// 默认构造函数
std::multiset<int> c0;

// 列表初始化
std::multiset<int> c1 = {1, 2, 3, 4, 5};

// 赋值构造函数
std::multiset<int> c2(c1);

// 范围构造函数
std::multiset<int> c3(c1.begin(), c1.end());

// 结构体对象
struct classcomp{
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};
std::multiset<int, classcomp> c4;

// lambda对象初始化
constexpr auto funcomp = [](const char& lhs, const char& rhs) {return lhs<rhs;};
std::multiset<int, decltype(funcomp)> c5(funcomp);
```

- 主要特性

1. 自动排序：元素在插入时会自动按照排序准则进行排序。
2. 允许重复元素：与 std::set 不同，std::multiset 允许存储多个具有相同键的元素。
3. 高效查找：由于元素是有序的，查找操作的时间复杂度为对数级别。

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向容器中第一个元素的迭代器。|
| end | 返回指向容器中最后一个元素之后位置的迭代器。|
| rbegin | 返回指向容器中第一个元素的反向迭代器。|
| rend | 返回指向容器中第一个元素之前位置的迭代器。|
| cbegin | 返回指向容器中第一个元素的常量迭代器。|
| cend | 返回指向容器中最后一个元素之后位置的常量迭代器。|
| crbegin | 返回指向容器中第一个元素的常量反向迭代器。|
| crend | 返回指向容器中最后一个元素之后位置的常量反向迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查容器是否为空。|
| size | 返回容器中的元素数量。|
| max_size | 返回容器可以容纳的最大元素数量。|
| insert | 插入一个元素或元素范围。|
| erase | 从容器中删除一个或多个元素。|
| swap | 交换两个容器的内容。|
| clear | 清空容器中的所有元素。|
| emplace | 在容器中直接构造元素。|
| emplace_hint | 在容器中的指定位置直接构造元素。|
| key_comp | 返回用于对容器中的键进行排序的比较对象副本。|
| value_comp | 获取用于比较容器中值的函数对象。|
| find | 在容器中查找具有特定键的元素。|
| count | 返回容器中具有特定键的元素数量。|
| lower_bound | 返回一个迭代器，此迭代器指向容器中其键值等于或大于指定键的键值的第一个元素。|
| upper_bound | 返回一个迭代器，此迭代器指向容器中其键值大于指定键的键值的第一个元素。|
| equal_range | 返回一对迭代器, 此迭代器对中的第一个迭代器指向map中其键大于指定键的第一个元素。|
| get_allocator | 获取用于构造容器的分配器对象的副本。|

## priority_queue

std::priority_queue是C++标准库中的一个容器适配器，它提供了一种优先队列的实现。

- 格式

```cpp
template <class T, 
        class Container = vector<T>,  
        class Compare = less<typename Container::value_type> > 
class priority_queue;
```

相关网址：https://cplusplus.com/reference/queue/priority_queue/

- 常用构造方法

```cpp
// 默认构造函数
std::priority_queue<int> c0;

// 自定义allocator构造函数
std::priority_queue<int, std::vector<int>> c1;

// 赋值构造函数
std::priority_queue<int> c2(c1);

// 结构体对象
struct classcomp{
    bool operator() (const char& lhs, const char& rhs) const
    {return lh < rhs;}
};
std::priority_queue<int, std::vector<int>, classcomp> c3;

// lambda对象初始化
constexpr auto funcomp = [](const char& lhs, const char& rhs) {return lhs<rhs;};
std::priority_queue<int, std::vector<int>, decltype(funcomp)> c4(funcomp);
```

- 主要特点

1. 自动排序：元素在插入时会自动按照优先级进行排序。默认情况下，使用 < 运算符进行比较，即最大元素总是位于队首。
2. 高效操作：插入和删除操作的时间复杂度为对数级别，即 O(log n)。
3. 基于堆实现：std::priority_queue通常基于堆数据结构实现，这使得它在处理大量数据时非常高效。

- 迭代器

无

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查队列是否为空。|
| size | 返回队列中元素的数量。|
| top | 返回队列中第一个元素的引用。|
| push | 将元素添加到队列的末尾。|
| emplace | 在队列的末尾直接构造元素。|
| pop | 移除队列中第一个元素。|
| swap | 交换两个队列的内容。|
| uses_allocator | 获取用于构造队列的分配器对象的副本。|

## queue

std::queue是C++ 标准库中的一个容器适配器，它提供了一种先进先出（FIFO）的数据结构。

- 格式

```cpp
template <class T, 
        class Container = deque<T> > 
class queue;
```

相关网址：https://cplusplus.com/reference/queue/queue/

- 常用构造方法

```cpp
// 默认构造函数
std::queue<int> c0;

// 自定义allocator构造函数
std::queue<int, std::vector<int>> c1;

// 赋值构造函数
std::queue<int> c2(c0);
```

- 主要特点

1. 先进先出：元素按照插入的顺序进行处理，先插入的元素先被取出。
2. 基于其他容器实现：std::queue 通常基于 std::deque 或 std::list 实现，但也可以使用其他容器。
3. 高效操作：插入和删除操作的时间复杂度为常数级别，即 O(1)。

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查队列是否为空。|
| size | 获取队列中的元素数量。|
| front | 返回队列中第一个元素的引用。|
| back | 返回队列中最后一个元素的引用。|
| push | 将元素添加到队列的末尾。|
| emplace | 在队列的末尾直接构造元素。|
| pop | 移除队列中第一个元素。|
| swap | 交换两个队列的内容。|
| uses_allocator | 获取用于构造队列的分配器对象的副本。|

## set

std::set是按照特定顺序存储元素的容器，其中多个元素可以具有相同的值。

- 格式

```cpp
template < class T,          // set::key_type/value_type           
        class Compare = less<T>,        // set::key_compare/value_compare           
        class Alloc = allocator<T> >    // set::allocator_type
class set;
```

相关网址: https://cplusplus.com/reference/set/set/

- 常用构造方法

```cpp
// 默认构造函数
std::set<int> c0;

// 列表初始化
std::set<int> c1 = {1, 2, 3, 4, 5};

// 赋值构造函数
std::set<int> c2(c1);

// 范围构造函数
std::set<int> c3(c1.begin(), c1.end());

// 结构体对象
struct classcomp{
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};
std::set<int, classcomp> c4;

// lambda对象初始化
constexpr auto funcomp = [](const char& lhs, const char& rhs) {return lhs<rhs;};
std::set<int, decltype(funcomp)> c5(funcomp);
```

- 主要特性

1. 元素唯一性：std::set 中的每个元素都是唯一的，不允许重复。
2. 自动排序：元素在插入时会自动按照排序准则进行排序。
3. 高效查找：由于元素是有序的，查找操作的时间复杂度为对数级别，即 O(log n)

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向容器中第一个元素的迭代器。|
| end | 返回指向容器中最后一个元素之后位置的迭代器。|
| rbegin | 返回指向容器中第一个元素的反向迭代器。|
| rend | 返回指向容器中第一个元素之前位置的迭代器。|
| cbegin | 返回指向容器中第一个元素的常量迭代器。|
| cend | 返回指向容器中最后一个元素之后位置的常量迭代器。|
| crbegin | 返回指向容器中第一个元素的常量反向迭代器。|
| crend | 返回指向容器中最后一个元素之后位置的常量反向迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查容器是否为空。|
| size | 返回容器中的元素数量。|
| max_size | 返回容器可以容纳的最大元素数量。|
| insert | 插入一个元素或元素范围。|
| erase | 从容器中删除一个或多个元素。|
| swap | 交换两个容器的内容。|
| clear | 清空容器中的所有元素。|
| emplace | 在容器中直接构造元素。|
| emplace_hint | 在容器中的指定位置直接构造元素。|
| key_comp | 返回用于对容器中的键进行排序的比较对象副本。|
| value_comp | 获取用于比较容器中值的函数对象。|
| find | 在容器中查找具有特定键的元素。|
| count | 返回容器中具有特定键的元素数量。|
| lower_bound | 返回一个迭代器，此迭代器指向容器中其键值等于或大于指定键的键值的第一个元素。|
| upper_bound | 返回一个迭代器，此迭代器指向容器中其键值大于指定键的键值的第一个元素。|
| equal_range | 返回一对迭代器, 此迭代器对中的第一个迭代器指向map中其键大于指定键的第一个元素。|
| get_allocator | 获取用于构造容器的分配器对象的副本。|

## stack

std::stack是C++标准库中的一个容器适配器，它提供了一种后进先出（LIFO）的数据结构。

- 格式

```cpp
template <class T, class Container = deque<T> > class stack;
```

相关网址: https://cplusplus.com/reference/stack/stack/

- 常用构造方法

```cpp
// 默认构造函数
std::stack<int> c0;

// 自定义allocator构造函数
std::stack<int, std::vector<int>> c1;

// 赋值构造函数
std::stack<int> c2(c0);
```

- 主要特点

1. 后进先出：元素按照插入的顺序的逆序进行处理，最后插入的元素最先被取出。
2. 基于其他容器实现：std::stack通常基于std::deque或std::vector实现，但也可以使用其他容器。
3. 高效操作：插入和删除操作的时间复杂度为常数级别，即O(1)

- 迭代器

无

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 检查栈是否为空。|
| size | 返回栈中元素的个数。|
| top | 返回栈顶元素的引用。|
| push | 将元素压入栈顶。|
| emplace | 在栈顶直接构造元素。|
| pop | 移除栈顶元素。|
| swap | 交换两个栈的内容。|
| uses_allocator | 获取用于构造栈的分配器对象的副本。|

## unordered_map

std::unordered_map是C++标准库中的一个关联容器，它提供了一种键值对的存储方式，其中键是唯一的，而值可以重复。unordered_map和map的区别是，unordered_map使用哈希表作为底层数据结构，并不进行排序。

- 格式

```cpp
template < class Key,                                   // unordered_map::key_type           
        class T,                                        // unordered_map::mapped_type           
        class Hash = hash<Key>,                         // unordered_map::hasher           
        class Pred = equal_to<Key>,                     // unordered_map::key_equal           
        class Alloc = allocator< pair<const Key,T> >    // unordered_map::allocator_type           
        > class unordered_map;
```

相关网址: https://www.cplusplus.com/reference/unordered_map/unordered_map/

- 常用构造方法

```cpp
// 默认构造函数
std::unordered_map<int, std::string> c0 = {
{1, "one"}
};

// 自定义allocator构造函数
std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<const int, std::string>>> c1;

// 赋值构造函数
std::unordered_map<int, std::string> c2(c0);

// 范围构造函数
std::unordered_map<int, std::string> c3(c0.begin(), c0.end());
```

- 主要特点

1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于键的顺序。
2. 快速查找：由于使用哈希表实现，查找操作非常高效。
3. 键唯一性：每个键在容器中只能出现一次。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向容器中第一个元素的迭代器。|
| end | 返回指向容器中最后一个元素之后位置的迭代器。|
| cbegin | 返回指向容器中第一个元素的常量迭代器。|
| cend | 返回指向容器中最后一个元素之后位置的常量迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| empty | 判断容器是否为空。|
| size | 返回容器中元素的个数。|
| max_size | 返回容器中可容纳的元素最大个数。|
| operator= | 拷贝赋值。|
| operator[] | 访问元素。|
| at | 访问元素。|
| find | 查找元素。|
| count | 统计元素个数。|
| equal_range | 查找元素范围。|
| emplace | 在容器中直接构造元素。|
| emplace_hint | 在容器中的指定位置直接构造元素。|
| insert | 插入一个元素或元素范围。|
| erase | 从容器中删除一个或多个元素。|
| clear | 清空容器中的所有元素。|
| swap | 交换两个容器的内容。|
| bucket_count | 获取存储桶的数量。|
| max_bucket_count | 获取存储桶的最大数量。|
| bucket_size | 获取存储桶中元素的数量。|
| bucket | 获取元素所在的存储桶编号。|
| load_factor | 获取负载因子。|
| max_load_factor | 获取负载因子的最大值。|
| rehash | 重新计算容器的存储桶数量。|
| reserve | 预留空间。|
| hash_function | 获取哈希函数。|
| key_eq | 获取比较函数。|
| get_allocator | 获取用于构造容器的分配器对象的副本。|

## unordered_multimap

std::unordered_multimap 是 C++ 标准库中的一个关联容器，它允许存储多个具有相同键的键值对。

主要特点：
1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于键的顺序。
2. 快速查找：由于使用哈希表实现，查找操作非常高效。
3. 键可重复：每个键可以在容器中出现多次。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

成员函数：
begin：返回指向容器中第一个元素的迭代器。
bucket：返回给定键所在的桶的索引。
bucket_count：返回容器中桶的数量。
bucket_size：返回给定桶中的元素数量。
cbegin：返回指向容器中第一个元素的常量迭代器。
cend：返回指向容器中最后一个元素的常量迭代器。
clear：移除容器中的所有元素。
contains(c++20)：检查容器是否包含具有给定键的元素。
count：返回容器中具有给定键的元素数量。
emplace：在容器中直接构造元素。
emplace_hint：在给定位置构造元素。
empty：检查容器是否为空。
end：返回指向容器中最后一个元素的迭代器。
equal_range：返回一个包含容器中具有给定键的所有元素的迭代器范围。
erase：移除容器中具有给定键的元素。
find：查找具有给定键的元素。
get_allocator：返回用于分配内存的分配器对象。
hash_function：返回用于计算哈希值的函数对象。
insert：插入元素到容器中。
key_eq：返回用于比较键的函数对象。
load_factor：返回当前容器的负载因子。
max_bucket_count：返回容器中桶的最大数量。
max_load_factor：返回容器的最大负载因子。
max_size：返回容器中可容纳的最大元素数量。
rehash：重新哈希容器。
size：返回容器中元素的数量。
swap：交换两个容器的内容。

## unordered_multiset

std::unordered_multiset 是 C++ 标准库中的一个容器，它类似于
std::unordered_set，但允许存储多个具有相同值的元素

主要特点：
1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于其值。
2. 允许重复元素：与 std::unordered_set 不同，std::unordered_multiset 允许存储多个具有相同值的元素。
3. 快速查找：由于使用哈希表实现，查找操作的平均时间复杂度为常数时间 O(1)。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

成员函数：
begin: 返回指向容器中第一个元素的迭代器。
bucket: 返回给定值所在的桶的索引。
bucket_count: 返回桶的数量。
bucket_size: 返回给定桶中的元素数量。
cbegin: 返回指向容器中第一个元素的常量迭代器。
cend: 返回指向容器中最后一个元素的下一个位置的常量迭代器。
clear: 移除容器中的所有元素。
count: 返回容器中具有给定值的元素数量。
contains: 检查容器中是否存在具有给定值的元素。
emplace: 在容器中构造并插入一个新元素。
emplace_hint: 在容器中构造并插入一个新元素，使用给定的提示位置。
empty: 检查容器是否为空。
end: 返回指向容器中最后一个元素的下一个位置的迭代器。
equal_range: 返回一个范围，其中包含具有给定值的所有元素。
erase: 移除容器中具有给定值的所有元素。
find: 查找容器中具有给定值的元素。
get_allocator: 返回用于分配内存的分配器对象。
hash_function: 返回用于计算哈希值的哈希函数对象。
insert: 插入元素到容器中。
key_eq: 返回用于比较键的相等性谓词。
load_factor: 返回当前的负载因子。
max_bucket_count: 返回最大桶数。
max_load_factor: 返回最大负载因子。
max_size: 返回容器可容纳的最大元素数量。
rehash: 重新哈希容器，以减少冲突。
size: 返回容器中的元素数量。
swap: 交换两个容器的内容。

## unordered_set

std::unordered_set 是 C++ 标准库中的一个容器，它提供了一种无序的、不允许重复元素的集合。

主要特点：
1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于其值。
2. 不允许重复元素：每个元素在容器中只能出现一次。
3. 快速查找：由于使用哈希表实现，查找操作非常高效。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

成员函数：
begin：返回指向容器中第一个元素的迭代器。
bucket：返回给定元素所在的桶的索引。
bucket_count：返回桶的数量。
bucket_size：返回给定桶中的元素数量。
cbegin：返回指向容器中第一个元素的常量迭代器。
cend：返回指向容器中最后一个元素的下一个位置的常量迭代器。
clear：清除容器中的所有元素。
contains(c++20):检查容器是否包含指定元素。
emplace(c++11):在容器中直接构造元素。
emplace_hint(c++11):在指定位置构造元素。
empty：检查容器是否为空。
end：返回指向容器中最后一个元素的下一个位置的迭代器。
equal_range：返回一个包含等于给定值的元素范围的迭代器对。
erase：从容器中删除指定元素或范围内的元素。
find：查找容器中等于给定值的元素。
get_allocator：返回用于分配内存的分配器。
hash_function：返回用于计算哈希值的函数对象。
insert：向容器中插入元素。
key_eq：返回用于比较键的函数对象。
load_factor：返回当前负载因子。
max_bucket_count：返回最大桶数。
max_load_factor：返回最大负载因子。
max_size：返回容器可以容纳的最大元素数量。
rehash：重新分配桶的数量。
reserve：为容器预留足够的空间。
size：返回容器中元素的数量。
swap：交换两个容器的内容。

## vector

std::vector是C++标准模板库（STL）中的一个容器，它表示一个动态数组，能够在运行时改变其大小。std::vector是一个模板类，因此可以存储任何类型的元素。

- 格式

```cpp
template < class T, class Alloc = allocator<T> > 
class vector; // generic template
```

相关网址: https://www.cplusplus.com/reference/vector/vector/

- 常用构造方法

```cpp
// 默认构造函数
std::vector<int> c0;

// 列表初始化
std::vector<int> c1{1, 2, 3, 4, 5};

// 赋值构造函数
std::vector<int> c2(c1);

// 范围构造函数
std::vector<int> c3(c1.begin(), c1.end());

// 长度2
std::vector<int> c4(2);

// 长度2，值3
std::vector<int> c5(2, 3);
```

- 特性

1. 动态大小：std::vector的大小可以在运行时动态增长或缩小。
2. 连续存储：元素在内存中是连续存储的，这使得随机访问非常高效。
3. 自动内存管理：std::vector负责管理其内部的内存，包括分配和释放。
4. 高效的插入和删除：在末尾插入和删除元素的操作通常是高效的（摊销常数时间复杂度），但在中间或开头插入/删除元素可能会导致元素的移动，效率较低。

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向容器中第一个元素的迭代器。 |
| end | 返回指向容器中最后一个元素的下一个位置的迭代器。 |
| rbegin | 返回指向容器中最后一个元素的迭代器。 |
| rend | 返回指向容器中第一个元素的前一个位置的迭代器。 |
| cbegin | 返回指向容器中第一个元素的常量迭代器。 |
| cend | 返回指向容器中最后一个元素的下一个位置的常量迭代器。 |
| crbegin | 返回指向容器中最后一个元素的常量迭代器。 |
| crend | 返回指向容器中第一个元素的前一个位置的常量迭代器。 |

- 成员函数

| 方法 | 说明 |
| --- | --- |
| size | 返回容器中元素的数量。 |
| max_size | 返回容器可以容纳的最大元素数量。 |
| resize | 设置容器的大小。 |
| capacity | 获取容器的容量。 |
| empty | 检查容器是否为空。 |
| reserve | 为容器预留足够的空间。 |
| shrink_to_fit | 缩小容器以适应其当前大小。 |
| operator[] | 访问容器中指定位置的元素。 |
| operator= | 赋值运算符 |
| at | 访问容器中指定位置的元素。 |
| front | 访问容器中的第一个元素。 |
| back | 访问容器中的最后一个元素。 |
| data | 返回一个指向容器中第一个元素的指针。 |
| assign | 赋值容器 |
| push_back | 在容器的末尾添加一个元素。 |
| pop_back | 删除容器中的最后一个元素。 |
| insert | 在指定位置插入元素。 |
| erase | 删除容器中的元素。 |
| swap | 交换两个容器的内容。 |
| clear | 删除容器中的所有元素。 |
| emplace | 在指定位置构造元素。 |
| emplace_back | 在容器的末尾构造元素。 |
| get_allocator | 获取用于分配内存的分配器。 |

## algorithm

C++支持一系列算法用于操作STL容器中的元素，这些算法包括查找、排序、修改等。

- 查找算法

1. std::all_of：检查容器中的所有元素是否都满足特定条件。
2. std::any_of：检查容器中的任意一个元素是否满足特定条件。
3. std::none_of：检查容器中的所有元素是否都不满足特定条件。
4. std::adjacent_find：查找容器中相邻的重复元素的位置。 
5. std::find：用于在指定范围内查找等于给定值的首个元素。
6. std::find_if：在容器中查找的是满足特定条件的元素。
7. std::find_first_of：在容器中查找另一个容器中任意一个元素的位置。
8. std::find_end：在容器中查找另一个容器的最后一个匹配子序列的位置。
9. std::find_if_not：在容器中查找不满足特定条件的元素的位置。
10. std::search：在容器中查找另一个容器的首次出现的位置。
11. std::search_n：在容器中查找指定数量的连续元素。
12. std::binary_search：在已排序的容器中进行二分查找。

- 排序算法

1. std::sort：对容器中的元素进行排序。
2. std::stable_sort：对容器中的元素进行稳定排序。
3. std::partial_sort：对容器中的部分元素进行排序。
4. std::partial_sort_copy：对容器中的部分元素进行排序，并将结果复制到另一个容器中。
5. std::is_sorted：检查容器中的元素是否已排序。
6. std::is_sorted_until：查找容器中未排序的元素的位置。
9. std::random_shuffle：随机打乱容器中的元素。
10. std::reverse：反转容器中的元素顺序。
11. std::rotate: 旋转容器中的元素。
12. std::rotate_copy：旋转容器中的元素，并将结果复制到另一个容器中。

- 变换算法

1. std::transform：对容器中的元素进行转换操作。
2. std::replace：将容器中所有等于指定值的元素替换为新值。
3. std::replace_if：将容器中满足特定条件的元素替换为新值
4. std::replace_copy：将容器中所有等于指定值的元素替换为新值，并将结果复制到另一个容器中。
5. std::replace_copy_if：将容器中满足特定条件的元素替换为新值，并将结果复制到另一个容器中。

- 计数算法

1. std::count：统计容器中在指定范围内等于给定值的元素个数。
2. std::count_if：统计容器中满足特定条件的元素的个数。
3. std::accumulate：对容器中的元素进行累加操作。
4. std::inner_product：计算两个容器中对应元素的乘积的和。
5. std::partial_sum：计算容器中元素的累计和，存放到另一个容器中。
6. std::adjacent_difference：计算容器中相邻元素的差。

- 比较算法

1. std::equal：比较两个容器中的元素是否相等。
2. std::mismatch：查找两个容器中第一个不匹配的元素的位置。
3. std::lexicographical_compare：按字典顺序比较两个容器中的元素。

- 生成算法

1. std::generate：使用生成器函数生成元素并填充容器。
2. std::generate_n：使用生成器函数生成n个元素并填充容器。
3. std::fill：将容器中的所有元素设置为指定的值。
4. std::fill_n：将容器中的前 n 个元素设置为指定的值。
5. std::copy：将容器中的元素复制到另一个容器中。
6. std::copy_if：将容器中满足特定条件的元素复制到另一个容器中。
7. std::copy_n：将容器中的前 n 个元素复制到另一个容器中。
8. std::copy_backward：将容器中的元素从后往前复制到另一个容器中。

- 移除算法

1. std::remove：移除容器中满足特定条件的元素，把不需要移除的放在前部，原容器大小不变。
2. std::remove_if：移除容器中满足特定条件的元素，把不需要移除的放在前部，原容器大小不变。
3. std::remove_copy：将容器中等于特定值的元素复制到另一个容器中，同时移除不符合条件的元素。
4. std::remove_copy_if：将容器中满足特定条件的元素复制到另一个容器中。
5. std::unique：移除容器中**相邻的重复元素**, 把不重复的元素放在前部，原容器大小不变。
6. std::unique_copy：移除容器中相邻的重复元素，并将结果复制到另一个容器中。

- 分割算法

1. std::partition：将容器中的元素根据指定条件进行分割, 满足条件的元素会被移动到容器的前面，不保证顺序不变。
2. std::stable_partition：将容器中的元素根据指定条件进行稳定分割，满足条件的元素会被移动到容器的前面，保证顺序不变。
3. std::partition_copy：将容器中的元素根据指定条件进行分割，并将结果复制到另一个容器中。
4. std::partition_point：返回容器中第一个满足指定条件的元素的位置。

- 归并算法

1. std::merge：将两个已排序的容器合并为一个已排序的容器。
2. std::inplace_merge：对容器中的两个已排序的子序列进行原地合并。
3. std::set_union：计算两个已排序容器的并集。
4. std::set_intersection：计算两个已排序容器的交集。
5. std::set_difference：计算两个已排序容器的差集。
6. std::set_symmetric_difference：计算两个已排序容器的对称差集。

- 堆算法

1. std::make_heap：将容器转换为堆。
1. std::push_heap：将元素添加到堆中。
2. std::pop_heap：从堆中移除顶部元素。
3. std::make_heap：将容器转换为堆。
4. std::sort_heap：对堆进行排序。

- 其它算法

1. std::sample: 从容器中随机选择指定数量的元素。
2. std::clamp: 限制值在指定范围内。
3. std::reduce：对容器中的元素进行归约操作。
4. std::transform_reduce：对容器中的元素进行转换操作，并对结果进行归约操作。
5. std::gcd: 计算两个整数的最大公约数。
6. std::lcm: 计算两个整数的最小公倍数。
7. std::to_chars: 将字符串转换为字符数组。
8. std::from_chars: 将字符数组转换为字符串。
