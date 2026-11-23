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
            for(const auto &item : list)
            {
                append_node(item);
            }
        }

        //深拷贝, 两个对象持有各自独立的节点
        DoubleList(const DoubleList& rhs) {
            for(const Node<T> *src = rhs.head; src != nullptr; src = src->next) {
                append_node(src->data);
            }
        }

        DoubleList& operator=(const DoubleList& rhs) {
            if (this != &rhs) {
                DoubleList tmp(rhs);
                swap(tmp);
            }
            return *this;
        }

        DoubleList(DoubleList&& rhs) noexcept:head{rhs.head}, tail{rhs.tail} {
            rhs.head = nullptr;
            rhs.tail = nullptr;
        }

        DoubleList& operator=(DoubleList&& rhs) noexcept {
            if (this != &rhs) {
                clear();
                head = rhs.head;
                tail = rhs.tail;
                rhs.head = nullptr;
                rhs.tail = nullptr;
            }
            return *this;
        }

        //析构函数释放全部节点
        ~DoubleList() {
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

        void show_reseverd() const {
            Node<T>* ptr = tail;

            while(ptr != nullptr) {
                std::cout<<ptr->data<<" ";
                ptr = ptr->prev;
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

            Node<T> *nod = new Node<T>{std::move(elem), nullptr, nullptr};

            if (index == 0) {
                nod->prev = nullptr;
                nod->next = head;
                head = nod;
                if(nod->next == nullptr) {
                    tail = head;
                } else {
                    nod->next->prev = nod;
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
            if (index < 0 || head == nullptr) {
                return ERROR;
            }

            if (index == 0) {
                Node<T> *nod = head;
                head = head->next;
                if(head != nullptr) {
                    head->prev = nullptr;
                } else {
                    //删除唯一节点后 tail 必须同步置空, 否则悬垂
                    tail = nullptr;
                }
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
                return ERROR;
            }

            Node<T> *nod = p->next;
            p->next = nod->next;
            if(p->next != nullptr) {
                p->next->prev = p;
            } else {
                tail = p;
            }
            delete nod;
            return OK;
        }

    private:
        //在尾部追加节点, 保持 prev 链与 tail 正确
        void append_node(const T& value) {
            Node<T> *nod = new Node<T>{value, nullptr, tail};
            if (tail != nullptr) {
                tail->next = nod;
            } else {
                head = nod;
            }
            tail = nod;
        }

        void clear() noexcept {
            while (head != nullptr) {
                Node<T> *next = head->next;
                delete head;
                head = next;
            }
            tail = nullptr;
        }

        void swap(DoubleList& rhs) noexcept {
            std::swap(head, rhs.head);
            std::swap(tail, rhs.tail);
        }

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