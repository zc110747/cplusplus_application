# cplusplus_application

## 综述

C++是支持多范式不同实践的语言，往往很多应用也仅仅是C++的实践的一小部分，在学习C++的过程中，保持着好奇心，结合规范和实践，通过积累总结逐渐扩展对C++的认知，这也是这个工程的由来。本系列主要用于C++的学习，包含基础应用，也有扩展和参考各工具书的学习实践，可以把本系列当作C++学习中的目录指引，遇到不理解的概念，可以参考相应例程去了解，也可以参考相关资料进行说明。

本系列中大部分例程都是单项目文件，可以直接在线运行。

本项目的参考资料和书籍如下.

- 《C++ Primer Plus》
- 《C++语言的设计和演化》
- 《深入理解C++11 - C++11新特性解析与应用》
- 《现代C++语言核心特性解析》
- [cpp主要语法说明网址](https://cplusplus.com/reference/)
- [cpp在线测试环境](https://gcc.godbolt.org/)
- [microsoft关于C++的说明](https://learn.microsoft.com/zh-cn/cpp/standard-library/cpp-standard-library-reference?view=msvc-170)

## 目录

- [兼容C的语法](#c_style)
- [命名空间](#namespace)
- [引用特性](#reference)
- [类的特性](#class)
- [模板(template)](#template)
- [标准模板库(STL)](#stl)
- [标准库(std)](#std)
- [异常(exception)](#exception)
- [布局(layout)](#layout)
- [智能指针](#smart_pointer)
- [类型转换](#type_conversion)
- [多线程和协程](#thread_coroutine)
- [Modern C++扩展功能](#modern_cpp)
- [工具类应用](#tools)
- [c++ 20扩展语法](#cpp20)

## C++知识点总结

### c_style

兼容C的语法包含关键字, 函数，循环，数组和指针等, 这部分建议阅读《C Primer Plus》.

- 关键字包含如下
  - 基本数据类型，如int, char, float, double, bool, void, long, short, unsigned, signed
  - 控制语句，如if, else, while, do, for, switch, case, default, break, continue, goto, return
  - 存储类，如auto, extern, static, register, const, volatile, mutable, thread_local
  - 复合类型，如struct, union, enum
  - 预定义类型，#ifdef, #else, #endif, #define, #undef, #include, #line, #error, #pragma
  - 其它关键字，const, volatile, sizeof
- 函数
  - 函数声明，函数定义，函数调用
- 循环
  - while, do...while, for
- 数组和指针
  - 指针的概念，指针的运算，指针的类型
  - 指针的引用，指针的解引用
  - 指针的数组，指针的函数
- 预定义指令
  - #include : 引入头文件
  - #define, #undef : 定义和取消宏
  - #ifdef, #else, #endif : 条件编译
  - #if, #elif, #else, #endif : 条件编译
  - #pragma和_Pragma : 用于控制编译器的行为
  - #error : 当编译器遇到错误时，它会输出一条错误消息，并停止编译
  - #line : 指定当前文件的行号和文件名
  - #include_next : 引入下一个头文件
- 常见预定义宏

```cpp
__STDC__          //当编译器与C99标准兼容时，该宏被定义为1
__STDC_HOSTED__   //当编译器与C99标准兼容时，该宏被定义为1
__func__          //该宏被定义为当前函数的名称
__FILE__          //该宏被定义为当前文件的名称
__LINE__          //该宏被定义为当前行的行号
__DATE__          //该宏被定义为当前日期
__TIME__          //该宏被定义为当前时间
__cplusplus       //该宏被定义为199711L
__VA_ARGS__       //是一个预处理器宏，表示变长参数
```

### namespace

命名空间（namespace）是C++中用于解决命名冲突的一种机制

1. 命名空间的定义格式
2. 命名空间的嵌套, 嵌套命名空间是指在一个命名空间内部再定义一个命名空间
3. 内联命名空间, 内联命名空间是指在C++17及以上版本中，可以使用inline关键字来定义一个内联命名空间。
4. using关键字，用于声明使用命名空间

### reference

引用是C++中一种特殊的变量类型，它可以看作是一个变量的别名，引用的主要特点是：

左值：是指那些可以出现在赋值操作符左边的表达式，即可以被取地址的表达式。左值通常有一个持久的内存地址，
并且在表达式结束后仍然存在

右值：是指那些不能出现在赋值操作符左边的表达式，即不能被取地址的表达式。右值通常是一个临时对象，
或者是一个常量值，分为将亡值(expiring rvalue)和纯右值(pure rvalue)

左值引用：是对左值的引用，它允许你通过引用修改原始左值。

右值引用：是对右值的引用，它允许你延长右值的生命周期，并在某些情况下实现移动语义。

移动语义（Move Semantics）：是C++11引入的一个重要特性，它允许对象的资源（如内存、文件句柄等）在不进行深拷贝的情况下
从一个对象转移到另一个对象，从而提高程序的性能和效率。

引用折叠：是指在模板推导或类型别名中，当引用类型与其他类型组合时，编译器会根据一定的规则进行类型折叠。

万能引用（Universal Reference）：是C++11中引入的一个概念，它是一种特殊的引用类型，可以绑定到左值或右值。万能引用通常用于模板函数中，以实现完美转发（Perfect Forwarding）。

std::ref和std::cref：是 C++ 标准库中的函数模板，用于创建 std::reference_wrapper 对象，
这些对象可以像引用一样使用，但可以被复制和存储，主要用于和std::bind配合使用

### class

在C++中，类是一种用户定义的数据类型，它封装了数据和操作这些数据的函数。类是面向对象编程的基本构建块，它允许你将数据和操作这些数据的函数组合在一起，形成一个逻辑单元。

1. 类的封装和权限控制，包括private, protected, public
2. 类的构造函数，包含默认构造函数，拷贝构造函数，移动构造函数，删除构造函数，显示构造函数，委托构造函数，继承构造函数
3. 类的析构函数，包含默认析构函数，删除析构函数
4. 友元函数和友元类，友元函数和友元类可以访问类的所有成员
5. 类的运算符重载operator
6. 类的继承和权限控制
7. 多重继承和虚继承
8. const和mutable在类中的运用
9. 虚函数，纯虚函数，接口类
10. final和override关键字
11. 类的重写，重载和隐藏
12. 列表初始化，非静态成员的立即初始化，std::initializer_list
13. 静态数据成员的inline初始化
14. 静态数据成员，静态函数和单例模式
15. 非静态数据成员的sizeof
15. RAII(Resource Acquisition is initialization)
16. 强枚举类型(enum class)
17. 非受限联合体(union)

### template

模板，是一种泛型编程的工具，它允许程序员编写通用的代码，而不是针对特定的数据类型。

1. 模板类型分为函数模板，类模板，变量模板，别名模板(using声明)
2. 模板按照参数分为非类型模板和类型模板。
3. class和typename都可以用来声明类型模板参数，相同点和区别
4. 可变参数模板是一种特殊的模板，它允许你在模板中使用可变数量的参数，sizeof...用于查询参数数目。
5. 模板的特化和偏特化
6. 默认模板参数:是指在模板定义中为模板参数提供一个默认值。
7. ADL查找规则，ADL（Argument-Dependent Lookup）
8. 用户自定义推导指引（User-Defined Deduction Guides），它允许程序员为类模板的构造函数提供自定义的类型推导规则
9. SFINAE（Substitution Failure Is Not An Error）用于在编译时根据模板参数的类型来选择不同的函数重载或模板特化
10. 外部模板（Explicit Template Instantiation）是C++中的一个特性，它允许程序员在一个编译单元中显式地实例化一个模板
11. 模板元编程是一种编程范式，它使用模板来编写程序，在编译时进行计算和生成代码

### stl

STL（Standard Template Library）是C++标准库的一部分，它提供了一组通用的容器、算法和函数，用于处理数据结构和算法的实现。

容器

1. array：固定大小的数组，提供了边界检查和随机访问功能。
2. deque：双端队列，支持在两端进行高效的插入和删除操作。
3. forward_list：单向链表，支持在链表头部进行高效的插入和删除操作。
4. list：双向链表，支持在链表任意位置进行高效的插入和删除操作。
5. map：关联容器，存储键值对，按键进行排序。
6. multimap：关联容器，存储键值对，按键进行排序。\
7. multiset: 关联容器，存储唯一的元素，按键进行排序。
8. priority_queue：优先队列，支持按照优先级进行元素的插入和删除操作。
9. queue：队列，遵循先进先出（FIFO）原则。
10. set：关联容器，存储唯一的元素，按键进行排序。
11. stack：栈，遵循后进先出（LIFO）原则。
12. unordered_map：无序关联容器，存储键值对，不进行排序。
13. unordered_multimap：无序关联容器，存储键值对，不进行排序。
14. unordered_multiset：无序关联容器，存储唯一的元素，不进行排序。
15. unordered_set：无序关联容器，存储唯一的元素，不进行排序。
16. vector：动态数组，支持随机访问和高效的尾部插入和删除操作。

迭代器

1. 正向迭代器（iterator）：可以向前遍历容器中的元素
2. 反向迭代器（reverse_iterator）：可以向后遍历容器中的元素
3. 常量正向迭代器（const_iterator）：只能读取容器中的元素，不能修改
4. 常量反向迭代器（const_reverse_iterator）：只能读取容器中的元素，不能修改

算法

1. 查找算法：find, find_if, find_first_of, find_end, search, search_n
2. 排序算法：sort, stable_sort, partial_sort, partial_sort_copy, nth_element
3. 变换算法：transform, replace, replace_if, replace_copy, replace_copy_if
4. 计数算法：count, count_if, accumulate, inner_product, partial_sum, adjacent_difference
5. 比较算法：equal, mismatch, lexicographical_compare
6. 生成算法：generate, generate_n, fill, fill_n, copy, copy_n, copy_if, copy_backward
7. 移除算法：remove, remove_if, remove_copy, remove_copy_if, unique, unique_copy
8. 分割算法：partition, stable_partition, partition_copy, partition_point
9. 归并算法：merge, inplace_merge, set_union, set_intersection, set_difference, set_symmetric_difference
10. 堆算法：push_heap, pop_heap, make_heap, sort_heap

### std

C++标准库是C++编程语言的一部分，它提供了一组通用的类和函数，用于处理各种常见的编程任务。标准库被组织成多个头文件，每个头文件包含一组相关的功能。以下是一些主要的C++标准库组件。

1. any: 可以存储任何类型的对象
2. bitset：位集合类，用于存储和操作二进制位。
3. locale：本地化类，用于处理本地化相关的功能。
4. new: 内存分配和释放
5. optional：可选值类，用于表示可能存在或不存在的值。
6. random：随机数生成器，提供了各种随机数生成算法。
7. regex：正则表达式类，用于处理正则表达式。
8. span：范围类，用于表示数组或容器的一部分。
9. stream: 输入输出流类，包含istream，ostream, iosteam，sstream，fstream，
10. string：字符串类，提供了字符串的操作和处理功能。
11. string_view：字符串视图类，提供了对字符串的只读访问。
12. tuple：元组类，用于存储多个不同类型的值。
13. type_traits：类型特性类，提供了类型的特性信息。
14. variant：变体类，用于存储不同类型的值。

### exception

C++ 异常处理(exception)是一种机制，用于在程序运行时处理错误和异常情况。它允许程序在遇到错误时抛出异常，并在适当的地方捕获和处理这些异常，从而避免程序崩溃或产生不可预测的行为。

1. try...catch语法格式
2. throw关键字
3. 异常传递和noexcept关键字
4. 异常类和用户自定义异常类

- std::exception：所有标准异常类的基类，提供了一个虚函数what()，用于返回异常的描述信息
- std::bad_alloc：当new操作符无法分配内存时抛出。
- std::bad_cast: 当dynamic_cast转换失败时抛出
- std::bad_typeid: 当typeid操作符无法获取类型信息时抛出
- std::bad_function_call: 当std::function对象无法调用时抛出
- std::invalid_argument: 当传递给函数的参数无效时抛出
- std::out_of_range: 当访问容器或数组时超出范围时抛出
- std::length_error: 当字符串长度超过允许的最大值时抛出
- std::logic_error: 当逻辑错误发生时抛出
- std::runtime_error: 当运行时错误发生时抛出
- std::overflow_error: 当数值溢出时抛出
- std::underflow_error: 当数值下溢时抛出
- std::system_error: 当系统错误发生时抛出

### layout

C++的内存布局是指在内存中如何组织和存储对象、变量和函数等，这里主要指类的内存布局。

1. 平凡类型，平凡类型（Trivial Type）指的是那些可以通过简单的内存拷贝（如 memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。
2. 平凡可复制类型（Trivially Copyable Type）指的是那些可以通过简单的内存拷贝进行复制，并且在复制过程中不会有任何特殊行为的类型。
3. 标准布局类型（Standard Layout Type）指的是那些具有特定标准布局的类型
4. 聚合类型（Aggregate Type）指的是那些可以通过简单的初始化列表进行初始化的类型
5. POD类型（Plain Old Data Type，POD）指的是满足平凡和标准布局的类型
6. 虚表和类的内存布局
7. 数据对齐(alignas和alignof)，alignas和alignof是C++11引入的关键字，用于指定类型的对齐方式。alignas用于指定类型的对齐方式，alignof用于获取类型的对齐方式。

### smart_pointer

智能指针是C++中的一种特殊类型的指针，它提供了自动内存管理的功能。智能指针通过在对象不再被使用时自动释放其所指向的内存，从而避免了内存泄漏的问题。C++标准库提供了三种主要的智能指针：unique_ptr、shared_ptr和weak_ptr。

1. unique_ptr：是一种独占所有权的智能指针，它确保同一时间只有一个unique_ptr指向某个对象。当unique_ptr被销毁时，它所指向的对象也会被自动删除。
2. shared_ptr：是一种共享所有权的智能指针，它允许多个shared_ptr指向同一个对象。当最后一个指向对象的shared_ptr被销毁时，对象才会被自动删除。
3. enable_shared_from_this: 用于在shared_ptr中创建指向当前对象的shared_ptr
4. weak_ptr：是一种弱引用的智能指针，它不拥有对象的所有权，而是指向一个shared_ptr所管理的对象。当最后一个指向对象的shared_ptr被销毁时，weak_ptr会自动失效。

### type_conversion

1. RTTI(Run-Time Type Information)：运行时类型信息，它是一种机制，允许程序在运行时获取对象的类型信息。
2. typeid返回一个std::type_info对象，该对象包含了对象的类型信息
3. 隐式转换和显示标识符(explicit)，隐式转换是指在不需要显式类型转换操作符的情况下，编译器自动将一种类型的值转换为另一种类型的值。显式标识符（explicit）是一个关键字，用于修饰类的构造函数。当构造函数被声明为显式时，编译器将禁止隐式转换，即禁止使用单个参数的构造函数进行隐式类型转换。
4. dynamic_cast：用于在运行时进行类型转换，它可以将一个指向基类的指针或引用转换为指向派生类的指针或引用。
5. static_cast：用于在编译时进行类型转换，它可以将一个表达式转换为指定的类型。
6. reinterpret_cast：用于在编译时进行类型转换，它可以将一个指针或引用转换为另一个类型的指针或引用。
7. const_cast：用于在编译时进行类型转换，它可以将一个常量指针或引用转换为一个非常量指针或引用。

### thread_coroutine

1. atomic：是一个模板类，用于提供原子操作。原子操作是指不可被中断的一个或一系列操作，要么全部执行，要么全部不执行。std::atomic 可以用于确保在多线程环境下对共享变量的操作是线程安全的。
2. bind：是一个函数模板，用于将一个函数或函数对象与一组参数绑定在一起，生成一个新的可调用对象。
3. chrono：是一个时间库，提供了各种时间相关的类型和函数。
4. condition_variable：是一个类，用于线程间的同步。它提供了一种机制，使得线程可以等待某个条件的满足，当条件满足时，线程可以被唤醒。
5. future：是一个类，用于表示异步操作的结果。
6. mutex和lock_guard：是一个类，用于线程间的同步。它提供了一种机制，使得多个线程可以互斥地访问共享资源。
7. thread：是一个类，用于创建和管理线程。
8. thread_local：是一个关键字，用于声明线程局部变量。
9. timer: 是一个类，用于创建和管理定时器。

### modern_cpp

- attribute属性说明符
  - [[noreturn]]: 表示函数不会返回，通常用于标记函数不会正常返回
  - [[carries_dependency]]: 用于在多线程编程中优化内存访问顺序
  - [[fallthrough]]: 表示在switch语句中，当一个case语句执行完后，会继续执行下一个case语句，而不是跳出switch语句
  - [[nodiscard]]: 表示函数的返回值可能会被忽略
  - [[maybe_unused]]: 表示变量可能会被忽略
  - [[likely]]: 表示某个条件很可能为真
  - [[unlikely]]: 表示某个条件很可能为假
  - [[no_unique_address]]: 表示变量的地址可能会被重复使用
- auto类型推导占位符，decltype类型推导表达式和函数返回类型后置语法
- constexpr常量表达式
- declval在不创建对象实例的情况下获取类型的右值引用
- 扩展的改进
  - if和switch语句支持初始化
  - nullptr空指针
  - 结构化绑定:auto[]表达式
  - for循环的扩展for(key:expression) do;
- lambda表达式和funtional库
- 用户自定义字面量，字符串字面量，单引号作为整数分隔符
- 断言和静态断言(static_assert)

### tools

主要包含C++应用中涉及的工具类和GCC相关的功能说明，也包含未整理仅验证的代码。

1. RVO/NRVO（Return Value Optimization）返回值优化
2. void_t 是一个在C++17中引入的模板元编程工具，它通常用于检测某个类型是否具有特定的成员或特性

### cpp20

1. concepts/requires: 用于在编译时检查模板参数是否满足特定的要求。concepts 是一种用于描述类型要求的语法，而 requires 则是用于在模板定义中指定这些要求的关键字
2. module：用于模块化编程的语法。module 是一种用于定义和组织代码的语法，它可以用于将代码分割成多个模块，并在需要时进行动态加载和卸载。
3. ranges：用于表示和操作序列的语法。ranges 是一种用于描述序列的语法，它可以用于表示容器、数组、迭代器等序列。
4. coroutines(协程)： 用于实现异步编程的语法。coroutines 是一种用于实现异步编程的语法，它可以用于在不阻塞线程的情况下执行异步操作。
