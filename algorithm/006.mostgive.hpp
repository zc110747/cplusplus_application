//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      mostgive.cpp
//
//  Purpose:
//  给定一个按非递减顺序排列的数字数组 digits。你可以用任意次数 digits[i] 来写的数字。
//  例如，如果 digits = ['1','3','5']，我们可以写数字，如 '13', '551', 和 '1351315'。
//  返回 可以生成的小于或等于给定整数 n 的正整数的个数 。
//  ["1", "3", "5"]
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

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

namespace ALG_MOSTGIVE
{
    class Solution {
    public:
        int atMostNGivenDigitSet(std::vector<std::string>& digits, int n) {
            int num = digits.size();
            std::string str = std::to_string(n);
            int out_num = 0;

            //小于位数的，值为平方的累加和
            for (int i = 1; i < str.size(); i++) {
                out_num += std::pow(num, i); 
            }

            //处理与n位数相同的情况
            for (int i = 0; i < str.size(); i++) {
                bool hasSame = false;
                for (const auto& digit : digits) {
                    if (digit[0] < str[i]) {
                        out_num += std::pow(num, str.size() - i - 1);
                    } else if (digit[0] == str[i]) {
                        hasSame = true;
                    }
                }
                if (!hasSame) {
                    return out_num;
                }
            }

            //如果所有位都能匹配，加上自身
            return out_num + 1;
        }
    };

    void test()
    {
        std::cout<<"\n==============="<<__FILE__<<"======================"<<std::endl;

        Solution sol;
        std::vector<std::string> val = {"1", "3", "5", "7"};
        std::cout<<"most give: "<<sol.atMostNGivenDigitSet(val, 100)<<std::endl;
    }
}