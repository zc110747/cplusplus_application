## threading

线程库是C++提供的用于线程创建和管理，线程间交互，原子类型，条件变量和互斥量以及超时处理的函数库。

- [atomic](#atomic)
  - [atomic_flag](#atomic_flag)
  - [memory_order](#memory_order)
- [barrier](#barrier)
- [chrono](#chrono)
  - [system_clock](#system_clock)
  - [steady_clock](#steady_clock)
  - [high_resolution_clock](#high_resolution_clock)
- [condition_variable](#condition_variable)
- [execution](#execution)
- [future](#future)
- [latch](#latch)
- [mutex](#mutex)
  - [recursive_mutex](#recursive_mutex)
  - [lock_guard](#lock_guard)
  - [unique_lock](#unique_lock)
  - [scoped_lock](#scoped_lock)
- [semaphore](#semaphore)
  - [counting_semaphore](#counting_semaphore)
  - [binary_semaphore](#binary_semaphore)
- [shared_mutex](#shared_mutex)
  - [shared_timed_mutex](#shared_timed_mutex)
- [stop_token](#stop_token)
- [syncstream](#syncstream)
- [thread](#thread)
  - [thread_local](#thread_local)

## atomic

atomic是C++标准库中的模板类，定义在<atomic>头文件里，用于实现原子操作。原子操作指的是不可被中断的操作，在多线程环境下能保证数据的一致性和线程安全，避免数据竞争问题。

atomic参考网址：<https://en.cppreference.com/w/cpp/header/atomic.html>

- 格式

```cpp
template <class T> struct atomic;
```

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

另外，atomic也支持一系列函数来处理原子操作，具体如下所示。

| 函数 | 描述 |
| --- | --- |
| atomic_is_lock_free | 返回一个布尔值，表示当前对象是否是原子的 |
| atomic_fetch_add | 获取对象的值并加上给定值 |
| atomic_fetch_sub | 获取对象的值并减去给定值 |
| atomic_fetch_and | 获取对象的值并和给定值按位与 |
| atomic_fetch_or | 获取对象的值并和给定值按位或 |
| atomic_fetch_xor | 获取对象的值并和给定值按位异或 |
| atomic_wait | 等待对象的值不等于给定值 |
| atomic_notify_one | 通知一个在atomic_wait中阻塞的线程 |
| atomic_notify_all | 通知所有在atomic_wait中阻塞的线程 |
| atomic_flag_test | 测试标志 |
| atomic_flag_test_and_set | 测试并设置标志 |
| atomic_flag_clear | 清除标志 |
| atomic_flag_notify_one | 通知一个在atomic_flag_wait中阻塞的线程 |
| atomic_flag_notify_all | 通知所有在atomic_flag_wait中阻塞的线程 |
| atomic_thread_fence | 线程内存屏障 |
| atomic_signal_fence | 信号内存屏障 |

具体示例如下所示。

```cpp
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

int main(int argc, char const *argv[]) 
{
    std::atomic<int> a1(0);

    // atomic_thread_fence
    atomic_thread_fence(std::memory_order_acquire);
    std::cout << "atomic_thread_fence: " << a1 << std::endl;

    // atomic_signal_fence
    atomic_signal_fence(std::memory_order_acquire);
    std::cout << "atomic_signal_fence: " << a1 << std::endl;

    if (atomic_is_lock_free(&a1)) {
        std::cout << "a1 is lock free" << std::endl;
    } else {
        std::cout << "a1 is not lock free" << std::endl;
    }

    // atomic_fetch_add
    atomic_fetch_add(&a1, 1);
    std::cout << "atomic_fetch_add: " << a1 << std::endl;

    // atomic_fetch_sub
    atomic_fetch_sub(&a1, 1);
    std::cout << "atomic_fetch_sub: " << a1 << std::endl;

    // atomic_store
    atomic_store(&a1, 10);
    std::cout << "atomic_store: " << a1 << std::endl;

    // atomic_load
    auto val = atomic_load(&a1);
    std::cout << "atomic_load: " << val << std::endl;

    // atomic_exchange
    atomic_exchange(&a1, 100);
    std::cout << "atomic_exchange: " << a1 << std::endl;

    // atomic_fetch_and
    atomic_fetch_and(&a1, 0x55);
    std::cout << "atomic_fetch_and: " << a1 << std::endl;

    // atomic_fetch_or
    atomic_fetch_or(&a1, 0xAA);
    std::cout << "atomic_fetch_or: " << a1 << std::endl;

    // atomic_fetch_xor
    atomic_fetch_xor(&a1, 0x55);
    std::cout << "atomic_fetch_xor: " << a1 << std::endl;

    // atomic_wait
    atomic_wait(&a1, 100);
    std::cout << "atomic_wait: " << a1 << std::endl;

    // atomic_notify_one
    atomic_notify_one(&a1);
    std::cout << "atomic_notify_one: " << a1 << std::endl;

    // atomic_notify_all
    atomic_notify_all(&a1);
    std::cout << "atomic_notify_all: " << a1 << std::endl;
}
```

### atomic_flag

std::atomic_flag是C++标准库中最简单的原子类型，定义于<atomic>头文件。它是一种无锁的布尔标志，主要用于实现简单的同步机制，像自旋锁这类场景；

- 格式

```cpp
struct atomic_flag;
```

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
class spin_lock {
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

class spin_lock_func {
    std::atomic_flag _lock_flag{ATOMIC_FLAG_INIT};
public:
    void lock() {
        while (atomic_flag_test_and_set(&_lock_flag)) {
            // 自旋等待锁释放
        }
    }

    void unlock() {
        atomic_flag_clear(&_lock_flag);
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

spin_lock spinLock;
spin_lock_func spinLock1;

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
            LockGuard<spin_lock> guard(spinLock);
            ++sharedData2;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // 通过自旋锁保护sharedData
    for (int i = 0; i < 100; ++i) {
        spinLock1.lock();
        ++sharedData;
        spinLock1.unlock();
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

std::memory_order是C++标准库中用于控制原子操作内存顺序的枚举类型，定义于<atomic>头文件。它提供了多种选项，用于指定原子操作的内存顺序，具体如下所示。

- std::memory_order_relaxed：宽松顺序，不保证任何顺序关系。
- std::memory_order_consume：消费顺序，保证在当前线程中，后续的读操作不会被重排到当前操作之前。
- std::memory_order_acquire：获取顺序，保证在当前线程中，当前操作之前的读操作不会被重排到当前操作之后。
- std::memory_order_release: 用于存储（写入）原子变量。当一个线程以 std::memory_order_release 方式写入原子变量时，该线程之前的所有内存操作都不能重排到这个写入操作之后，确保其他线程以std::memory_order_acquire方式读取该原子变量时，能看到这些之前的内存操作结果。
- std::memory_order_acq_rel：获取-释放顺序，同时包含获取和释放的语义。
- std::memory_order_seq_cst：这是默认的内存顺序，提供最强的顺序保证。所有使用 std::memory_order_seq_cst 的原子操作在所有线程中看起来都是以相同的顺序执行的，就好像所有操作都按顺序依次执行一样。不过，这种顺序保证会带来一定的性能开销。

注意: 除非你清楚的知道要优化什么，不然都使用默认的内存顺序std::memory_order_seq_cst。

具体示例如下所示。

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

    for (int i = 0; i < 5; i++)  {
        threads.emplace_back(reader);
    }
    std::thread t1(writer);

    for (auto& t : threads) {
        t.join();
    }
    t1.join();

    return 0;
}
```

## barrier

std::barrier是C++20引入的同步原语，定义在<barrier>头文件，用于多线程同步，确保一组线程在特定点同步，所有线程到达该点后才能继续执行后续操作。

barrier参考网址: <https://en.cppreference.com/w/cpp/header/barrier>

barrier格式如下所示。

```cpp
// barrier
// completionFunction: 是一个无参数的函数对象。
template< class CompletionFunction = /* see below */ >
class barrier;

// constructor
// expected: 表示barrier的线程数。
// f: 是一个无参数的函数对象，会在所有线程到达barrier时调用。
constexpr explicit barrier( std::ptrdiff_t expected,
                            CompletionFunction f = CompletionFunction());
```

barrier使用方法如下。

| 方法 | 描述 |
| --- | --- |
| arrive | 线程到达barrier |
| wait | 线程等待barrier |
| arrive_and_wait | 线程到达barrier并等待 |
| arrive_and_drop | 线程到达barrier并放弃barrier |
| max | 获取barrier的线程数，静态 |

具体示例如下所示。

```cpp
#include <iostream>
#include <barrier>
#include <thread>
#include <vector>
#include <functional>

void call_completion(void) {
    std::cout << "Completion!" << std::endl;
}

int main(int argc, char const *argv[])
{
    constexpr int nums = 3;

    std::barrier sync_barrier(nums, call_completion);

    std::vector<std::thread> threads;

    // max
    std::cout << "sync_barrier count: " << sync_barrier.max() << std::endl;
    std::cout << "sync_barrier count: " << std::barrier<std::function<void()>>::max() << std::endl;

    // arrive_and_wait
    for (int i = 0; i < nums; i++)  {
        threads.emplace_back([&sync_barrier]() {
            std::cout << "Thread " << std::this_thread::get_id() << " arrives." << std::endl;
            sync_barrier.arrive_and_wait();
            std::cout << "Thread " << std::this_thread::get_id() << " continues." << std::endl;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
```

## chrono

chrono是一个非常重要的库，它提供了一套时间和日期的处理工具。chrono依赖于系统时钟，修改系统时钟可能会影响chrono的行为。

chrono参考网址: https://en.cppreference.com/w/cpp/header/chrono.html

std::chrono下主要包含system_clock、high_resolution_clock和steady_clock三种时钟，也支持一系列延时操作，具体如下所示。

| 函数 | 描述 |
| --- | --- |
| std::chrono::nanoseconds | 纳秒 |
| std::chrono::microseconds | 微秒 |
| std::chrono::milliseconds | 毫秒 |
| std::chrono::seconds | 秒 |
| std::chrono::minutes | 分钟 |
| std::chrono::hours | 小时 |
| std::chrono::days | 天数 |
| std::chrono::weeks | 周数 |
| std::chrono::months | 月数 |
| std::chrono::years | 年数 |

### system_clock

system_clock是系统范围的实时时钟，受系统时钟调整所影响。

主要支持方法如下。

- time_point：获取当前的时间点，内部存储了std::chrono::duration类型的时间间隔；其格式如下所示。

```cpp
// time_point
// Clock: 选择的基础时钟，可以是system_clock、steady_clock、high_resolution_clock中的一个。
// Duration: 时间间隔，可以是nanoseconds、microseconds、milliseconds、seconds、minutes、hours等。
template<
    class Clock,
    class Duration = typename Clock::duration
> class time_point;


// duration
// Rep: 时间间隔的存储类型，可以是int、long、double等。
// Period: 时间间隔的单位，默认是std::ratio<1>，表示秒。
template<
    class Rep,
    class Period = std::ratio<1>
> class duration;
```

注意：对于system_clock，其默认的system_clock::duration为std::chrono::seconds。

- to_time_t：将时间点转换为time_t, 时间单位为秒.
- from_time_t：将time_t转换为时间点, 单位time_point的时间点。
- time_point_cast：将一个时间点转换为另一个时间点(time_point)，转换时，时间间隔保持不变。

具体示例如下所示。

```cpp
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

// 定义时间，可以使用s、ms、ns作为后缀
using namespace std::chrono_literals;

using cur_clock = std::chrono::system_clock;
using cur_ms = std::chrono::milliseconds;
using cur_s = std::chrono::seconds;

template<class duration>
using cur_time_point = std::chrono::time_point<cur_clock, duration>;

int main(int argc, const char *argv[])
{
    std::chrono::duration<int, std::ratio<60*60*24>> one_day(1);    //声明一个1天的时间间隔

    // 获取当前时间
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point tomorrow = today + one_day;

    // 转换为time_t
    std::time_t tt;

    tt = std::chrono::system_clock::to_time_t ( today );
    std::cout << "today is: " << ctime(&tt);

    tt = std::chrono::system_clock::to_time_t( tomorrow );
    std::cout << "tomorrow will be: " << ctime(&tt);

    // 转换为time_point
    auto tt2 = std::chrono::system_clock::from_time_t(tt);
    std::cout << "tt2 is: " << tt2.time_since_epoch().count() << std::endl;

    std::chrono::duration<double> diff = tomorrow - today;
    std::cout << "count between today and tomorrow: " << diff.count() << std::endl;

    // time_point_cast转换
    cur_time_point<cur_s> ctp1{4s};
    auto ctp2 = std::chrono::time_point_cast<cur_ms>(ctp1);
    std::cout << "ctp1: " << ctp1.time_since_epoch().count() << std::endl;
    std::cout << "ctp2: " << ctp2.time_since_epoch().count() << std::endl;

    // C++20，新增sys_time、sys_seconds和sys_days
    std::chrono::sys_time t1 = std::chrono::system_clock::now();
    std::cout << "t1: " << t1.time_since_epoch().count() << std::endl;

    return 0;
}
```

### steady_clock

steady_clock是一个单调递增的时钟，不受系统时钟调整的影响，是专门用来计算时间间隔的，其支持方法和system_clock类似。

具体示例如下所示。

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

high_resolution_clock是一个高精度的时钟，其支持方法和system_clock类似，但是它的时间单位更细，通常是纳秒级别的。

具体示例如下所示。

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

condition_variable是C++11引入的一个类模板，用于在多线程环境中进行线程同步。它通常与std::mutex结合使用，用于在一个线程等待某个条件满足时阻塞该线程，而在另一个线程满足该条件时通知等待的线程继续执行。

condition_variable参考网址：<https://en.cppreference.com/w/cpp/thread/condition_variable.html>

具体格式如下所示。

```cpp
class condition_variable;
```

支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| wait | 阻塞当前线程，直到条件变量被通知 |
| wait_for | 阻塞当前线程，直到条件变量被通知或超时 |
| wait_until | 阻塞当前线程，直到条件变量被通知或超时时间到达 |
| notify_one | 通知一个等待的线程继续执行 |
| notify_all | 通知所有等待的线程继续执行 |
| native_handle | 获取底层的条件变量句柄 |

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <chrono>

std::condition_variable cv;
std::mutex mt;
std::atomic<bool> ready(false);

void work_thread(void)
{
    std::unique_lock<std::mutex> lock(mt);
    
    cv.wait(lock, [] { return ready.load(); });

    std::cout << "work_thread run " << std::endl;
}

int main(int argc, const char *argv[])
{
    std::thread t1(work_thread);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ready = true;

    std::cout << "ready is: " << ready << std::endl;

    cv.notify_one();
    
    t1.join();
    
    std::cout << "work_thread join" << std::endl;
    return 0;
}
```

## execution

std::execution是C++20引入的一个命名空间，用于定义异步任务的执行策略。它包含了两个枚举值：std::launch::async和std::launch::deferred。

- std::launch::async表示立即启动一个新线程执行任务
- std::launch::deferred表示延迟执行，直到调用future对象的get()方法时才启动新线程

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

## latch

std::latch是C++20引入的同步原语，定义在<latch>头文件中，用于多线程同步，可让一组线程等待直到某个事件完成。它的工作模式是一次性的，一旦计数器减到0，就不能再重置。

具体网址: https://en.cppreference.com/w/cpp/thread/latch.html

其构造函数如下所示。

```cpp
// 构造函数，声明latch对象
// expected: 初始化计数器的数值()
constexpr explicit latch( std::ptrdiff_t expected );
```

支持方法如下。

| 方法 | 说明 |
| --- | --- |
| count_down | 减少计数器的值 |
| wait | 阻塞当前线程，直到计数器减到0 |
| try_wait | 尝试等待计数器减到0，不阻塞线程 |
| arrive_and_wait | 减少计数器的值并阻塞当前线程，直到计数器减到0 |
| max | 返回计数器的最大值 |

具体示例如下所示。

```cpp
int main(int argc, const char* argv[])
{
    constexpr int num_threads = 4;

    std::latch work_done(num_threads);
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&work_done]() {
            std::cout << "线程 " << std::this_thread::get_id() << " 开始工作..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "线程 " << std::this_thread::get_id() << " 完成工作，计数器减 1。" << std::endl;
            work_done.count_down();
        });
    }

    work_done.wait();
    std::cout << "所有线程工作完成，主线程继续执行。" << std::endl;

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
```

## mutex

std::mutex是C++标准库中用于实现线程同步的互斥锁类，定义在 <mutex> 头文件中。它的主要作用是保护共享资源，防止多个线程同时访问这些资源而导致数据竞争和不一致的问题。

std::mutex参考链接: <https://en.cppreference.com/w/cpp/header/mutex.html>

std::mutex支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| unlock | 释放互斥锁 |
| native_handle | 返回底层操作系统的互斥锁句柄 |

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

int count = 0, cout1 = 0;
std::mutex mt, mt1;
void work_thread(void)
{
    mt.lock();
    count++;
    mt.unlock();

    if (mt1.try_lock()) {
        cout1++;
        mt1.unlock();
    }
}

int main(int argc, const char *argv[])
{
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++) {
        threads.push_back(std::thread(work_thread));
    }
    
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "count: " << count << std::endl;
    std::cout << "cout1: " << cout1 << std::endl;
    return 0;
}
```

timed_mutex则和mutex类似，增加了超时等待的功能，支持的方法如下。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| unlock | 释放互斥锁 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| try_lock_for | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则等待指定的时间 |
| try_lock_until | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则等待指定的时间 |
| native_handle | 获取底层操作系统的互斥锁句柄 |

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <string>

class demo
{
private:
    std::timed_mutex tm;
    std::string value_;

public:
    void fun1(void)
    {
        tm.lock();
        value_ = "1";
        std::cout << "fun1, value_ is: " << value_ << '\n';
        tm.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if(tm.try_lock()) {
            value_ = "2";
            std::cout << "fun1, value_ is: " << value_ << '\n';
            tm.unlock();
        }

        if(tm.try_lock_for(std::chrono::milliseconds(10))) {
            value_ = "3";
            std::cout << "fun1, value_ is: " << value_ << '\n';
            tm.unlock();
        }

        if(tm.try_lock_until(std::chrono::system_clock::now() + std::chrono::milliseconds(10))) {
            value_ = "4";
            std::cout << "fun1, value_ is: " << value_ << '\n';
            tm.unlock();
        }
    }
};

int main(int argc, const char *argv[])
{
    demo d;
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(&demo::fun1, &d));
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
```

另外mutex中支持只调用一次的接口std::call_once, 用于在多个线程中只执行一次某个操作。其函数原型如下。

```cpp
// call_once
// flag: std::once_flag对象, 用于标识该函数是否已经执行过了(执行过后不在执行)
// f: 函数对象或函数指针，该函数对象或函数指针将只被调用一次
// Args: 可变参数，函数参数
template< class Callable, class... Args >
void call_once( std::once_flag& flag, Callable&& f, Args&&... args );
```

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class demo
{
public:
   inline static std::once_flag flag;

   void fun1(void)
   {
        std::call_once(flag, [this]() {
            std::cout << "fun1" << '\n';
        });
   }
};

int main(int argc, const char *argv[])
{ 
    demo d;
    std::vector<std::thread> threads;

    // 创建5个线程，但只执行一次fun1
    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(&demo::fun1, &d));
    }

    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
```

### recursive_mutex

std::recursive_mutex是C++标准库中提供的一种递归互斥锁，定义在 <mutex> 头文件中。与普通的互斥锁不同，递归互斥锁允许同一个线程多次获取同一个互斥锁，而不会发生死锁。这在某些情况下非常有用，比如在递归函数或嵌套的锁保护下。

std::recursive_mutex支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| unlock | 释放互斥锁 |
| native_handle | 返回底层操作系统的互斥锁句柄 |

具体示例如下所示。

```cpp
#include <iostream>
#include <mutex>
#include <thread>
 
class demo
{
private:
    std::recursive_mutex m;
    
    std::string value_;

public:
    void fun1()
    {
        std::unique_lock<std::recursive_mutex> lk(m);
        value_ = "fun1";
        std::cout << "fun1, value_ is: " << value_ << '\n';
        std::cout << "fun1, native_handle: " << m.native_handle() << '\n';
    }

    void fun2()
    {
        std::unique_lock<std::recursive_mutex> lk(m);
        value_ = "fun2";
        std::cout << "fun2, value_ is: " << value_ << '\n';
        fun1(); // recursive lock becomes useful here
        std::cout << "back fun2, value_ is: " << value_ << '\n';
        std::cout << "fun2, native_handle: " << m.native_handle() << '\n';
    }
};
 
int main(int argc, const char *argv[])
{
    demo d1;
    std::thread t1(&demo::fun1, &d1);
    std::thread t2(&demo::fun2, &d1);
   
    t1.join();
    t2.join();

    return 0;
}
```

recursive_timed_mutex是C++标准库中提供的一种递归定时互斥锁，定义在 <mutex> 头文件中。它结合了递归互斥锁和定时互斥锁的功能，允许同一个线程多次获取同一个互斥锁，并且在等待互斥锁时可以设置超时时间。

recursive_timed_mutex支持的方法如下。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| try_lock_for | 尝试获取互斥锁，指定超时时间 |
| try_lock_until | 尝试获取互斥锁，指定到超时时间点 |
| unlock | 释放互斥锁 |
| native_handle | 返回底层操作系统的互斥锁句柄 |

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <string>

class demo
{
private:
    std::recursive_timed_mutex rtm;
    std::string value_;
public:
    void fun1()
    { 
        rtm.lock();
        value_ = "1";
        std::cout << "fun1, value_ is: " << value_ << '\n';
        fun2();
        rtm.unlock();

        if(rtm.try_lock()) {
            value_ = "2";
            std::cout << "fun1, value_ is: " << value_ << '\n';
            rtm.unlock();
        }

        if(rtm.try_lock_for(std::chrono::milliseconds(10))) {
            value_ = "3";
            std::cout << "fun1, value_ is: " << value_ << '\n';
            rtm.unlock();
        }

        if(rtm.try_lock_until(std::chrono::system_clock::now() + std::chrono::milliseconds(10))) {
            value_ = "4";
            std::cout << "fun1, value_ is: " << value_ << '\n';
            rtm.unlock();
        }
    }

    void fun2() {
        std::unique_lock<std::recursive_timed_mutex> lk(rtm);
        value_ = "fun2";
        std::cout << "fun2, value_ is: " << value_ << '\n';
    }
};

int main(int argc, const char *argv[])
{
    demo d;
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(&demo::fun1, &d));
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
```

### lock_guard

std::lock_guard是C++标准库中提供的一种RAII（资源获取即初始化）风格的互斥锁包装器，定义在 <mutex> 头文件中。它的主要作用是在构造函数中获取互斥锁，在析构函数中释放互斥锁，确保互斥锁的正确获取和释放。lock_guard仅用于通过RAII管理互斥锁的资源，而不是直接使用互斥锁。

关于lock_guard的原型如下所示。

```cpp
// 声明lock_guard类型
template <class Mutex> class lock_guard;
```

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class demo
{
private:
    std::mutex mtx;

public:
    void fun1()
    {
        std::lock_guard<std::mutex> lock(mtx);
        // 临界区代码
        std::cout << "fun1" << '\n';
    }
};

int main(int argc, const char *argv[])
{
    demo d;
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(&demo::fun1, &d));
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
```

### unique_lock

std::unique_lock是C++标准库中提供的一种RAII风格的互斥锁包装器，定义在 <mutex> 头文件中。与std::lock_guard不同，std::unique_lock提供了更多的灵活性，例如延迟锁定、超时等待等。

unique_lock格式原型如下所示。

```cpp
// 声明unique_lock类型
template <class Mutex> class unique_lock;
```

unique_lock支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| try_lock_for | 尝试获取互斥锁，指定超时时间 |
| try_lock_until | 尝试获取互斥锁，指定到超时时间点 |
| unlock | 释放互斥锁 |
| swap | 交换两个unique_lock对象的状态 |
| release | 释放互斥锁，返回是否成功 |
| native_handle | 返回底层操作系统的互斥锁句柄 |

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

int count = 0, count1 = 0;
std::mutex mt, mt1;
void work_thread(void)
{
    {
        // 自动释放
        std::unique_lock<std::mutex> lck(mt);
        count++;
    }

    // 手动解锁
    std::unique_lock<std::mutex> lck(mt);
    count++;
    lck.unlock();

    if (lck.try_lock()) {
        count1++;
        lck.unlock();
    }
    
    // 手动释放
    std::unique_lock<std::mutex> lck1(mt);
    count1++;
    lck.release();  
}

int main(int argc, const char *argv[])
{
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++) {
        threads.push_back(std::thread(work_thread));
    }
    
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "count: " << count << std::endl;
    std::cout << "count1: " << count1 << std::endl;
    return 0;
}
```

### scoped_lock

std::scoped_lock是C++17引入的模板类，定义在 <mutex> 头文件中，用于在作用域内管理多个互斥锁，避免死锁问题。它可以同时锁定多个互斥锁，并且在其生命周期结束时自动解锁这些互斥锁。

scoped_lock格式原型如下所示。

```cpp
// 模板类scoped_lock
// MutexTypes... - 互斥锁类型
template< class... MutexTypes >
class scoped_lock;
```

关于scoped_lock，具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

class demo
{
public:
    std::mutex mtx;
    
    int count{0};
};

void thread_function(demo &d1, demo &d2, int data) 
{
    std::scoped_lock lock(d1.mtx, d2.mtx);
   
    d1.count += data;
    d2.count += data;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main(int argc, const char *argv[])
{ 
    std::vector<std::thread> vec_thread;
    demo d1, d2;

    vec_thread.push_back(std::thread(thread_function, std::ref(d1), std::ref(d2), 1));
    vec_thread.push_back(std::thread(thread_function, std::ref(d1), std::ref(d2), 2));

    for (auto& t : vec_thread) {
        t.join();
    }

    std::cout << "d1.count: " << d1.count << std::endl;
    std::cout << "d2.count: " << d2.count << std::endl;

    return 0;
}
```

## semaphore

semaphore是C++20引入的同步原义库文件，用于多线程同步，可让一组线程等待直到某个事件完成。对于semaphore库文件，主要支持counting_semaphore和binary_semaphore.

semaphore参考网址: https://en.cppreference.com/w/cpp/header/semaphore.html

### counting_semaphore

counting_semaphore是C++20引入的一种信号量，用于控制对共享资源的访问。它可以用于实现生产者-消费者模型、线程池等场景。

counting_semaphore的主要方法如下所示。

| 方法 | 描述 |
| --- | --- |
| release | 增加信号量的值 |
| acquire | 减少信号量的值，如果值为0，则阻塞当前线程 |
| try_acquire | 尝试减少信号量的值，如果值为0，则返回false，否则返回true |
| try_acquire_for | 尝试减少信号量的值，指定超时时间 |
| try_acquire_until | 尝试减少信号量的值，指定到超时时间点 |
| max(static) | 返回信号量允许的最大计数 |

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <chrono>

std::counting_semaphore<1> sem(0);

void reader(void)
{
    sem.acquire();
    std::cout << "reader semaphore" << std::endl;
}

void writer(void)
{
    std::cout << "writer semaphore" << std::endl;
    sem.release();
}

int main(int argc, const char *argv[])
{
    std::thread t1(receiver);
    std::thread t2(sender);
    
    t1.join();
    t2.join();
    return 0;
}
```

### binary_semaphore

binary_semaphore是对于counting_semaphore的一种特殊情况，它的最大计数为1，只允许有0和1两种状态。其它方法和处理和counting_semaphore相同。

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <chrono>

std::binary_semaphore sem(0);

void reader(void)
{
    sem.acquire();
    std::cout << "reader semaphore" << std::endl;
}

void writer(void)
{
    std::cout << "writer semaphore" << std::endl;
    sem.release();
}

int main(int argc, const char *argv[])
{
    std::thread t1(receiver);
    std::thread t2(sender);
    
    t1.join();
    t2.join();
    return 0;
}
```

## shared_mutex

shared_mutex是C++14引入的一种读写锁头文件。与普通的互斥锁（如mutex）不同，shared_mutex允许多个线程同时进行读操作，但在写操作时会独占资源，确保数据的一致性。这种特性使得它在多读少写的场景下能显著提高并发性能。

shared_mutex参考网址：<https://en.cppreference.com/w/cpp/header/shared_mutex.html>

对于shared_mutex，支持如下特性：

- 共享锁（shared lock）：多个线程可同时获取，用于"读操作"（不修改数据）。
- 独占锁（exclusive lock）：仅允许一个线程获取，用于"写操作"（修改数据）。

在实际应用中，共享锁可以多个线程同时获取，而独占锁只能有一个线程获取，当有独占锁被获取时，其他线程无论是获取共享锁还是独占锁，都必须等待；而共享锁被获取时，其他线程只能获取共享锁，不能获取独占锁。对于shared_mutex支持如下方法。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| unlock | 释放互斥锁 |
| lock_shared | 获取共享所有权的互斥锁，如果互斥锁不可用则阻塞 |
| try_lock_shared | 尝试获取互斥锁以实现共享所有权，如果互斥锁不可用则返回。 |
| unlock_shared | 释放互斥锁的共享所有权 |
| native_handle | 返回底层操作系统的互斥锁句柄 |

shared_lock支持搭配unique_lock、lock_guard和shared_lock使用。其中搭配unique_lock和lock_guard时，相当于调用lock、unlock方法；而搭配shared_lock时，相当于调用lock_shared、unlock_shared方法。

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include <vector>

class demo
{
public:
    demo() = default;
    ~demo() = default;

    void show_val(void)
    {
        smtx_.lock_shared();
        std::cout << "read val: " << val_ << std::endl;
        smtx_.unlock_shared();
    }

    void increment(void)
    {  
        std::lock_guard<std::shared_mutex> lck(smtx_); // lock_guard，自动lock、unlock
        val_++;
    }

    void reset(void)
    {   
        // try_lock
        if(smtx_.try_lock())
        {
            val_ = 0;
            smtx_.unlock();
        }
    }

    int get(void)
    {   
        std::shared_lock<std::shared_mutex> lck(smtx_);
        int val = val_;
        return val;
    }

private:
    /// @brief val_
    /// @brief 共享变量val_，多个线程可以同时读取
    int val_{0};

    /// @brief smtx_
    /// @brief 共享互斥锁smtx_，用于保护val_
    std::shared_mutex smtx_;
};

int main(int argc, const char *argv[])
{
    std::cout << "================= SHARD_MUTEX_RAII ===================" << std::endl;
    std::vector<std::thread> threads;
    demo d1;

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(&demo::increment, &d1));
        threads.push_back(std::thread(&demo::show_val, &d1));
    }

    std::cout << "get: " << d1.get() << std::endl;
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "get: " << d1.get() << std::endl;
    d1.reset();
    std::cout << "reset val: " << d1.get() << std::endl;
    return 0;
} 
```

### shared_timed_mutex

std::shared_timed_mutex是C++14引入的，带时间限制的读写锁，用于多线程同步，可让一组线程等待直到某个事件完成。与shared_mutex相比，它支持超时等待，即如果在指定时间内没有获取到锁，线程会返回失败。

std::shared_timed_mutex支持的方法如下所示。

| 方法 | 描述 |
| --- | --- |
| lock | 阻塞当前线程，直到互斥锁被获取 |
| try_lock | 尝试获取互斥锁，如果互斥锁已被其他线程获取，则返回false，否则返回true |
| try_lock_for | 尝试获取互斥锁，指定超时时间 |
| try_lock_until | 尝试获取互斥锁，指定到超时时间点 |
| unlock | 释放互斥锁 |
| lock_shared | 获取共享所有权的互斥锁，如果互斥锁不可用则阻塞 |
| try_lock_shared | 尝试获取互斥锁以实现共享所有权，如果互斥锁不可用则返回。 |
| try_lock_shared_for | 尝试获取互斥锁以实现共享所有权，指定超时时间 |
| try_lock_shared_until | 尝试获取互斥锁以实现共享所有权，指定到超时时间点 |
| unlock_shared | 释放互斥锁的共享所有权 |

具体示例如下所示。

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include <vector>

class demo
{
public:
    demo() = default;
    ~demo() = default;

    void show_val(void)
    {
        if (smtx_.try_lock_shared_for(std::chrono::seconds(1)))
        {
            std::cout << "read val: " << val_ << std::endl;
            smtx_.unlock_shared();
        }
    }

    void increment(void)
    {  
        if (smtx_.try_lock_for(std::chrono::seconds(1)))
        {
            val_++;
            smtx_.unlock();
        }
    }

    void reset(void)
    {   
        // try_lock
        if(smtx_.try_lock())
        {
            val_ = 0;
            smtx_.unlock();
        }
    }

    int get(void)
    {   
        std::shared_lock<std::shared_timed_mutex> lck(smtx_);
        int val = val_;
        return val;
    }

private:
    /// @brief val_
    /// @brief 共享变量val_，多个线程可以同时读取
    int val_{0};

    /// @brief smtx_
    /// @brief 共享互斥锁smtx_，用于保护val_
    std::shared_timed_mutex smtx_;
};

int main(int argc, const char *argv[])
{
    std::cout << "================= SHARD_TIME_MUTEX ===================" << std::endl;
    std::vector<std::thread> threads;
    demo d1;

    for (int i = 0; i < 5; i++) {
        threads.push_back(std::thread(&demo::increment, &d1));
        threads.push_back(std::thread(&demo::show_val, &d1));
    }

    std::cout << "get: " << d1.get() << std::endl;
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "get: " << d1.get() << std::endl;
    d1.reset();
    std::cout << "reset val: " << d1.get() << std::endl;
    return 0;
}
```

## stop_token

std::stop_token是C++20引入的同步原语，定义在<stop_token>头文件中，用于多线程同步，可让一组线程等待直到某个事件完成。它的工作模式是基于计数器的，初始值为0，当调用release()方法时，计数器会增加，当调用acquire()方法时，计数器会减少。如果计数器为0，调用acquire()方法的线程会阻塞，直到计数器增加到非0值。

## syncstream

std::syncstream是C++20引入的同步原语，定义在<syncstream>头文件中，用于多线程同步，可让一组线程等待直到某个事件完成。它的工作模式是基于计数器的，初始值为0，当调用release()方法时，计数器会增加，当调用acquire()方法时，计数器会减少。如果计数器为0，调用acquire()方法的线程会阻塞，直到计数器增加到非0值。

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

具体示例代码如下所示。

```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>

class demo
{
private:
    std::mutex mtx;
    int count{0};

public:
    void work() {
        std::this_thread::yield();
        std::lock_guard<std::mutex> lock(mtx);
        count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    int get_count() {
        std::lock_guard<std::mutex> lock(mtx);
        return count;
    }
};

int main(int argc, char *argv[])
{
    demo d;
    std::vector<std::thread> vec_thread;

    for (int i = 0; i < 5; i++) {
        vec_thread.push_back(std::thread(&demo::work, &d));
    }

    for (auto& t : vec_thread) {
        t.join();
    }

    std::cout << "count: " << d.get_count() << std::endl;
    return 0;
}
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
