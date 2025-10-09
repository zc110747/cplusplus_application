## threading

线程库是C++提供的用于线程创建和管理，线程间交互，原子类型，条件变量和互斥量以及超时处理的函数库。

- [atomic](#atomic)
  - [atomic_flag](#atomic_flag)
  - [memory_order](#memory_order)
- [chrono](#chrono)
  - [system_clock](#system_clock)
  - [steady_clock](#steady_clock)
  - [high_resolution_clock](#high_resolution_clock)
- [condition_variable](#condition_variable)
- [future](#future)
- [mutex](#mutex)
- [shared_mutex](#shared_mutex)
- [thread](#thread)
  - [thread_local](#thread_local)

## atomic

std::atomic是C++标准库中的模板类，定义在<atomic>头文件里，用于实现原子操作。原子操作指的是不可被中断的操作，在多线程环境下能保证数据的一致性和线程安全，避免数据竞争问题。

- 格式

```cpp
template <class T> struct atomic;
```

相关网址: https://cplusplus.com/reference/atomic/

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

int main(int argc, const char *argv[])
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

std::chrono是一个非常重要的库，它提供了一套时间和日期的处理工具。chrono依赖于系统时钟，修改系统时钟可能会影响chrono的行为。

相关网址: https://cplusplus.com/reference/chrono/

std::chrono下主要包含system_clock、high_resolution_clock和steady_clock三种时钟，具体说明如下所示。

### system_clock

system_clock是系统范围的实时时钟，受系统时钟调整所影响。

主要支持方法如下。

1. time_point：获取当前的时间点
2. to_time_t：将时间点转换为time_t, 时间单位为秒
3. from_time_t：将time_t转换为时间点, 单位time_point

```cpp
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

int main(int argc, const char *argv[])
{
    std::chrono::duration<int, std::ratio<60*60*24> > one_day(1);    //声明一个1天的时间间隔

    // 获取当前时间
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point tomorrow = today + one_day;

    std::time_t tt;

    tt = std::chrono::system_clock::to_time_t ( today );
    std::cout << "today is: " << ctime(&tt);

    tt = std::chrono::system_clock::to_time_t( tomorrow );
    std::cout << "tomorrow will be: " << ctime(&tt);

    auto tt2 = std::chrono::system_clock::from_time_t(tt);
    std::cout << "tt2 is: " << tt2.time_since_epoch().count() << std::endl;

    std::chrono::duration<double> diff = tomorrow - today;
    std::cout << "count between today and tomorrow: " << diff.count() << std::endl;

    return 0;
}
```

### steady_clock

steady_clock是一个单调递增的时钟，不受系统时钟调整的影响，是专门用来计算时间间隔的。

```cpp
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

int main (int argc, const char *argv[])
{
    // 获取当前时间
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    std::cout << "printing out 1000 stars...\n";
    for (int i=0; i<1000; ++i) {
        std::cout << "*";
    }
    std::cout << std::endl;

    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

    // 计算时间间隔
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "It took me " << time_span.count() << " seconds." << std::endl;

    return 0;
}
```

### high_resolution_clock

high_resolution_clock是一个高精度的时钟，与system_clock类似，但是它的时间单位更细，通常是纳秒级别的。

主要支持方法如下。

1. time_point：获取当前的时间点
2. to_time_t：将时间点转换为time_t, 时间单位为秒
3. from_time_t：将time_t转换为时间点, 单位time_point

```cpp
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

int main(int argc, const char *argv[])
{
    std::chrono::duration<int, std::ratio<60*60*24> > one_day(1);    //声明一个1天的时间间隔

    // 获取当前时间
    std::chrono::high_resolution_clock::time_point today = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point tomorrow = today + one_day;

    std::time_t tt;

    // to_time_t 时间单位为秒
    tt = std::chrono::high_resolution_clock::to_time_t ( today );
    std::cout << "today is: " << ctime(&tt);

    tt = std::chrono::high_resolution_clock::to_time_t( tomorrow );
    std::cout << "tomorrow will be: " << ctime(&tt);

    // from_time_t 转换成time_point
    std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::from_time_t(tt);
    std::cout << "time is: " << time.time_since_epoch().count() << std::endl;

    std::chrono::duration<double> diff = tomorrow - today;
    std::cout << "count between today and tomorrow: " << diff.count() << std::endl;
    return 0;
}
```

## condition_variable

## future

std::future是C++11引入的一个类模板，用于表示异步操作的结果。它提供了一种机制，允许在一个线程中等待另一个线程的结果。

相关说明：https://cplusplus.com/reference/future/

### async

std::async是C++11引入的一个函数模板，用于启动异步任务。它的主要作用是在一个独立的线程中执行一个函数，并返回一个std::future对象，用于获取函数的返回值。

- 函数原型

```cpp
template <class Fn, class... Args>  future<typename result_of<Fn(Args...)>::type>
async (Fn&& fn, Args&&... args);

// 函数参数说明
// @policy: 异步任务的执行策略，可选值为std::launch::async、std::launch::deferred
//          std::launch::async表示立即启动一个新线程执行任务
//          std::launch::deferred表示延迟执行，直到调用future对象的get()方法时才启动新线程
// @fn: 要异步执行的函数或函数对象
// @args: 传递给函数的参数
template <class Fn, class... Args>  future<typename result_of<Fn(Args...)>::type>    
async (launch policy, Fn&& fn, Args&&... args);
```

具体示例如下所示。

```cpp
#include <iostream>
#include <future>

int long_running_task(int value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return value * 2;
}

int test(void)
{
    std::cout << "==================== ASYNC ====================" << std::endl;

    std::future<int> result = std::async(std::launch::async, long_running_task, 10);
    std::cout << "主线程正在执行其他操作..." << std::endl;
    int final_result = result.get();
    std::cout << "异步任务的结果: " << final_result << std::endl;

    return 0;
}
```

### promise

std::promise是C++11引入的一个类模板，用于在一个线程中设置值或异常，然后在另一个线程中通过关联的std::future对象获取这个值或异常，以此实现线程间的同步和数据传递。它允许线程之间进行数据交换。std::promise只能调用一次get_future()方法，返回一个std::future对象，该对象可以获取promise对象中设置的值或异常。

- 函数原型

```cpp
template <class T>  promise;

template <class R&> promise<R&>;
```

promise类模板支持方法如下。

| 方法 | 描述 |
| --- | --- |
| set_value | 设置promise对象的共享状态为已就绪，同时将值设置为指定的参数 |
| set_exception | 设置promise对象的共享状态为已就绪，同时将异常设置为指定的参数 |
| get_future | 返回关联的std::future对象，用于获取promise对象的共享状态和值 |
| set_value_at_thread_exit | 设置promise对象的共享状态为已就绪，同时将值设置为指定的参数，在当前线程退出时执行 |
| set_exception_at_thread_exit | 设置promise对象的共享状态为已就绪，同时将异常设置为指定的参数，在当前线程退出时执行 |

具体使用方法如下。



## mutex

std::mutex是C++标准库中用于实现线程同步的互斥锁类，定义在 <mutex> 头文件中。它的主要作用是保护共享资源，防止多个线程同时访问这些资源而导致数据竞争和不一致的问题。

std::mutex参考链接: https://cplusplus.com/reference/mutex/

std::mutex支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| unlock | 释放互斥锁 |
| native_handle | 返回底层操作系统的互斥锁句柄 |

### recursive_mutex

std::recursive_mutex是C++标准库中提供的一种递归互斥锁，定义在 <mutex> 头文件中。与普通的互斥锁不同，递归互斥锁允许同一个线程多次获取同一个互斥锁，而不会发生死锁。这在某些情况下非常有用，比如在递归函数或嵌套的锁保护下。

std::recursive_mutex支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| try_lock_for | 尝试获取互斥锁，指定超时时间 |
| try_lock_until | 尝试获取互斥锁，指定到超时时间点 |
| unlock | 释放互斥锁 |
| native_handle | 返回底层操作系统的互斥锁句柄 |

### scoped_lock

std::scoped_lock是C++17引入的模板类，定义在 <mutex> 头文件中，用于在作用域内管理多个互斥锁，避免死锁问题。它可以同时锁定多个互斥锁，并且在其生命周期结束时自动解锁这些互斥锁

### lock_guard

std::lock_guard是C++标准库中提供的一种RAII（资源获取即初始化）风格的互斥锁包装器，定义在 <mutex> 头文件中。它的主要作用是在构造函数中获取互斥锁，在析构函数中释放互斥锁，确保互斥锁的正确获取和释放。

- 函数原型

```cpp
// 声明lock_guard类型
template <class Mutex> class lock_guard;
```

- 示例代码

```cpp
#include <iostream>
#include <thread>
#include <mutex>

int main(int argc, const char *argv[])
{
    std::mutex mtx;
    {
        std::lock_guard<std::mutex> lock(mtx);
        // 临界区代码

    } // 锁在lock_guard对象销毁时自动释放
    
    return 0;
}
```

## shared_mutex

std::shared_mutex是C++17引入的一种读写锁，位于<shared_mutex>头文件中。与普通的互斥锁（如 std::mutex）不同，std::shared_mutex允许多个线程同时进行读操作，但在写操作时会独占资源，确保数据的一致性。这种特性使得它在多读少写的场景下能显著提高并发性能。

对于shared_mutex，支持如下特性：：

1. 共享锁（shared lock）：多个线程可同时获取，用于“读操作”（不修改数据）。
2. 独占锁（exclusive lock）：仅允许一个线程获取，用于“写操作”（修改数据）。

对于shared_mutex，支持如下方法。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| unlock | 释放互斥锁 |
| native_handle | 返回底层操作系统的互斥锁句柄 |
| lock_shared | 阻塞当前线程，直到共享锁被获取 |
| try_lock_shared | 尝试获取共享锁，如果共享锁已被其他线程获取，则返回false，否则返回true |
| unlock_shared | 释放共享锁 |

或者搭配lock_guard和unique_lock使用。

```cpp
// 搭配shared_lock使用
// 相当于lck.lock_shared()和lck.unlock_shared()
std::shared_mutex mtx;
{
    std::shared_lock<std::shared_mutex> lck(mtx);
}

// 搭配unique_lock使用
// 相当于lck.lock()和lck.unlock()
std::shared_mutex mtx;
{
    std::unique_lock<std::shared_mutex> lck(mtx);
}
```

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

std::thread参考链接: https://cplusplus.com/reference/thread/

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

thread_local是C++11引入的存储类型说明符，用于声明线程局部变量。每个线程都有该变量的独立副本，线程间的修改互不影响，这对于多线程编程中避免数据竞争非常有用。声明thread_local的变量，在每个线程中都有独立的副本，不会被其他线程访问到。

具体示例代码如下所示。

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
