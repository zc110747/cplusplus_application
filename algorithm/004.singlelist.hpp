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
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <string>
#include <utility>

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
        SingleList():name_("SingleList") {
        }
        SingleList(std::initializer_list<T> list):name_("SingleList") {
            Node<T> **dst = &head;
            for(const auto &item : list)
            {
                *dst = new Node<T>{item, nullptr};
                dst = &(*dst)->next;
            }
        }

        //深拷贝, 两个对象持有各自独立的节点
        SingleList(const SingleList& rhs):name_(rhs.name_) {
            Node<T> **dst = &head;
            for(const Node<T> *src = rhs.head; src != nullptr; src = src->next) {
                *dst = new Node<T>{src->data, nullptr};
                dst = &(*dst)->next;
            }
        }

        SingleList& operator=(const SingleList& rhs) {
            if (this != &rhs) {
                SingleList tmp(rhs);
                swap(tmp);
            }
            return *this;
        }

        //移动构造直接接管对方的链, 不复制节点
        SingleList(SingleList&& rhs) noexcept:head{rhs.head}, name_{rhs.name_} {
            rhs.head = nullptr;
        }

        SingleList& operator=(SingleList&& rhs) noexcept {
            if (this != &rhs) {
                clear();
                head = rhs.head;
                rhs.head = nullptr;
            }
            return *this;
        }

        //析构函数释放全部节点
        ~SingleList() {
            clear();
        }
    
        void show() const {
            Node<T>* ptr = head;
    
            while(ptr != nullptr) {
                std::cout<<ptr->data<<" ";
                ptr = ptr->next;
            }
            std::cout<<std::endl;
        }
    
        //左值实参复制一份, 不做隐式移动
        STATUS insert_elem(int index, const T& elem) {
            T copy(elem);
            return insert_elem(index, std::move(copy));
        }
    
        STATUS insert_elem(int index, T&& elem) {
            if (index < 0) {
                return ERROR;
            }

            Node<T> *p = head;
            if (index != 0) {
                int j = 1;
                while (p && j < index) {
                    p = p->next;
                    ++j;
                }
                if (p == nullptr) {
                    return ERROR;
                }
            }

            Node<T> *nod = new Node<T>{std::move(elem), nullptr};

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
            if (index < 0 || head == nullptr) {
                std::cout<<"delete, nullptr!"<<std::endl;
                return ERROR;
            }

            if (index == 0) {
                Node<T> *nod = head;
                head = head->next;
                delete nod;
                return OK;
            }

            int j = 1;
            Node<T> *p = head;
            while (p && j < index) {
                p = p->next;
                ++j;
            }

            //目标位置不存在时 p 或 p->next 为空, 不能继续解引用
            if (p == nullptr || p->next == nullptr) {
                std::cout<<"delete, nullptr!"<<std::endl;
                return ERROR;
            }

            Node<T> *nod = p->next;
            p->next = nod->next;
            delete nod;
            return OK;
        }
    
    private:
        void clear() noexcept {
            while (head != nullptr) {
                Node<T> *next = head->next;
                delete head;
                head = next;
            }
        }

        void swap(SingleList& rhs) noexcept {
            std::swap(head, rhs.head);
            std::swap(name_, rhs.name_);
        }

        Node<T> *head{nullptr};

        const char *name_{nullptr};
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
