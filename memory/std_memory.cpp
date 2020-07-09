/**********************************************************
 * 说明: memory 用于处理智能指针的C++接口实现
 * get      获取memory内的指针地址
 * reset    释放智能指针资源，同时置空
 * release  将智能指针置空，但不释放资源，资源等待作用域结束释放
 * swap     交换内部资源的使用权，被交换的对象置空
************************************************************/
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>

class deleter 
{  
public:
    deleter():m_count(0) {}
    template <class T>
    void operator()(T* p) {
        std::cout << "delete:"<<++m_count<<std::endl;
        delete p;
        p = nullptr;
    }
private:
    int m_count;
};

int main(int argc, char* argv[])
{
    //使用unique_ptr和share_ptr管理智能指针，auto_ptr被废弃
    //get
    std::unique_ptr<int> unique_ptr_int0(new int(1));
    int *punique_ptr = unique_ptr_int0.get();
    std::cout<<"unique_ptr_int0 val:"<<*unique_ptr_int0<<" "<<*punique_ptr<<std::endl;

    //swap 
    #define MEMORY_BUF_SIZE 2
    std::unique_ptr<int[]> unique_ptr_int1(new int[MEMORY_BUF_SIZE]);
    unique_ptr_int1[0] = 10;
    unique_ptr_int1[1] = 20;
    std::cout<<"unique_ptr_int1 val:";
    std::for_each(unique_ptr_int1.get(), unique_ptr_int1.get()+MEMORY_BUF_SIZE, [](int &n){
        std::cout<<n<<" ";
    });
    std::cout<<std::endl;
    std::cout<<"unique_ptr_int2 val:";
    std::unique_ptr<int[]> unique_ptr_int2;
    unique_ptr_int2.swap(unique_ptr_int1);
    std::for_each(unique_ptr_int2.get(), unique_ptr_int2.get()+MEMORY_BUF_SIZE, [](int &n){
        std::cout<<n<<" ";
    });
    std::cout<<std::endl;
    std::cout<<"unique_ptr_int1 empty:"<<(unique_ptr_int1.get() == nullptr)<<std::endl;

    //reset, release
    std::cout<<"reset, ";
    std::unique_ptr<int[], deleter> unique_ptr_int3{new int[3]};
    unique_ptr_int3.reset();
    std::unique_ptr<int[], deleter> unique_ptr_int4{new int[3]};
    unique_ptr_int3.release();

    //operator
    std::unique_ptr<std::string> unique_ptr_str0(new std::string("hi, uq0 string"));
    std::cout<<"unique_ptr_str0 val:"<<*unique_ptr_str0<<std::endl;
    std::unique_ptr<std::string> unique_ptr_str1;
    unique_ptr_str1 = std::unique_ptr<std::string>(new std::string("hi, uq1 string"));
    std::cout<<"unique_ptr_str1 val:"<<*unique_ptr_str1<<std::endl;
    std::unique_ptr<std::string> unique_ptr_str2;
    //unique_ptr不允许直接赋值
    //unique_ptr_str2 = unique_ptr_str1;
    unique_ptr_str2 = std::move(unique_ptr_str1);
    std::cout<<*unique_ptr_str2<<std::endl;
    if(unique_ptr_str1.get() == nullptr)
    {
        std::cout<<"unique_ptr_str1 empty"<<std::endl;
    }

    //基于内部计数的共享内存
    std::shared_ptr<int> share_ptr_int0(new int(1));
    auto share_ptr_str1 = share_ptr_int0;
    std::cout<<"share_ptr_int0: "<<*share_ptr_int0<<" "<<*share_ptr_str1<<std::endl;
    auto share_ptr_int1 = std::move(share_ptr_int0);
    std::cout<<"share_ptr_int0: "<<*share_ptr_int1<<std::endl;
    if(share_ptr_int0.get() == nullptr)
    {
        std::cout<<"share_ptr_int0 empty"<<std::endl;
    }
}
