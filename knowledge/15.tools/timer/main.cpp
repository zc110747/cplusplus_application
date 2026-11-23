
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <cstdint>
#include <pthread.h>

class TIME_MANAGE_CV
{
public:
    TIME_MANAGE_CV() = default;
    TIME_MANAGE_CV(const TIME_MANAGE_CV& timer) = delete;
    TIME_MANAGE_CV& operator=(const TIME_MANAGE_CV& timer) = delete;

    //析构时 join 工作线程, 线程退出后才允许对象销毁
    ~TIME_MANAGE_CV() {
        stop();
    }

    /// \brief start
    /// 启动线程运行，实现定时器功能
    /// \param interval - 定时器执行的间隔时间
    /// \param task - 定时器执行的回调函数
    void start(int interval, std::function<void()> task)
    {
        if (is_running_.load())
            return;

        {
            std::lock_guard<std::mutex> lock{mutex_};
            millisecond_ = interval;
            timeout_handler_ = task;
        }

        //旧线程可能停在回调里, 先回收再启动
        if (worker_.joinable()) {
            worker_.join();
        }

        is_running_.store(true);
        worker_ = std::thread(&TIME_MANAGE_CV::run, this);
    }

    /// @brief stop
    /// 运行停止函数，结束循环执行并回收线程
    void stop() {
        if (!is_running_.exchange(false)) {
            return;
        }

        cond_.notify_all();

        if (worker_.joinable()) {
            worker_.join();
        }
    }

    void test() {
        std::atomic<int> index{0};

        start(100, [&](){
            std::cout<<"cv timer loop: "<<index++<<std::endl;
        });

        while(index.load() < 5) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        stop();
        std::cout<<"cv timer stopped, loops: "<<index.load()<<std::endl;

        index.store(0);
        start(100, [&](){
            std::cout<<"cv new timer loop: "<<index++<<std::endl;
        });

        while(index.load() < 5) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        stop();
        std::cout<<"cv new timer stopped, loops: "<<index.load()<<std::endl;
    }
    
private:
    /// \brief run
    /// 线程执行函数，该函数在独立线程中执行，用于实现超时循环
    void run() {
        std::unique_lock<std::mutex> lock{mutex_};

        while (is_running_.load()) {
            //谓词为真说明收到停止请求; 返回 false 表示等到超时(伪唤醒也会继续等)
            if (cond_.wait_for(lock, std::chrono::milliseconds(millisecond_),
                               [this] { return !is_running_.load(); })) {
                break;
            }

            //在锁外执行回调, 避免回调里再调用 start/stop 造成死锁
            std::function<void()> task = timeout_handler_;
            lock.unlock();
            if (task) {
                task();
            }
            lock.lock();
        }
        std::cout<<"thread finished!\n";
    }

private:
    /// \brief is_running_
    /// - 判断线程是否允许运行
    std::atomic<bool> is_running_ = false;

    /// \brief worker_
    /// - 工作线程, 由 stop()/析构负责回收
    std::thread worker_;

    /// \brief mutex_
    /// - mutex_ used for condition.
    std::mutex mutex_;

    /// \brief cond_
    /// - 线程通讯变量
    std::condition_variable cond_;

    /// \brief timeout_handler_
    /// - 超时执行函数
    std::function<void()> timeout_handler_;

    /// \brief millisecond_
    /// - ms wait for timeout.
    uint32_t millisecond_{1};
};

class TIME_MANAGE_POSIX
{
public:
    /// \brief constructor
    /// pthread 对象在构造时初始化, 保证析构时销毁的一定是已初始化的对象
    TIME_MANAGE_POSIX(){
        pthread_mutex_init(&m_mutex_, nullptr);

        pthread_condattr_t attr;
        pthread_condattr_init(&attr);

        //winpthreads 不支持 CLOCK_MONOTONIC 条件变量(EINVAL), 必须按实际生效的时钟算超时
        use_monotonic_ = (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) == 0);
        if (!use_monotonic_) {
            std::cout<<"condattr_setclock(MONOTONIC) unsupported, fallback to CLOCK_REALTIME\n";
        }

        pthread_cond_init(&m_cond_, &attr);
        pthread_condattr_destroy(&attr);
    }

    TIME_MANAGE_POSIX(const TIME_MANAGE_POSIX &time) = delete;
    TIME_MANAGE_POSIX& operator=(const TIME_MANAGE_POSIX &time) = delete;

    /// \brief destructor
    /// 先 join 工作线程, 再销毁条件变量和互斥量
    ~TIME_MANAGE_POSIX(){
        stop();

        pthread_cond_destroy(&m_cond_);
        pthread_mutex_destroy(&m_mutex_);
    }

    /// \brief start
    /// - This method is used to start TIME_MANAGE_POSIX thread
    /// \param interval - time for the thread run period
    /// \param task - task when the thread run.
    void start(int interval, std::function<void()> task)
    {
        //已在运行则直接返回(停止标志为 false 且线程仍持有)
        if (!stop_loop_.load() && worker_.joinable())
            return;

        if (worker_.joinable()) {
            worker_.join();
        }

        pthread_mutex_lock(&m_mutex_);
        millisecond_ = static_cast<uint64_t>(interval);
        timeout_handler_ = task;
        pthread_mutex_unlock(&m_mutex_);

        stop_loop_.store(false);
        worker_ = std::thread(&TIME_MANAGE_POSIX::run, this);
    }

    /// \brief stop
    /// - This method is used to stop one TIME_MANAGE_POSIX thread
    void stop()
    {
        if (stop_loop_.exchange(true))
            return;
        
        pthread_mutex_lock(&m_mutex_);
        pthread_cond_broadcast(&m_cond_);
        pthread_mutex_unlock(&m_mutex_);

        if (worker_.joinable()) {
            worker_.join();
        }
    }

    void test() {
        std::atomic<int> index{0};

        start(100, [&](){
            std::cout<<"posix timer loop: "<<index++<<std::endl;
        });

        while(index.load() < 5) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        stop();
        std::cout<<"posix timer stopped, loops: "<<index.load()<<std::endl;

        index.store(0);
        start(100, [&](){
            std::cout<<"posix new timer loop: "<<index++<<std::endl;
        });

        while(index.load() < 5) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        stop();
        std::cout<<"posix new timer stopped, loops: "<<index.load()<<std::endl;
    }

private:
    /// \brief run
    /// - This method is used to run the loop by thread
    void run()
    {
        struct timespec ts;
        uint64_t millisecond;
        clockid_t clock_id = use_monotonic_ ? CLOCK_MONOTONIC : CLOCK_REALTIME;

        //不能在 run() 里清零停止标志, 否则 stop() 的请求会丢失
        while (!stop_loop_.load())
        {
            clock_gettime(clock_id, &ts);
            millisecond = (static_cast<uint64_t>(ts.tv_sec) * 1000) + (static_cast<uint64_t>(ts.tv_nsec) / 1000000);
            millisecond += millisecond_;
            ts.tv_sec = static_cast<time_t>(millisecond / 1000);
            ts.tv_nsec = static_cast<long>((millisecond % 1000) * 1000 * 1000);

            pthread_mutex_lock(&m_mutex_);
            auto stauts = pthread_cond_timedwait(&m_cond_, &m_mutex_, &ts);
            std::function<void()> task = timeout_handler_;
            pthread_mutex_unlock(&m_mutex_);

            if (stauts == ETIMEDOUT) {
                if (task) {
                    task();
                }
            } else {
                break;
            }
        }
        std::cout<<"TIME_MANAGE_POSIX finished\n";
    }

private:
    /// \brief worker_
    /// - 工作线程, 由 stop()/析构负责回收
    std::thread worker_;

    /// \brief m_cond_
    /// - cond used to wait timeout.
    pthread_cond_t m_cond_;

    /// \brief m_mutex
    /// - mutex protect
    pthread_mutex_t m_mutex_;

    /// \brief millisecond_
    /// - ms wait for timeout.
    uint64_t millisecond_{1};

    /// \brief timeout_handler_
    /// - function run when timeout.
    std::function<void()> timeout_handler_;

    /// \brief stop_loop_
    /// - loop flag until stop.
    std::atomic<bool> stop_loop_{false};

    /// \brief use_monotonic_
    /// - 条件变量是否成功绑定到 CLOCK_MONOTONIC
    bool use_monotonic_{false};
};

class CONDITION_ANY
{
public:
    void test(void) {
        std::thread task(&CONDITION_ANY::run, this);

        std::unique_lock<std::mutex> lock(mutex_);
        if (cond_.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::seconds(5), [&]{ return ready_; })) {
            std::cout << "Singal action." << std::endl;
        } else {
            std::cout << "Timeout action." << std::endl;
        }
        lock.unlock();
        task.join();
    }

private:
    void run() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        {
            std::lock_guard<std::mutex> lock(mutex_);
            ready_ = true;
        }
        cond_.notify_one(); 
    }

private:
    std::mutex mutex_;
    std::condition_variable_any cond_;
    bool ready_{false};
};

int main()
{
    TIME_MANAGE_CV mcv;
    mcv.test();

    TIME_MANAGE_POSIX mpo;
    mpo.test();

    CONDITION_ANY ca1;
    ca1.test();
    return 0;
}
