/**********************************************************
 * 说明: memory 用于处理智能指针的C++接口实现
************************************************************/
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char* argv[])
{
    //使用unique_ptr和share_ptr管理智能指针，auto_ptr被废弃
    std::unique_ptr<int> uptr0(new int(1));
    int *p_int0 = uptr0.get();
    std::cout<<*uptr0<<" "<<*p_int0<<std::endl;

    std::unique_ptr<std::string> u_pstr0(new std::string("hi, uq0 string"));
    std::cout<<*u_pstr0<<std::endl;
    std::unique_ptr<std::string> u_pstr1;
    u_pstr1 = std::unique_ptr<std::string>(new std::string("hi, uq1 string"));
    std::cout<<*u_pstr1<<std::endl;
    std::unique_ptr<std::string> u_pstr2;
    //unique_ptr不允许直接赋值
    //u_pstr2 = u_pstr1;
    u_pstr2 = std::move(u_pstr1);
    std::cout<<*u_pstr2<<std::endl;
    if(u_pstr1.get() == nullptr)
    {
        std::cout<<"u_pstr1 empty"<<std::endl;
    }

    //基于内部计数的共享内存
    std::shared_ptr<int> sotr1(new int(1));
    auto sotr2 = sotr1;
    std::cout<<*sotr1<<" "<<*sotr2<<std::endl;
    auto sotr3 = std::move(sotr1);
    std::cout<<*sotr3<<std::endl;
    if(sotr1.get() == nullptr)
    {
        std::cout<<"sotr1 empty"<<std::endl;
    }
    
    //[]
    std::unique_ptr<int[]> buffer(new int[2]);
    buffer[0] = 10;
    buffer[1] = 20;
    std::for_each(buffer.get(), buffer.get()+2, [](int &n){
        std::cout<<n<<" ";
    });
    std::cout<<std::endl;
}
