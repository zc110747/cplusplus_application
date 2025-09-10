## threading

线程库是C++提供的用于线程创建和管理，线程间交互，原子类型，条件变量和互斥量以及超时处理的函数库。

- [atomic](#atomic)
  - [atomic_flag](#atomic_flag)
  - [memory_order](#memory_order)
- [bind](#bind)
- [chrono](#chrono)
- [condition_variable](#condition_variable)
- [functional](#functional)
- [future](#future)
- [mutex](#mutex)
- [thread](#thread)
  - [thread_local](#thread_local)

## atomic

std::atomic是C++标准库中的模板类，定义在<atomic>头文件里，用于实现原子操作。原子操作指的是不可被中断的操作，在多线程环境下能保证数据的一致性和线程安全，避免数据竞争问题。

- 格式

```cpp
template <class T> struct atomic;
```

相关网址: https://cplusplus.com/reference/atomic/atomic/

- 成员函数

| 函数 | 描述 |
| --- | --- |
| is_lock_free | 返回一个布尔值，表示当前对象是否是原子的 |
| store | 设置对象的值 |
| load | 获取对象的值 |
| operator= | 设置对象的值 |
| operator T() | 获取对象的值 |
| exchange | 设置对象的值并返回旧值 |
| compare_exchange_weak | 比较并交换值，返回是否交换成功 |
| compare_exchange_strong | 比较并交换值，返回是否交换成功 |
| fetch_add | 获取对象的值并加上给定值 |
| fetch_sub | 获取对象的值并减去给定值 |
| fetch_and | 获取对象的值并和给定值按位与 |
| fetch_or | 获取对象的值并和给定值按位或 |
| fetch_xor | 获取对象的值并和给定值按位异或 |
| operator++ | 自增 |
| operator-- | 自减 |

- 示例

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

struct Node { int value; Node* next; };
std::atomic<Node*> list_head (nullptr);

void append (int val) 
{
    Node* oldHead = list_head;
    Node* newNode = new Node {val,oldHead};

    // what follows is equivalent to: list_head = newNode, but in a thread-safe way:
    while (!list_head.compare_exchange_weak(oldHead,newNode)) {
        newNode->next = oldHead;
    }
}

int main(int argc, char const *argv[]) 
{
    std::atomic<int> a(0);

    // store, load, T()
    a.store(1);
    std::cout << a.load() << std::endl;
    if (a == 1) {
        std::cout << "a == 1" << std::endl;
    }

    a.fetch_add(1);
    std::cout << "fetch_add: " << a << std::endl;

    a = 3;
    std::cout << "operator=: " << a << std::endl;

    a.fetch_sub(1);
    std::cout << "fetch_sub=: " << a << std::endl;

    a++;
    std::cout << "operator++: " << a << std::endl;

    a--;
    std::cout << "operator--: " << a << std::endl;

    // spawn 10 threads to fill the linked list:
    std::vector<std::thread> threads;
    for (int i=0; i<10; ++i) {
        threads.push_back(std::thread(append,i));
    }
    for (auto& th : threads) {
        th.join();
    }

    // print contents:
    for (Node* it = list_head; it!=nullptr; it=it->next) {
        std::cout << ' ' << it->value;
    }
    std::cout << '\n';

    // cleanup:
    Node* it; 
    while (it=list_head) {
        list_head=it->next;
        delete it;
    }
}
```

### atomic_flag

std::atomic_flag是C++标准库中最简单的原子类型，定义于<atomic>头文件。它是一种无锁的布尔标志，主要用于实现简单的同步机制，像自旋锁这类场景；

- 格式

```cpp
struct atomic_flag;
```

相关网址: https://cplusplus.com/reference/atomic/atomic_flag/

- 成员函数

| 函数 | 描述 |
| --- | --- |
| test_and_set | 测试并设置标志 |
| clear | 清除标志 |

- 示例

```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

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

// RAII 自旋锁守卫类
template<typename T>
class LockGuard {
public:
    LockGuard(T& spinLock) : _spinLock(spinLock) {
        _spinLock.lock();
    }
    ~LockGuard() {
        _spinLock.unlock();
    }
private:
    T& _spinLock;
};

SpinLock spinLock;
int sharedData = 0;
int sharedData2 = 0;

void worker(void)
{
    // 通过自旋锁保护sharedData
    for (int i = 0; i < 100; ++i) {
        spinLock.lock();
        ++sharedData;
        spinLock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // 通过自旋锁保护sharedData2
    for (int i = 0; i < 100; ++i) {
        {
            LockGuard<SpinLock> guard(spinLock);
            ++sharedData2;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char const *argv[]) 
{ 
    std::cout << "================ Atomic flag test ================" << std::endl;

    std::vector<std::thread> threads;

    // 创建多个线程
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker);
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Shared data: " << sharedData << std::endl;    // 1000
    std::cout << "Shared data2: " << sharedData2 << std::endl;  // 1000
    return 0;
}
```

### memory_order

std::memory_order是C++标准库中用于控制原子操作内存顺序的枚举类型，定义于<atomic>头文件。它提供了多种选项，用于指定原子操作的内存顺序，包括：

- std::memory_order_relaxed：宽松顺序，不保证任何顺序关系。
- std::memory_order_consume：消费顺序，保证在当前线程中，后续的读操作不会被重排到当前操作之前。
- std::memory_order_acquire：获取顺序，保证在当前线程中，当前操作之前的读操作不会被重排到当前操作之后。
- std::memory_order_release: 用于存储（写入）原子变量。当一个线程以 std::memory_order_release 方式写入原子变量时，该线程之前的所有内存操作都不能重排到这个写入操作之后，确保其他线程以std::memory_order_acquire方式读取该原子变量时，能看到这些之前的内存操作结果。
- std::memory_order_acq_rel：获取-释放顺序，同时包含获取和释放的语义。
- std::memory_order_seq_cst：这是默认的内存顺序，提供最强的顺序保证。所有使用 std::memory_order_seq_cst 的原子操作在所有线程中看起来都是以相同的顺序执行的，就好像所有操作都按顺序依次执行一样。不过，这种顺序保证会带来一定的性能开销。

```cpp
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

std::atomic<int> data(0);
std::atomic<bool> ready(false);

void writer() {
    data.store(42, std::memory_order_relaxed);
    ready.store(true, std::memory_order_relaxed);
}

void reader() {
    while (!ready.load(std::memory_order_relaxed)) {
    }
    std::cout << "Data: " << data.load(std::memory_order_relaxed) << std::endl;
}

int main(int argc, char *argv[])
{ 
    std::cout << "================ Memory order test ================" << std::endl;
    
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) 
    {
        threads.emplace_back(reader);
    }
    std::thread t1(writer);

    for (auto& t : threads) 
    {
        t.join();
    }
    t2.join();

    return 0;
}
```

## chrono

## condition_variable

## functional

## future

## mutex

### lock_guard

### scoped_lock

std::scoped_lock 
C++17 引入的模板类，定义在 <mutex> 头文件中，用于在作用域内管理多个互斥锁，避免死锁问题。它可以同时锁定多个互斥锁，并且在其生命周期结束时自动解锁这些互斥锁

## thread

std::thread是C++11 引入的标准库类，定义在<thread>头文件中，用于创建和管理线程。它提供了一种跨平台的方式来实现多线程编程，使得开发者可以方便地在C++程序中创建新的执行线程。

- 格式

```cpp 
// 默认初始化
thread() noexcept; 

// 带函数和可选参数的初始化
template <class Fn, class... Args>
explicit thread (Fn&& fn, Args&&... args);

// 移动构造函数
thread (thread&& x);
```

相关网址: https://cplusplus.com/reference/thread/thread/thread/

- 成员函数

| 成员函数 | 描述 |
| --- | --- |
| operator= | 移动赋值运算符(之前thread放弃资源) |
| get_id | 获取线程ID |
| joinable | 检查线程是否可等待(join、detach或者移动后返回false) |
| join | 等待线程完成 |
| detach | 分离线程，使线程在后台运行 |
| swap | 交换线程 |
| native_handle | 获取线程的原生句柄 |
| hardware_concurrency | 获取系统支持的并发线程数 |

this_thread定义用于获取当前thread的信息，包含如下所示。

```cpp
std::this_thread::get_id() // 获取线程id
std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 线程休眠100ms
std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::seconds(1)); // 线程直到某个时间，休眠1s
std::this_thread::yield(); // 线程放弃执行，让其他线程执行(让出cpu时间，等待满足某些条件)
```
### thread_local

thread_local 是C++11引入的存储类型说明符，用于声明线程局部变量。每个线程都有该变量的独立副本，线程间的修改互不影响，这对于多线程编程中避免数据竞争非常有用。声明thread_local的变量，在每个线程中都有独立的副本，不会被其他线程访问到。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <string>

int main(int argc, char *argv[])
{
    std::cout << "================ Thread local test ================" << std::endl;
    
    thread_local int lcoal_var = 1;
    std::mutex mutex_lock;

    // 引用变量，lambda函数
    auto func = [&](std::string str) -> void {
        lcoal_var++;
        std::lock_guard<std::mutex> lock(mutex_lock);
        std::cout << str << ": " << lcoal_var << std::endl;
    };

    std::thread t1(func, "t1");
    std::thread t2(func, "t2");

    t1.join();
    t2.join();

    // t1: 2
    // t2: 2
    // thread_local var: 1
    std::cout << "thread_local var: " << lcoal_var << std::endl;
    return 0;
}
```
