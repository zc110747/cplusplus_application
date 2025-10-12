//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      doublelist.cpp
//
//  Purpose:
//      线性表，双向链表     
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

namespace ALG_DOUBLE_LIST 
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
        struct Node *prev;
    };

    template<class T>
    class DoubleList
    {
    public:
        DoubleList() = default;
        DoubleList(std::initializer_list<T> list) {
            if(list.size() > 0) {
                Node<T>* ptr = nullptr;
                for(const auto *item = list.begin(); item != list.end(); ++item)
                {
                    if(head == nullptr) {
                        head = new Node<T>;
                        ptr = head;
                        tail = head;
                        ptr->next = nullptr;
                        ptr->prev = nullptr;
                    } else {
                        ptr->next = new Node<T>;
                        ptr->next->prev = ptr;
                        ptr = ptr->next;
                        ptr->next = nullptr;
                    }
                    ptr->data = *item;
                }
                tail = ptr;
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

        void show_reseverd() {
            Node<T>* ptr = tail;

            while(ptr != nullptr) {
                std::cout<<ptr->data<<" ";
                ptr = ptr->prev;
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
                nod->prev = nullptr;
                nod->next = head;
                head = nod;
                if(head->next == nullptr) {
                    tail = head;
                } else {
                    head->next->prev = nod;
                }
            } else {
                nod->prev = p;
                nod->next = p->next;
                if(nod->next != nullptr) {
                    nod->next->prev = nod;
                } else {
                    tail = nod;
                }
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
                return ERROR;
            
            if (index == 0) {
                Node<T> *nod = head;
                head = head->next;
                if(head != nullptr) {
                    head->prev = nullptr;
                }
                delete nod;
            } else {
                Node<T> *nod = p->next;
                p->next = nod->next;
                if(p->next != nullptr) {
                    p->next->prev = p;
                } else {
                    tail = p;
                }
                delete nod;
            }
            return OK;
        }

    private:
        Node<T> *head{nullptr};
        Node<T> *tail{nullptr};
    };


    void test()
    {
        std::cout<<"\n==============="<<__FILE__<<"======================"<<std::endl;
        
        DoubleList<std::string> single_str{"is", "linelist", "string", "test"};
        
        single_str.insert_elem(0, "this");
        single_str.show();
        single_str.show_reseverd();

        single_str.insert_elem(5, "!");
        single_str.show();
        single_str.show_reseverd();

        single_str.insert_elem(8, "aa");
        single_str.show();
        single_str.show_reseverd();

        single_str.delete_elem(2);
        single_str.show();
        single_str.show_reseverd();

        std::cout<<single_str.delete_elem(4)<<std::endl;
        single_str.show();
        single_str.show_reseverd();

        DoubleList<int> list_val = {1};
        list_val.delete_elem(0);
        list_val.insert_elem(0, 5);
        list_val.insert_elem(0, 6);
        list_val.show();
        list_val.show_reseverd();     
    }
}