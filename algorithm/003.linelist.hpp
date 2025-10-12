//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      linelist.cpp
//
//  Purpose:
//      线性表      
//
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <iostream>
#include <string>

namespace ALG_LINE_LIST
{
    #define MAXSIZE    30

    typedef enum {
        OK = 0,
        ERROR = 1,
    }STATUS;
    
    template<class T, int N=MAXSIZE>
    class LineList
    {
    public:
        LineList() = default;
        LineList(std::initializer_list<T> list) {
            if(list.size() < N) {
                for(const auto *item = list.begin(); item != list.end(); ++item)
                {
                    data_[length_++] = *item;
                }
            }
        }
    
        bool empty() {
            if (length_ == 0) {
                return true;
            }
            return false;
        }
        
        bool full() {
            if (length_ == N) {
                return true;
            }
            return false;
        }
    
        int length() {
            return length_;
        }
    
        STATUS get_elem(int index, T *elem) {
            if (length_ == 0 || index < 0 ||  index >= length_ || elem == nullptr) {
                return ERROR;
            }
    
            *elem = data_[index];
            return OK;
        }
    
        int locate_elem(T &elem, bool FromHead = true) 
        {
            return locate_elem(std::move(elem), FromHead);
        }
    
        int locate_elem(T &&elem, bool FromHead = true) {
            int index;
    
            if(length_ == 0)
                return -1;
    
            if (FromHead) {
                for (index=0; index<length_; index++) {
                    if(data_[index] == elem) {
                        return index;
                    }
                }
            } else {
                for (index=length_-1; index>=0; index--) {
                    if(data_[index] == elem) {
                        return index;
                    }
                }
            }
    
            return -1;
        }
    
        int count_elem(T elem) {
            int index;
            int count = 0;
    
            for (index=0; index<length_; index++){
                if(data_[index] == elem) {
                    count++;
                }
            }
            return count;
        }
    
        STATUS insert_elem(int index, T& elem) {
            return insert_elem(index, std::move(elem));
        }
    
        STATUS insert_elem(int index, T&& elem) {
            if (length_ == N) {
                return ERROR;
            }
            
            if (index<0 || index>length_) {
                return ERROR;
            } 
            
            if (index < length_) {
                int k;
                for (k=length_-1; k>=index; k--) {
                    data_[k+1] = data_[k];
                }
            }
    
            data_[index] = elem;
            length_++;
            return OK;
        }
    
        STATUS delete_elem(int index) {
            if (length_ == 0) {
                return ERROR;
            }
            
            if (index<0 || index>=length_) {
                return ERROR;
            } 
            
            if (index < length_) {
                int k;
                for (k=index; k<length_-1; k++) {
                    data_[k] = data_[k+1];
                }
            }
            length_--;
            return OK;
        }
    
        void show() {
            
            int index;
    
            for(index=0; index<length_; index++){
                std::cout<<data_[index]<<" ";
            }
            std::cout<<std::endl;
        }
    
    private:
        T data_[N];
        int length_{0};
    };
    
    void test()
    {
        std::cout<<"\n==============="<<__FILE__<<"======================"<<std::endl;
        
        int a = 10;
        LineList<int, 20> line_list;

        line_list.insert_elem(0, a);
        line_list.insert_elem(0, 5);
        line_list.insert_elem(1, 4);
        line_list.insert_elem(2, 3);
        line_list.insert_elem(3, 2);
        line_list.insert_elem(4, 1);
        line_list.show();
        line_list.insert_elem(1, 6);
        line_list.show();
        line_list.insert_elem(3, 5);
        line_list.show();
        line_list.delete_elem(2);
        line_list.show();

        std::cout<<line_list.locate_elem(3)<<std::endl;
        std::cout<<line_list.locate_elem(-5)<<std::endl;
        std::cout<<line_list.count_elem(5)<<std::endl;

        LineList<std::string, 20> line_list_str{"This", "is", "linelist", "string", "test!"};
        line_list_str.show();
        std::cout<<line_list_str.locate_elem("is")<<std::endl;
    }
}
