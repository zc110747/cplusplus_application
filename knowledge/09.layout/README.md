# layout

## trivial_copyable
 
平凡可拷贝类型（Trivial Copyable）是指那些可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。

1. 没有虚函数和虚基类
2. 没有自定义的析构函数
3. 基类和非静态成员本身满足平凡可拷贝类型

在C++中，可以使用std::is_trivially_copyable_v来判断一个类型是否是平凡可拷贝类型。

```cpp
#include <iostream>
#include <string.h>
#include <type_traits>

// 没有虚函数和虚基类
class MyClass {
public:
    MyClass(int x, double y) : x_(x), y_(y) {
    }
public:
    int x_;
    double y_;
};

class MyClass2:public MyClass {  
};

class MyClass3 {
public:
    MyClass3() {
        ptr_ = new int(10);
    }
    ~MyClass3() {
        delete ptr_;
        prt = nullptr;
    }
private:
    int *ptr_{nullptr};
};


int main(int argc, char *argv[]) 
{
    std::cout << std::boolalpha;
    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;
    std::cout << "MyClass2 is trivially copyable: " << std::is_trivially_copyable_v<MyClass2> << std::endl;

    MyClass obj1(1, 2);
    obj1.x_ = 10;

    MyClass obj2(3, 4);

    // 平凡可拷贝类型，可以直接使用memcpy进行复制
    memcpy(&obj2, &obj1, sizeof(MyClass));
    std::cout << "obj2.x: " << obj2.x_ << std::endl;

    MyClass3 obj3;
    MyClass3 obj4;

    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass3> << std::endl;

    return 0;
}
```

## trivial_type

平凡类型（Trivial Type）是C++中的一个概念，它指的是那些可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。

1. 所有构造函数和析构函数均为默认(不能存在用户自定义的构造，拷贝构造，移动构造，移动赋值，析构函数等)。
2. 拷贝赋值运算符和移动赋值运算符均为默认(不能存在用户自定义的赋值函数) 。
3. 基类和非静态成员都是平凡类型。

在C++中，可以使用std::is_trivial_v来判断一个类型是否是平凡类型。

注意: 平凡类型要求在平凡可拷贝类型的基础上，需要满足构造函数、析构函数、移动\拷贝构造函数、赋值函数没有特殊行为，从而避免直接复制时，重复析构的问题。

```cpp
#include <iostream>
#include <string.h>
#include <type_traits>

// 没有虚函数和虚基类
class MyClass {
public:
    MyClass(int x, double y) : x_(x), y_(y) {
    }
public:
    int x_;
    double y_;
};

class MyClass1 {
public:
    MyClass1() = default;
public:
    int x_;
    double y_;
};

int main(int argc, char *argv[])
{
    std::cout << std::boolalpha;
    std::cout << "MyClass is trivial: " << std::is_trivial_v<MyClass> << std::endl;
    std::cout << "MyClass1 is trivial: " << std::is_trivial_v<MyClass1> << std::endl;
    std::cout << "MyClass is trivial copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;
    std::cout << "MyClass1 is trivial copyable: " << std::is_trivially_copyable_v<MyClass1> << std::endl;

    return 0;
}
```

## standard_layout

标准布局类型（standard-layout）是C++中的一个概念，它指的是那些内存布局是标准的、可预测的类型。

- 所有非静态成员具有相同的访问权限控制
- 没有虚函数和虚基类
- 基类和派生类的非静态成员不能有相同的名称
- 类中的第一个非静态成员的类型与其基类和本身类型不同
- 所有非静态成员都是标准布局类型，其基类也符合标准布局类型

在C++中，可以使用std::is_standard_layout_v来判断一个类型是否是标准布局类型。

1. 标准布局类型可以直接进行内存拷贝，但不一定是平凡类型，因为它可能有用户自定义的构造函数或其他特殊成员函数
2. 平凡类型可以通过简单的内存拷贝进行复制，并且在复制过程中不会有任何特殊行为，但不一定是标准布局类型，因为它可能有不同的访问控制或其他不符合标准布局类型要求的特性。

## aggregate

聚合类型（Aggregate Type）是指那些可以进行聚合初始化（Aggregate Initialization）的类型(C++20允许带基类的聚合类型)

- 没有用户声明，提高或者继承的构造函数(包含default)
- 没有用户声明，提高或者继承的赋值运算符
- 没有虚函数
- 没有私有或保护的非静态成员
- 没有私有，保护或虚继承的基类

聚合类型支持默认的列表初始化, 可通过is_aggregate_v判断

## pod

POD类型(Plain Old Data)

兼容C语言的类型，C++可以直接调用POD类型的数据, 拥有POD特征的类，直接字节拷贝或二进制拷贝

- 满足平凡类型(trivial)
- 是标准类型布局(standard-layout)
- 所有非静态成员都是POD类型

可通过is_pod_v判断
 
POD类型优点：

- 字节赋值，可以安全的使用memset和memcpy对pod类型进行初始化。
- 提供对C内存布局兼容，POD类型数据在C和C++间的操作总是安全的
- 保证了静态初始化的安全有效性(放入目标文件的.bss字段，初始化之间被赋值0)

## class_memory

- 空类占用1个字节，以使对象占用有效地址
- 虚继承，类或者其基类有虚函数，会让类增加虚表指针
- 虚表指针的个数在基类或者单继承中最多只有一个，多重继承则跟基类中本身带虚指针对象的数目一致。
- 静态成员变量和函数不占用对象的空间
