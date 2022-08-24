/************************************************************************************
 * std::string
 * https://www.cplusplus.com/reference/string/string/
 * Strings are objects that represent sequences of characters
 * 1.Need compile with command -std=c++11
 * 2.Generate excutable file studystring
 * 3.More reference shows in function show_string_help or command "studystring -h"
***********************************************************************************/
#include <string>
#include <iostream>
#include <algorithm>
#include <unistd.h>

using std::cout;
using std::endl;

void string_study_handle(void)
{
    std::string userstring{"usr string start."};
    std::string startstring = std::string("Let's go!");

    //append, operator+=, operator[]
    userstring.append("Let's go!");
    cout<<"append:"<<userstring<<endl;
    userstring.append(startstring, 6, 3);
    cout<<"append:"<<userstring<<endl;
    userstring += "everyone!";
    cout<<"operator +:"<<userstring<<endl;
    cout<<"operator []:"<<userstring[0]<<endl;

    //assign
    std::string copystring;
    copystring.assign(userstring);
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

    //get_allocator
    char *p;
    p = startstring.get_allocator().allocate(5);
    p[0] = 'h';
    p[1] = 'e';
    p[2] = '\0';
    cout<<"get_allocator:"<<p<<endl;
    startstring.get_allocator().destroy(p);

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
}


void show_string_help(void)
{
    std::string helpstring;

    helpstring.append("append               Extends the string by appending additional characters at the end of its current value.\n");
    helpstring.append("assign               Assigns a new value to the string, replacing its current contents.\n");
    helpstring.append("at                   Returns a reference to the character at position pos in the string.\n");
    helpstring.append("back                 Returns a reference to the last character of the string.\n");
    helpstring.append("begin                Returns an iterator pointing to the first character of the string.\n");
    helpstring.append("capacity             Returns the size of the storage space currently allocated for the string, expressed in terms of bytes.\n");
    helpstring.append("cbegin               Returns a const_iterator pointing to the first character of the string.\n");
    helpstring.append("cend                 Returns a const_iterator pointing to the past-the-end character of the string.\n");
    helpstring.append("clear                Erases the contents of the string, which becomes an empty string (with a length of 0 characters).\n");
    helpstring.append("compare              Compares the value of the string object (or a substring) to the sequence of characters specified by its arguments.\n");
    helpstring.append("copy                 Copies a substring of the current value of the string object into the array pointed by s.\n");
    helpstring.append("crbegin              Returns a const_reverse_iterator pointing to the last character of the string (i.e., its reverse beginning).\n");
    helpstring.append("crend                Returns a const_reverse_iterator pointing to the theoretical character preceding the first character of the string.\n");
    helpstring.append("c_str                Returns a pointer to an array that contains a null-terminated sequence of characters (i.e., a C-string) representing the current value of the string object.\n");
    helpstring.append("data                 Returns a pointer to an array that contains the same sequence of characters as the characters that make up the value of the string object.\n");
    helpstring.append("empty                Returns whether the string is empty (i.e. whether its length is 0).\n");
    helpstring.append("end                  Returns an iterator pointing to the past-the-end character of the string.\n");
    helpstring.append("erase                Erases part of the string, reducing its length.\n");
    helpstring.append("find                 Searches the string for the first occurrence of the sequence specified by its arguments.\n");
    helpstring.append("find_first_not_of    Searches the string for the first character that does not match any of the characters specified in its arguments.\n");
    helpstring.append("find_first_of        Searches the string for the first character that matches any of the characters specified in its arguments.\n");
    helpstring.append("find_last_not_of     Searches the string for the last character that does not match any of the characters specified in its arguments.\n");
    helpstring.append("find_last_of         Searches the string for the last character that matches any of the characters specified in its arguments.\n");
    helpstring.append("front                Returns a reference to the first character of the string.\n");
    helpstring.append("get_alloctor         Returns a copy of the allocator object associated with the string.\n");
    helpstring.append("insert               Inserts additional characters into the string right before the character indicated by pos.\n");
    helpstring.append("length               Returns the length of the string, in terms of bytes.\n");
    helpstring.append("max_size             Returns the maximum length the string can reach.\n");
    helpstring.append("operator+=           Extends the string by appending additional characters at the end of its current value.\n");
    helpstring.append("operator=            Assigns a new value to the string, replacing its current contents.\n");
    helpstring.append("operator[]           Returns a reference to the character at position pos in the string.\n");
    helpstring.append("pop_back             Erases the last character of the string, effectively reducing its length by one.\n");
    helpstring.append("push_back            Appends character c to the end of the string, increasing its length by one.\n");
    helpstring.append("rbegin               Returns a reverse iterator pointing to the last character of the string (i.e., its reverse beginning).\n");
    helpstring.append("rend                 Returns a reverse iterator pointing to the theoretical element preceding the first character of the string.\n");
    helpstring.append("replace              Replaces the portion of the string that begins at character pos and spans len characters by new contents.\n");
    helpstring.append("reserve              Requests that the string capacity be adapted to a planned change in size to a length of up to n characters.\n");
    helpstring.append("resize               Resizes the string to a length of n characters.\n");
    helpstring.append("rfind                Searches the string for the last occurrence of the sequence specified by its arguments.\n");
    helpstring.append("shrink_to_fit        Requests the string to reduce its capacity to fit its size.\n");
    helpstring.append("size                 Returns the length of the string, in terms of bytes.\n");
    helpstring.append("substr               Returns a newly constructed string object with its value initialized to a copy of a substring of this object.\n");
    helpstring.append("swap                 Exchanges the content of the container by the content of str, which is another string object. Lengths may differ.\n");

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
        show_string_help();
    else
        string_study_handle();

    return 0;
} 
