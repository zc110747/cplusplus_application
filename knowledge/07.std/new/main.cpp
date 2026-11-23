//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. std::new
//
// Author:
//      @公众号：<嵌入式技术总结>
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <type_traits>
#include <memory>
#include <string>
#include <vector>
#include <new>
#include <mutex>
#include <thread>
#include <chrono>
#include <cstddef>
#include <cstdlib>

namespace NEW_BASE
{
    class demo {
    public:
        demo() {
            std::cout << "demo constructor" << std::endl;
        }
        ~demo() {
            std::cout << "demo destructor" << std::endl;
        }

        void print(void) {
            std::cout << "demo print" << std::endl;
        }
    };

    int test(void)
    {
        std::cout << "============ NEW_BASE ==============" << std::endl;

        // new - delete
        int *pi1 = new int;
        *pi1 = 100;
        std::cout << "pi1: " << *pi1 << std::endl;
        delete pi1;

        int *pi2 = new int(200);
        std::cout << "pi2: " << *pi2 << std::endl;
        delete pi2;

        // new - delete[]
        int *pi3 = new int[5];
        for(int i = 0; i < 5; i++) {
            pi3[i] = i;
        }
        for(int i = 0; i < 5; i++) {
            std::cout << "pi3[" << i << "] = " << pi3[i] << ", ";
        }
        std::cout << std::endl;
        delete[] pi3;

        // new 数组初始化
        int *pi4 = new int[5]{1, 2, 3, 4, 5};
        for(int i = 0; i < 5; i++) {
            std::cout << "pi4[" << i << "] = " << pi4[i] << ", ";
        }
        std::cout << std::endl;
        delete[] pi4;

        // new - delete 类对象
        demo* pDemo = new demo();
        pDemo->print();
        delete pDemo;

        auto p = malloc(sizeof(demo));
        demo* pDemo1 = new(p) demo;
        pDemo1->print();
        pDemo1->~demo();
        free(p);

        // 通过unique_ptr管理指针对象
        {
            auto vptr = std::unique_ptr<int[]>(new int[5]);
            for(int i = 0; i < 5; i++) {
                vptr[i] = i;
            }
            for(int i = 0; i < 5; i++) {
                std::cout << "vptr[" << i << "] = " << vptr[i] << ", ";
            }
            std::cout << std::endl;
        }

        {
            auto vptr = std::unique_ptr<demo>(new demo());
            vptr->print();
        }

        return 0;
    }
}

namespace NEW_REDEFINE
{
    // 线程安全的内存管理函数
    template<bool is_safe = true>
    class MEM_ALLOC
    {
    private:
        // 分配块头部: 记录长度, 并保证返回给用户的内存满足 max_align_t 对齐
        struct alignas(std::max_align_t) block_header {
            size_t size;
        };

        inline static size_t mem_size = 0;
        inline static std::mutex mtx;

    public:
        MEM_ALLOC(){
        }
        ~MEM_ALLOC(){
        }

    public:
        // 统一分配入口: 头部记录长度, 无尺寸的 delete 也能正确归还
        static void *allocate(size_t size) noexcept {
            void *raw = malloc(sizeof(block_header) + size);
            if(raw == nullptr) {
                return nullptr;
            }

            block_header *header = static_cast<block_header *>(raw);
            header->size = size;
            mem_size += size;
            return static_cast<void *>(header + 1);
        }

        // 统一释放入口: 从头部取回长度, 不依赖 delete 的 size 参数
        static void release(void *ptr) noexcept {
            if(ptr == nullptr) {
                return;
            }

            block_header *header = static_cast<block_header *>(ptr) - 1;
            if(mem_size >= header->size) {
                mem_size -= header->size;
            } else {
                mem_size = 0;
            }
            free(header);
        }

        static void *new_with_throw(size_t size) {
            void *pT = allocate(size);
            if(pT == nullptr) {
                throw std::bad_alloc();
            }
            return pT;
        }

        static void *new_without_throw(size_t size) noexcept {
            return allocate(size);
        }

        void *operator new(size_t size) {
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                return new_with_throw(size);
            } else {
                return new_with_throw(size);
            }
        }

        void *operator new(size_t size, const int& id) {
            (void)id;
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                return new_with_throw(size);
            } else {
                return new_with_throw(size);
            }
        }

        void *operator new(size_t size, const std::nothrow_t&) noexcept {
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                return new_without_throw(size);
            } else {
                return new_without_throw(size);
            }
        }

        void *operator new[](size_t size) {
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                return new_with_throw(size);
            } else {
                return new_with_throw(size);
            }
        }

        // 无尺寸释放: 长度取自分配时写入的头部
        void operator delete(void *ptr) noexcept {
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                release(ptr);
            } else {
                release(ptr);
            }
        }

        void operator delete[](void *ptr) noexcept {
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                release(ptr);
            } else {
                release(ptr);
            }
        }

        // 带尺寸释放: 保留以匹配 sized deallocation, 长度仍以头部为准
        void operator delete(void *ptr, size_t size) noexcept {
            (void)size;
            operator delete(ptr);
        }

        void operator delete[](void *ptr, size_t size) noexcept {
            (void)size;
            operator delete[](ptr);
        }

        // new(std::nothrow) 失败或构造抛异常时的配对版本
        void operator delete(void *ptr, const std::nothrow_t&) noexcept {
            operator delete(ptr);
        }

        void operator delete[](void *ptr, const std::nothrow_t&) noexcept {
            operator delete[](ptr);
        }

        static size_t get_size(void)  { return mem_size; }
    };

    class USER_SAFE: public MEM_ALLOC<true>
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
    
    class demo{
    public:
        demo() {
            std::cout << "demo()" << std::endl;
        }
        ~demo() {
            std::cout << "~demo()" << std::endl;
        }

        void* operator new(size_t size) {
            std::cout << "new demo" << std::endl;
            return ::operator new(size);
        }

        //与 operator new 配对的普通释放版本, 供 destroying delete 之外的场景使用
        void operator delete(void *ptr) noexcept {
            std::cout << "delete demo(void*)" << std::endl;
            ::operator delete(ptr);
        }

        void operator delete(demo *ptr, std::destroying_delete_t) {
            std::cout << "delete demo" << std::endl;
            ::operator delete(ptr);
        }
    };

    void memory_process_task(void)
    {
        for (int i = 0; i < 200; i++) {
            auto *pm = new USER_SAFE;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            delete pm;

            pm = new(std::nothrow) USER_SAFE;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            delete pm;
        } 
    }

    int test(void)
    {
        std::cout << "=============== NEW_REDEFIINE ===============" << std::endl;
        auto *pm1 = new USER_SAFE;
        std::cout<<"safety memory:"<<pm1->get_size()<<std::endl;

        auto *pm2 = new USER_SAFE[2];
        std::cout<<"safety memory:"<<pm2->get_size()<<std::endl;

        auto *pm3 = new(std::nothrow) USER_SAFE;
        if(pm3 == nullptr) {
            std::cout<<"new failed"<<std::endl;
        } else {
            std::cout<<"safety memory:"<<pm3->get_size()<<std::endl;
        }

        delete pm1;
        delete[] pm2;
        delete pm3;

        std::cout<<"safety memory:"<<MEM_ALLOC<true>::get_size()<<std::endl; 
        
        std::vector<std::thread> threads;
        for (int i = 0; i < 5; i++) {
            threads.emplace_back(memory_process_task);
        }
        for (auto& thread : threads) {
            thread.join();
        }
        std::cout<<"safety memory:"<<MEM_ALLOC<true>::get_size()<<std::endl; 

        auto *pm4 = new demo;
        pm4->~demo();
        delete pm4;
        return 0;
    }
}

int main()
{
    NEW_BASE::test();

    NEW_REDEFINE::test();
    return 0;
}
