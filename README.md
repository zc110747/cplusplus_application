# cplusplus_application

## 综述

本工程主要用于C++的学习，包含基础应用，也有扩展和参考各工具书的学习实践.
主要参考引用路径为:https://cplusplus.com/
代码测试环境:https://gcc.godbolt.org/
C++是支持多范式不同实践的语言，往往很多应用也仅仅是C++的实践的一小部分，在学习C++的过程中，保持着好奇心，结合规范和实践，通过积累总结逐渐扩展对C++的认知，这也是这个工程的由来。
本项目的参考书籍如下:
<C++ Primer Plus>  
<C++语言的设计和演化>  
<深入理解C++11 - C++11新特性解析与应用>  
<现代C++语言核心特性解析>  


## C++知识点总结

* 兼容C语法部分(包含KeyWords, 函数，循环，指针等)
* 引用特性  
  左值，右值，将亡值，左值引用，右值引用，移动语义(move), 万能引用和引用折叠，完美转发
* 类的特性
  封装, 访问控制(private, protected, public)  
  继承，权限控制(private, protected, public)  
  友元函数和友元类  
  多重继承和虚继承  
  虚函数，纯虚函数，接口类  
  派生，基类和派生类，overwrite、overload和override  
  overwrite和final表示符  
  构造函数(默认构造函数，删除构造函数，显示构造函数，委托构造函数，继承构造函数)  
  运算符重载  
  列表初始化  
  RAII(Resource Acquisition is initialization)  
* C++对象模型布局
  AggregateType，TrivialType, TrivalCopyable, Standard-layout Type, POD，虚表和类的内存布局
* TypeCast  
  RTTI(Run-Time Type Information), 显示和隐式转换(explicit关键字)，typeid, const_cast, reinterpret_cast, static_cast, dynamic_cast  
* 模板  
  模板类和模板函数(template, class, typename)  
  模板的全特化和偏特化
  可变参模板
  SFINAE(Search Failed Is Not An Error)  
* STL(标准模板库)  
  容器: array, deque, forward_list, list, map, queue, set, stack, unordered_map, unordered_set, vector
  迭代器: iterator
  算法(Algorithms): https://cplusplus.com/reference/algorithm/  
* 多线程  
  atomic, condition_variable, future, mutex, thread
* 现代C++功能添加(C++11~)  
  auto  
  decltype  
  lambda  
  chrono  


  
