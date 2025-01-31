//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      maxnum.cpp
//
//  Purpose:
//  给定一个按非递减顺序排列的数字数组 digits。你可以用任意次数 digits[i] 来写的数字。
//  例如，如果 digits = ['1','3','5']，我们可以写数字，如 '13', '551', 和 '1351315'。
//  返回 可以生成的小于或等于给定整数 n 的正整数的个数 。
//  ["1", "3", "5"]   
//  136 3 + 3*3 + 2*3
//  134 3 + 3*3 + 2*2
//  
//  Author:
//     	@听心跳的声音
//
//  Assumptions:
//
//  Revision History:
//      4/19/2023   Create New Version
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

class Solution {
public:
    int atMostNGivenDigitSet(std::vector<std::string>& digits, int n) {
        int num = digits.size();
        std::string str = std::to_string(n);
        std::vector<int> digits_int;
        int out_num = 0;

        //小于位数的，值为平方的累加和
        for (int i=1; i<str.size(); i++) {
            out_num += pow(num, i); 
        }
        return 0;
    }
};

namespace MAXNUM
{
    void run()
    {
        Solution sol;
        std::vector<std::string> val = {"1", "3", "5", "7"};
        sol.atMostNGivenDigitSet(val, 100);
    }
}