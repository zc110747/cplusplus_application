/*
std::atomic 是 C++ 标准库中的模板类，定义在 <atomic> 头文件里，用于实现原子操作。原子操作指的是不可被中断的操作，在多线程环境下能保证数据的一致性和线程安全，避免数据竞争问题

std::atomic_flag 是C++标准库中最简单的原子类型，定义于 <atomic> 头文件。它是一种无锁的布尔标志，主要用于实现简单的同步机制，像自旋锁这类场景；test_and_set()和clear()方法都可以指定内存顺序，用于控制原子操作的同步语义

std::memory_order 是C++标准库中用于控制原子操作内存顺序的枚举类型，定义于 <atomic> 头文件。它提供了多种选项，用于指定原子操作的内存顺序，包括：
- std::memory_order_relaxed：宽松顺序，不保证任何顺序关系。
- std::memory_order_consume：消费顺序，保证在当前线程中，后续的读操作不会被重排到当前操作之前。
- std::memory_order_release: 用于存储（写入）原子变量。当一个线程以 std::memory_order_release 方式写入原子变量时，该线程之前的所有内存操作都不能重排到这个写入操作之后，确保其他线程以 std::memory_order_acquire 方式读取该原子变量时，能看到这些之前的内存操作结果
- std::memory_order_seq_cst：这是默认的内存顺序，提供最强的顺序保证。所有使用 std::memory_order_seq_cst 的原子操作在所有线程中看起来都是以相同的顺序执行的，就好像所有操作都按顺序依次执行一样。不过，这种顺序保证会带来一定的性能开销。
*/
#include <atomic>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>    // std::chrono::seconds
#include <vector>

std::atomic<int> value = 0;

void task0(void)
{
    int index = 0;
    for(index = 0; index<1; index++)
    {
        value++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void task1(void)
{
    int index = 0;
    for(index = 0; index<1; index++)
    {
        value++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// 自旋锁类
class SpinLock {
private:
    std::atomic_flag _lock_flag{ATOMIC_FLAG_INIT};

public:
    void lock() {
        while (_lock_flag.test_and_set(std::memory_order_acquire)) {
            // 自旋等待锁释放
        }
    }

    void unlock() {
        _lock_flag.clear(std::memory_order_release);
    }
};

SpinLock spinLock;
int sharedData = 0;

void worker(void)
{
    for (int i = 0; i < 100; ++i) {
        spinLock.lock();
        ++sharedData;
        spinLock.unlock();
    }
}

std::atomic<int> data(0);
std::atomic<bool> ready(false);

void writer() {
    data.store(42, std::memory_order_release);
    ready.store(true, std::memory_order_release);
}

void reader() {
    while (!ready.load(std::memory_order_acquire));
    std::cout << "Data: " << data.load(std::memory_order_acquire) << std::endl;
}

int main(int argc, char* argv[])
{
    {
        std::thread thread_0(task0);
        std::thread thread_1(task1);
    
        thread_0.join();
        thread_1.join();
    
        std::cout<<value<<std::endl;
        std::cout<<value.load()<<std::endl;
    }

    {
        std::vector<std::thread> threads;

        // 创建多个线程
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back(worker);
        }
    
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }
    
        std::cout << "Shared data: " << sharedData << std::endl;
    }

    {
        std::thread t1(writer);
        std::thread t2(reader);
    
        t1.join();
        t2.join();       
    }
    return 0;
} 
