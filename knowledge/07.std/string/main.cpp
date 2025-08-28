/*
std::string 是 C++ 标准库中的一个类，用于表示和操作字符串。
它提供了丰富的成员函数和操作符，使得字符串的处理变得更加方便和高效

构造函数：
std::string()：默认构造函数，创建一个空字符串。
std::string(const char* s)：使用 C 风格的字符串 s 构造字符串。
std::string(const std::string& str)：使用另一个字符串 str 构造字符串。

成员函数：
append：向字符串末尾添加另一个字符串或字符。
assign：将字符串设置为另一个字符串或字符。
at：返回指定位置的字符，并进行边界检查。
back：返回字符串的最后一个字符。
begin：返回指向字符串开头的迭代器。
c_str：返回指向字符串内部 C 风格字符串的指针。
capacity：返回字符串的容量。
cbegin：返回指向字符串开头的常量迭代器。
cend：返回指向字符串末尾的常量迭代器。
clear：清空字符串。
compare：比较两个字符串的大小。
copy：将字符串的一部分复制到另一个字符数组中。
crbegin：返回指向字符串开头的常量反向迭代器。
crend：返回指向字符串末尾的常量反向迭代器。
data：返回指向字符串内部字符数组的指针。
empty：检查字符串是否为空。
end：返回指向字符串末尾的迭代器。
ends_with(c++20)：检查字符串是否以指定的后缀结尾。
erase：删除字符串中的一部分。
find：查找子字符串的位置。
find_first_of：查找第一个匹配的字符或子字符串的位置。
find_last_of：查找最后一个匹配的字符或子字符串的位置。
find_first_not_of：查找第一个不匹配的字符或子字符串的位置。
find_last_not_of：查找最后一个不匹配的字符或子字符串的位置。
front：返回字符串的第一个字符。
get_allocator：返回字符串的分配器。
insert：在指定位置插入另一个字符串或字符。
length：返回字符串的长度。
max_size：返回字符串的最大可能长度。
pop_back：删除字符串的最后一个字符。
push_back：在字符串末尾添加一个字符。
rbegin：返回指向字符串开头的反向迭代器。
rend：返回指向字符串末尾的反向迭代器。
replace：替换字符串中的一部分。
reserve：为字符串预留存储空间。
resize：调整字符串的大小。
rfind：反向查找子字符串的位置。
shrink_to_fit：将字符串的容量缩小到与长度相同。
size：返回字符串的长度。
starts_with(c++20)：检查字符串是否以指定的前缀开头。
substr：返回字符串的子串。
swap：交换两个字符串的内容。
operator+=: 字符串拼接操作符。
operator[]：访问字符串中的字符。
operator!=：不等于操作符。
operator==：等于操作符。
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
