/************************************************************************************
 * std::atomic
 * https://www.cplusplus.com/reference/bitset/bitset/
 * A bitset stores bits (elements with only two possible values: 0 or 1, true or false, ...).
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studybitset
 * 3.More reference shows in function show_bitset_help or command "studybitset -h"
***********************************************************************************/
#include <atomic>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>    // std::chrono::seconds

using std::cout;
using std::endl;

std::atomic<int> value = 0;

void task0(void)
{
    int index = 0;
    for(index = 0; index<5; index++)
    {
        value++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void task1(void)
{
    int index = 0;
    for(index = 0; index<5; index++)
    {
        value++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
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

    std::thread thread_0(task0);
    std::thread thread_1(task1);

    thread_0.join();
    thread_1.join();

    std::cout<<value<<std::endl;
    std::cout<<value.load()<<std::endl;
    
    return 0;
} 
