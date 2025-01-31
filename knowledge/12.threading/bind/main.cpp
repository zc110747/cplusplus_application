/************************************************************************************
 * std::bind
 * 头文件#include <functional>
***********************************************************************************/
#include <iostream>
#include <thread>
#include <unistd.h>
#include <functional>

using std::cout;
using std::endl;

void print_sum(int a, int b) 
{
    std::cout << "Sum: " << (a + b) << std::endl;
}

void add_val(int &a, int &b)
{
    a++;
    b++;
}

class Object
{
public:
    Object() = default;

    void obj_run(int a) 
    {
        cout<<a<<endl;
    }

    // 绑定类中的对象
    void init(int a) 
    {
        auto bound_func = std::bind(&Object::obj_run, this, std::placeholders::_1);
        bound_func(a);
    }
};

int main(int argc, char* argv[])
{
    // std::bind绑定函数，带参数(最大支持29个参数)
    auto bound_func = std::bind(print_sum, 5, std::placeholders::_1);
    bound_func(10);

    // std::bind使用引用
    int a=2, b=3;
    add_val(a, b);
    cout<<a<<" "<<b<<endl;
    auto bind_add_0 = std::bind(add_val, a, b);
    bind_add_0();
    cout<<a<<" "<<b<<endl;
    auto bind_add_1 = std::bind(add_val, std::ref(a), std::ref(b));
    bind_add_1();
    cout<<a<<" "<<b<<endl;

    // std::bind绑定类对象
    Object obj;
    obj.init(3);

    // 绑定类对象实例
    auto boundFunc = std::bind(&Object::obj_run, &obj, std::placeholders::_1);
    boundFunc(4);

    return 0;
}
