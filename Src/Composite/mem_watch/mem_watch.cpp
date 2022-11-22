/**********************************************************
 * 说明: MEM_TEST.cpp
 * 自定义重载new, delete, new[], delete[]
 * 用于C++环境下自定义的memory管理
 * 可以监测内存是否泄漏
************************************************************/
#include <iostream>
#include <type_traits>
#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <new>

class MEM_TEST
{
private:
    /* data */
    inline static std::atomic<size_t> mem_size = 0;

public:
    MEM_TEST(){
        std::cout<<"MEM_TEST"<<" | ";
    }
    ~MEM_TEST(){
        std::cout<<"~MEM_TEST"<<" | ";
    }
public:
    void *operator new(size_t size)
    {
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL)
        {
            std::bad_alloc ba;
            throw ba;
        }

        mem_size += size;
        std::cout<<"operator new:"<<size<<" | ";
        return pT;
    }

    void *operator new(size_t size, const std::nothrow_t&) noexcept
    {
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL)
            return nullptr;

        mem_size += size;
        std::cout<<"operator new nothrow:"<<size<<" | ";
        return pT;
    }

    void *operator new(size_t size, const int& id)
    {
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL)
        {
            std::bad_alloc ba;
            throw ba;
        }

        mem_size += size;
        std::cout<<"operator new id:"<<size<<" "<<id<<" | ";
        return pT;
    }
    
    void *operator new[](size_t size)
    {
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL)
        {
            std::bad_alloc ba;
            throw ba;
        }

        mem_size += size;
        std::cout<<"operator new[]:"<<size<<" | ";
        return pT;
    }

    void operator delete(void *ptr, size_t size)
    {
        if(ptr != nullptr)
        {
            std::cout<<"operator delete!"<<size<<" | ";
            free(ptr);
            mem_size -= size;
        }
    }

    void operator delete[](void *ptr, size_t size)
    {
        if(ptr != nullptr)
        {
            std::cout<<"operator delete[]!"<<size<<" | ";
            free(ptr);
            mem_size -= size;
        }
    }

    static int get_size()  { return mem_size; }
};

class User: public MEM_TEST
{
public:
    int a;

public:
    User(){
        std::cout<<"User"<<" | ";
    }
    ~User(){
        std::cout<<"~User"<<" | ";
    }
};

int main(int argc, char *argv[])
{
    User *pm = new User;
    pm->a = 1;
    std::cout<<MEM_TEST::get_size()<<" | ";
    delete pm;
    pm = NULL;
    std::cout<<MEM_TEST::get_size()<<std::endl;

    User* pm1 = new User[2];
    std::cout<<MEM_TEST::get_size()<<" | ";
    delete[] pm1;
    pm1 = NULL;
    std::cout<<MEM_TEST::get_size()<<std::endl;

    User *pm2 = new(std::nothrow) User;
    std::cout<<MEM_TEST::get_size()<<" | ";
    delete pm2;
    pm2 = NULL;
    std::cout<<MEM_TEST::get_size()<<std::endl;

    User *pm3 = new(5) User;
    std::cout<<MEM_TEST::get_size()<<" | ";
    delete pm3;
    pm3 = NULL;
    std::cout<<MEM_TEST::get_size()<<std::endl;
    return 0;
}


