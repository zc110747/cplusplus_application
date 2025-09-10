# smart_point

智能指针是C++中的一种特殊类型的指针，它提供了自动内存管理的功能。智能指针通过在对象不再被使用时自动释放其所指向的内存，从而避免了内存泄漏的问题。C++标准库提供了三种主要的智能指针：unique_ptr、shared_ptr和weak_ptr。

- [std::unique_ptr](#unique_ptr)
- [std::shared_ptr](#shared_ptr)
- [std::weak_ptr](#weak_ptr)

## unique_ptr

unique_ptr是一种独占所有权的智能指针，它确保同一时间只有一个unique_ptr指向某个对象。

unique_ptr的主要特点如下：

1. 独占所有权：unique_ptr确保同一时间只有一个unique_ptr指向某个对象，避免了多个指针同时修改同一块内存的问题。
2. 自动内存管理：当unique_ptr被销毁时，它所指向的对象也会被自动删除，避免了内存泄漏。
3. 不支持复制操作：unique_ptr不支持复制操作，防止了多个指针指向同一个对象的问题。
4. 支持移动操作：可以通过std::move将unique_ptr的所有权转移给另一个unique_ptr，避免了复制操作的性能问题。

当unique_ptr被销毁时，它所指向的对象也会被自动删除。unique_ptr不支持复制操作，但可以通过std::move进行所有权的转移。

**注: std::make_unique是创建std::unique_ptr实例的推荐方式。**

```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() : data_(1) {
        std::cout << "MyClass constructor" << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destructor" << std::endl;
    }
    void show(void) {
        std::cout << "data_: " << data_ << std::endl;
    }
private:
    int data_;
};

int main(int argc, char *argv[]) 
{
    // 申请单个变量
    std::unique_ptr<int> p1 = std::make_unique<int>(1);
    std::cout << *p1 << std::endl;
    *p1 = 2;
    std::cout << *p1 << std::endl;
    std::unique_ptr<int> p2 = std::move(p1);  // 此时p1失效
    std::cout << *p2 << std::endl;

    // 申请数组
    std::unique_ptr<int[]> p3 = std::make_unique<int[]>(10);
    for (int i = 0; i < 10; i++) {
        p3[i] = i;
    }
    for (int i = 0; i < 10; i++) {
        std::cout << p3[i] << " ";
    }
    std::cout << std::endl;

    // 申请自定义类型
    {
        std::unique_ptr<MyClass> p4 = std::make_unique<MyClass>();
        p4->show();
    }

    return 0;
}
```

## shared_ptr

shared_ptr是一种共享所有权的智能指针，它允许多个shared_ptr指向同一个对象。

实践中，当最后一个指向对象的shared_ptr被销毁时，对象才会被自动删除。shared_ptr通过引用计数来管理对象的生命周期。

shared_ptr的主要特点如下：

1. 共享所有权：多个shared_ptr可以指向同一个对象，共享对对象的所有权。
2. 自动内存管理：当最后一个指向对象的shared_ptr被销毁时，对象才会被自动删除，避免了内存泄漏。
3. 引用计数：shared_ptr使用引用计数来跟踪有多少个指针指向同一个对象，当引用计数为0时，对象才会被删除。
4. 线程安全：shared_ptr在多线程环境下是线程安全的，多个线程可以同时访问和修改shared_ptr指向的对象。

**注: std::make_shared是创建std::shared_ptr实例的推荐方式。**

```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() : data_(1) {
        std::cout << "MyClass constructor" << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destructor" << std::endl;
    }
    void show(void) {
        std::cout << "data_: " << data_ << std::endl;
    }
private:
    int data_;
};

int main(int argc, char *argv[]) 
{
    // 申请单个变量
    std::shared_ptr<int> p1 = std::make_shared<int>(1);
    std::cout << "p1 value:" <<  *p1 << std::endl;
    *p1 = 2;
    std::cout << "p1 value:" << *p1 << std::endl;

    {
        std::shared_ptr<int> p2 = p1;
        std::cout << "p2 value:" << *p2 << std::endl;
        std::cout << "p1 use_count:" << p1.use_count() << std::endl;
    }
    std::cout << "p1 use_count:" <<  p1.use_count() << std::endl;

    // 申请数组(C++20以上支持)
    std::shared_ptr<int[]> p3 = std::make_shared<int[]>(10);
    for (int i = 0; i < 10; i++) {
        p3[i] = i;
    }
    for (int i = 0; i < 10; i++) {
        std::cout << p3[i] << " ";
    }
    std::cout << std::endl;

    // 申请自定义类型
    {
        std::shared_ptr<MyClass> p4 = std::make_shared<MyClass>();
        p4->show();
    }

    return 0;
}
```

## weak_ptr

weak_ptr是一种弱引用的智能指针，它指向由shared_ptr管理的对象，但不会增加对象的引用计数。weak_ptr主要用于解决循环引用的问题，避免内存泄漏。

weak_ptr的主要特点如下：

1. 弱引用：weak_ptr不会增加对象的引用计数，不会影响对象的生命周期。
2. 无所有权：weak_ptr不拥有对象的所有权，不能直接访问对象的成员。
3. 过期检测：weak_ptr可以检测所指向的对象是否已过期，避免访问已被删除的对象。
4. 转换为shared_ptr：可以通过调用lock()方法将weak_ptr转换为shared_ptr，以获取对象的所有权。

具体示例如下。

```cpp
#include <iostream>
#include <memory>

class Node {
public:
    int data_;
    
    // 使用 weak_ptr 避免循环引用
    std::weak_ptr<Node> next_;
    std::weak_ptr<Node> prev_;
    
    Node(int data) : data_(data) {
        std::cout << "Node " << data_ << " constructor" << std::endl;
    }
    
    // 检查并打印下一个节点
    void show_next() {
        std::shared_ptr<Node> next_shared = next_.lock();
        if (next_shared) {
            std::cout << "Next node data: " << next_shared->data_ << std::endl;
        } else {
            std::cout << "Next node has been destroyed" << std::endl;
        }
    }
    
    ~Node() {
        std::cout << "Node " << data_ << " destructor" << std::endl;
    }
};

// 演示循环引用问题的解决方案
class Parent;
class Child;

class Parent {
public:
    int id_;
    std::shared_ptr<Child> child_;
    Parent(int id) : id_(id) {
        std::cout << "Parent " << id_ << " constructor" << std::endl;
    }
    
    ~Parent() {
        std::cout << "Parent " << id_ << " destructor" << std::endl;
    }
};

class Child {
public:
    int id_;
    // 使用 weak_ptr 避免循环引用
    std::weak_ptr<Parent> parent_;
    Child(int id) : id_(id) {
        std::cout << "Child " << id_ << " constructor" << std::endl;
    }
    
    void show_parent() {
        std::shared_ptr<Parent> parent_shared = parent_.lock();
        if (parent_shared) {
            std::cout << "Parent id: " << parent_shared->id_ << std::endl;
        } else {
            std::cout << "Parent has been destroyed" << std::endl;
        }
    }
    
    ~Child() {
        std::cout << "Child " << id_ << " destructor" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    std::cout << "=== weak_ptr 基本使用 ===" << std::endl;
    {
        std::shared_ptr<int> shared = std::make_shared<int>(42);
        std::weak_ptr<int> weak = shared;
        
        std::cout << "shared use_count: " << shared.use_count() << std::endl;
        std::cout << "weak is expired: " << std::boolalpha << weak.expired() << std::endl;
        
        // 通过 lock() 获取 shared_ptr
        std::shared_ptr<int> locked = weak.lock();
        if (locked) {
            std::cout << "locked value: " << *locked << std::endl;
        }
    }
    std::cout << std::endl;
    
    std::cout << "=== 链表节点示例 ===" << std::endl;
    {
        auto node1 = std::make_shared<Node>(1);
        auto node2 = std::make_shared<Node>(2);
        
        // 使用 weak_ptr 避免循环引用
        node1->next_ = node2;
        node2->prev_ = node1;
        
        node1->show_next();
        node2->show_next(); // 这会显示 "Next node has been destroyed"
    }
    std::cout << std::endl;
    
    std::cout << "=== 父子关系示例 ===" << std::endl;
    {
        auto parent = std::make_shared<Parent>(1);
        auto child = std::make_shared<Child>(1);
        
        parent->child_ = child;
        child->parent_ = parent; // 使用 weak_ptr 避免循环引用
        
        child->show_parent();
    }
    // 对象应该能正确析构，没有内存泄漏
    
    return 0;
}
```
