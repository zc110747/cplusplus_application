# reference

- [值和引用](#value_reference)
- [引用折叠、万能引用和完美转发](#reference_fold)
- [ref和cref应用](#ref_cref)
- [返回主页](../../README.md)

## value_reference

对于C++来说，值分为左值和右值。

- 左值是指那些可以出现在赋值操作符左边的表达式，即可以被取地址的表达式。左值通常有一个持久的内存地址，并且在表达式结束后仍然存在。
- 右值是指那些只能出现在赋值操作符右边的表达式，即不能被取地址的表达式。右值通常是临时的，表达式结束后就不再存在。

右值分为将亡值(expiring rvalue)和纯右值(pure rvalue)。

- 纯右值(pure rvalue): 临时变量，不和对象关联的字面量，值返回的函数返回值，lambda表达式等。
- 将亡值(expiring rvalue): std::move返回值、T&&右值引用的函数返回值、或者访问临时对象的参数。

**左值引用**是对左值的引用，它允许你通过引用修改原始左值。

```cpp
// 格式：类型& 引用名 = 左值表达式;
int x = 10;
int &ref = x;
```

**右值引用**是对右值的引用，它允许你延长右值的生命周期，并在某些情况下实现移动语义。

```cpp  
// 格式：类型&& 引用名 = 右值表达式;
int x = 10;
int &&rref = std::move(x);
```

std::move时C++11引入的一个函数，它将左值转换为右值引用，从而触发移动语义。std::move返回一个右值引用，其功能等同于如下实现。

```cpp
// std::move等同于static_cast<T&&>(x)
int x = 10;
int &&rref = static_cast<int&&>(x);
```

**移动语义**（Move Semantics）是C++11引入的一个重要特性，它允许对象的资源（如内存、文件句柄等）在不进行深拷贝的情况下从一个对象转移到另一个对象，从而提高程序的性能和效率。

移动语义通过如下方式处理。

- 右值引用（Rvalue Reference）
- 移动构造函数（Move Constructor）
- 移动赋值运算符（Move Assignment Operator）

具体示例如下所示。

```cpp
#include <iostream>

class demo
{
public:
    /// constructor
    demo() {
        std::cout << "constructor" << std::endl;
    }

    /// copy constructor
    demo(const demo &other) {
        std::cout << "copy constructor" << std::endl;
    }

    /// move constructor
    demo(demo &&other) {
        std::cout << "move constructor" << std::endl;
    }
    
    /// move assignment operator
    /// 移动构造函数和赋值构造函数的返回值需要是引用类型，否则会造成复制
    demo& operator=(demo &&other) {
        std::cout << "move assignment operator" << std::endl;
        return *this;
    }

    ~demo() {
        std::cout << "destructor" << std::endl;
    }

    void show(void) {
        std::cout << "show" << std::endl;
    }
};


int func() {
    return 1;
}

int main(int argc, char *argv[]) 
{
    int x = 10;             //x是左值, 10是右值
    int &ref = x;           //左值引用
    int &&rref = 10;        //右值引用
    int &&rref2 = func();   //右值引用
    int &&rref3 = std::move(x); //右值引用，左值绑定到右值需要用到std::move

    std::cout << "x: " << x << std::endl;       //10
    ref = 5;
    std::cout << "x: " << x << std::endl;       //5
    rref3 = 8;
    std::cout << "x: " << x << std::endl;       //8
    std::cout << "rref2: " << rref2 << std::endl; //1


    std::cout << "test 1:" << std::endl;
    {
        demo d1;
        demo&& d2 = std::move(d1);
        d2.show();
    }

    std::cout << "\ntest 2:" << std::endl;
    {
        demo d1;
        demo d3 = std::move(d1);
        d3.show();
    }

    std::cout << "\ntest 3:" << std::endl;
    {
        demo d1, d4;
        d4 = std::move(d1);
        d4.show();
    }

    return 0;
}
```

## reference_fold

**引用折叠**是指在模板推导或类型别名中，当引用类型与其他类型组合时，编译器会根据一定的规则进行类型折叠。

引用折叠的规则如下：

1. 左值引用类型与所有类型组合都是左值引用类型
2. 右值引用类型与非左值引用组合则是右值引用类型

| 类模板型 | 实际类型 | 组合类型 |
| --- | --- | --- |
| T& | R | R& |
| T& | R& | R& |
| T& | R&& | R& |
| T&& | R | R&& |
| T&& | R& | R& |
| T&& | R&& | R&& |

**万能引用**（Universal Reference）是C++11中引入的一个概念，它是一种特殊的引用类型，可以绑定到左值或右值。

万能引用通常用于模板函数中，以实现完美转发（Perfect Forwarding）,只有在模板类型推导的上下文中，T&&才是万能引用。若T是明确指定的类型，T&&就只是普通的右值引用。

```cpp
template <typename T>
void forward(T&& param) {
    // 这里的param是一个万能引用，可以绑定到左值或右值
    // 可以使用std::forward来保留param的左值或右值属性
    someFunction(std::forward<T>(param));
}
```

**完美转发**是指在模板函数中，通过std::forward函数将参数以原始的左值或右值类型传递给另一个函数，从而保留参数的原始属性。

```cpp
// 完美转发示例
std::forward<T>(t)
```

## ref_cref

std::ref和std::cref是C++标准库中的函数模板，用于创建std::reference_wrapper对象，这些对象可以像引用一样使用，但可以被复制和存储。

1. std::ref函数接受一个左值引用参数，并返回一个std::reference_wrapper对象，该对象可以被复制和存储，并且可以像引用一样使用。
2. std::cref 函数接受一个常量左值引用参数，并返回一个 std::reference_wrapper 对象，该对象可以被复制和存储，并且可以像常量引用一样使用。

std::ref和std::cref的主要用途是与std::bind函数一起使用，以实现函数对象的参数绑定。

```cpp
#include <functional>
#include <iostream>

void func(int &a, int &b)
{
    a += 1;
    b += 1;
}

int main(int argc, char *argv[]) 
{
    // 示例
    int x = 10;
    std::reference_wrapper<int> ref = std::ref(x);
    ref.get() = 20; // 修改x的值
    std::cout << x << std::endl; // 输出 20

    // 示例
    const int y = 30;
    std::reference_wrapper<const int> cref = std::cref(y);
    std::cout << cref.get() << std::endl; // 输出 y 的值
    
    // 示例
    int a = 1;
    int b = 2;
    std::function<void()> f = std::bind(func, std::ref(a), std::ref(b));
    f();
    std::cout << a << " " << b << std::endl; // 输出 2 3
    return 0;
}
```
