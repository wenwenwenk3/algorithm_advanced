//
// Created by kai.chen on 2022/4/7.
//
//  1. 优势洗牌（即田忌赛马）

#ifndef ALGORITHM_ADVANCED_GREED_ALGORTHM_H
#define ALGORITHM_ADVANCED_GREED_ALGORTHM_H
#include <vector>
using namespace std;

// 1. 优势洗牌
// 题目描述：给定两个大小相等的数组 A 和 B，A 相对于 B 的优势可以用满足 A[i] > B[i] 的索引 i 的数目来描述
//      请返回 A 的任意排列，使其相对于 B 的优势最大化。
// 思路： "田忌赛马"思想：从对方最小的牌开始，选我方比它强且尽量小的牌跟他抵消使尽可能赢的场数最多。如果牌比对方最小牌还小的必输等最后剩那些位置依次插进去就行
typedef struct{
    int score;
    int index;
}IndexTable;

bool cmp(const IndexTable& a, const IndexTable& b){
    return a.score < b.score;
}
vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
    // sort nums1
    sort(nums1.begin(), nums1.end());

    // sort nums2
    int n = nums2.size();
    vector<IndexTable> pairnums2;
    pairnums2.reserve(n);
    for(int i = 0; i < n; i++){
        pairnums2.emplace_back(IndexTable{nums2[i], i});
    }
    sort(pairnums2.begin(), pairnums2.end(), cmp);

    // 开始田忌赛马
    vector<int> resArr(n, -1);
    vector<int> remain;
    int i = 0, j = 0;
    while(i < n && j < n){
        if(nums1[i] > pairnums2[j].score){
            int idx = pairnums2[j].index;
            resArr[idx] = nums1[i];
            i++, j++;
        }else{
            remain.push_back(nums1[i]);
            i++;
        }
    }

    int pos = 0;
    if(j < n){ // 有场次必须输
        for(int k = 0; k < n; k++){ // 填充那些输的场次，顺序无所谓反正都要输
            if(resArr[k] == -1){
                resArr[k] = remain[pos++];
            }
        }
    }

    return resArr;
}


#endif //ALGORITHM_ADVANCED_GREED_ALGORTHM_H
