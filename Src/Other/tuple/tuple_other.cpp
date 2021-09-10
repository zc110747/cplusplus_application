/************************************************************************************
 * std::tuple
 * https://www.cplusplus.com/reference/tuple/tuple/
 * A tuple is an object capable to hold a collection of elements. Each element can 
 * be of a different type.
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studytuple
 * 3.More reference shows in function show_tuple_help or command "studytuple -h"
***********************************************************************************/
#include <tuple>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>

using std::cout;
using std::endl;

void tuple_study_handle(void)
{
    //forward_as_tuple
    auto usertuple = std::forward_as_tuple("hello", 24, 'a');
    cout<<"forward_as_tuple;"<<std::get<0>(usertuple)<<" "<<std::get<1>(usertuple)<<" ";
    cout<<std::get<2>(usertuple)<<endl;

    //make_tuple
    std::tuple<std::string, char, int>  maketuple = std::make_tuple("hello", 'm', 24);
    cout<<"make tuple:"<<std::get<0>(maketuple)<<" "<<std::get<1>(maketuple)<<" ";
    cout<<std::get<2>(maketuple)<<endl;

    //tie
    std::string tiestr;
    char tiechar;
    int tieint;
    std::tie(tiestr, tiechar, tieint) = maketuple;
    cout<<"tie:"<<tiestr<<" "<<tiechar<<" "<<tieint<<endl;

    //tuple_cat
    std::pair<int,char> mypair (10,'a');
    auto cattuple = std::tuple_cat(maketuple, std::tuple<int, char>(mypair));
    cout<<"tuple_cat:"<<std::get<0>(cattuple)<<" "<<std::get<1>(cattuple)<<" ";
    cout<<std::get<2>(cattuple)<<" "<<std::get<3>(cattuple)<<" ";
    cout<<std::get<4>(cattuple)<<endl;
}


void show_tuple_help(void)
{
    std::string helpstring;

    helpstring.append("forward_as_tuple     Constructs a tuple object with rvalue references to the elements in args suitable to be forwarded as argument to a function.\n");
    helpstring.append("get                  Returns a reference to the Ith element of tuple tpl.\n");
    helpstring.append("make_tuple           Constructs an object of the appropriate tuple type to contain the elements specified in args.\n");
    helpstring.append("tie                  Constructs a tuple object whose elements are references to the arguments in args, in the same order.\n");
    helpstring.append("tuple_cat            Constructs an object of the appropriate tuple type to contain a concatenation of the elements of all the tuples in tpls, in the same order.\n");

    cout<<helpstring<<endl;
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
        show_tuple_help();
    else
        tuple_study_handle();

    return 0;
} 
