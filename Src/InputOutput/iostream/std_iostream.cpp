/**********************************************************
 * 说明: iostream, fstream
 * 用于处理I/O流和文件I/O流的C++库
 * open     打开文件
 * is_open  判断文件是否打开
 * write    向文件里面写入数据
 * close    关闭文件, 同时将缓冲区数据写入硬盘
 * flush    清空缓冲区，并写入硬盘
 * tellg    从输入/输出流中获取当前位置
 * seekg    设置在输入/输出流中的位置
************************************************************/
#include<iostream>
#include<fstream>
#include<string>
#include<memory>
#include<errno.h>
#include<string.h>

int main(int argc, char *argv[])
{
    std::ofstream ofstream_file0("0.txt");
    
    //<<, is_open, close
    if(ofstream_file0.is_open())
    {
        std::string str_val0 = "string value write";
        ofstream_file0<<"hello file I/O Test"<<std::endl;
        ofstream_file0.write(str_val0.data(), str_val0.size());
        ofstream_file0.close();
    }

    /*
        ios::app：　　　 //以追加的方式打开文件  
        ios::ate：　　　 //文件打开后定位到文件尾，ios:app就包含有此属性  
        ios::binary：　 //以二进制方式打开文件，缺省的方式是文本方式。两种方式的区别见前文  
        ios::in：　　　  //文件以输入方式打开（文件数据输入到内存）  
        ios::out：　　　 //文件以输出方式打开（内存数据输出到文件）  
        ios::nocreate： //不建立文件，所以文件不存在时打开失败  
        ios::noreplace：//不覆盖文件，所以打开文件时如果文件存在失败  
        ios::trunc：　  //如果文件存在，把文件长度设为0(覆盖文件)
    */
    std::ofstream ofstream_file1;
    ofstream_file1.open("0.txt", std::ios::app);
    if(ofstream_file1.is_open())
    {
        ofstream_file1.seekp(0, std::ios::end);
        ofstream_file1<<std::endl;
        ofstream_file1.write("seekp", 6);
        ofstream_file1.close();
    }

    /*
        ios::beg：　　　 //移动到文件的起始
        ios::cur
        ios::end：　　　 //移动到文件的末尾
    */
    std::ifstream ifstream_file0;
    ifstream_file0.open("0.txt");
    if(ifstream_file0.is_open())
    {
        ifstream_file0.seekg(0, std::ios::end);
        int length = ifstream_file0.tellg();
        std::unique_ptr<char[]> unique_ptr_char(new char[length]);

        //原始字符串字面量
        std::cout<<R"("0.txt length":)"<<length<<std::endl;
        ifstream_file0.seekg(0, std::ios::beg);
        ifstream_file0.read(unique_ptr_char.get(), length);
        std::string string_val0(unique_ptr_char.get(), unique_ptr_char.get()+length);
        std::cout<<string_val0<<std::endl;
        ifstream_file0.close();
    }

    std::fstream fstream_file0;
    fstream_file0.open("1.txt", std::ios::in | std::ios::out | std::ios::trunc);
    if(fstream_file0.is_open())
    {
        fstream_file0<<"test for val"<<std::endl;
        std::string str_val{"write val ok"};
        fstream_file0.write(str_val.data(), str_val.size());
        fstream_file0.seekg(0, std::ios::end);
        fstream_file0.flush();
        int length = fstream_file0.tellg();
        fstream_file0.seekg(0, std::ios::beg);
        std::unique_ptr<char[]> unique_ptr_char(new char[length]);
        fstream_file0.read(unique_ptr_char.get(), length);
        std::string string_val1(unique_ptr_char.get(), unique_ptr_char.get()+length);
        std::cout<<string_val1<<std::endl; 
        fstream_file0.close();
    }
    else
    {
        std::cout<<"file open error:"<<strerror(errno)<<std::endl;
    }
    
}


