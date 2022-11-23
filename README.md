# cplusplus_application

## 综述

本工程主要用于C++的学习，包含基础应用，也有扩展和参考各工具书的学习实践.  
主要参考引用路径为:https://cplusplus.com/reference/  
代码测试环境:https://gcc.godbolt.org/  
C++是支持多范式不同实践的语言，往往很多应用也仅仅是C++的实践的一小部分，在学习C++的过程中，保持着好奇心，结合规范和实践，通过积累总结逐渐扩展对C++的认知，这也是这个工程的由来。
本项目的参考书籍如下:  
<C++ Primer Plus>  
<C++语言的设计和演化>  
<深入理解C++11 - C++11新特性解析与应用>  
<现代C++语言核心特性解析>  


## C++知识点总结

* 兼容C语法部分(包含KeyWords, 函数，循环，指针，sizeof等)  
  
* 命名空间(namespace)
  内联命名空间
  嵌套命令空间
  using用于声明使用命名空间
  Code: Src/KnowledgePoint/namespace/

* 引用特性  
  左值，右值，将亡值  
  左值引用，右值引用  
  移动语义(move)   
  万能引用和引用折叠  
  完美转发    
  std::ref引用转换  
  
* 类的应用
  封装, 访问控制(private, protected, public)  
  继承，权限控制(private, protected, public)  
  友元函数和友元类(friend)  
  多重继承和虚继承  
  虚函数，纯虚函数，接口类  
  派生，基类和派生类，overwrite、overload和override  
  overwrite和final表示符  
  构造函数(默认构造函数-default，删除构造函数-delete，显示构造函数-explicit，委托构造函数-构造函数调用其它构造函数，继承构造函数)  
  运算符重载(operator+)  
  列表初始化(initializer_list)
  RAII(Resource Acquisition is initialization)和std::lock_guard  
  非静态数据成员的默认初始化(=, {})，联合体
  静态数据成员(非const)的inline初始化 
  强枚举类型(enum class)

* C++对象模型布局
  AggregateType，TrivialType, TrivalCopyable, Standard-layout Type, POD，虚表和类的内存布局  
    
* TypeCast转换  
  RTTI(Run-Time Type Information), 显示和隐式转换(explicit关键字)，typeid, const_cast, reinterpret_cast, static_cast, dynamic_cast  
    
* 模板(template)
  模板类和模板函数(class, typename)  
  模板的全特化和偏特化
  默认模板参数
  外部模板
  可变参数模板，sizeof...
  SFINAE(Search Failed Is Not An Error)  
  using声明别名模板  
  模板元编程  
  
* STL(标准模板库)  
  容器: array, deque, forward_list, list, map, queue, set, stack, unordered_map, unordered_set, vector
  迭代器: iterator  
  构造对象: make_pair, make_tuple  
  算法(Algorithms): https://cplusplus.com/reference/algorithm/    
  其它标准库:string, tuple, bitset
  结构化绑定:auto[]表达式 
  for循环的扩展for(key:expression) do;

* 智能指针
  unique_ptr, shared_ptr和weak_ptr

* 多线程和协程
  atomic原子操作
  thread
  mutex互斥量
  chrono
  std::bind
  thread_local线程局部存储  
  condition_variable
  future(协程) 

* 现代C++功能添加(C++11~). 在上面已经描述部分C++11后新增功能，这里显示未提到功能
  auto类型推导占位符
  decltype类型推导表达式
  函数返回类型后置
  lambda表达式, std::function
  if和switch语句支持初始化
  静态断言(static_assert)
  constexpr常量表达式
  noexcept关键字
  nullptr空指针
  数据对齐(alignas和alignof)
  attribute属性说明符, [[noreturn]], [[carries_dependency]],[[carries_dependency]], [[fallthrough]], [[nodiscard]], [[maybe_unused]], [[likely]], [[unlikely]], [[no_unique_address]]
  用户自定义字面量，字符串字面量，单引号作为整数分隔符  

* Boost库扩展

  上面总结只是列出了我目前了解的C++知识点，在后续的学习中将梳理这些知识，分类进行整理说明，如果接触了解新的知识，也会持续更新该文档和内容代码。



  

  
