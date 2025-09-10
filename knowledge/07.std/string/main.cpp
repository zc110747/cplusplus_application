/*

*/
#include <string>
#include <iostream>
#include <algorithm>
#include <unistd.h>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    std::string usr_str{"ustr,"};
    std::string startstring = std::string("Let's go!");

    //append, operator+=, operator[]
    usr_str.append("Let's go!");
    cout<<"append:"<<usr_str<<endl;
    usr_str.append(startstring, 6, 3);
    cout<<"append:"<<usr_str<<endl;
    usr_str += "everyone!";
    cout<<"operator +:"<<usr_str<<endl;
    cout<<"operator []:"<<usr_str[0]<<endl;
    usr_str = usr_str + " test";
    cout<<"operator: "<<usr_str<<endl;
    auto equal0 = (usr_str != "test");
    auto equal1 = (usr_str == "test");
    cout<<"operator !=/== "<<equal0<<" | "<<equal1<<endl;

    //assign
    std::string copystring;
    copystring.assign(usr_str);
    cout<<"assign:"<<copystring<<endl;

    //at, back, front
    char& atval = copystring.at(1);
    char& backval = copystring.back();
    std::string::reference frontval = copystring.front();
    cout<<"at:"<<atval<<endl;
    cout<<"back:"<<backval<<endl;
    cout<<"front:"<<frontval<<endl;

    //begin, end, cbegin, cend, rbegin, rend, crbegin, crend
    std::string::iterator iterbegin = copystring.begin();
    std::string::iterator iterend = copystring.end();
    std::string::const_iterator itercbegin = copystring.cbegin();
    std::string::const_iterator itercend = copystring.cend();
    std::string::reverse_iterator iterrbegin = copystring.rbegin();
    std::string::reverse_iterator iterrend = copystring.rend();
    std::string::const_reverse_iterator itercrbegin = copystring.crbegin();
    std::string::const_reverse_iterator itercrend = copystring.crbegin();
    cout<<"begin:"<<*iterbegin<<" "<<"end:"<<*(iterend-1)<<" ";
    cout<<"cbegin:"<<*itercbegin<<" "<<"cend:"<<*(itercend-1)<<" ";
    cout<<"rbegin:"<<*iterrbegin<<" "<<"rend:"<<*(iterrend-1)<<" ";
    cout<<"crbegin:"<<*itercrbegin<<" "<<"crend:"<<*(itercrend-1)<<" "<<endl;
    
    //resize, shrink_to_fit
    copystring.resize(5);
    cout<<"resize:"<<copystring<<endl;
    copystring.shrink_to_fit();
    cout<<"shrink_to_fit:"<<copystring.capacity()<<endl;
    copystring.reserve(10);
    cout<<"reserve:"<<copystring.capacity()<<endl;

    //capacity, size, clear, max_size
    cout<<"capacity:"<<copystring.capacity()<<endl;
    cout<<"length:"<<copystring.length()<<endl;
    cout<<"size:"<<copystring.size()<<" ";
    copystring.clear();
    cout<<"size:"<<copystring.size()<<endl;
    cout<<"max_size:"<<copystring.max_size()<<endl;
    cout<<"empty:"<<copystring.empty()<<endl;

    //compare
    if(startstring.compare("Let's go!") == 0)
    {
        cout<<"comapre equal!"<<endl;
    }
    else
    {
        cout<<"compare no equal!"<<endl;
    }

    //copy
    char buffer[10] = {0};
    startstring.copy(buffer, 6, 0);
    cout<<"copy:"<<buffer<<endl;
    
    //c_str
    const char *pcstr;
    pcstr = startstring.c_str();
    cout<<"c_str:"<<pcstr<<endl;

    //data
    const char *pcdata;
    pcdata = startstring.data();
    cout<<"data:"<<pcdata<<endl;

    //for-auto
    cout<<"for auto:";
    for(auto val:startstring)
    {
        cout<<val<<" ";
    }
    cout<<endl;

    //erase
    startstring.erase(2, 1);
    cout<<"erase:"<<startstring<<endl;

    //pop_back, push_back
    startstring.pop_back();
    cout<<"pop_back:"<<startstring<<endl;
    startstring.push_back('c');
    cout<<"push_back:"<<startstring<<endl;

    //insert
    startstring.insert(0, "hello~ ");
    cout<<"insert:"<<startstring<<endl;

    //swap
    std::string swapstring;
    swapstring.swap(startstring);
    cout<<"swap:"<<swapstring<<endl;

    //find, rfind, find_first_of
    std::string::size_type position;
    position = swapstring.find("he");
    if(position != swapstring.npos)
    {
        cout<<"find position"<<position<<endl;
    }
    cout<<"find_first_of:"<<swapstring.find_first_of("he")<<endl; 
    cout<<"find_first_not_of:"<<swapstring.find_first_not_of("he")<<endl; 
    cout<<"find_last_of:"<<swapstring.find_last_of("he")<<endl; 
    cout<<"find_last_not_of:"<<swapstring.find_last_not_of("he")<<endl;   
    cout<<"rfind:"<<swapstring.rfind("he")<<endl;

    //replace
    swapstring.replace(swapstring.begin(), swapstring.begin()+2, "she");
    cout<<"replace:"<<swapstring<<endl;

    //substr
    std::string substring = swapstring.substr(1, 3);
    cout<<"substr:"<<substring<<endl;

    return 0;
} 
