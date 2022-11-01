_Pragma("once")

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
    Semaphore(int count=0):count(count) {}
    virtual ~Semaphore(){}

    void signal()
    {
        std::lock_guard<std::mutex> lock{mt};
        ++count;
        if(count <= 0)
        {
            ++wakeups;
            cond.notify_one();
        }
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock{mt};
        --count;
        if(count<0)
        {
            cond.wait(lock, [this](){
                return wakeups > 0;
            });
            --wakeups;
        }
    }
private:
    int count;
    int wakeups{0};
    std::mutex mt;
    std::condition_variable cond;
};

void semaphore_process(void);