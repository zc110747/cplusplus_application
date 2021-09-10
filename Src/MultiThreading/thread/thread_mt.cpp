/************************************************************************************
 * std::thread
 * https://www.cplusplus.com/reference/thread/thread/
 * A thread of execution is a sequence of instructions that can be executed concurrently 
 * with other such sequences in multithreading environments, while sharing a same 
 * address space
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studythread_mt
 * 3.More reference shows in function show_map_help or command "studythread_mt -h"
***********************************************************************************/
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>    // std::chrono::seconds
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

static volatile int val;

void loop_task(string str, int n)
{
    for(int i=0; i<n; i++)
    {
        cout<<"times "<<i<<" : "<<str<<endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    //get_id
    cout<<"loop_task id: 0x"<<std::hex<<std::this_thread::get_id()<<endl;
} 

void basic_task(void)
{
    cout<<"basic_task id: 0x"<<std::hex<<std::this_thread::get_id()<<endl;
} 

int thread_study_handle(void)
{
    try
    {
        std::thread userloopthread(loop_task, "hello world!", 3);
        std::thread userbasethread(basic_task);

        //native_handle, get_id
        auto nh = userbasethread.native_handle();
        cout<<"native_handle: 0x"<<std::hex<<nh<<endl;
        cout<<"get_id: 0x"<<userbasethread.get_id()<<endl;

        //joinable, join
        cout<<"joinable:"<<userbasethread.joinable()<<endl;

        //join
        userloopthread.join();
        userbasethread.join();

        //detach
        std::thread userdetachthread(loop_task, "detech task!", 1);
        userdetachthread.detach();

        sleep(2);
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

    helpstring.append("detach          Detaches the thread represented by the object from the calling thread, allowing them to execute independently from each other.\n");
    helpstring.append("get_id          Returns the thread id.\n");
    helpstring.append("join            This synchronizes the moment this function returns with the completion of all the operations in the thread.\n");
    helpstring.append("joinable        Returns whether the thread object is joinable.\n");
    helpstring.append("native_handle   This member function is only present in class thread if the library implementation supports it.\n");
    helpstring.append("operator=       If the object is currently not joinable, it acquires the thread of execution represented by rhs (if any).\n");
    helpstring.append("swap            Swaps the state of the object with that of x.\n");
    
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
