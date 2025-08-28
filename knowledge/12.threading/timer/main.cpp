
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <unistd.h>

class TIME_MANAGE_CV
{
public:
    TIME_MANAGE_CV() = default;
    TIME_MANAGE_CV(const TIME_MANAGE_CV& timer) = delete;

    ~TIME_MANAGE_CV() {
        stop();
    }

    /// \brief start
    /// 启动线程运行，实现定时器功能
    /// \param interval - 定时器执行的间隔时间
    /// \param task - 定时器执行的回调函数
    void start(int interval, std::function<void()> task)
    {
        if (is_running_)
            return;

        millisecond_ = interval;
        timeout_handler_ = task;
        is_running_ = true;

        //start thread
        std::thread(std::bind(&TIME_MANAGE_CV::run, this)).detach();
    }

    /// @brief stop
    /// 运行停止函数，结束循环执行
    void stop() {
        if(!is_running_) {
            return;
        }

        is_running_ = false;
        cond_.notify_all();
    }

    void test() {
        int index = 0;

        start(1000, [&](){
            std::cout<<"cv timer loop: "<<index++<<std::endl;
        });

        while(1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (index == 5) {
                stop();
                index++;
            } else if (index > 5) {                  
                index = 0;
                start(1000, [&](){
                    std::cout<<"cv new timer loop: "<<index++<<std::endl;
                });
            }
        }
    }
    
private:
    /// \brief run
    /// 线程执行函数，该函数在独立线程中执行，用于实现超时循环
    void run() {
        while(is_running_) {
            std::unique_lock<std::mutex> lock{mutex_};
            auto stauts = cond_.wait_for(lock, std::chrono::milliseconds(millisecond_));
            if (stauts == std::cv_status::timeout) {
                if (timeout_handler_) {
                    timeout_handler_();
                }
            } else {
                break;
            }
        }
        std::cout<<"thread finished!\n";
    }

private:
    /// \brief is_running_
    /// - 判断线程释放允许
    std::atomic<bool> is_running_ = false;

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
    TIME_MANAGE_POSIX() = default;
    TIME_MANAGE_POSIX(const TIME_MANAGE_POSIX &time) = delete;

    /// \brief destructor
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
        pthread_condattr_t attr;

        millisecond_ = interval;
        timeout_handler_ = task;

        pthread_mutex_init(&m_mutex_, NULL);
        pthread_condattr_init(&attr);
        pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
        pthread_cond_init(&m_cond_, &attr); 

        //start thread
        std::thread(std::bind(&TIME_MANAGE_POSIX::run, this)).detach();
    }

    /// \brief stop
    /// - This method is used to stop one TIME_MANAGE_POSIX thread
    void stop()
    {
        if (stop_loop_)
            return;
        
        stop_loop_ = true;
        pthread_mutex_lock(&m_mutex_);
        pthread_cond_broadcast(&m_cond_);
        pthread_mutex_unlock(&m_mutex_);
    }

    void test() {
        int index = 0;

        start(1000, [&](){
            std::cout<<"posix timer loop: "<<index++<<std::endl;
        });

        while(1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (index == 5) {
                stop();
                index++;
            } else if (index > 5) {                  
                index = 0;
                start(1000, [&](){
                    std::cout<<"posix new timer loop: "<<index++<<std::endl;
                });
            }
        }
    }

private:
    /// \brief run
    /// - This method is used to run the koop by thread
    void run()
    {
        struct timespec ts;
        uint64_t millisecond;

        stop_loop_ = false;

        while (!stop_loop_)
        {
            clock_gettime(CLOCK_MONOTONIC, &ts);
            millisecond = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000) + millisecond_;
            ts.tv_sec = millisecond / 1000;
            ts.tv_nsec = (millisecond - ((millisecond / 1000) * 1000)) * 1000 * 1000;

            pthread_mutex_lock(&m_mutex_);
            auto stauts = pthread_cond_timedwait(&m_cond_, &m_mutex_, &ts);
            pthread_mutex_unlock(&m_mutex_);

            if (stauts == ETIMEDOUT)
            {
                if (timeout_handler_)
                {
                    timeout_handler_();
                }
            }
            else
            {
                std::cout<<"TIME_MANAGE_POSIX finished\n";
            }
        }
    }

private:
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
};

class CONDITION_ANY
{
public:
    void test(void) {
        std::thread task(std::bind(&CONDITION_ANY::run, this));

        std::unique_lock<std::mutex> lock(mutex_);
        if (cond_.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::seconds(5), [&]{ return ready_; })) {
            std::cout << "Singal action." << std::endl;
        } else {
            std::cout << "Timeout action." << std::endl;
        }
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

int main(int argc, char* argv[])
{
    // TIME_MANAGE_CV mcv;
    // mcv.test();

    // TIME_MANAGE_POSIX mpo;
    // mpo.test();

    CONDITION_ANY ca1;
    ca1.test();
    return 0;
}

