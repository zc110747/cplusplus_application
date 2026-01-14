//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      reserved.cpp
//      反转链表
//      如:1->2->3->5->7
//      输出: 7->5->3->2->1
//      解题思路:
//      对于任意node
//      tmp = cur([2])->next;
//      cur([2])->next = pre([1]);
//      pre = cur([2])
//      cur = tmp;
//      1.创建链表，内部包含一个值，和next指针
//      2.创建反转链表，输入是链表值
//      3.定义两个指针，分别是pre和cur，用于存储反转链表信息pre=nullptr, cur=head
//      4.定义指针node暂存next的指针值,直到cur为nullptr
//          node = cur->next;
//          cur->next = pre;
//          pre = cur;
//          cur = node;
//       5.返回cur值，即为翻转后的起始指针
//  Purpose:
//      
//  Author:
//     	@公众号 <嵌入式技术总结>
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <ostream>

namespace ALG_RESERVED
{
    struct ListNode
    {
        int val_;
        ListNode *next;
        ListNode(int value):val_(value), next{nullptr}{
        }
    };

    std::ostream& operator<<(std::ostream &os, ListNode* ptr)
    {
        while(ptr != nullptr)
        {
            os<<ptr->val_<<",";
            ptr = ptr->next;
        }
        return os;
    }

    ListNode* reserved(ListNode *p1)
    {
        ListNode *pre = nullptr;
        ListNode *cur = p1;

        while(cur != nullptr)
        {
            ListNode* tmp = cur->next;
            cur->next = pre;
            pre = cur;
            cur = tmp;
        }
        return pre;
    }

    void test()
    {
        std::cout<<"\n==============="<<__FILE__<<"======================"<<std::endl;
        
        ListNode *p1 = new ListNode(1);
        ListNode *p2 = new ListNode(2);
        ListNode *p3 = new ListNode(3);
        ListNode *p4 = new ListNode(5);
        
        p1->next = p2;
        p2->next = p3;
        p3->next = p4;

        auto point = reserved(p1);
        std::cout<<point<<std::endl;
    }
}
