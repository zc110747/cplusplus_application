/************************************************************************************
 * std::mutex
 * reference:http://www.cplusplus.com/reference/mutex/mutex/
 * Allow mutual exclusion (mutex) of concurrent execution of critical sections of code, 
 * allowing to explicitly avoid data races.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlmap
 * 3.More reference shows in function show_map_help or command "studymap -h"
***********************************************************************************/
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>    // std::chrono::seconds
#include <mutex>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;
using std::mutex;

static volatile int val;
mutex usrmutex;

void loop_task(int n)
{
    auto id = std::this_thread::get_id();

    //lock, unlock
    usrmutex.lock();
    for(int i=0; i<n; i++)
    {
        cout<<"lock id is 0x"<<std::hex<<id<<endl;
    }
    usrmutex.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));  

    //try_lock, native_handle
    if(usrmutex.try_lock())
    {
        cout<<"try_lock id is 0x:"<<std::hex<<id<<endl;
        cout<<"native_handle:"<<usrmutex.native_handle()<<endl;
        usrmutex.unlock();
    }
    else
    {
        usrmutex.lock();
        cout<<"try_lock failed!, id is 0x"<<std::hex<<id<<endl;
        usrmutex.unlock();
    }
}

int thread_study_handle(void)
{
    try
    {
        std::thread userthread1(loop_task, 3);
        std::thread userthread2(loop_task, 2);

        userthread1.join();
        userthread2.join();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}

void show_thread_help(void)
{
    string helpstring;

    helpstring.append("lock             The calling thread locks the mutex, blocking if necessary.\n");
    helpstring.append("trylock          Attempts to lock the mutex, without blocking.\n");
    helpstring.append("unlock           Unlocks the mutex, releasing ownership over it.\n");
    helpstring.append("native_handle    This member function is only present in class mutex if the library implementation supports it.\n");
    
    cout<<helpstring;
}

int main(int argc, char* argv[])
{
    int opt, mode(0);

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
        {
        case 'h':
            mode = 1;
            break;
        
        default:

            break;
        }
    }

    if(mode == 1)
        show_thread_help();
    else
        thread_study_handle();

    return 0;
} 
