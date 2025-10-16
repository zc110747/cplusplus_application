//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2022-by Persional Inc.  
//  All Rights Reserved
//
//  Name:
//      mediatwo.cpp
//
//  Purpose:
//      寻找两个正序数组的中位数
//      给定两个大小分别为m和n的正序（从小到大）数组nums1和nums2。请找出并返回这两个正序数组的中位数。
//      解题思路：
//      1. 首先，我们需要将两个数组合并成一个有序数组。
//      2. 然后，我们可以根据数组长度的奇偶性来确定中位数的位置。
//      3. 如果数组长度为奇数，中位数就是合并后的数组中间的元素；
//      如果数组长度为偶数，中位数就是合并后的数组中间两个元素的平均值。
//      4. 为了避免创建一个新的数组，我们可以使用双指针来遍历两个数组，
//      比较当前指针所指向的元素大小，将较小的元素放入合并后的数组中。
//      5. 最后，我们根据合并后的数组长度来计算中位数。
//      时间复杂度：O(m+n)，其中 m 和 n 分别是两个数组的长度。
//      空间复杂度：O(1)。
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

#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>

namespace ALG_MEDIATWO
{
    class Solution {
    public:
        double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
            if(nums1.size() > nums2.size()) {
                return findMedianSortedArrays(nums2, nums1);
            }
            
            int m = nums1.size();
            int n = nums2.size();
            int left = 0, right = m;
            
            while(left <= right) {
                int partitionX = (left + right) / 2;
                int partitionY = (m + n + 1) / 2 - partitionX;
                
                int maxLeftX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
                int minRightX = (partitionX == m) ? INT_MAX : nums1[partitionX];
                
                int maxLeftY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
                int minRightY = (partitionY == n) ? INT_MAX : nums2[partitionY];
                
                if(maxLeftX <= minRightY && maxLeftY <= minRightX) {
                    if((m + n) % 2 == 0) {
                        return (std::max(maxLeftX, maxLeftY) + std::min(minRightX, minRightY)) / 2.0;
                    } else {
                        return std::max(maxLeftX, maxLeftY);
                    }
                } else if(maxLeftX > minRightY) {
                    right = partitionX - 1;
                } else {
                    left = partitionX + 1;
                }
            }
            
            return 0.0;
        }

        double findMedianSortedArrays2(std::vector<int>& nums1, std::vector<int>& nums2) {
            //合并两个有序数组
            std::vector<int> merged;
            merged.reserve(nums1.size() + nums2.size());
            std::merge(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), std::back_inserter(merged));

            //计算中位数
            int size = merged.size();
            if(size % 2 == 0) {
                return (merged[size / 2 - 1] + merged[size / 2]) / 2.0;
            } else {
                return merged[size / 2];
            }
        }

        double findMedianSortedArrays3(std::vector<int>& nums1, std::vector<int>& nums2) {
            //合并两个有序数组
            std::vector<int> merged = nums1;
            merged.insert(merged.end(), nums2.begin(), nums2.end());
            std::sort(merged.begin(), merged.end());

            //计算中位数
            int size = merged.size();
            if(size % 2 == 0) {
                return (merged[size / 2 - 1] + merged[size / 2]) / 2.0;
            } else {
                return merged[size / 2];
            }
        }
    };

    void test()
    {
        std::cout<<"\n==============="<<__FILE__<<"======================"<<std::endl;
        
        Solution sol;
        std::vector<int> nums1 = {1, 3, 4, 15, 28, 30};
        std::vector<int> nums2 = {2, 4, 5, 7, 8, 12, 64};
        std::cout<<"median: "<<sol.findMedianSortedArrays(nums1, nums2)<<std::endl;
        std::cout<<"median: "<<sol.findMedianSortedArrays2(nums1, nums2)<<std::endl;
        std::cout<<"median: "<<sol.findMedianSortedArrays3(nums1, nums2)<<std::endl;
    }
}