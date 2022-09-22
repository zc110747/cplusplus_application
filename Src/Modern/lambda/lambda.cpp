/************************************************************************************
 * lambda
 * 
***********************************************************************************/
#include "lambda.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

static void lambda_capture(void);
static void lambda_algorithm(void);
static void lambda_extend(void);

int lambda_process(void)
{
    srand((unsigned int)time(NULL));

    lambda_capture();

    lambda_algorithm();

    lambda_extend();
    return 0;
}

class func_
{
public:
    void operator() (){
        cout<<"test"<<" | ";
    }
};

static void lambda_extend(void)
{
    FUNCTION_START()

    auto foo = [](auto &&a){
        return a;
    };
    cout<<foo("hello")<<" | ";

    func_ func;
    func();

    FUNCTION_END()
}

template<typename T>
struct is_std_vector: std::false_type{};

template<typename T>
struct is_std_vector<std::vector<T>&>: std::true_type{};

static void lambda_algorithm(void)
{
    FUNCTION_START()

    std::vector<int> x(3);
    std::vector<int> y(4);

    auto show_vec = [](auto &&x){
        static_assert(is_std_vector<decltype(x)>::value, "Not vector!");

        //遍历
        //for_each
        for_each(x.cbegin(), x.cend(), [](auto &&val){
            cout<<val<<" ";
        });
        cout<<" | ";
    };

    //填充
    //generate, generate_n
    //fill, fill_n
    std::generate(x.begin(), x.end(), []{
        return rand()%100;
    });
    show_vec(x);

    std::generate_n(y.begin(), y.size(), []{
        return rand()%100;
    });
    show_vec(y);
    
    std::fill(x.begin(), x.end(), rand()%100);
    show_vec(x);

    std::fill_n(x.begin(), x.size(), rand()%100);
    show_vec(x);

    std::string str{"Hello World!"};
    std::string result;
    result.resize(str.length());

    //遍历
    //for_each, transform
    std::transform(str.begin(), str.end(), result.begin(), [](int _C)->int{
        if(_C >= 'A' &&  _C <= 'Z')
        {
            _C = _C - 'A' + 'a'; 
        }
        return _C;
    });
    cout<<result<<" | ";

    std::transform(str.begin(), str.end(), result.begin(), ::tolower); //::toupper
    cout<<result<<" | ";

    //反转，旋转
    //reserve, reserve_copy, rotate, rotate_copy
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2;
    
    v2.resize(v1.size());
    std::cout<<"\n  ";
    reverse(v1.begin(), v1.end());
    show_vec(v1);

    reverse_copy(v1.begin(), v1.end(), v2.begin());
    show_vec(v2);

    rotate(v1.begin(), v1.begin()+2, v1.end());
    show_vec(v1);

    rotate_copy(v1.begin(), v1.begin()+2, v1.end(), v2.begin());
    show_vec(v2);

    //随机函数
    random_shuffle(v2.begin(), v2.end(), [&v2](int val){
        return rand()%(v2.size());
    });
    show_vec(v2);

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
            cout<<"this:"<<thiscap()<<" | ";
        }

        void show_capture(){
            int b = 1;
            auto thiscap = [=, *this](){
                return a+b;
            };
            cout<<"*this:"<<thiscap()<<" | ";
        }
    private:
        int a{2};
    };
    lambda_class().show();
    lambda_class().show_capture();

    //无状态lambda可以隐式转换为函数指针
    auto foo = [](int a){
        return a;
    };
    cout<<(*foo)(3)<<" | ";

    //初始化捕获
    std::string str = "Hello";
    auto foo_1 = [str = std::move(str)](){
        return str + " World!";
    };
    cout<<foo_1();

    FUNCTION_END()
}