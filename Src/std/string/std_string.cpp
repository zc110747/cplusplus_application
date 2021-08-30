/**********************************************************
 * 说明: string 用于处理字符串的C++标准库
 * append           用于字符串的添加
 * size             当前字符串长度
 * length           等同于size，当前字符串长度
 * max_size         当前字符串的最大长度
 * capacity         字符串当前的容量
 * c_str, data      获取内部的字符串起始指针,只读
 * begin, end       返回内部迭代器的首尾指针
 * empty            判断字符串是否为空
 * erase            删除指定长度，或者迭代器范围内的数据
 * clear            清除字符串内部数据，只影响size，不影响capacity
 * resize           重新分配字符串空间，不满足的空间用指定字符补足
 * at               返回指定地址的字符
 * front, back      返回首/末尾的字符串中字符
 * push_back        在字符串末尾追加数据
 * pop_back         弹出字符串末尾的数据
 * assign, swap     将字符串复制到另一个字符串
 * swap             将字符串交换到另一个字符串，被交换的字符串清空资源
 * insert           在字符串中插入字符串或者字符
 * find/find_first_of/rfind 在字符串中找寻另一个字符串
 * replace          替换字符串中的指定字符
 * substr           从字符串中截取子字符并返回
************************************************************/
#include <string>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[])
{
    std::string string_val0{"string val 0/1!"};
    std::string string_val1(string_val0);

    //operator, append
    std::cout<<string_val0<<std::endl;
    std::cout<<string_val1<<std::endl;
    if(string_val0 == string_val1)
    {
        std::cout<<"String Equal!"<<std::endl;
    }
    string_val0 += " test";
    std::cout<<string_val0<<std::endl;
    string_val0.append(" ");
    string_val0.append("success");
    std::cout<<string_val0<<std::endl;

    //size max_size capacity
    std::cout<<"string_val0 size:"<<string_val0.size()<<std::endl;
    std::cout<<"string_val0 max size:"<<string_val0.max_size()<<std::endl;
    std::cout<<"string_val0 capacity:"<<string_val0.capacity()<<std::endl;

    //c_str和data
    std::cout<<"string_val1 c_str val: ";
    const char *pStr = string_val1.c_str();
    for(int index=0; index<string_val1.size(); index++)
    {
        std::cout<<*(pStr+index)<<string_val1[index]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<"string_val1 data val: ";
    pStr = string_val1.data();
    for(int index=0; index<string_val1.size(); index++)
    {
        std::cout<<*(pStr+index)<<string_val1[index]<<" ";
    }
    std::cout<<std::endl;

    //empty, clear
    std::cout<<"string_val1 Empty:"<<string_val1.empty()<<std::endl;
    string_val1.clear();
    std::cout<<"string_val1 Empty:"<<string_val1.empty()<<std::endl;
    std::cout<<"string_val1 Capacity:"<<string_val1.capacity()<<std::endl;
    std::cout<<"string_val1 size:"<<string_val1.size()<<std::endl;
    std::cout<<"string_val1 size:"<<string_val1.length()<<std::endl;

    //resize
    string_val1.resize(10, 'a');
    std::cout<<"string_val1 resize val:"<<string_val1<<std::endl;
    string_val1.resize(3, 'a');
    std::cout<<"string_val1 resize val:"<<string_val1<<std::endl;

    //at, front, back
    std::cout<<"string_val0 at:"<<string_val0.at(1)<<std::endl;
    std::cout<<"string_val0 front:"<<string_val0.front()<<std::endl;
    std::cout<<"string_val0 back:"<<string_val0.back()<<std::endl;

    //pop_back, push_back
    string_val0.pop_back();
    std::cout<<"string_val0 pop val:"<<string_val0<<std::endl;
    string_val0.push_back('c');
    std::cout<<"string_val0 push val:"<<string_val0<<std::endl;

    //assign, swap
    std::string string_val2;
    string_val2.assign(string_val0);
    std::cout<<"string_val2 assign val:"<<string_val2<<std::endl;
    std::cout<<"string_val0 assign size:"<<string_val0.size()<<std::endl;
    std::string string_val3;
    string_val3.swap(string_val2);
    std::cout<<"string_val3 swap val:"<<string_val3<<std::endl;
    std::cout<<"string_val2 swap size:"<<string_val2.size()<<std::endl;

    //for_each
    std::cout<<"string_val3 for val:";
    for(auto ref_val:string_val3)
    {
        std::cout<<ref_val;
    }
    std::cout<<std::endl;
    std::cout<<"string_val3 iterator val:";
    for(std::string::iterator ref_iterator=string_val3.begin(); ref_iterator!=string_val3.end(); ref_iterator++)
    {
        std::cout<<*ref_iterator;
    }
    std::cout<<std::endl;
    std::cout<<"string_val3 for_each val:";
    std::for_each(string_val3.begin(), string_val3.end(), [](const char &ref_val){
        std::cout<<ref_val;
    });
    std::cout<<std::endl;

    //insert
    std::cout<<"string_val3 insert val:";
    string_val3.insert(0, 2, 'a'); //位置，长度, 字符
    string_val3.insert(0, "hello~ "); //位置，字符串
    string_val3.insert(0, "My World ", 3); //位置，字符串，字符串中的长度
    std::cout<<string_val3<<std::endl;

    //find, rfind
    std::string::size_type position;
    position = string_val3.find("he");
    if(position != string_val3.npos)
    {
        std::cout<<"find position"<<position<<std::endl;
    }
    std::cout<<"find first of:"<<string_val3.find_first_of("he")<<std::endl; //返回子串中字符串首次出现的地点
    std::cout<<"find index:"<<string_val3.find("he", 3)<<std::endl; //从指定字段开始返回子串中字符串首次出现的地点
    std::cout<<"rfind:"<<string_val3.rfind("he")<<std::endl;

    //replace
    std::string string_val4 = "";
    string_val4 += "!$$$test";
    string_val4.replace(string_val4.find('$'), 1, "*"); //从起始地址替换指定长度的数据 
    std::cout<<"replace:"<<string_val4<<std::endl;
    string_val4.replace(string_val4.begin(), string_val4.begin()+6, "test!"); //替换迭代器的范围值
    std::cout<<"replace:"<<string_val4<<std::endl;

    //erase
    std::string string_val5{"132432521"};
    for(auto ref_iter=string_val5.begin(); ref_iter!=string_val5.end();)
    {
        if(*ref_iter == '2')
        {
            ref_iter=string_val5.erase(ref_iter);
        }
        else
        {
            ref_iter++;
        }
    }
    std::cout<<"string_val5 val:"<<string_val5<<std::endl;  
    string_val5.erase(4);
    std::cout<<"string_val5 val:"<<string_val5<<std::endl;
    auto string_val6 = string_val1.substr(0, 2);
    std::cout<<"string_val6 val:"<<string_val6<<std::endl;
}
