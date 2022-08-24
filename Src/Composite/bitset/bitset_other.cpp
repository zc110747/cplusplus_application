/************************************************************************************
 * std::bitset
 * https://www.cplusplus.com/reference/bitset/bitset/
 * A bitset stores bits (elements with only two possible values: 0 or 1, true or false, ...).
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studybitset
 * 3.More reference shows in function show_bitset_help or command "studybitset -h"
***********************************************************************************/
#include <bitset>
#include <iostream>
#include <algorithm>
#include <string>
#include <unistd.h>

using std::cout;
using std::endl;

void bitset_study_handle(void)
{
    std::bitset<8> userbitset(std::string("11111111"));
    std::bitset<4> lowbitset(std::string("0110"));

    //count, any, none, all
    cout<<"count:"<<userbitset.count()<<endl;
    cout<<std::boolalpha;
    cout<<"all:"<<userbitset.all()<<endl;
    cout<<"any:"<<userbitset.any()<<endl;
    cout<<"none:"<<userbitset.none()<<endl;

    //to_string
    cout<<"count:"<<lowbitset.count()<<endl;
    cout<<std::boolalpha;
    cout<<"all:"<<lowbitset.all()<<endl;
    cout<<"any:"<<lowbitset.any()<<endl;
    cout<<"none:"<<lowbitset.none()<<endl;
    cout<<"lowbitset:"<<lowbitset.to_string()<<endl;

    //flip
    cout<<"flip:"<<lowbitset.flip(2)<<endl;
    cout<<"flip:"<<lowbitset.flip()<<endl;

    //operator[]
    cout<<"operator []"<<lowbitset[1]<<endl;
    
    //size
    cout<<"size:"<<lowbitset.size()<<endl;
    for(int index=0; index<lowbitset.size(); index++)
    {
        cout<<"index:"<<index<<" value:"<<lowbitset.test(index)<<" ";
    }
    cout<<endl;
    
    //reset
    lowbitset.reset();
    cout<<"reset:"<<lowbitset<<endl;

    //set
    lowbitset.set(2,true);
    cout<<"set:"<<lowbitset<<endl;

    //to_ullong, to_ulong
    cout<<"ullong:"<<lowbitset.to_ullong()<<endl;
    cout<<"ulong:"<<lowbitset.to_ulong()<<endl;
}


void show_bitset_help(void)
{
    std::string helpstring;

    helpstring.append("all          Returns whether all of the bits in the bitset are set (to one).\n");
    helpstring.append("any          Returns whether any of the bits is set.\n");
    helpstring.append("count        Returns the number of bits in the bitset that are set.\n");
    helpstring.append("flip         Flips bit values converting zeros into ones and ones into zeros.\n");
    helpstring.append("none         Returns whether none of the bits is set.\n");
    helpstring.append("operator[]   The function returns the value (or a reference) to the bit at position pos.\n");
    helpstring.append("reset        Resets bits to zero.\n");
    helpstring.append("set          Sets bits.\n");
    helpstring.append("size         Returns the number of bits in the bitset.\n");
    helpstring.append("test         Returns whether the bit at position pos is set.\n");
    helpstring.append("to_string    Constructs a basic_string object that represents the bits in the bitset as a succession of zeros and/or ones.\n");
    helpstring.append("to_ullong    Returns an unsigned long long with the integer value that has the same bits set as the bitset.\n");
    helpstring.append("to_ulong      Returns an unsigned long with the integer value that has the same bits set as the bitset.\n");

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
        show_bitset_help();
    else
        bitset_study_handle();

    return 0;
} 
