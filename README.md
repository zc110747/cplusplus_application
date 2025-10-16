# cplusplus_application

## 综述

C++是支持多范式不同实践的语言，往往很多应用也仅仅是C++的实践的一小部分。在学习C++的过程中，保持着好奇心，结合规范和实践，通过积累总结逐渐扩展对C++的认知，这也是这个工程的由来。本系列主要用于C++的学习，包含基础应用，也有扩展和参考各工具书的学习实践。可以把本系列当作C++学习中的目录指引，遇到不理解的概念，可以参考相应例程去了解，也可以参考相关资料进行说明。

本系列中大部分例程都是单项目文件，可以直接在线运行。另外测试使用版本是g++9.5.0、同时使用g++15.2.0验证，早期版本可能编译报错，一般为不支持最新特性。

建议在如下环境测试。

- [cpp在线测试环境](https://gcc.godbolt.org/)

本项目的参考资料和书籍如下。

- 《C++ Primer Plus》
- 《C++语言的设计和演化》
- 《深入理解C++11 - C++11新特性解析与应用》
- 《现代C++语言核心特性解析》
- [cpp语法说明网址](https://cplusplus.com/reference/)
- [cpp参考手册](https://en.cppreference.com/w/cpp.html)
- [microsoft关于C++的说明](https://learn.microsoft.com/zh-cn/cpp/standard-library/cpp-standard-library-reference?view=msvc-170)

## 目录

- [CPP基础内容:兼容C的语法](#cpp_base_style)
- [命名空间](#namespace)
- [引用特性](#reference)
- [类的特性](#class)
- [模板(template)](#template)
- [标准模板库(STL)](#stl)
- [标准库(STD)](#std)
- [异常(exception)](#exception)
- [布局(layout)](#layout)
- [智能指针](#smart_pointer)
- [类型转换](#type_conversion)
- [多线程和协程](#thread_coroutine)
- [Modern C++扩展功能](#modern_cpp)
- [c++ 2020~扩展语法](#cpp_new_feature)
- [工具类应用](#tools)

## C++知识点总结

### cpp_base_style

cpp基础说明包含兼容C的语法，以及宏定义等。兼容C的语法包含关键字, 函数，循环，数组和指针等, 这部分建议阅读《C Primer Plus》。

- 关键字包含如下
  - 基本数据类型，如int, char, float, double, bool, void, long, short, unsigned, signed
  - 控制语句，如if, else, while, do...while, for, switch/case, default, break, continue, goto, return
  - 存储类，如auto(C++修改了新的含义), extern, static, register, const, volatile, mutable, thread_local
  - 复合类型，如struct, union, enum
  - 预定义类型，#ifdef, #else, #endif, #define, #undef, #include, #line, #error, #pragma
  - 其它关键字，const, volatile, sizeof
- 函数
  - 函数声明，函数定义，函数调用
  - 函数的默认参数允许你为函数的一个或多个参数提供默认值
- 循环
  - while, do...while, for
- 数组和指针
  - 指针的概念，指针的运算，指针的类型
  - 指针的引用，指针的解引用
  - 指针的数组，指针的函数
- C基础库支持，包含cstdio、cstdlib、cstring、cmath、ctime、cassert、cctype等
- 预定义指令
  - #include : 引入头文件
  - #define, #undef : 定义和取消宏
  - #ifdef, #else, #endif : 条件编译
  - #if, #elif, #else, #endif : 条件编译
  - #pragma和_Pragma : 用于控制编译器的行为，#pragma once定义只编译一次
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

- limits: C++标准库中的头文件，包含了关于整数类型的限制和范围的常量。
- stdfloat: C++标准库中的头文件，包含了关于浮点数类型的限制和范围的常量。
- version: C++20引入的版本类，用于表示C++标准的版本号和配置信息。
- numbers: C++20引入的数值类，提供了数值相关的功能。

详细文档: [支持C语法说明](./knowledge/01.base_style/README.md)

### namespace

命名空间（namespace）是C++中用于解决命名冲突的一种机制。

1. 命名空间的定义格式: `namespace 命名空间名 { 命名空间成员 }`。
2. 命名空间的嵌套, 嵌套命名空间是指在一个命名空间内部再定义一个命名空间：`namespace 命名空间名 { namespace 子命名空间名 { 命名空间成员 } }`。
3. 内联命名空间, 内联命名空间是指在C++17及以上版本中，可以使用inline关键字来定义一个内联命名空间。内联命名空间的成员可以直接在全局命名空间中访问，而不需要使用命名空间限定符。
4. using关键字，用于声明使用命名空间中的成员, 格式为`using 命名空间名::成员名`。

详细文档: [命名空间说明](./knowledge/02.namespace/README.md)

### reference

引用是C++中一种特殊的变量类型，它可以看作是一个变量的别名。

- 左值：是指那些可以出现在赋值操作符左边的表达式，即可以被取地址的表达式。左值通常有一个持久的内存地址，并且在表达式结束后仍然存在。
- 右值：是指那些不能出现在赋值操作符左边的表达式，即不能被取地址的表达式。右值通常是一个临时对象，或者是一个常量值，分为将亡值(expiring rvalue)和纯右值(pure rvalue)。
- 左值引用：是对左值的引用，它允许你通过引用修改原始左值。常量左值引用则可以绑定到任何右值，也可以绑定到任何左值。
- 右值引用：是对右值的引用，它允许你延长右值的生命周期，并在某些情况下实现移动语义。
- 移动语义（Move Semantics）：是C++11引入的一个重要特性，它允许对象的资源（如内存、文件句柄等）在不进行深拷贝的情况下从一个对象转移到另一个对象，从而提高程序的性能和效率。使用std::move可以将左值转换为右值引用，从而能够赋值给右值引用。
- 引用折叠：是指在模板推导或类型别名中，当引用类型与其他类型组合时，编译器会根据一定的规则进行类型折叠。
- 万能引用（Universal Reference）：是C++11中引入的一个概念，它是一种特殊的引用类型，可以绑定到左值或右值。万能引用通常用于模板函数中，以实现完美转发（Perfect Forwarding），其格式为std::forward\<T\>(t)。
- std::ref和std::cref变量：是C++标准库中的函数模板，用于创建std::reference_wrapper对象，这些对象可以像引用一样使用，但可以被复制和存储，主要用于和std::bind配合使用。

详细文档: [引用说明](./knowledge/03.reference/README.md)

### class

在C++中，类是一种用户定义的数据类型，它封装了数据和操作这些数据的函数。类是面向对象编程的基本构建块，它允许你将数据和操作这些数据的函数组合在一起，形成一个逻辑单元，类的相关知识点如下所示。

- 类的封装和权限控制(private、protected、public)。
- 类的构造函数，包含默认构造函数、拷贝构造函数，移动构造函数，删除构造函数，显示构造函数，委托构造函数，继承构造函数。
- 类的析构函数，包含默认析构函数、删除析构函数、显示删除在类中的应用。
- 类的友元函数和友元类，友元函数和友元类可以访问类的所有成员。
- 类中的this指针，this指针是一个指向当前对象的指针，它在类的非静态成员函数中使用。
- 类的运算符重载operator。
- 类的继承和权限控制(private、protected、public)，多重继承和虚继承。
- const和mutable在类中的运用。
- 虚函数，包含虚函数定义、纯虚函数、接口类、虚函数表、虚析构函数。
- 多态，静态多态和动态多态。
- 动态绑定，虚函数表和虚函数指针。
- final和override关键字。
- 类的重写、重载和隐藏，重载的编译器实现细节。
- 列表初始化、非静态成员的立即初始化、std::initializer_list实现自定义类型的列表初始化。
- 静态数据成员、静态函数、静态数据成员的inline初始化、单例模式。
- 非静态数据成员的sizeof计算。
- RAII(Resource Acquisition Is Initialization)。
- 枚举类型的列表初始化，强枚举类型(enum class)。
- 非受限联合体(union)。
- 函数的局部类，允许定义函数内部的类。
- 类的默认比较函数，默认情况下，类的比较函数是按成员变量的顺序进行比较的。

详细文档: [类的说明](./knowledge/04.class/README.md)

### template

模板，是一种泛型编程的工具，它允许程序员编写通用的代码，而不是针对特定的数据类型。

- 模板类型分为函数模板，类模板，变量模板。
- 模板参数类型，可以分为类型模板（typename、class）和非类型模板（整型、指针、引用、枚举）。
- 类型别名和别名模板，使用using关键字声明类型别名，支持template指定别名模板。
- class和typename都可以用来声明类型模板参数，相同点和区别。
- 可变参数模板是一种特殊的模板，它允许你在模板中使用可变数量的参数，sizeof...用于查询参数数目。
- 使用auto声明非类型模板参数。
- 折叠表达式，用于对模板参数包在二元运算符上进行折叠操作。
- 模板的特化和偏特化。
- 默认模板参数:是指在模板定义中为模板参数提供一个默认值。
- ADL查找规则，ADL（Argument-Dependent Lookup）。
- 用户自定义推导指引（User-Defined Deduction Guides），它允许程序员为类模板的构造函数提供自定义的类型推导规则。
- SFINAE（Substitution Failure Is Not An Error）用于在编译时根据模板参数的类型来选择不同的函数重载或模板特化。
- enable_if/enable_if_t：用于在编译时根据条件选择是否启用某个函数或模板特化。
- 外部模板（Explicit Template Instantiation）是C++中的一个特性，它允许程序员在一个编译单元中显式地实例化一个模板。
- 模板元编程是一种编程范式，它使用模板来编写程序，在编译时进行计算和生成代码。

详细文档: [模板功能说明](./knowledge/05.template/README.md)

### stl

STL（Standard Template Library）是C++标准库的一部分，它提供了一组通用的容器、算法和函数，用于处理数据结构和算法的实现。

容器

- array：固定大小的数组，提供了边界检查和随机访问功能。
- deque：双端队列，支持在两端进行高效的插入和删除操作。
- flat_map：C++23引入的关联容器，存储键值对，按键进行排序。
- flat_set：C++23引入的关联容器，存储唯一的元素，按键进行排序。
- forward_list：单向链表，支持在链表头部进行高效的插入和删除操作。
- list：双向链表，支持在链表任意位置进行高效的插入和删除操作。
- map：关联容器，存储键值对，按键进行排序。
- multimap：关联容器，存储键值对，按键进行排序。
- multiset: 关联容器，存储唯一的元素，按键进行排序。
- priority_queue：优先队列，支持按照优先级进行元素的插入和删除操作。
- queue：队列，遵循先进先出（FIFO）原则。
- set：关联容器，存储唯一的元素，按键进行排序。
- stack：栈，遵循后进先出（LIFO）原则。
- unordered_map：无序关联容器，存储键值对，不进行排序。
- unordered_multimap：无序关联容器，存储键值对，不进行排序。
- unordered_multiset：无序关联容器，存储唯一的元素，不进行排序。
- unordered_set：无序关联容器，存储唯一的元素，不进行排序。
- vector：动态数组，支持随机访问和高效的尾部插入和删除操作。

迭代器

- 正向迭代器（iterator）：可以向前遍历容器中的元素。
- 反向迭代器（reverse_iterator）：可以向后遍历容器中的元素。
- 常量正向迭代器（const_iterator）：只能读取容器中的元素，不能修改。
- 常量反向迭代器（const_reverse_iterator）：只能读取容器中的元素，不能修改。

算法

- 查找算法：all_of，any_of，none_of，adjacent_find，find, find_if, find_first_of, find_end, search, search_n，binary_search。
- 排序算法：sort, stable_sort, partial_sort, partial_sort_copy, is_sorted, is_sorted_until，random_shuffle，reserve，rotate，rotate_copy。
- 变换算法：transform, replace, replace_if, replace_copy, replace_copy_if。
- 计数算法：count, count_if, accumulate, inner_product, partial_sum, adjacent_difference。
- 比较算法：equal, mismatch, lexicographical_compare。
- 生成算法：generate, generate_n, fill, fill_n, copy, copy_n, copy_if, copy_backward。
- 移除算法：remove, remove_if, remove_copy, remove_copy_if, unique, unique_copy。
- 分割算法：partition, stable_partition, partition_copy, partition_point。
- 归并算法：merge, inplace_merge, set_union, set_intersection, set_difference, set_symmetric_difference。
- 堆算法：push_heap, pop_heap, make_heap, sort_heap。
- 其它算法: sample, clamp, reduce, transform_reduce, gcd, lcm, to_chars, from_chars。

详细文档: [STL功能说明](./knowledge/06.stl/README.md)

### std

C++标准库是C++编程语言的一部分，它提供了一组通用的类和函数，用于处理各种常见的编程任务。标准库被组织成多个头文件，每个头文件包含一组相关的功能。以下是一些主要的C++标准库组件。

- any: 可以存储任何类型的对象。
  - std::in_place_type：是C++17引入的一个函数模板，用于在原地构造对象。
  - std::make_any：是C++17引入的一个函数模板，用于创建any对象。
  - std::any_cast：是C++17引入的一个函数模板，用于将any对象转换为指定类型的对象。
- bit: C++20引入的位操作类，用于对二进制位进行操作。
  - bitset：位集合类，用于存储和操作二进制位。
  - std::bit_cast：是C++20引入的一个函数模板，用于在不同类型的对象之间进行位级转换。
- charconv：C++17引入的字符转换类，用于在不同类型的字符之间进行转换。
- compare: C++20引入的比较类，提供了比较相关的功能。
- complex: 提供了复数相关的运算功能。
- filesystem: C++17引入的文件系统类，提供了跨平台的文件系统操作功能。
- functional: 是函数对象的包装器，用于将函数、函数指针、函数对象等转换为可调用对象。
  - std::function：是C++11引入的一个通用的多态函数包装器，它可以存储、复制和调用任何可调用对象，如函数指针、函数对象、lambda表达式等。
  - std::bind：是函数模板，用于将一个函数或函数对象与一组参数绑定在一起，生成一个新的可调用对象。
  - std::invoke：是C++17引入的一个函数模板，用于调用一个函数对象或函数指针，并传入参数。
- generator: C++23引入的生成器类，用于生成序列的值。
- locale：本地化类，用于处理本地化相关的功能。
- memory_resource: C++17引入的内存资源类, 用于管理内存分配和释放。
- new: 内存分配和释放管理。
- optional: C++17引入可选值类，用于表示可能存在或不存在的值。
  - std::optional：类模板，用于表示可能存在或不存在的值。
  - std::nullopt：空值类型，用于表示std::optional对象没有值。
- random：随机数生成器，提供了各种随机数生成算法。
- ratio: 有理数类，用于表示有理数。
- regex：正则表达式类，用于处理正则表达式。
- scope_allocator: C++17引入的作用域分配器类，用于在不同作用域之间分配内存。
- stream: 输入输出流类，包含istream，ostream, ofstream，ifstream，stringstream，cin, cout, cerr，clog。
  - std::iostream：是C++标准库中的一个类，用于处理输入输出流。它是std::istream和std::ostream的基类，提供了基本的输入输出功能。
  - std::fstream：是C++标准库中的一个类，用于处理文件流。它是std::iostream的子类，提供了文件的读取和写入功能。
  - std::sstream：是C++标准库中的一个类，用于处理字符串流。它是std::iostream的子类，提供了字符串的读取和写入功能。
- string：字符串类，提供了字符串的操作和处理功能。
- string_view：C++17引入的字符串视图类，提供了对字符串的只读访问。
- source_location：C++20支持的源位置类，用于获取当前函数的源位置信息。
- tuple：元组类，用于存储多个不同类型的值。
  - std::apply：C++17引入的一个函数模板，用于将一个元组中的元素应用到一个函数或lambda表达式中。
- type_traits：类型特性类，提供了类型的特性信息。另外支持通过std::enable_if和std::disable_if来进行条件编译，用于模板函数和模板类的SFINAE查询。
- utility: 供了一系列实用工具和通用类型，如std::swap, std::make_pair、std::move，std::forward，std::declval等。
  - std::declval在不创建对象实例的情况下获取类型的右值引用。
- valarray：定长数组类，用于存储固定长度的元素。
- variant：C++17引入的变体类，用于存储不同类型的值。

详细文档: [标准库说明](./knowledge/07.std/README.md)

### exception

C++异常处理(exception)是一种机制，用于在程序运行时处理错误和异常情况。它允许程序在遇到错误时抛出异常，并在适当的地方捕获和处理这些异常，从而避免程序崩溃或产生不可预测的行为。

- try...catch语法格式。
- throw关键字。
- 异常传递和noexcept关键字。
- 异常类和用户自定义异常类。
  - std::exception：所有标准异常类的基类，提供了一个虚函数what()，用于返回异常的描述信息
  - std::bad_alloc：当new操作符无法分配内存时抛出。
  - std::bad_cast: 当dynamic_cast转换失败时抛出。
  - std::bad_typeid: 当typeid操作符无法获取类型信息时抛出。
  - std::bad_function_call: 当std::function对象无法调用时抛出。
  - std::invalid_argument: 当传递给函数的参数无效时抛出。
  - std::out_of_range: 当访问容器或数组时超出范围时抛出。
  - std::length_error: 当字符串长度超过允许的最大值时抛出。
  - std::logic_error: 当逻辑错误发生时抛出。
  - std::runtime_error: 当运行时错误发生时抛出。
  - std::overflow_error: 当数值溢出时抛出。
  - std::underflow_error: 当数值下溢时抛出。
  - std::system_error: 当系统错误发生时抛出。
- std::expected，C++23提供用于表示一个操作可能成功返回一个值，也可能失败并返回一个错误信息。
- system_error: C++11引入的系统错误类，用于表示系统相关的错误。

详细文档: [异常处理说明](./knowledge/08.exception/README.md)

### layout

C++的内存布局是指在内存中如何组织和存储对象、变量和函数等，这里主要指类的内存布局。

- 平凡类型（Trivial Type）指的是那些可以通过简单的内存拷贝（如 memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。
- 平凡可复制类型（Trivially Copyable Type）指的是那些可以通过简单的内存拷贝进行复制，并且在复制过程中不会有任何特殊行为的类型。
- 标准布局类型（Standard Layout Type）指的是那些具有特定标准布局的类型。
- 聚合类型（Aggregate Type）指的是那些可以通过简单的初始化列表进行初始化的类型；聚合类型支持带基类，并通过指定初始化进行初始化。
- POD类型（Plain Old Data Type，POD）指的是满足平凡和标准布局的类型。
- 虚表和类的内存布局。
- 数据对齐相关处理。
  - alignas用于指定类型的对齐值，alignas设置的值必须是2的幂值，否则无法通过编译。
  - alignof用于获取类型的对齐值，可以获取类、结构体以及其成员变量的对齐值。
  - std::alignment_of：用于获取类型的对齐值，返回一个std::size_t类型的值。
  - std::aligned_storage_t和std::aligned_union_t: 在C++23里已经弃用，不建议使用。

详细文档: [内存布局说明](./knowledge/09.layout/README.md)

### smart_pointer

智能指针是C++中的一种特殊类型的指针，它提供了自动内存管理的功能。智能指针通过在对象不再被使用时自动释放其所指向的内存，从而避免了内存泄漏的问题，智能指针定义在<memory>头文件中。

C++标准库提供了三种主要的智能指针：unique_ptr、shared_ptr和weak_ptr。

- unique_ptr：是一种独占所有权的智能指针，它确保同一时间只有一个unique_ptr指向某个对象。当unique_ptr被销毁时，它所指向的对象也会被自动删除，std::make_unique是创建std::unique_ptr实例的推荐方式。
- shared_ptr：是一种共享所有权的智能指针，它允许多个shared_ptr指向同一个对象。当最后一个指向对象的shared_ptr被销毁时，对象才会被自动删除。
  - enable_shared_from_this: 用于在shared_ptr中创建指向当前对象的shared_ptr。
- weak_ptr：是一种弱引用的智能指针，它不拥有对象的所有权，而是指向一个shared_ptr所管理的对象。当最后一个指向对象的shared_ptr被销毁时，weak_ptr会自动失效。

详细文档: [智能指针说明](./knowledge/10.smart_pointer/README.md)

### type_conversion

- RTTI(Run-Time Type Information)：运行时类型信息，它是一种机制，允许程序在运行时获取对象的类型信息。
- typeid返回一个std::type_info对象，该对象包含了对象的类型信息。
- 隐式转换和显示标识符(explicit)，隐式转换是指在不需要显式类型转换操作符的情况下，编译器自动将一种类型的值转换为另一种类型的值。显式标识符（explicit）是一个关键字，用于修饰类的构造函数。当构造函数被声明为显式时，编译器将禁止隐式转换，即禁止使用单个参数的构造函数进行隐式类型转换。
- dynamic_cast：用于在运行时进行类型转换，它可以将一个指向基类的指针或引用转换为指向派生类的指针或引用。
- static_cast：用于在编译时进行类型转换，它可以将一个表达式转换为指定的类型。
- reinterpret_cast：用于在编译时进行类型转换，它可以将一个指针或引用转换为另一个类型的指针或引用。
- const_cast：用于在编译时进行类型转换，它可以将一个常量指针或引用转换为一个非常量指针或引用。

详细文档: [类型转换说明](./knowledge/11.type_conversion/README.md)

### thread_coroutine

- atomic包含原子操作的处理，
  - std::atomic是模板类，用于提供原子操作。
  - std::atomic_flag则是最简单的原子类型，用于实现简单的同步机制。
  - std::memory_order和内存一致性问题。std::memory_order是枚举类型，用于指定原子操作的内存顺序，以此来协调多线程间的内存访问，确保程序在多线程环境下的正确性与性能。
- barrier: C++20引入的用于线程同步的类，它允许多个线程在某个点上等待，直到所有线程都到达该点。
- chrono是时间库，提供了各种时间相关的类型和函数。
- condition_variable用于线程间的同步的类，使得线程可以等待某个条件的满足，当条件满足时，线程可以被唤醒。
- execution是C++17新增的库，用于控制并发执行的策略和方式。
- future包含异步操作的类，使得线程可以等待某个异步操作的完成，并获取其结果。
  - std::future是类模板，用于表示异步操作的结果。
  - std::async是函数模板，用于启动一个异步操作。
  - std::promise是类模板，用于在一个线程中设置一个值，并在另一个线程中获取该值。
- latch: C++20引入的用于多线程同步的类，可让一组线程等待直到某个事件完成。
- mutex包含互斥锁和锁guard类，用于管理线程资源互斥访问。
  - std::mutex是互斥锁，使得多个线程可以互斥地访问共享资源。
  - std::timed_mutex是带超时功能的互斥锁，允许在指定时间内等待获取互斥锁。
  - std::recursive_mutex是递归互斥锁，允许多个线程递归地获取同一个互斥锁。
  - std::recursive_timed_mutex是递归带超时功能的互斥锁，允许多个线程递归地获取同一个互斥锁。
  - std::call_once是函数模板，用于保证多线程环境下某个函数只被调用一次。
  - std::lock_guard是类模板，用于在构造函数中获取互斥锁，在析构函数中释放互斥锁。
  - std::unique_lock是类模板，用于在构造函数中获取互斥锁，在析构函数中释放互斥锁。
  - std::scope_lock是类模板，用于在构造函数中获取多个互斥锁，在析构函数中释放多个互斥锁。
- semaphore: C++20引入的用于多线程同步的类，可让一组线程等待直到某个事件完成。
  - std::counting_semaphore是计数信号量，用于控制多个线程对共享资源的访问。
  - std::binary_semaphore是计数信号量设置为1的二进制信号量，用于控制多个线程对共享资源的访问。
- shared_mutex：C++17引入的共享互斥锁，允许多个线程同时读共享资源，但只允许一个线程写共享资源。
- stop_token: C++20引入的协作式取消机制，用于线程间通信请求停止操作。
- syncstream: C++20引入的同步流类，用于在多线程环境下安全地输出到流。
- thread是C++用于管理线程的类，它提供了创建、管理和同步线程的功能。
  - std::thread是类模板，用于创建和管理线程。
  - thread_local：是关键字，用于声明线程局部存储变量。

详细文档: [线程和协程说明](./knowledge/12.thread_coroutine/README.md)

### modern_cpp

- attribute属性说明符
  - [[noreturn]]: 表示函数不会返回，通常用于标记函数不会正常返回。
  - [[carries_dependency]]: 用于在多线程编程中优化内存访问顺序。
  - [[fallthrough]]: 表示在switch语句中，当一个case语句执行完后，会继续执行下一个case语句，而不是跳出switch语句。
  - [[nodiscard]]: 表示函数的返回值可能会被忽略。
  - [[maybe_unused]]: 表示变量可能会被忽略。
  - [[likely]]: 表示某个条件很可能为真。
  - [[unlikely]]: 表示某个条件很可能为假。
  - [[no_unique_address]]: 表示变量的地址可能会被重复使用。
- 类型推导方法
  - auto类型推导占位符，auto关键字用于自动类型推导，它可以根据变量的初始化表达式来推断变量的类型。
    - 返回类型推导是C++14引入的特性，允许编译器自动推导函数的返回类型，简化代码编写。
  - decltype类型推导表达式和函数返回类型后置语法
    - decltype(auto)，C++14引入，与auto相比，推导时保留cv限定符(const、volatile)。
- constexpr常量表达式、编译器条件判断
  - constexpr是C++11引入的关键字，用于声明常量表达式, const表示常量。
  - constexpr再C++14中放宽限制，constexpr函数支持局部变量和赋值语句、if/switch条件语句、for/while/do-while循环语句，以及修改对象的非const成员等。
  - 编译期条件判断是C++17后引入的，支持if constexpr和switch constexpr进行条件判断。
  - lambda表达式支持constexpr(C++17)，即可以在编译时对lambda表达式进行求值，不满足则退化为普通函数。
  - consteval是C++20引入的关键字，用于声明一个函数只能在编译时调用，不能在运行时调用，这种函数被称为立即函数。
- 扩展的改进
  - 带初始化的条件语句if, switch(C++17)。
  - nullptr空指针和std::nullptr_t空指针类型。
  - 结构化绑定(C++17)：auto[]表达式。
  - for循环的扩展for(key:expression)语句、for支持初始化语句(C++17)、自定义类型支持for扩展。
- lambda表达式、函数对象、泛型lambda表达式、lamdba捕获初始化器
  - lambda表达式是一种匿名函数，它可以捕获外部变量并在需要时执行。
  - 函数对象是重载了函数调用运算符()的类，可以像函数一样被调用(仿函数)。
  - 泛型lambda表达式(C++14引入)，允许在lambda表达式中使用auto占位符来表示参数类型。
  - lambda捕获初始化器(C++14引入)，允许在lambda捕获列表中对捕获的变量进行自定义初始化，还能创建新的局部变量供lambda函数体使用。
- 字面量和自定义实现
  - 字符串字面量，允许使用字符串字面量R("")来创建字符串对象。
  - 用户自定义字面量，允许用户自定义字符串字面量，并定义自己的字符串字面量处理函数。
  - 单引号作为整数分隔符，允许使用单引号来分隔整数字面量，提高代码的可读性。
- 断言和静态断言(static_assert)
  - assert是宏，用于在运行时检查条件是否为真，如果条件不为真，则输出错误信息并终止程序。
  - static_assert是静态断言，用于在编译时检查条件是否为真，如果条件不为真，则输出错误信息并终止编译。

详细文档: [现代C++功能说明](./knowledge/13.modern_cpp/README.md)

### cpp_new_feature

- concepts/requires：用于在编译时检查模板参数是否满足特定的要求。concepts 是一种用于描述类型要求的语法，而 requires 则是用于在模板定义中指定这些要求的关键字。
- module：用于模块化编程的语法。module是一种用于定义和组织代码的语法，它可以用于将代码分割成多个模块，并在需要时进行动态加载和卸载。
- ranges：用于表示和操作序列的语法。ranges是一种用于描述序列的语法，它可以用于表示容器、数组、迭代器等序列。
  - std::ranges::views：用于创建和操作序列的语法。std::ranges::views 是一种用于创建和操作序列的语法，它可以用于创建新的序列，或者对现有的序列进行转换和操作。
  - std::ranges::actions：用于对序列进行操作的语法。std::ranges::actions 是一种用于对序列进行操作的语法，它可以用于对现有的序列进行转换和操作。
- coroutines(协程)：用于实现异步编程的语法。coroutines 是一种用于实现异步编程的语法，它可以用于在不阻塞线程的情况下执行异步操作。
- print: C++23引入的打印函数，用于在标准输出流中打印值。
- stacktrace: C++23引入的栈跟踪类，用于获取当前线程的调用栈信息。
- span：C++20引入的范围类，用于表示数组或容器的一部分。
  - mdspan: C++23引入的多维数组视图类，用于表示和操作多维数组。
  - spanstream: C++23引入的范围流类，用于在流中操作范围。
- format: C++20引入的格式化字符串库，用于格式化字符串。
- 三向比较运算符：<=>，C++20引入的运算符，用于比较两个对象的大小关系。另外支持std::strong_ordering、std::weak_ordering和std::partial_ordering。
- C++26新增功能(目前不一定支持)
  - contracts: C++26引入的合约编程功能，用于在编译时检查函数的前置条件、后置条件和不变量。
  - hazard_pointer: C++26引入的 hazard pointer 机制，用于解决并发编程中的内存管理问题。
  - rcu: C++26引入的 RCU(Read-Copy-Update) 机制，用于在并发编程中实现无锁的数据结构。
  - text_encoding: C++26引入的文本编码库，用于处理不同编码的文本。
  - debugger: C++26引入的调试器工具类，用于调试C++程序。
  - hive: C++26引入的 hive 机制，用于在并发编程中实现无锁的数据结构。
  - linalg: C++26引入的线性代数库，用于进行矩阵和向量的计算。
  - simd: C++26引入的 SIMD(Single Instruction, Multiple Data) 库，用于进行并行计算。

详细文档: [2020~ C++新增功能](./knowledge/14.cpp_new_feature/README.md)

### tools

主要包含C++应用中涉及的工具类和GCC相关的功能说明，也包含未整理仅验证的代码。

- exec: 执行系统命令的函数。
- image: 图片处理工具类。
- logger: 日志工具类。
- math_tools: 数学工具类。
- multi_queue: 多队列管理
- safety_cpp: 安全C++工具类。
- thread_pool: 线程池工具类。
- timer: 定时器工具类。
- void_t: C++17中引入的模板元编程工具，它通常用于检测某个类型是否具有特定的成员或特性。
- gcc支持编译命令行参数。
  - -std=c++17 ：指定使用C++17标准编译。
  - -Wall：显示所有警告信息。
  - -Wextra：显示额外的警告信息。
  - -Werror：将警告视为错误。
  - -O2：开启优化等级2，常见的优化等级有-O0、-O1、-O2、-O3、-Os。
  - -fno-elide-constructors：禁用构造函数的省略优化，确保在需要时调用构造函数。
  - -g：生成调试信息，支持使用gdb调试。
  - -fcoroutines：开启C++20协程支持。
  - -fmodules-ts：开启C++20模块支持。
  - -fconcepts：开启C++20概念支持。
  - -fconcepts-diagnostics-depth=2：开启C++20概念诊断深度2，用于显示更详细的概念错误信息。
  - -l[lib]：链接指定的库文件。例如，-lm 链接数学库、-lpthread 链接线程库。
