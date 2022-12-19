_Pragma("once")

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
    Semaphore(int count = 0):wakeups(count) {}
        virtual ~Semaphore() {}
    
    void signal()
    {
        std::lock_guard<std::mutex> lock(mt);
        ++wakeups;
        cv.notify_one();
    }
    
    bool wait(uint32_t timeout=0xffffffff)
    {
        bool ret = false;
        std::unique_lock<std::mutex> lock(mt);
        ret = cv.wait_for(lock, 
            std::chrono::milliseconds(timeout), 
            [this]()->bool{return wakeups > 0;});
        if(ret && wakeups>0)
            --wakeups;
        return ret;
    }

private:
    std::mutex mt;
    std::condition_variable cv;
    int wakeups;
};