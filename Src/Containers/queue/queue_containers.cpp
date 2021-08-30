/************************************************************************************
 * std::queue
 * reference:http://www.cplusplus.com/reference/queue/queue/
 * ueues are a type of container adaptor, specifically designed to operate in a FIFO 
 * context (first-in first-out), where elements are inserted into one end of the 
 * container and extracted from the other
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystlqueue
 * 3.More reference shows in function show_queue_help or command "studyqueue -h"
***********************************************************************************/
#include <queue>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl;

typedef enum
{
    SHOW_MODE_ENUM_LAMBDA = 0,
    SHOW_MODE_ENUM_TYPE_AUTO,
    SHOW_MODE_ENUM_TYPE_ITERATOR,
    SHOW_MODE_ENUM_TYPE_OPERATOR
}SHOW_MODE_ENUM;

void queue_study_handle(void)
{
    //queue
    std::queue<string> userqueue;

    //emplace, push, size, empty
    userqueue.push("first");
    userqueue.push("second");
    userqueue.push("third");
    userqueue.emplace("fourth");
    userqueue.push("fifth");
    cout<<"size:"<<userqueue.size()<<endl;
    cout<<"empty:"<<userqueue.empty()<<endl;

    //front
    string &strfront = userqueue.front();
    cout<<"front: "<<strfront<<endl;

    //back
    string &strback = userqueue.back();
    cout<<"back: "<<strback<<endl;

    //swap
    std::queue<string> swapqueue;
    swapqueue.swap(userqueue);

    //pop
    cout<<"swapqueue:";
    int size = swapqueue.size();
    for(int index=0; index<size; index++)
    {
        cout<<swapqueue.front()<<" ";
        swapqueue.pop();
    }
    cout<<endl;

    //priority
    std::priority_queue<int> userpriority_queue;

    //emplace, push, size, empty
    userpriority_queue.push(7);
    userpriority_queue.push(5);
    userpriority_queue.push(6);
    userpriority_queue.emplace(1);
    userpriority_queue.push(3);
    cout<<"pr_size:"<<userpriority_queue.size()<<endl;
    cout<<"pr_empty:"<<userpriority_queue.empty()<<endl;

    //swap
    std::priority_queue<int> swappriority_queue;
    swappriority_queue.swap(userpriority_queue);

    //pop
    cout<<"swappriority_queue:";
    int pr_size = swappriority_queue.size();
    for(int index=0; index<pr_size; index++)
    {
        cout<<swappriority_queue.top()<<" ";
        swappriority_queue.pop();
    }
    cout<<endl;
}


void show_queue_help(void)
{
    string helpstring;

    helpstring.append("back             Returns a reference to the last element(queue only).\n");
    helpstring.append("emplace          Insert a new element at the end of the queue.\n");
    helpstring.append("empty            Returns whehter the queue is empty.\n");
    helpstring.append("front            Returns a refrence to the first element in the queue(queue only).\n");
    helpstring.append("pop              Remvoes the next element in the queue.\n");
    helpstring.append("push             Adds a new element at the end of the queue.\n");
    helpstring.append("size             Returns number of elements in the queues.\n");    
    helpstring.append("swap             Exchanges the contents of the container adaptor (*this) by those of x.\n");
    helpstring.append("top              Returns a constant reference to the top element in the priority_queue(priority_queue only).\n");

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
        show_queue_help();
    else
        queue_study_handle();

    return 0;
} 