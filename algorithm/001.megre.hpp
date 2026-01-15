//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      megre.cpp
//
//  Purpose:
//      合并两个有序列表，如:1->2->3   1->2
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
//     	@公众号：<嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <ostream>

namespace ALG_MEGRE
{
    struct list_node
    {
        int val_;
        list_node *next;
        list_node(int value):val_(value), next{nullptr} {
        }
    };

    std::ostream& operator<<(std::ostream &os, list_node* ptr)
    {
        while (ptr != nullptr) {
            os<<ptr->val_<<",";
            ptr = ptr->next;
        }
        return os;
    }
    
    list_node* megre(list_node *p1, list_node *p2)
    {
        list_node *head, *node;
        
        if(p1 == nullptr) return p2;
        if(p2 == nullptr) return p1;

        if (p1->val_ < p2->val_) {
            head = p1;
            p1 = p1->next;
        } else {
            head = p2;
            p2 = p2->next;
        }
        node = head;

        while (p1 != nullptr && p2 != nullptr) {
            if (p1->val_ < p2->val_) {
                node->next = p1;
                p1 = p1->next;
            } else {
                node->next = p2;
                p2 = p2->next;
            }
            node = node->next;
        }
        
        if (p1 != nullptr) node->next = p1;
        if (p2 != nullptr) node->next = p2;

        return head;
    }

    void test()
    {
        std::cout<<"\n==============="<<__FILE__<<"======================"<<std::endl;
        
        list_node *p1 = new list_node(1);
        list_node *p2 = new list_node(2);
        list_node *p3 = new list_node(3);
        list_node *p4 = new list_node(5);
        p1->next = p2;
        p2->next = p3;
        p3->next = p4;
        
        list_node *c1 = new list_node(1);
        list_node *c2 = new list_node(2);
        list_node *c3 = new list_node(4);
        c1->next = c2;
        c2->next = c3;

        auto point = megre(p1, c1);
        std::cout<<point<<std::endl;
    }
}
