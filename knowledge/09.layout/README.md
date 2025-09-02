# layout

<<<<<<< HEAD
- [平凡可拷贝类型](#trivial_copyable)
- [平凡类型](#trivial_type)
- [标准布局类型](#standard_layout)
- [聚合类型](#aggregate_type)
- [POD类型](#pod_type)
- [类存储空间](#class_layout)

## trivial_copyable
 
平凡可拷贝类型（Trivial Copyable）是指那些可以通过简单的内存拷贝（如memcpy，memset，memmove）进行处理，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。

平凡可拷贝类型的要求如下所示。

1. 没有虚函数和虚基类：该类型不能包含虚函数表或虚继承。
2. 没有自定义的析构函数：该类型应使用编译器提供的默认析构函数。
3. 可以有自定义的构造函数，但是不能有拷贝构造函数、移动构造函数、拷贝赋值函数、移动赋值函数。
3. 基类和非静态成员本身满足平凡可拷贝类型：该类型的基类和非静态成员变量也必须是平凡可拷贝类型。

在C++中，可以使用std::is_trivially_copyable_v来判断一个类型是否是平凡可拷贝类型。

参考示例如下。

=======
## trivial_copyable
 
平凡可拷贝类型（Trivial Copyable）是指那些可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。

1. 没有虚函数和虚基类
2. 没有自定义的析构函数
3. 基类和非静态成员本身满足平凡可拷贝类型

在C++中，可以使用std::is_trivially_copyable_v来判断一个类型是否是平凡可拷贝类型。

>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
```cpp
#include <iostream>
#include <string.h>
#include <type_traits>

<<<<<<< HEAD
class MyClass {
public:
    // 构造函数，true
    MyClass(int x, double y) : x_(x), y_(y) {
    }
    MyClass() {
        x_ = 0;
        y_ = 0;
    }
=======
// 没有虚函数和虚基类
class MyClass {
public:
    MyClass(int x, double y) : x_(x), y_(y) {
    }
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
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
<<<<<<< HEAD

    // 析构函数，false
    ~MyClass3() {
        delete ptr_;
        ptr_ = nullptr;
=======
    ~MyClass3() {
        delete ptr_;
        prt = nullptr;
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
    }
private:
    int *ptr_{nullptr};
};

<<<<<<< HEAD
class MyClass1 {
public:
    // 拷贝构造函数，false
    // MyClass1(const MyClass1& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    // }

    // 移动构造函数，false
    // MyClass1(const MyClass1&& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    // }

    // 拷贝赋值函数，false
    // MyClass1& operator=(const MyClass1& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    //     return *this;
    // }

    // 移动赋值函数，false
    // MyClass1& operator=(const MyClass1&& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    //     return *this;
    // }

    // 析构函数，false
    ~MyClass1() {
    }
public:
    int x_;
    double y_;
};
=======
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb

int main(int argc, char *argv[]) 
{
    std::cout << std::boolalpha;
<<<<<<< HEAD
    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;       //true
    std::cout << "MyClass1 is trivially copyable: " << std::is_trivially_copyable_v<MyClass1> << std::endl;     //false
    std::cout << "MyClass2 is trivially copyable: " << std::is_trivially_copyable_v<MyClass2> << std::endl;     //true
=======
    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;
    std::cout << "MyClass2 is trivially copyable: " << std::is_trivially_copyable_v<MyClass2> << std::endl;
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb

    MyClass obj1(1, 2);
    obj1.x_ = 10;

    MyClass obj2(3, 4);

    // 平凡可拷贝类型，可以直接使用memcpy进行复制
    memcpy(&obj2, &obj1, sizeof(MyClass));
    std::cout << "obj2.x: " << obj2.x_ << std::endl;

    MyClass3 obj3;
<<<<<<< HEAD

    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass3> << std::endl;      //false
=======
    MyClass3 obj4;

    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass3> << std::endl;
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb

    return 0;
}
```

## trivial_type

平凡类型（Trivial Type）是C++中的一个概念，它指的是那些可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。

<<<<<<< HEAD
平凡类型的要求如下所示。

1. 没有虚函数和虚基类：该类型不能包含虚函数表或虚继承。
2. 所有构造函数和析构函数均为默认：不能存在用户自定义的构造函数，拷贝构造，移动构造，析构函数。
3. 拷贝赋值运算符和移动赋值运算符均为默认：不能存在用户自定义的赋值函数和移动赋值函数。
4. 基类和非静态成员都是平凡类型：该类型的基类和非静态成员变量也必须是平凡类型。

在C++中，可以使用std::is_trivial_v来判断一个类型是否是平凡类型。

注意: **平凡类型要求在平凡可拷贝类型的基础上，需要满足构造函数没有特殊行为**。从而避免直接复制时，重复析构的问题。平凡类型一定是平凡可拷贝类型，但是平凡可拷贝类型不一定是平凡类型，平凡可拷贝类型可以有自定义构造函数。

参考示例如下。

```cpp
#include <iostream>
#include <type_traits>

class MyClass {
public:
    MyClass() = default;
=======
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
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
public:
    int x_;
    double y_;
};

class MyClass1 {
public:
<<<<<<< HEAD
    // 自定义构造函数，false
    // MyClass1(int x, double y) : x_(x), y_(y) {
    // }

    // 拷贝构造函数，false
    // MyClass1(const MyClass1& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    // }

    // 移动构造函数，false
    // MyClass1(const MyClass1&& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    // }

    // 拷贝赋值函数，false
    // MyClass1& operator=(const MyClass1& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    //     return *this;
    // }

    // 移动赋值函数，false
    // MyClass1& operator=(const MyClass1&& obj) {
    //     x_ = obj.x_;
    //     y_ = obj.y_;
    //     return *this;
    // }

    // 析构函数，false
    ~MyClass1() {
    }
=======
    MyClass1() = default;
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
public:
    int x_;
    double y_;
};

<<<<<<< HEAD
class MyClass2: public MyClass {
public:
    MyClass2() = default;
};

int main(int argc, char *argv[])
{
    std::cout << std::boolalpha;
    std::cout << "MyClass is trivial: " << std::is_trivial_v<MyClass> << std::endl;                         //false
    std::cout << "MyClass1 is trivial: " << std::is_trivial_v<MyClass1> << std::endl;                       //true
    std::cout << "MyClass2 is trivial: " << std::is_trivial_v<MyClass2> << std::endl;                       //true
=======
int main(int argc, char *argv[])
{
    std::cout << std::boolalpha;
    std::cout << "MyClass is trivial: " << std::is_trivial_v<MyClass> << std::endl;
    std::cout << "MyClass1 is trivial: " << std::is_trivial_v<MyClass1> << std::endl;
    std::cout << "MyClass is trivial copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;
    std::cout << "MyClass1 is trivial copyable: " << std::is_trivially_copyable_v<MyClass1> << std::endl;
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb

    return 0;
}
```

## standard_layout

<<<<<<< HEAD
标准布局类型（Standard-layout Type）是C++中的一种类型，指的是那些内存布局是标准的、可预测的类型。标准布局类型的重要特性是它的内存布局在不同编译器和平台下是一致的，这对于跨平台的二进制数据交换具有重要意义。

标准布局类型有以下要求：

1. 没有虚函数和虚继承：类型不能包含虚函数表或虚继承机制。
2. 所有非静态成员变量具有相同的访问权限控制：所有的成员变量必须都是public、protected或private。
3. 在类或结构体继承时，满足以下两种情况之一:
    1. 派生类中有非静态成员变量，且基类中没有非静态成员变量
    2. 基类有非静态成员变量，而派生类没有非静态成员变量。
4. 类中的第一个非静态成员的类型与其基类和本身类型不同。
5. 所有非静态成员都是标准布局类型，其基类也符合标准布局类型。

在C++中，可以使用std::is_standard_layout_v来判断一个类型是否是标准布局类型。

1. 标准布局类型可以直接进行内存拷贝，但不一定是平凡类型；因为它可能有用户自定义的构造函数或其他特殊成员函数。
2. 平凡类型可以通过简单的内存拷贝进行复制，并且在复制过程中不会有任何特殊行为；但不一定是标准布局类型。因为它可能有不同的访问控制或其他不符合标准布局类型要求的特性。

参考示例如下。

```cpp
#include <iostream>
#include <type_traits>

class MyClass {
public:
    MyClass() = default;

    int x_;
};

class MyClass1 {
public:
    MyClass1() = default;

public:
    int x_;

// 非静态成员具有不同的权限，false
private:
    int y_;
};

class MyClass2: public MyClass {
public:
    MyClass2() = default;

    // 派生类和基类都有非静态变量，false
    int z_;
};

class MyClass3: public MyClass {
};

class MyClass4 {
public:
    // 虚函数，false
    // virtual void func() {}
};

int main(int argc, char *argv[])
{ 
    std::cout << std::boolalpha;
    std::cout << "MyClass is standard layout: " << std::is_standard_layout_v<MyClass> << std::endl;     //true 
    std::cout << "MyClass1 is standard layout: " << std::is_standard_layout_v<MyClass1> << std::endl;   //false
    std::cout << "MyClass2 is standard layout: " << std::is_standard_layout_v<MyClass2> << std::endl;   //false
    std::cout << "MyClass3 is standard layout: " << std::is_standard_layout_v<MyClass3> << std::endl;   //true
    std::cout << "MyClass4 is standard layout: " << std::is_standard_layout_v<MyClass4> << std::endl;   //true
    return 0;
}
```

## aggregate_type

聚合类型（Aggregate Type）是指那些可以进行聚合初始化（Aggregate Initialization）的类型。这是一种特殊的初始化方式，允许使用花括号 {} 直接初始化类的成员。

聚合类型要求如下所示。

1. 没有用户声明或者继承的构造函数(包含default)，类型不能有用户自定义的构造函数。
2. 没有用户声明或者继承的赋值运算符，不能有用户自定义的赋值函数。
3. 没有虚函数，类型不能包含虚函数。
4. 没有私有或保护的非静态成员，所有非静态成员必须是公有的。
5. 没有私有，保护或虚继承的基类，基类必须是公有且非虚继承的。

注意: C++20新增支持带基类的聚合类型。

聚合类型支持默认的列表初始化, 可通过std::is_aggregate_v判断是否是聚合类型。

具体示例如下。

```cpp
#include <iostream>
#include <type_traits>

class MyStruct {
public:
    int x_;
    double y_;
};

class MyStruct1: public MyStruct {
public:
    int z_;
};

class MyStruct2 {
private:
    int w_;
};

int main(int argc, char *argv[]) 
{
    MyStruct s = {1, 2.0}; // 聚合初始化

    std::cout << s.x_ << " " << s.y_ << std::endl;

    std::cout << std::boolalpha;
    std::cout << std::is_aggregate_v<MyStruct> << std::endl;            //true

    MyStruct1 s1 = {1, 2.0, 3};
    std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

    std::cout << std::is_aggregate_v<MyStruct1> << std::endl;           //true
    std::cout << std::is_aggregate_v<MyStruct2> << std::endl;           //false

    return 0;
}
```

## pod_type

POD类型(Plain Old Data)是兼容C语言的类型，C++可以直接调用POD类型的数据。 拥有POD特征的类，可以直接进行字节拷贝或二进制拷贝。

1. 满足平凡类型（trivial）：类型的所有构造函数、析构函数、赋值运算符都是默认的，没有用户自定义的特殊行为。
2. 满足标准布局类型（standard-layout）：类型的内存布局是标准的、可预测的，它保证了类型在不同平台上的ABI兼容。
3. 所有非静态成员都是POD类型。

可通过std::is_pod_v判断是否是POD类型。
=======
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
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
 
POD类型优点：

- 字节赋值，可以安全的使用memset和memcpy对pod类型进行初始化。
- 提供对C内存布局兼容，POD类型数据在C和C++间的操作总是安全的
- 保证了静态初始化的安全有效性(放入目标文件的.bss字段，初始化之间被赋值0)

<<<<<<< HEAD
具体示例如下。

```cpp
#include <iostream>
#include <type_traits>

class MyStruct {
public:
    int x_;
    double y_;
};

class MyStruct1: public MyStruct {
public:
    int z_;
};

template <typename T>
bool pod_value = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

int main(int argc, char *argv[]) 
{
    MyStruct s = {1, 2.0};

    std::cout << s.x_ << " " << s.y_ << std::endl;
    std::cout << std::boolalpha;
    std::cout << std::is_pod_v<MyStruct> << std::endl;            //true

    MyStruct1 s1 = {1, 2.0, 3};
    std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;
    std::cout << std::is_pod_v<MyStruct1> << std::endl;           //false
    std::cout << pod_value<MyStruct1> << std::endl;           //false

    return 0;
}
```

## class_layout

对象的内存占用布局如下所示。

1. 空类占用1个字节，以使对象占用有效地址，如果有变量，占用变量的内存。
2. 虚继承，类或者其基类有虚函数，会让类增加虚表指针，占用平台指针大小的内存。
3. 虚表指针的个数在基类或者单继承中最多只有一个，多重继承则跟基类中本身带虚指针对象的数目一致。
4. 静态成员变量和函数不占用对象的空间。

具体示例如下。

```cpp
#include <iostream>
#include <type_traits>

class MyClass {
};

class MyClass1: public MyClass {
public:
    virtual void func1() {
    }
};

class MyClass2 {
    static inline int x_{1};
};

class MyClass3 {
public:
    int x_{1};
};

int main(int argc, char *argv[]) 
{
    std::cout << sizeof(MyClass) << std::endl;      // 1
    std::cout << sizeof(MyClass1) << std::endl;     // 8
    std::cout << sizeof(MyClass2) << std::endl;     // 1
    std::cout << sizeof(MyClass3) << std::endl;     // 4

    return 0;
}
``
=======
## class_memory

- 空类占用1个字节，以使对象占用有效地址
- 虚继承，类或者其基类有虚函数，会让类增加虚表指针
- 虚表指针的个数在基类或者单继承中最多只有一个，多重继承则跟基类中本身带虚指针对象的数目一致。
- 静态成员变量和函数不占用对象的空间
>>>>>>> 62558f2a9fe27a1dd64315cc3ed8ad9fadd71fcb
