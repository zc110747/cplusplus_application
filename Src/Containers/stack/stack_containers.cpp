/************************************************************************************
 * std::stack
 * reference:http://www.cplusplus.com/reference/stack/stack/
 * ueues are a type of container adaptor, specifically designed to operate in a FIFO 
 * context (first-in first-out), where elements are inserted into one end of the 
 * container and extracted from the other
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystack
 * 3.More reference shows in function show_stack_help or command "studystack -h"
***********************************************************************************/
#include <stack>
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

void stack_study_handle(void)
{
    //stack
    std::stack<string> userstack;

    //emplace, push, size, empty
    userstack.push("first");
    userstack.push("second");
    userstack.push("third");
    userstack.emplace("fourth");
    userstack.push("fifth");
    cout<<"size:"<<userstack.size()<<endl;
    cout<<"empty:"<<userstack.empty()<<endl;

    //emplace
    userstack.emplace("sixth");

    //top
    string &strfront = userstack.top();
    cout<<"top: "<<strfront<<endl;

    //swap
    std::stack<string> swapstack;
    swapstack.swap(userstack);

    //pop
    cout<<"swapstack:";
    int size = swapstack.size();
    for(int index=0; index<size; index++)
    {
        cout<<swapstack.top()<<" ";
        swapstack.pop();
    }
    cout<<endl;
}


void show_stack_help(void)
{
    string helpstring;

    helpstring.append("emplace          Adds a new element at the top of the stack.\n");
    helpstring.append("empty            Returns whehter the stack is empty.\n");
    helpstring.append("pop              Removes the element on top of the stack.\n");
    helpstring.append("push             Inserts a new element at the top of the stack.\n");
    helpstring.append("size             Returns number of elements in the stacks.\n");    
    helpstring.append("swap             Exchanges the contents of the container adaptor (*this) by those of x.\n");
    helpstring.append("top              Returns a constant reference to the top element in the stack.\n");

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
        show_stack_help();
    else
        stack_study_handle();

    return 0;
} 