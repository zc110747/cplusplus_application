/*
new，delete, new[]，delete[]

new 操作符，它用于动态分配内存
delete 操作符，它用于释放动态分配的内存
new[] 操作符，它用于动态分配数组
delete[] 操作符，它用于释放动态分配的数组

new 操作符的语法如下：
pointer = new type;
pointer = new type(initializer);

delete 操作符的语法如下：
delete pointer;

new[] 操作符的语法如下：
pointer = new type[size];

delete[] 操作符的语法如下：
delete[] pointer;


*/
#include <iostream>
#include <type_traits>
#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <new>
#include <mutex>

// 线程安全的内存管理函数
class MEM_SAFE_ALLOC
{
private:
    /* data */
    inline static std::atomic<size_t> mem_size = 0;
    inline static std::mutex mtx;

public:
    MEM_SAFE_ALLOC(){
    }
    ~MEM_SAFE_ALLOC(){
    }

public:
    void *operator new(size_t size)
    {
        std::lock_guard<std::mutex> lk(mtx);
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL) {
            std::bad_alloc ba;
            throw ba;
        }
        mem_size += size;
        return pT;
    }

    void *operator new(size_t size, const std::nothrow_t&) noexcept
    {
        std::lock_guard<std::mutex> lk(mtx);
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL) {
            return nullptr;
        }
        mem_size += size;
        return pT;
    }

    void *operator new(size_t size, const int& id)
    {
        std::lock_guard<std::mutex> lk(mtx);
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL) {
            std::bad_alloc ba;
            throw ba;
        }
        mem_size += size;
        return pT;
    }

    void *operator new[](size_t size) {
        std::lock_guard<std::mutex> lk(mtx);
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL)
        {
            std::bad_alloc ba;
            throw ba;
        }
        mem_size += size;
        return pT;
    }

    void operator delete(void *ptr, size_t size)
    {
        std::lock_guard<std::mutex> lk(mtx);
        if(ptr!= nullptr)
        {
            free(ptr);
            mem_size -= size;
        }
    }

    void operator delete[](void *ptr, size_t size)
    {
        std::lock_guard<std::mutex> lk(mtx);
        if(ptr!= nullptr)
        {
            free(ptr);
            mem_size -= size;
        }
    }

    static int get_size()  { return mem_size; }
};

// 非线程安全的内存管理函数
class MEM_ALLOC
{
private:
    /* data */
    inline static std::atomic<size_t> mem_size = 0;

public:
    MEM_ALLOC(){
    }
    ~MEM_ALLOC(){
    }
public:
    void *operator new(size_t size)
    {
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL) {
            std::bad_alloc ba;
            throw ba;
        }

        mem_size += size;
        return pT;
    }

    void *operator new(size_t size, const std::nothrow_t&) noexcept
    {
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL) {
            return nullptr;
        }

        mem_size += size;
        return pT;
    }

    void *operator new(size_t size, const int& id)
    {
        void *pT = static_cast<void *>(malloc(size));
        if(pT == NULL) {
            std::bad_alloc ba;
            throw ba;
        }

        mem_size += size;
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
        return pT;
    }

    void operator delete(void *ptr, size_t size)
    {
        if(ptr != nullptr)
        {
            free(ptr);
            mem_size -= size;
        }
    }

    void operator delete[](void *ptr, size_t size)
    {
        if(ptr != nullptr)
        {
            free(ptr);
            mem_size -= size;
        }
    }

    static int get_size()  { return mem_size; }
};

class USER: public MEM_ALLOC
{
public:
    int a{0};

public:
    USER(){
    }
    ~USER(){
    }
};

class USER_SAFE: public MEM_SAFE_ALLOC
{
public:
    float a{0};
    int b{-1};

public:
    USER_SAFE(){
    }
    ~USER_SAFE(){
    }
};

int main(int argc, char *argv[])
{
    USER *pm = new(std::nothrow) USER;
    pm->a = 1;
    std::cout<<"memory:"<<USER::get_size()<<std::endl;
    delete pm;
    pm = NULL;
    std::cout<<"memory:"<<USER::get_size()<<std::endl;

    USER* pm1 = new USER[2];
    std::cout<<"memory:"<<USER::get_size()<<std::endl;
    delete[] pm1;
    pm1 = NULL;
    std::cout<<"memory:"<<USER::get_size()<<std::endl;

    USER *pm2 = new(std::nothrow) USER;
    std::cout<<"memory:"<<USER::get_size()<<std::endl;
    delete pm2;
    pm2 = NULL;
    std::cout<<"memory:"<<USER::get_size()<<std::endl;

    auto *su1 = new USER_SAFE;
    std::cout<<"safety memory:"<<su1->get_size()<<std::endl;

    auto *su2 = new USER_SAFE[2];
    std::cout<<"safety memory:"<<su2->get_size()<<std::endl;
    
    delete su1;
    delete[] su2;
    std::cout<<"safety memory:"<<((USER_SAFE *)NULL)->get_size()<<std::endl;   
    return 0;
}
