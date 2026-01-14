# stl

STL（Standard Template Library）是C++标准库的一部分，它基于template提供了一组通用的容器、算法和函数，用于处理数据结构和算法的实现。

本篇中描述的STL容器如下所示。

- [std::array](#array)
- [std::deque](#deque)
- [std::flat_map](#flat_map)
- [std::flat_set](#flat_set)
- [std::forward_list](#forward_list)
- [std::list](#list)
- [std::map](#map)
- [std::mdspan](#mdspan)
- [std::multimap](#multimap)
- [std::multiset](#multiset)
- [std::priority_queue](#priority_queue)
- [std::queue](#queue)
- [std::set](#set)
- [std::span](#span)
  - [spanstream](#spanstream)
- [std::stack](#stack)
- [std::unordered_map](#unordered_map)
- [std::unordered_multimap](#unordered_multimap)
- [std::unordered_multiset](#unordered_multiset)
- [std::unordered_set](#unordered_set)
- [std::vector](#vector)
- [algorithm](#algorithm)
  - [查询算法](#search_algorithms)
  - [排序算法](#sort_algorithms)
  - [变换算法](#transform_algorithms)
  - [计数算法](#count_algorithms)
  - [比较算法](#compare_algorithms)
  - [生成算法](#generate_algorithms)
  - [移除算法](#remove_algorithms)
  - [分割算法](#partition_algorithms)
  - [归并算法](#merge_algorithms)
  - [堆栈算法](#heap_algorithms)
  - [其他算法](#other_algorithms)
- [返回主页](../../README.md)

## array

std::array它提供了固定大小的数组功能，它们保存按严格线性序列排序的特定数量的元素。

std::array参考网址: <https://en.cppreference.com/w/cpp/container/array.html>

- 格式

```cpp
// array 数组原型
// T 为元素类型
// N 为元素数量
template<
    class T,
    std::size_t N
> struct array;
```

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

详细测试代码见: [array验证代码](./array/main.cpp)

## deque

std::deque它代表双端队列（double-ended queue）。双端队列是一种序列容器，允许在两端（前端和后端）快速插入和删除元素。

std::deque参考网址: <https://en.cppreference.com/w/cpp/container/deque.html>

- 格式

```cpp
// deque 双端队列原型
// T 为元素类型
// Allocator 为分配器类型
template<
    class T,
    class Allocator = std::allocator<T>
> class deque;
```

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

详细测试代码见: [deque验证代码](./deque/main.cpp)

## flat_map

std::flat_map代表有序关联容器（ordered associative container），存储键值对，按键进行排序。

std::flat_map参考网址: <https://en.cppreference.com/w/cpp/container/flat_map.html>

- 格式

```cpp
/// flat_map 映射原型
/// Key 为键的类型
/// T 为值类型
/// Compare 为比较函数
/// KeyContainer 为键容器类型
/// MappedContainer 为值容器类型
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class KeyContainer = std::vector<Key>,
    class MappedContainer = std::vector<T>
> class flat_map;
```

- 常用构造方法

```cpp
// 默认构造函数
std::flat_map<int, std::string> c0;

// 列表初始化
std::flat_map<int, std::string> c1 = {{1, "one"}, {2, "two"}, {3, "three"}};

// 赋值构造函数
std::flat_map<int, std::string> c2(c1);

// 范围构造函数
std::flat_map<int, std::string> c3(c1.begin(), c1.end());
```

- 主要特性

1. 基于有序向量(排序数组)实现的映射，连续内存存储。
2. 键值对存储在有序向量中，键值对之间是连续存储的。
3. 提供更好的缓存局部性和内存效率。

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
| crend | 返回指向容器中第一个元素之前位置的常量反向迭代器。|

- 成员函数

| 方法 | 说明 |
| --- | --- |
| at | 通过边界检查来访问指定元素。|
| operator[] | 访问指定键的元素。|
| empty | 检查容器是否为空。|
| size | 返回容器中元素的个数。|
| max_size | 返回容器可以容纳的最大元素个数。|
| emplace | 在指定位置构造一个元素。|
| emplace_hint | 在指定位置带初始化构造一个元素。|
| insert | 插入一个元素。|
| insert_range | 插入指定范围内的元素。|
| insert_or_assign | 插入一个元素，如果元素已存在则赋值给它。|
| extract | 删除并返回指定键的元素。|
| replace | 替换指定键的元素。|
| erase | 删除指定键的元素。|
| clear | 删除所有元素。|
| find | 查找指定键的元素。|
| count | 返回指定键的元素个数。|
| contains | 检查指定键的元素是否存在。|
| lower_bound | 返回第一个不小于指定键的元素的迭代器。|
| upper_bound | 返回第一个大于指定键的元素的迭代器。|
| equal_range | 返回指定键的元素范围。|
| key_comp | 返回键的比较函数。|
| value_comp | 获取值比较函数。|
| keys | 获取所有键。|
| values | 获取所有值。|

## flat_set

std::flat_set 它代表有序关联容器（ordered associative container），存储唯一的元素，按键进行排序。

std::flat_set参考网址: <https://en.cppreference.com/w/cpp/container/flat_set.html>  

- 格式

```cpp
// flat_set 集合原型
// Key 为元素类型
// Compare 为比较函数
// Allocator 为分配器类型
template<
    class Key,
    class Compare = std::less<Key>,
    class KeyContainer = std::vector<Key>
> class flat_set;
```

- 常用构造方法

```cpp
// 默认构造函数
std::flat_set<int> c0;

// 列表初始化
std::flat_set<int> c1 = {1, 2, 3, 4, 5};

// 赋值构造函数
std::flat_set<int> c2(c1);

// 范围构造函数
std::flat_set<int> c3(c1.begin(), c1.end());

// 结构体对象
struct classcomp{
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};
std::flat_set<int, classcomp> c4;

// lambda对象初始化
constexpr auto funcomp = [](const char& lhs, const char& rhs) {return lhs<rhs;};
std::flat_set<int, decltype(funcomp)> c5(funcomp);
```

- 主要特性

1. 元素唯一性：std::flat_set中的每个元素都是唯一的，不允许重复。
2. 自动排序：元素在插入时会自动按照排序准则进行排序。
3. 高效查找：由于元素是有序的，查找操作的时间复杂度为对数级别，即 O(log n)。
4. 基于有序向量(排序数组)实现的集合，连续内存存储。

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

## forward_list

std::forward_list 它代表单向链表（singly linked list）。单向链表是一种线性数据结构，其中每个元素（节点）包含一个值和指向下一个元素的指针。

std::forward_list参考网址: <https://en.cppreference.com/w/cpp/container/forward_list.html>

- 格式

```cpp
// forward_list 单向链表原型
// T 为元素类型
// Allocator 为分配器类型
template<
    class T,
    class Allocator = std::allocator<T>
> class forward_list;
```

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

详细测试代码见: [forward_list验证代码](./forward_list/main.cpp)

## list 

std::list是C++标准库中的一个容器类，它提供了一种双向链表的数据结构。与std::forward_list不同，std::list允许在链表的任何位置进行插入和删除操作，并且提供了更多的成员函数和算法。

std::list参考网址: <https://en.cppreference.com/w/cpp/container/list.html>

- 格式

```cpp
// list 链表原型
// T 为元素类型
// Allocator 为分配器类型
template<
    class T,
    class Allocator = std::allocator<T>
> class list;
```

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

详细测试代码见: [list验证代码](./list/main.cpp)

## map

std::map是C++ 标准库中的一个关联容器，它存储的元素是键值对（key-value pairs）。每个元素由一个键（key）和一个值（value）组成，键是唯一的，每个键对应一个值。其中的元素按照键的顺序自动排序，默认情况下是按照升序排列，支持自定义比较函数进行键值的比较排序。

std::map参考网址: <https://en.cppreference.com/w/cpp/container/map.html>

- 格式

```cpp
// map 映射原型
// Key 为键的类型
// T 为值类型
// Compare 为比较函数
// Allocator 为分配器类型
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class map;
```

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

详细测试代码见: [map验证代码](./map/main.cpp)

### mdspan

mdspan是C++23引入的一个容器视图类，它提供了一种轻量级的、非拥有的方式来访问多维连续的对象序列，用于处理动态大小和静态大小数组。mdspan提供了与数组相关的操作，如访问元素、获取大小和范围。mdspan与原始的数组结构具有相同的内存地址，因此可以通过修改mdspan来修改原始数据，这样就扩展了数组的访问。

mdspan参考网址: <https://en.cppreference.com/w/cpp/header/mdspan.html>

mdspan参考格式如下。

```cpp
// mdspan 多维连续对象序列视图原型
// T 为元素类型
// Extents 为扩展维度
// LayoutPolicy 为布局策略
// AccessorPolicy 为访问策略
template<
    class T,
    class Extents,
    class LayoutPolicy = std::layout_right,
    class AccessorPolicy = std::default_accessor<T>
> class mdspan;
```

## multimap

std::multimap是C++标准库中的一个关联容器，它允许存储多个具有相同键的键值对。

std::multimap参考网址: <https://en.cppreference.com/w/cpp/container/multimap.html>

- 格式

```cpp
// multimap 多相同键值映射原型
// Key 为键的类型
// T 为值类型
// Compare 为比较函数
// Allocator 为分配器类型
template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class multimap;
```

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

详细测试代码见: [multimap验证代码](./multimap/main.cpp)

## multiset

std::multiset是按照特定顺序存储元素的容器，其中多个元素可以具有相同的值。

std::multiset参考网址: <https://en.cppreference.com/w/cpp/container/multiset.html>

- 格式

```cpp
// multiset 多相同值集合原型
// Key 为键的类型
// Compare 为比较函数
// Allocator 为分配器类型
template<
    class Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
> class multiset;
```

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

详细测试代码见: [multiset验证代码](./multiset/main.cpp)

## priority_queue

std::priority_queue是C++标准库中的一个容器适配器，它提供了一种优先队列的实现。

std::priority_queue参考网址: <https://en.cppreference.com/w/cpp/container/priority_queue.html>

- 格式

```cpp
// priority_queue 优先队列原型
// T 为元素类型
// Container 为存储元素的容器类型
// Compare 为比较函数
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

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

详细测试代码见: [priority_queue验证代码](./priority_queue/main.cpp)

## queue

std::queue是C++ 标准库中的一个容器适配器，它提供了一种先进先出（FIFO）的数据结构。

std::queue参考网址: <https://en.cppreference.com/w/cpp/container/queue.html>

- 格式

```cpp
// queue 队列原型
// T 为元素类型
// Container 为存储元素的容器类型
template<
    class T,
    class Container = std::deque<T>
> class queue;
```

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

详细测试代码见: [queue验证代码](./queue/main.cpp)

## set

std::set是按照特定顺序存储元素的容器，其中多个元素可以具有相同的值。

std::set参考网址: <https://en.cppreference.com/w/cpp/container/set.html>

- 格式

```cpp
// set 集合原型
// Key 为元素类型
// Compare 为比较函数
// Allocator 为分配器类型
template<
    class Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
> class set;
```

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

详细测试代码见: [set验证代码](./set/main.cpp)

## span

span是C++20引入的一个容器视图类，它提供了一种轻量级的、非拥有的方式来访问连续的对象序列，用于处理动态大小和静态大小数组。span提供了与数组相关的操作，如访问元素、获取大小和范围。span与原始的数组结构具有相同的内存地址，因此可以通过修改span来修改原始数据，这样就扩展了数组的访问。

其结构原型如下所示。

```cpp
// span的结构原型
// T: 元素类型
// Extent: 元素数量，默认值为std::dynamic_extent，表示动态大小
template<
    class T,
    std::size_t Extent = std::dynamic_extent
> class span;
```

span参考网址: https://en.cppreference.com/w/cpp/container/span.html

常见构造方法:

```cpp
int arr[]{1, 2, 3};

// 通过数组创建span(默认动态长度)
std::span<int> span1(arr);

// 通过数组创建指定大小的span
std::span<int, 3> span2(arr);
```

主要特点：

1. 非拥有性：std::span 不拥有它所指向的数据，它只是提供了对数据的访问接口。这意味着std::span不会分配或释放内存，它只是引用了已经存在的内存。这表示具有两个特点，一是对于span的修改会同步修改原始数据，二是不能够改变span所引用数据的大小和内存，否则可能导致运行错误。
2. 连续内存访问：std::span 要求它所指向的数据是连续存储的，这使得它可以高效地进行随机访问和迭代。
3. 大小和范围：std::span 可以在编译时或运行时指定其大小，这使得它可以灵活地适应不同的使用场景。
4. 与标准库的集成：std::span 可以与标准库中的其他容器和算法无缝集成，使得代码更加简洁和高效。

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 返回指向第一个元素的迭代器 |
| cbegin | 创建一个只读迭代器，指向第一个元素(C++23) |
| end | 返回指向最后一个元素的迭代器 |
| cend | 创建一个只读迭代器，指向最后一个元素(C++23) |
| rbegin | 创建一个逆向迭代器，指向最后一个元素 |
| crbegin | 创建一个逆向只读迭代器，指向最后一个元素(C++23) |
| rend | 创建一个逆向迭代器，指向第一个元素 |
| crend | 创建一个逆向只读迭代器，指向第一个元素(C++23) |

- 方法

| 方法 | 说明 |
| --- | --- |
| front | 返回第一个元素 |
| back | 返回最后一个元素 |
| at | 返回指定索引的元素(C++26) |
| operator[] | 返回指定索引的元素 |
| data | 返回指向内容的元素指针 |
| size | 返回元素数量 |
| size_bytes | 返回元素占用的字节数 |
| empty | 判断是否为空 |
| first | 从起始索引截取指定数量的元素 |
| last | 从末尾索引截取指定数量的元素 |
| subspan | 获取子span |

std::span可以配合其它函数使用，具体如下。

| 函数 | 说明 |
| --- | --- |
| std::as_bytes | 将span转换为字节span |
| std::as_writable_bytes | 将span转换为可写字节span |

### spanstream

spanstream是C++23引入的一个流类，它提供了一种方便的方式来操作span对象。spanstream可以将span对象的内容写入流中，也可以从流中读取span对象的内容。

spanstream参考网址: <https://en.cppreference.com/w/cpp/header/spanstream.html>

spanstream主要支持如下方法和类型。

| 方法 | 说明 |
| --- | --- |
| basic_spanbuf | 基本的span缓冲区类 |
| basic_ispanstream | 基本的输入span流类 |
| basic_ospanstream | 基本的输出span流类 |
| basic_spanstream | 基本的span流类 |
| spanbuf | 对应basic_spanbuf<char> |
| wspanbuf | 对应basic_spanbuf<wchar_t> |
| ispanstream | 对应basic_ispanstream<char> |
| wispanstream | 对应basic_ispanstream<wchar_t> |
| ospanstream | 对应basic_ospanstream<char> |
| wospanstream | 对应basic_ospanstream<wchar_t> |
| spanstream | 对应basic_spanstream<char> |
| wspanstream | 对应basic_spanstream<wchar_t> |

具体示例如下所示。

```cpp
#include <span>
#include <iostream>
#include <spanstream>

int main(int argc, const char *argv[])
{
    // ospanstream(用ospanstream输出字符串)
    char out_buf[16];
    std::ospanstream ost{std::span<char>{out_buf}};
    
    // span
    ost << "123456";
    auto sp = ost.span();
    print_span<char>(sp, "sp");

    // operator=
    std::ospanstream&& ost1 = std::move(ost);
    print_span<char>(ost1.span(), "ost1");

    // rdbuf
    auto rbuf = ost1.rdbuf();
    print_span<char>(rbuf->span(), "ost1");

    // ispanstream(用ispanstream解析字符串)
    const char in_buf[] = "a b 24";
    std::ispanstream ist{std::span<const char>{in_buf}};
    char c;
    ist >> c;
    std::cout << "c: " << c << std::endl;
    ist >> c;
    std::cout << "c: " << c << std::endl;
    int i;
    ist >> i;
    std::cout << "i: " << i << std::endl;
    
    // 以span方式获取解析的字符串
    auto span_1 = ist.span();
    for (auto x : span_1) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## stack

std::stack是C++标准库中的一个容器适配器，它提供了一种后进先出（LIFO）的数据结构。

std::stack参考网址: <https://en.cppreference.com/w/cpp/container/stack.html>

- 格式

```cpp
// stack 栈原型
// T 为元素类型
// Container 为存储元素的容器类型
template<
    class T,
    class Container = std::deque<T>
> class stack;
```

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

详细测试代码见: [stack验证代码](./stack/main.cpp)

## unordered_map

std::unordered_map是C++标准库中的一个关联容器，它提供了一种键值对的存储方式；其中键是唯一的，而值可以重复。unordered_map和map的区别是，unordered_map使用哈希表作为底层数据结构，并不进行排序。

std::unordered_map参考网址: <https://en.cppreference.com/w/cpp/container/unordered_map.html>

- 格式

```cpp
// unordered_map 映射原型
// Key 为键的类型
// T 为值类型
// Hash 为哈希函数对象
// KeyEqual 为键相等比较函数对象
// Allocator 为分配器对象
template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class unordered_map;
```

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

详细测试代码见: [unordered_map验证代码](./unordered_map/main.cpp)

## unordered_multimap

std::unordered_multimap是C++标准库中的一个关联容器，它类似于unoredered_map，不过允许存储多个具有相同键的键值对，并且内部使用哈希表实现，排列是无序的。

std::unordered_multimap参考网址: <https://en.cppreference.com/w/cpp/container/unordered_multimap.html>

- 格式

```cpp
// unordered_multimap 映射原型
// Key 为键的类型
// T 为值类型
// Hash 为哈希函数对象
// KeyEqual 为键相等比较函数对象
// Allocator 为分配器对象
template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> class unordered_multimap;
```

- 常用构造方法

```cpp
// 默认构造函数
std::unordered_multimap<int, int> c0;

// 列表初始化
std::unordered_multimap<int, int> c1 = {
    {2, 1},
    {2, 2},
};

// 赋值构造函数
std::unordered_multimap<int, int> c2(c1);

// 范围构造函数
std::unordered_multimap<int, int> c3(c1.begin(), c1.end());

// 自定义allocator构造函数
std::unordered_multimap<int, std::string, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<const int, std::string>>> c4;
```

- 主要特点

1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于键的顺序。
2. 快速查找：由于使用哈希表实现，查找操作非常高效。
3. 键可重复：每个键可以在容器中出现多次。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

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
| size | 获取容器中元素的个数。|
| max_size | 获取容器中可容纳的元素最大个数。|
| operator= | 拷贝赋值。|
| operator[] | 访问元素。|
| at | 访问元素。|
| find | 查找元素。|
| count | 统计元素个数。|
| equal_range | 获取元素范围。|
| emplace | 在容器中直接构造元素。|
| emplace_hint | 在容器中的指定位置直接构造元素。|
| insert | 插入一个元素或元素范围。|
| erase | 从容器中删除一个或多个元素。|
| clear | 清空容器中的所有元素。|
| swap | 交换两个容器的内容。|
| bucket_count | 获取存储桶的数量。|
| max_bucket_count | 获取存储桶的最大数量。|
| bucket_size | 获取存储桶中元素数量。|
| bucket | 获取元素所在的存储桶编号。|
| load_factor | 获取负载因子。|
| max_load_factor | 获取最大负载因子。|
| rehash | 重新哈希。|
| reserve | 预留空间。|
| hash_function | 获取哈希函数。|
| key_eq | 获取比较函数。|
| get_allocator | 获取用于构造容器的分配器对象的副本。|

详细测试代码见: [unordered_multimap验证代码](./unordered_multimap/main.cpp)

## unordered_multiset

std::unordered_multiset是C++标准库中的一个容器，它类似于std::unordered_set，但允许存储多个具有相同值的元素。

std::unordered_multiset参考网址: <https://en.cppreference.com/w/cpp/container/unordered_multiset.html>

- 格式

```cpp
// unordered_multiset 映射原型
// Key 为键的类型
// Hash 为哈希函数对象
// KeyEqual 为键相等比较函数对象
// Allocator 为分配器对象
template<
    class Key,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<Key>
> class unordered_multiset;
```

- 常用构造方法

```cpp
// 默认构造函数
std::unordered_multiset<int> c0;

// 列表初始化
std::unordered_multiset<int> c1 = {1, 2, 3, 4, 5};

// 赋值构造函数
std::unordered_multiset<int> c2(c1);

// 范围构造函数
std::unordered_multiset<int> c3(c1.begin(), c1.end());

// 自定义allocator构造函数
std::unordered_multiset<int, std::hash<int>, std::equal_to<int>> c4;
```

- 主要特点

1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于其值。
2. 允许重复元素：与 std::unordered_set 不同，std::unordered_multiset 允许存储多个具有相同值的元素。
3. 快速查找：由于使用哈希表实现，查找操作的平均时间复杂度为常数时间 O(1)。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

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
| operator= | 赋值运算符。|
| empty | 判断容器是否为空。|
| size | 返回容器中元素的数目。|
| max_size | 返回容器中可容纳的元素数目的上限。|
| find | 返回指向指定元素的迭代器。|
| count | 返回指定元素的数量。|
| equal_range | 返回指定元素的迭代器范围。|
| emplace | 构造并插入元素。|
| emplace_hint | 构造并插入元素。|
| insert | 插入元素。|
| erase | 删除元素。|
| clear | 删除所有元素。|
| swap | 交换两个unordered_set对象。|
| bucket_count | 返回桶的数量。|
| max_bucket_count | 返回最大桶的数量。|
| bucket_size | 返回指定桶中的元素数量。|
| bucket | 返回元素所在的桶编号。|
| load_factor | 返回负载因子。|
| max_load_factor | 获取或设置负载因子。|
| rehash | 重新分配存储空间。|
| reserve | 预留空间。|
| hash_function | 获取哈希函数。|
| key_eq | 获取键比较函数。|
| get_allocator | 获取用于分配内存的分配器对象的副本。|

详细测试代码见: [unordered_multiset验证代码](./unordered_multiset/main.cpp)

## unordered_set

std::unordered_set是C++标准库中的一个容器，它提供了一种无序的、不允许重复元素的集合。

std::unordered_set参考网址: <https://en.cppreference.com/w/cpp/container/unordered_set.html>

- 格式

```cpp
// unordered_set 映射原型
// Key 为键的类型
// Hash 为哈希函数对象
// KeyEqual 为键相等比较函数对象
// Allocator 为分配器对象
template<
    class Key,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<Key>
> class unordered_set;
```

- 常用构造方法

```cpp
// 默认构造函数
std::unordered_set<int> c0;

// 列表初始化
std::unordered_set<int> c1 = {1, 2, 3, 4, 5};

// 赋值构造函数
std::unordered_set<int> c2(c1);

// 范围构造函数
std::unordered_set<int> c3(c1.begin(), c1.end());

// 自定义allocator构造函数
std::unordered_set<int, std::hash<int>, std::equal_to<int>> c4;
```

- 主要特点

1. 无序性：元素在容器中的存储顺序是无序的，即元素的存储位置不依赖于其值。
2. 不允许重复元素：每个元素在容器中只能出现一次。
3. 快速查找：由于使用哈希表实现，查找操作非常高效。
4. 动态内存管理：容器会自动管理内存，根据需要动态分配和释放内存。

- 迭代器

| 方法 | 说明 |
| --- | --- |
| begin | 获取容器的起始迭代器。 |
| end | 获取容器的结束迭代器。 |
| cbegin | 获取常量迭代器，指向容器的起始位置。 |
| cend | 获取常量迭代器，指向容器的结束位置。 |

- 成员函数

| 方法 | 说明 |
| --- | --- |
| operator= | 赋值运算符。|
| empty | 判断容器是否为空。|
| size | 返回容器中元素的数目。|
| max_size | 返回容器中可容纳的元素数目的上限。|
| find | 返回指向指定元素的迭代器。|
| count | 返回指定元素的数量。|
| equal_range | 返回指定元素的迭代器范围。|
| emplace | 构造并插入元素。|
| emplace_hint | 构造并插入元素。|
| insert | 插入元素。|
| erase | 删除元素。|
| clear | 删除所有元素。|
| swap | 交换两个unordered_set对象。|
| bucket_count | 返回桶的数量。|
| max_bucket_count | 返回最大桶的数量。|
| bucket_size | 返回指定桶中的元素数量。|
| bucket | 返回元素所在的桶编号。|
| load_factor | 返回负载因子。|
| max_load_factor | 获取或设置负载因子。|
| rehash | 重新分配存储空间。|
| reserve | 预留空间。|
| hash_function | 获取哈希函数。|
| key_eq | 获取键比较函数。|
| get_allocator | 获取用于分配内存的分配器对象的副本。|

详细测试代码见: [unordered_set验证代码](./unordered_set/main.cpp)

## vector

std::vector是C++标准模板库（STL）中的一个容器，它表示一个动态数组，能够在运行时改变其大小。std::vector是一个模板类，因此可以存储任何类型的元素。

std::vector参考网址: <https://en.cppreference.com/w/cpp/container/vector.html>

- 格式

```cpp
// vector 映射原型
// T 为元素类型
// Allocator 为分配器对象
template<
    class T,
    class Allocator = std::allocator<T>
> class vector;
```

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

详细测试代码见: [vector验证代码](./vector/main.cpp)

## algorithm

C++支持一系列算法用于操作STL容器中的元素，主要包含在头文件numeric和algorithm中。这些算法包括查找、排序、修改等。

- numeric的参考网址: <https://en.cppreference.com/w/cpp/header/numeric.html>
- algorithm的参考网址: <https://en.cppreference.com/w/cpp/header/algorithm.html>

### search_algorithms

查找算法定义了一系列用于在容器内搜索或统计元素数量的算法。

- std::all_of：检查容器中的所有元素是否都满足特定条件。
- std::any_of：检查容器中的任意一个元素是否满足特定条件。
- std::binary_search：在已排序的容器中进行二分查找。
- std::none_of：检查容器中的所有元素是否都不满足特定条件。
- std::adjacent_find：查找容器中相邻的重复元素的位置。 
- std::find：用于在指定范围内查找等于给定值的首个元素。
- std::find_if：在容器中查找的是满足特定条件的元素。
- std::find_first_of：在容器中查找另一个容器中任意一个元素的位置。
- std::find_end：在容器中查找另一个容器的最后一个匹配子序列的位置。
- std::find_if_not：在容器中查找不满足特定条件的元素的位置。
- std::search：在容器中查找另一个容器的首次出现的位置。
- std::search_n：在容器中查找指定数量的连续元素。

### sort_algorithms

排序算法定义了一系列用于对容器中的元素进行排序或检测是否已排序的算法。

- std::sort：对容器中的元素进行排序。
- std::stable_sort：对容器中的元素进行稳定排序。
- std::partial_sort：对容器中的部分元素进行排序。
- std::partial_sort_copy：对容器中的部分元素进行排序，并将结果复制到另一个容器中。
- std::is_sorted：检查容器中的元素是否已排序。
- std::is_sorted_until：查找容器中未排序的元素的位置。
- std::random_shuffle：随机打乱容器中的元素。
- std::reverse：反转容器中的元素顺序。
- std::rotate: 旋转容器中的元素。
- std::rotate_copy：旋转容器中的元素，并将结果复制到另一个容器中。

### transform_algorithms

变换算法定义了一系列用于对容器中的元素进行转换或修改的算法。

- std::transform：对容器中的元素进行转换操作。
- std::replace：将容器中所有等于指定值的元素替换为新值。
- std::replace_if：将容器中满足特定条件的元素替换为新值
- std::replace_copy：将容器中所有等于指定值的元素替换为新值，并将结果复制到另一个容器中。
- std::replace_copy_if：将容器中满足特定条件的元素替换为新值，并将结果复制到另一个容器中。

### count_algorithms

计数算法定义了一系列用于统计容器中元素的数量的算法。

- std::count：统计容器中在指定范围内等于给定值的元素个数。
- std::count_if：统计容器中满足特定条件的元素的个数。
- std::accumulate：对容器中的元素进行累加操作。
- std::inner_product：计算两个容器中对应元素的乘积的和。
- std::partial_sum：计算容器中元素的累计和，存放到另一个容器中。
- std::adjacent_difference：计算容器中相邻元素的差。

### compare_algorithms

比较算法定义了一系列用于比较容器中元素的算法。

- std::equal：比较两个容器中的元素是否相等。
- std::mismatch：查找两个容器中第一个不匹配的元素的位置。
- std::lexicographical_compare：按字典顺序比较两个容器中的元素。

### generate_algorithms

生成算法定义了一系列用于生成元素的算法。

- std::generate：使用生成器函数生成元素并填充容器。
- std::generate_n：使用生成器函数生成n个元素并填充容器。
- std::fill：将容器中的所有元素设置为指定的值。
- std::fill_n：将容器中的前 n 个元素设置为指定的值。
- std::copy：将容器中的元素复制到另一个容器中。
- std::copy_if：将容器中满足特定条件的元素复制到另一个容器中。
- std::copy_n：将容器中的前 n 个元素复制到另一个容器中。
- std::copy_backward：将容器中的元素从后往前复制到另一个容器中。

### remove_algorithms

移除算法定义了一系列用于移除容器中元素的算法。

- std::remove：移除容器中满足特定条件的元素，把不需要移除的放在前部，原容器大小不变。
- std::remove_if：移除容器中满足特定条件的元素，把不需要移除的放在前部，原容器大小不变。
- std::remove_copy：将容器中等于特定值的元素复制到另一个容器中，同时移除不符合条件的元素。
- std::remove_copy_if：将容器中满足特定条件的元素复制到另一个容器中。
- std::unique：移除容器中**相邻的重复元素**，把不重复的元素放在前部，原容器大小不变。
- std::unique_copy：移除容器中相邻的重复元素，并将结果复制到另一个容器中。

### partition_algorithms

分割算法定义了一系列用于将容器中的元素根据指定条件进行分割的算法。

- std::partition：将容器中的元素根据指定条件进行分割, 满足条件的元素会被移动到容器的前面，不保证顺序不变。
- std::stable_partition：将容器中的元素根据指定条件进行稳定分割，满足条件的元素会被移动到容器的前面，保证顺序不变。
- std::partition_copy：将容器中的元素根据指定条件进行分割，并将结果复制到另一个容器中。
- std::partition_point：返回容器中第一个满足指定条件的元素的位置。

### merge_algorithms

归并算法定义了一系列用于将两个已排序的容器合并为一个已排序的容器的算法。

- std::merge：将两个已排序的容器合并为一个已排序的容器。
- std::inplace_merge：对容器中的两个已排序的子序列进行原地合并。
- std::set_union：计算两个已排序容器的并集。
- std::set_intersection：计算两个已排序容器的交集。
- std::set_difference：计算两个已排序容器的差集。
- std::set_symmetric_difference：计算两个已排序容器的对称差集。

### heap_algorithms

堆算法定义了一系列用于操作堆的算法。

- std::make_heap：将容器转换为堆。
- std::push_heap：将元素添加到堆中。
- std::pop_heap：从堆中移除顶部元素。
- std::make_heap：将容器转换为堆。
- std::sort_heap：对堆进行排序。

### other_algorithms

- std::sample: 从容器中随机选择指定数量的元素。
- std::clamp: 限制值在指定范围内。
- std::reduce：对容器中的元素进行归约操作。
- std::transform_reduce：对容器中的元素进行转换操作，并对结果进行归约操作。
- std::gcd: 计算两个整数的最大公约数。
- std::lcm: 计算两个整数的最小公倍数。
- std::to_chars: 将字符串转换为字符数组。
- std::from_chars: 将字符数组转换为字符串。
- std::back_inserter：创建一个插入迭代器，用于将元素添加到容器的末尾(支持push_back的容器可以使用)
- std::front_inserter：创建一个插入迭代器，用于将元素添加到容器的开头(支持push_front的容器可以使用)
- std::inserter：创建一个插入迭代器，用于将元素添加到指定位置(支持insert的容器可以使用)
- std::greater<>() 执行大于比较操作
- std::less<>() 执行小于比较操作
- std::less_equal<>() 执行小于等于比较操作
- std::greater_equal<>() 执行大于等于比较操作
- std::equal_to<>() 执行等于比较操作
- std::not_equal_to<>() 执行不等于比较操作
- std::plus<>() 执行加法操作
- std::minus<>() 执行减法操作
- std::multiplies<>() 执行乘法操作
- std::divides<>() 执行除法操作
- std::modulus<>() 执行取余操作
- std::negate<>() 执行取反操作
