/**********************************************************
 * 说明: safety-c++ application
 * 本篇代码实现参考文章，非原创
 * https://mp.weixin.qq.com/s/ZZD8-vpGVoN-re6T6j3VrQ
 * 需要支持C++17, gcc version 7.5.0 allowed.
 * static_cast      用static_cast来限制指针的强制转换
 * enum class       增加枚举类型内值检查
 * static_assert    静态断言，编译期检查
 * std::enable_if_t<std::is_arithmetic_v<T>> 模板检查
 * [[nodiscard]]    返回值检查
************************************************************/
#include <iostream>
#include <type_traits>
#include <memory>
#include <string>
#include <vector>

class A
{
public:
    A(){}
        ~A(){}
    
    virtual void printf() = 0;
};

class B:public A
{
public:
    using A::A;
    void printf(){
        std::cout<<"B Printf"<<std::endl;
    }
};

class C:public A
{
public:
    using A::A;
    void printf(){
        std::cout<<"C Printf"<<std::endl;
    }
};

enum class MyColor
{
    Red, Green, Blue,
};

//static_cast
template<typename T, size_t H=10>
void func(T val)
{
    static_assert(std::is_same_v<T, int>, "must be int");
    static_assert(H<=10, "H Must be less than 10");
    std::cout<<"int val:0x"<<std::hex<<val+H<<std::endl;
} 

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
inline T Max(T a, T b){
    return a > b ? a : b;
}

[[nodiscard]] int connect(){
    std::cout<<"connect discard"<<std::endl;
    return 1;
};

int main(int argc, char *argv[])
{
    //static_cast 
    int a = 10;
    int *ptr = static_cast<int *>(&a);
    *ptr = 20;
    std::cout<<a<<std::endl;
    
    B MyB;
    C MyC;
    A *ptrA;
    ptrA = static_cast<A *>(&MyB);
    ptrA->printf();
    ptrA = static_cast<A *>(&MyC);
    ptrA->printf();

    //enum class
    MyColor MyColor;
    MyColor = MyColor::Red;
    if(MyColor == MyColor::Blue)
    {
        std::cout<<"Color Equal"<<std::endl;
    }
    else
    {
        std::cout<<"Color No Equal"<<std::endl;
    }

    //static_assert
    func(1);
    func<int, 5>(2);
    //func<int, 12>(2); //error: static assertion failed: H Must be less than 10
    //func(1.0f); //static assertion failed: must be int

    //is_arithmetic_v 
    std::cout<<Max(5, 4)<<std::endl;

    //[[nodiscard]]
    std::cout<<connect()<<std::endl;
    return 0;
}


