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
//      @zc
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
        /* data */
        inline static size_t mem_size = 0;
        inline static std::mutex mtx;

    public:
        MEM_ALLOC(){
        }
        ~MEM_ALLOC(){
        }

    public:
        static void *new_with_throw(size_t size) {
            void *pT = static_cast<void *>(malloc(size));
            if(pT == NULL) {
                std::bad_alloc ba;
                throw ba;
            }
            mem_size += size;
            return pT;
        }

        static void *new_without_throw(size_t size) noexcept{
            void *pT = static_cast<void *>(malloc(size));
            if(pT == NULL) {
                return nullptr;
            }
            mem_size += size;
            return pT;
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

        static void delete_ptr(void *ptr, size_t size) {
           if (ptr != nullptr) {
                free(ptr);
                mem_size -= size;
            }
        }

        void operator delete(void *ptr, size_t size) {
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                delete_ptr(ptr, size);
            } else {
                delete_ptr(ptr, size);
            }
        }

        void operator delete[](void *ptr, size_t size) {
            if constexpr(is_safe) {
                std::lock_guard<std::mutex> lk(mtx);
                delete_ptr(ptr, size);
            } else {
                delete_ptr(ptr, size);
            }
        }

        static int get_size()  { return mem_size; }
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
            std::cout<<"demo()"<<std::endl;
        }
        ~demo() {
            std::cout<<"~demo"<<std::endl;
        }

        void* operator new(size_t size) {
            std::cout<<"new demo"<<std::endl;
            return ::operator new(size);
        }

        void operator delete(demo *ptr, std::destroying_delete_t) {
            std::cout<<"delete demo"<<std::endl;
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

int main(int argc, const char *argv[])
{
    NEW_BASE::test();

    NEW_REDEFINE::test();
    return 0;
}
