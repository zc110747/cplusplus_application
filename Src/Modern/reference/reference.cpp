
#include <iostream>
#include <stdio.h>
#include <assert.h>

using namespace std;

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }   

void rlvalue_process();
void objX_process();
void objMemPool_process();
void AutoRefProcess();

char cacheBuf[1024];
int reference_process(void)
{
    rlvalue_process();
    objX_process();
    objMemPool_process();
    AutoRefProcess();

    cout<<"main finished"<<endl;
    return 0;
}

void rlvalue_process()
{
    FUNCTION_START()

    //左值:指向特定内存的具有名称的值(有具体内存地址，在作用域内生命周期内有效)
    //右值:不指向具体内存地址的匿名值
    int a = 1;          //a左值，1右值
    int &b = a;         //b左值引用
    const int &c = 1;   //对于右值，可通过const进行引用
    auto p = &"hello";  //字符串字面量是右值
    int &&d = 1;
    sprintf(cacheBuf, "a:%d, b:%d, c:%d, p:%s, d:%d", a, b, c, p, d);
    cout<<cacheBuf;

    FUNCTION_END()
}

void objX_process()
{
    FUNCTION_START()

    //常量左值引用可以绑定右值
    //make_x返回的即是右值
    class X
    {
    public:
        X() {
            cout<<__func__<<" Struct"<<" | ";
        }
        X(const X&) {
            cout<<__func__<<" Copy Struct"<<" | ";
        }
        X& operator = (const X&){
            cout<<__func__<<" Struct"<<" | ";
            return *this;
        }
        ~X(){
            cout<<__func__<<" Destruct"<<" | ";
        }
        void show()
        {
            cout<<"show X"<<endl;
        }
    };

    auto make_x = []()->X{ return X();};

    X x1;               //X Struct
    X x2(x1);           //X Copy Struct
    X x3(make_x());     //X Struct
    x3 = make_x();      //X Struct | operator= Struct | ~X Destruct
    X &&x4 = make_x();  //X Struct
    x4.show();          
        
    FUNCTION_END()
}

constexpr auto PoolSize = 4096;
void objMemPool_process()
{
    FUNCTION_START()

    class MemoryPool
    {
    public:
        MemoryPool(): pool_(new char[PoolSize]){
            cout<<__func__<<" Struct"<<" | ";
        }
        ~MemoryPool(){
            if(pool_ != nullptr)
            {
                cout<<__func__<<" Destruct"<<" | ";
                delete pool_;
                pool_ = nullptr;
            }
        }
        MemoryPool(const MemoryPool& other): pool_(new char [PoolSize]){
            cout<<__func__<<" Copy Struct"<<" | ";
            memcpy(pool_, other.pool_, PoolSize);
        }
        MemoryPool(MemoryPool&& other) noexcept{
            cout<<__func__<<" Move Struct"<<" | ";
            pool_ = other.pool_;
            other.pool_ = nullptr;
        }
        MemoryPool& operator= (const MemoryPool& other){
            cout<<__func__<<" Operator"<<" | ";
            char *memPool = pool_;
            pool_ = new char[PoolSize];
            memcpy(pool_, other.pool_, PoolSize);
            if(memPool != nullptr)
                delete memPool;
            return *this;
        }
        MemoryPool& operator= (MemoryPool &&other) noexcept{
            cout<<__func__<<" Move Operator"<<" | ";
            if(pool_ != nullptr)
            {
                delete pool_;
            }
            pool_ = other.pool_;
            other.pool_ = nullptr;
            return *this;
        }
        bool is_pool_valid(){
            return pool_ != nullptr;
        }
    public:
        int a{0};

    private:
        char *pool_;
    };

    auto make_pool = []()->MemoryPool{
        return MemoryPool();
    };

    {
        int a = MemoryPool().a;
        cout<<"a:"<<a<<" | ";
    }
    cout<<'\n';

    {
        MemoryPool pool;                            //MemoryPool Struct
        pool = make_pool();                         //MemoryPool Struct, operator= Move Operator 
        MemoryPool pool_a(pool);                    //MemoryPool Copy Struct                     1-Obj
        MemoryPool pool_b(std::move(pool));         //MemoryPool Move Struct                     1-Obj
        MemoryPool pool_c = std::move(make_pool()); //MemoryPool Struct, MemoryPool Move Struct  1-Obj
    }

    FUNCTION_END()
}


template<class T>
void RunCode(T &m){cout<<"lvalue ref"<<" | "; }

template<class T>
void RunCode(T &&m){cout<<"rvalue ref"<<" | ";}

template<class T>
void RunCode(const T &m){cout<<"const lvalue ref"<<" | "; }

template<class T>
void RunCode(const T &&m){cout<<"const rvalue ref"<<" | "; }

void AutoRefProcess()
{
    FUNCTION_START()
    
    auto get_val = []()->int{ return 5;};

    int x = get_val();
    const int &y = get_val();
    int &&z = get_val();
    const int w = 5;
    cout<<"x:"<<x<<" y:"<<y<<'\n';
    
    //C++14支持lambda的泛型
    auto perfect_forword = [](auto&& t){
        RunCode(std::forward<decltype(t)>(t));
    };

    perfect_forword(x);
    perfect_forword(y);
    perfect_forword(move(z));
    perfect_forword(move(w));
    FUNCTION_END()
}