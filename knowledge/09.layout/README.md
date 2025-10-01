# layout

- [平凡可拷贝类型](#trivial_copyable)
- [平凡类型](#trivial_type)
- [标准布局类型](#standard_layout)
- [聚合类型](#aggregate_type)
- [POD类型](#pod_type)
- [类存储空间](#class_layout)
- [对齐机制](#alignment)

## trivial_copyable
 
平凡可拷贝类型（Trivial Copyable）是指那些可以通过简单的内存拷贝（如memcpy，memset，memmove）进行处理，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。

平凡可拷贝类型的要求如下所示。

1. 没有虚函数和虚基类：该类型不能包含虚函数表或虚继承。
2. 没有自定义的析构函数：该类型应使用编译器提供的默认析构函数。
3. 可以有自定义的构造函数，但是不能有拷贝构造函数、移动构造函数、拷贝赋值函数、移动赋值函数。
3. 基类和非静态成员本身满足平凡可拷贝类型：该类型的基类和非静态成员变量也必须是平凡可拷贝类型。

在C++中，可以使用std::is_trivially_copyable_v来判断一个类型是否是平凡可拷贝类型。

参考示例如下。

```cpp
#include <iostream>
#include <string.h>
#include <type_traits>

class MyClass {
public:
    // 构造函数，true
    MyClass(int x, double y) : x_(x), y_(y) {
    }
    MyClass() {
        x_ = 0;
        y_ = 0;
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

    // 析构函数，false
    ~MyClass3() {
        delete ptr_;
        ptr_ = nullptr;
    }
private:
    int *ptr_{nullptr};
};

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

int main(int argc, char *argv[]) 
{
    std::cout << std::boolalpha;
    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;       //true
    std::cout << "MyClass1 is trivially copyable: " << std::is_trivially_copyable_v<MyClass1> << std::endl;     //false
    std::cout << "MyClass2 is trivially copyable: " << std::is_trivially_copyable_v<MyClass2> << std::endl;     //true

    MyClass obj1(1, 2);
    obj1.x_ = 10;

    MyClass obj2(3, 4);

    // 平凡可拷贝类型，可以直接使用memcpy进行复制
    memcpy(&obj2, &obj1, sizeof(MyClass));
    std::cout << "obj2.x: " << obj2.x_ << std::endl;

    MyClass3 obj3;

    std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass3> << std::endl;      //false

    return 0;
}
```

## trivial_type

平凡类型（Trivial Type）是C++中的一个概念，它指的是那些可以通过简单的内存拷贝（如memcpy）进行复制，并且在复制过程中不会有任何特殊行为（如调用构造函数或析构函数）的类型。

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
public:
    int x_;
    double y_;
};

class MyClass1 {
public:
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
public:
    int x_;
    double y_;
};

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

    return 0;
}
```

## standard_layout

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

聚合类型（Aggregate Type）是指那些可以进行聚合初始化（Aggregate Initialization）的类型。这是一种特殊的初始化方式，允许使用花括号 {} 直接初始化类的成员；C++20新增支持带基类的聚合类型，同时聚合类型支持使用"{}"和"()"进行初始化，其中{}初始化方式不支持缩窄转换，而()初始化方式支持缩窄转换。

聚合类型要求如下所示。

1. 没有用户声明或者继承的构造函数(包含default)，类型不能有用户自定义的构造函数。
2. 没有用户声明或者继承的赋值运算符，不能有用户自定义的赋值函数。
3. 没有虚函数，类型不能包含虚函数。
4. 没有私有或保护的非静态成员，所有非静态成员必须是公有的。
5. 没有私有，保护或虚继承的基类，基类必须是公有且非虚继承的。

聚合类型支持默认的列表初始化, 可通过std::is_aggregate_v判断是否是聚合类型；聚合类型支持指定初始化，可以通过成员名进行初始化，具有如下要求。

1. 指定的数据成员必须时非静态成员变量。
2. 每个非静态成员最多初始化一次。
3. 非静态成员变量的初始化顺序与声明顺序相同。
4. 针对联合体的数据成员初始化，只能指定一个。
5. 嵌套指定初始化不能使用直接嵌套指定，可以使用对象嵌套指定。
6. 使用指定初始化后，不能使用其它初始化混合使用，不能用于数组指定初始化。

具体示例如下。

```cpp
#include <iostream>
#include <type_traits>

class alg_demo {
public:
    int x_;
    double y_;
};

class alg_demo1: public alg_demo {
public:
    int z_;
};

class alg_demo2 {
private:
    int w_;
};

struct alg_demo3 {
    int x_;
    int y_;
};

union alg_demo4 {
    int x_;
    double y_;
};

struct alg_demo5 {
    alg_demo3 x_;
    alg_demo3 y_;
};

int main(int argc, char *argv[]) 
{
    alg_demo s = {1, 2.0}; // 聚合初始化

    std::cout << s.x_ << " " << s.y_ << std::endl;

    std::cout << std::boolalpha;
    std::cout << std::is_aggregate_v<alg_demo> << std::endl;            //true

    alg_demo1 s1 = {1, 2.0, 3};
    std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

    // 聚合类型也支持使用"()"进行初始化 C++20
    // 带基类的类型不支持
    alg_demo s2(1, 2.0);
    std::cout << s2.x_ << " " << s2.y_ << std::endl;

    std::cout << std::is_aggregate_v<alg_demo1> << std::endl;           //true
    std::cout << std::is_aggregate_v<alg_demo2> << std::endl;           //false

    // 指定初始化
    alg_demo alg1 = {.x_ = 1, .y_ = 2.0};
    std::cout << alg1.x_ << " " << alg1.y_ << std::endl;

    alg_demo alg2 = {.y_ = 2.0};    // x_= 0, y_= 2.0
    std::cout << alg2.x_ << " " << alg2.y_ << std::endl;

    // union 类型只能指定一个数据成员初始化
    alg_demo4 alg3 = {.y_ = 2.0};
    std::cout << alg3.x_ << " " << alg3.y_ << std::endl;

    // 嵌套指定初始化
    alg_demo5 alg4 = {.x_ = {1, 2}, .y_ = {3, 4}};
    std::cout << alg4.x_.x_ << " " << alg4.x_.y_ << " " << alg4.y_.x_ << " " << alg4.y_.y_ << std::endl;

    alg_demo5 alg5 = {.x_ = {.x_ = 1, .y_ = 2}, .y_ = {3, 4}};
    std::cout << alg5.x_.x_ << " " << alg5.x_.y_ << " " << alg5.y_.x_ << " " << alg5.y_.y_ << std::endl;
    return 0;
}
```

## pod_type

POD类型(Plain Old Data)是兼容C语言的类型，C++可以直接调用POD类型的数据。 拥有POD特征的类，可以直接进行字节拷贝或二进制拷贝。

1. 满足平凡类型（trivial）：类型的所有构造函数、析构函数、赋值运算符都是默认的，没有用户自定义的特殊行为。
2. 满足标准布局类型（standard-layout）：类型的内存布局是标准的、可预测的，它保证了类型在不同平台上的ABI兼容。
3. 所有非静态成员都是POD类型。

可通过std::is_pod_v判断是否是POD类型。
 
POD类型优点：

- 字节赋值，可以安全的使用memset和memcpy对pod类型进行初始化。
- 提供对C内存布局兼容，POD类型数据在C和C++间的操作总是安全的
- 保证了静态初始化的安全有效性(放入目标文件的.bss字段，初始化之间被赋值0)

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
#include <memory>
#include <functional>
#include <type_traits>

struct base
{
    char a;
    //reseved[2]
    short b;
    //reseved[4]
    double c;
};

template<class T> 
struct alignof_member
{ 
    char c; 
    T member; 
};

template <class T>
constexpr size_t offset_num = offsetof(alignof_member<T>, member);

// alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
// alignas设置的值必须是2的幂值，否则无法通过编译
// alignas设置值小于自然对齐值时，以自然对齐值为准
struct struct_align_default
{
    char a;
    short b;
};

struct alignas(1) struct_align_1
{
    char a;
    short b;
};

struct alignas(4) struct_align_4
{
    char a;
    short b;
};

struct alignas(8) struct_align_8
{
    char a;
    short b;
};

struct struct_align_with_alignas
{
    char a;
    alignas(4) short b;
};

int main(int argc, char *argv[]) 
{
    // offsetof用于获取结构体成员的偏移量
    std::cout << "struct_align_default: " << offset_num<char> << std::endl;
    std::cout << "struct_align_default: " << offset_num<int> << std::endl;
    std::cout << "struct_align_default: " << offset_num<double> << std::endl;

    // alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
    std::cout << "struct_align_default: " << sizeof(struct_align_default) << std::endl;
    std::cout << "struct_align_1: " << sizeof(struct_align_1) << std::endl;
    std::cout << "struct_align_4: " << sizeof(struct_align_4) << std::endl;
    std::cout << "struct_align_8: " << sizeof(struct_align_8) << std::endl;
    std::cout << "struct_align_with_alignas: " << sizeof(struct_align_with_alignas) << std::endl;

    // alignof用于获取结构体和结构体成员的对齐值
    std::cout << "struct_align_default: " << alignof(struct_align_default) << std::endl;
    std::cout << "struct_align_default::a: " << alignof(struct_align_default::a) << std::endl;
    std::cout << "struct_align_default::b: " << alignof(struct_align_default::b) << std::endl;
    std::cout << "struct_align_1: " << alignof(struct_align_1) << std::endl;

    // alignment_of用于获取结构体和结构体成员的对齐值
    std::cout << "struct_align_1: " << std::alignment_of<struct_align_1>::value << std::endl;
    std::cout << "struct_align_4: " << std::alignment_of<struct_align_4>::value << std::endl;
    std::cout << "struct_align_8: " << std::alignment_of<struct_align_8>::value << std::endl;

    // aligned_storage_t和aligned_union_t用于分配指定对齐长度和大小的一块内存(C++23已弃用，不建议使用)
    std::aligned_storage_t<256, 8> buffer;
    std::cout << sizeof(buffer) << " " << alignof(buffer) << std::endl;                          //256 8
    std::aligned_union_t<64, int, char, double> union_b;
    std::cout << sizeof(union_b) << " " << alignof(union_b) << std::endl;                        //64 8

    // std::align_alloc用于分配对齐的内存，返回一个指向对齐内存的指针
    int* p = reinterpret_cast<int*>(std::aligned_alloc(64, 512));
    *p = 1;
    std::cout << *p << std::endl;
    std::free(p);

    // 申请对齐的内存
    std::unique_ptr<int, std::function<void(int*)>> ptr(reinterpret_cast<int*>(std::aligned_alloc(64, 512)), 
                                                    [](int* p) { std::free(p); });
    *ptr = 1;
    std::cout << *ptr << std::endl;
    return 0; 
}
```
<<<<<<< HEAD
=======

## alignment

数据对齐相关处理。

- alignas用于指定类型的对齐值，alignas设置的值必须是2的幂值，否则无法通过编译。
- alignof用于获取类型的对齐值，可以获取类、结构体以及其成员变量的对齐值。
- std::alignment_of：用于获取类型的对齐值，返回一个std::size_t类型的值。

具体示例如下。

```cpp  
#include <iostream>
#include <type_traits>

struct base
{
    char a;
    //reseved[2]
    short b;
    //reseved[4]
    double c;
};

template<class T> 
struct alignof_member
{ 
    char c; 
    T member; 
};

template <class T>
constexpr size_t offset_num = offsetof(alignof_member<T>, member);


// alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
// alignas设置的值必须是2的幂值，否则无法通过编译
// alignas设置值小于自然对齐值时，以自然对齐值为准
struct struct_align_default
{
    char a;
    short b;
};

struct alignas(1) struct_align_1
{
    char a;
    short b;
};

struct alignas(4) struct_align_4
{
    char a;
    short b;
};

struct alignas(8) struct_align_8
{
    char a;
    short b;
};

struct struct_align_with_alignas
{
    char a;
    alignas(4) short b;
};

int main(int argc, char *argv[]) 
{
    // offsetof用于获取结构体成员的偏移量
    std::cout << "struct_align_default: " << offset_num<char> << std::endl;
    std::cout << "struct_align_default: " << offset_num<int> << std::endl;
    std::cout << "struct_align_default: " << offset_num<double> << std::endl;

    // alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
    std::cout << "struct_align_default: " << sizeof(struct_align_default) << std::endl;
    std::cout << "struct_align_1: " << sizeof(struct_align_1) << std::endl;
    std::cout << "struct_align_4: " << sizeof(struct_align_4) << std::endl;
    std::cout << "struct_align_8: " << sizeof(struct_align_8) << std::endl;
    std::cout << "struct_align_with_alignas: " << sizeof(struct_align_with_alignas) << std::endl;

    // alignof用于获取结构体和结构体成员的对齐值
    std::cout << "struct_align_default: " << alignof(struct_align_default) << std::endl;
    std::cout << "struct_align_default::a: " << alignof(struct_align_default::a) << std::endl;
    std::cout << "struct_align_default::b: " << alignof(struct_align_default::b) << std::endl;
    std::cout << "struct_align_1: " << alignof(struct_align_1) << std::endl;

    // alignment_of用于获取结构体和结构体成员的对齐值
    std::cout << "struct_align_1: " << std::alignment_of<struct_align_1>::value << std::endl;
    std::cout << "struct_align_4: " << std::alignment_of<struct_align_4>::value << std::endl;
    std::cout << "struct_align_8: " << std::alignment_of<struct_align_8>::value << std::endl;

    // aligned_storage_t和aligned_union_t用于分配指定对齐长度和大小的一块内存(C++23已弃用，不建议使用)
    std::aligned_storage_t<256, 8> buffer;
    std::cout << sizeof(buffer) << " " << alignof(buffer) << std::endl;                          //256 8
    std::aligned_union_t<64, int, char, double> union_b;
    std::cout << sizeof(union_b) << " " << alignof(union_b) << std::endl;                        //64 8
    return 0; 
} 
```
>>>>>>> 171775b3443edbb693d4aa0be5b9466005143b33
