//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      megre.cpp
//
//  Purpose:
//      按照顺序合并两个有序链表
//      如:1->2->3   1->2
//      1->1->2->2->3
//      解题思路:
//      1.创建链表，内部包含一个值，和next指针
//      2.创建合并函数，输入是两个链表首指针
//      3.如果判断任意链表为nullptr, 则直接返回另一个链表
//      4.比较两个链表，确定head指针, 并将定义为head的对应指针后移一位
//      5.定义指针node指向head，循环比较两个指针的值，哪个小，哪个放在node的后面, 并且node和选中的指针都指向下一位
//      6.直到两个指针中任意一个为nullptr，结束
//      7.将ptr指向不为nullptr的指针
//      8.返回head指针
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include "megre.hpp"
#include "../includes.hpp"

using namespace std;

namespace MEGRE
{
    struct list
    {
        int val_;
        list *next;
        list(int value):val_(value), next{nullptr}{
        }
    };

    ostream& operator<<(ostream &os, list* ptr)
    {
        while(ptr != nullptr)
        {
            os<<ptr->val_<<",";
            ptr = ptr->next;
        }
        return os;
    }
    
    list* megre(list *p1, list *p2)
    {
        list *head, *node;
        
        if(p1 == nullptr) return p2;
        if(p2 == nullptr) return p1;

        if(p1->val_ < p2->val_)
        {
            head = p1;
            p1 = p1->next;
        }
        else
        {
            head = p2;
            p2 = p2->next;
        }
        node = head;
        while(p1 != nullptr && p2 != nullptr)
        {
            if(p1->val_ < p2->val_)
            {
                node->next = p1;
                p1 = p1->next;
            }
            else
            {
                node->next = p2;
                p2 = p2->next;
            }
            node = node->next;
        }
        
        if(p1 != nullptr) node->next = p1;
        if(p2 != nullptr) node->next = p2;

        return head;
    }

    void test_study()
    {
        FUNCTION_START()
        list *p1 = new list(1);
        list *p2 = new list(2);
        list *p3 = new list(3);
        list *p4 = new list(5);
        p1->next = p2;
        p2->next = p3;
        p3->next = p4;
        
        list *c1 = new list(1);
        list *c2 = new list(2);
        list *c3 = new list(4);
        c1->next = c2;
        c2->next = c3;

        auto point = megre(p1, c1);
        cout<<point;


        FUNCTION_END()
    }
}
