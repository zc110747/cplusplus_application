//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      signallist.cpp
//
//  Purpose:
//      基于链表实现的线性表，支持增删查改功能    
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

namespace ALG_SINGAL_LIST
{
    #define MAXSIZE    30

    typedef enum {
        OK = 0,
        ERROR = 1,
    }STATUS;

    template<class T>
    struct Node
    {
        T data;
        struct Node *next;
    };
    
    template<class T>
    class SingleList
    {
    public:
        SingleList():name_(__func__) {
        }
        SingleList(std::initializer_list<T> list) {
            if(list.size() > 0) {
                Node<T>* ptr = nullptr;
                for(const auto *item = list.begin(); item != list.end(); ++item)
                {
                    if(head == nullptr) {
                        head = new Node<T>;
                        ptr = head;
                        ptr->next = nullptr;
                    } else {
                        ptr->next = new Node<T>;
                        ptr = ptr->next;
                        ptr->next = nullptr;
                    }
                    ptr->data = *item;
                }
            }
        }
    
        void show() {
            Node<T>* ptr = head;
    
            while(ptr != nullptr) {
                std::cout<<ptr->data<<" ";
                ptr = ptr->next;
            }
            std::cout<<std::endl;
        }
    
        STATUS insert_elem(int index, T& elem) {
            return insert_elem(index, std::move(elem));
        }
    
        STATUS insert_elem(int index, T&& elem) {
            int j = 1;
            Node<T> *p;
    
            p = head;
            while (p && j < index) {
                p = p->next;
                ++j;
            }
    
            if (!p && index != 0)
                return ERROR;
    
            Node<T> *nod = new Node<T>;
            nod->data = elem;
    
            if (index == 0) {
                nod->next = head;
                head = nod;
            } else {
                nod->next = p->next;
                p->next = nod;
            }
            return OK;
        }
    
        STATUS delete_elem(int index) {
            int j = 1;
            Node<T> *p;
    
            p = head;
            while (p && j < index) {
                p = p->next;
                ++j;
            }
    
            if (!p)
            {
                std::cout<<"delete, nullptr!"<<std::endl;
                return ERROR;
            }
            
            if (index == 0) {
                Node<T> *nod = head;
                head = head->next;
                delete nod;
            } else {
                Node<T> *nod = p->next;
                p->next = nod->next;
                delete nod;
            }
            return OK;
        }
    
    private:
        Node<T> *head{nullptr};

        char *name_;
    };
    
    void test()
    {
        std::cout<<"\n==============="<<__FILE__<<"======================"<<std::endl;
        
        SingleList<std::string> single_str{"This", "is", "linelist", "string", "test"};
    
        single_str.insert_elem(0, "t");
        single_str.show();    
        single_str.insert_elem(1, "a");
        single_str.show();
        single_str.insert_elem(8, "aa");
        single_str.show();
        single_str.delete_elem(2);
        single_str.show();
        std::cout<<single_str.delete_elem(5)<<std::endl;
        single_str.show();
    
        SingleList<int> val_list = {1};
        val_list.delete_elem(0);
        val_list.insert_elem(0, 5);
        val_list.show();
    }
}
