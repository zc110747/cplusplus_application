/************************************************************************************
 * lambda
 * 
***********************************************************************************/
#include "lambda.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

static void lambda_capture(void);
static void lambda_algorithm(void);

int lambda_process(void)
{
    lambda_capture();

    lambda_algorithm();
    return 0;
}

static void lambda_algorithm(void)
{
    FUNCTION_START()

    srand((unsigned int)time(NULL));
    std::vector<int> x(3);
    std::vector<int> y(4);

    auto show_vec = [](auto &&x){
        //遍历
        //for_each
        for_each(x.cbegin(), x.cend(), [](auto &&val){
            cout<<val<<" ";
        });
        cout<<" | ";
    };
    
    //填充
    //generate
    std::generate(x.begin(), x.end(), []{
        return rand()%100;
    });
    show_vec(x);

    //generate_n
    std::generate_n(y.begin(), y.size(), []{
        return rand()%100;
    });
    show_vec(y);
    
    //fill
    std::fill(x.begin(), x.end(), 5);
    show_vec(x);

    //fill_n
    std::fill(x.begin(), x.size(), 5);
    show_vec(x);

    //遍历

    FUNCTION_END() 
}

static void lambda_capture(void)
{
    FUNCTION_START()

    int x=0, y=0;
    static int a = 5;

    //值传递,创建时绑定,后续数据修改不影响
    auto add_0 = [x, y](int z)->int{  //[x, y, a] 不能捕获静态变量
        //x = 1; not allow, 值捕获默认为read-only
        cout<<"a:"<<a<<" | ";       //静态变量
        return x+y+z;
    };
    x = 1;
    y = 1;
    cout<<add_0(3)<<" ";
    cout<<x<<" "<<y<<" | ";    //3, 1, 1

    //引用传递, 内部和外部数据都互相修改
    auto add_1 = [&x, &y](int z)->int{
        return x+y+z;
    };
    x = 2;
    y = 2;
    cout<<add_1(3)<<" ";
    cout<<x<<" "<<y<<" | ";    //7, 2, 2

    auto modify0 = [&x, &y](int z)->int{
        x = 1;
        y = 1;
        return x+y+z;
    };
    cout<<modify0(3)<<" ";
    cout<<x<<" "<<y<<" | ";    //5, 1, 1

    //全局捕获(=, &)
    //值捕获
    auto allcap_0 = [=]() mutable{ //值捕获默认不可变，需要添加mutable才可以修改
        x += 1;
        y += 1;
        return x+y;
    };
    cout<<allcap_0()<<" "<<x<<" "<<y<<" | "; //

    //引用捕获
    auto allcap_1 = [&](){
        x += 1;
        y += 1;
        return x+y;
    };
    cout<<allcap_1()<<" "<<x<<" "<<y<<" | ";

    //this捕获
    class lambda_class
    {
    public:
        void show(){
            auto thiscap = [this](){
                return a;
            };
            cout<<"capture this:"<<thiscap()<<" | ";
        }
    private:
        int a{2};
    };
    lambda_class().show();

    FUNCTION_END()
}