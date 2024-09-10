//
// Created by kai.chen on 2022/1/3.
//
//  线段树
// - 线段树segmentTree 是一个二叉树，每个结点保存数组 nums 在区间 [s,e] 的最小值、最大值或者总和等信息。
// - 线段树可以用树也可以用数组（堆式存储）来实现。
// - 对于数组实现，假设根结点的下标为 0，如果一个结点在数组的下标为i，那么它的左子结点下标为i×2+1，右子结点下标为i×2+2。
//
/* *
    1. 数组不变，求区间和：              「前缀和」、
    2. 单点修改，区间查询：              「树状数组」、「线段树」
    3. 修改某个区间，单点查询：            「差分」、「线段树」；    模版题：1109航班预定 1094拼车 370区间加法  1893检查是否区域内所有整数都被覆盖
    4. 需要多次修改某个区间，区间查询：            「线段树」、「树状数组」（看修改区间范围大小）
    5. 数组区间修改，区间查询：线段树。            「线段树」、「树状数组」（看修改区间范围大小）

 * */


// 1. 我的日程安排表 III
//
//
#ifndef ALGORITHM_ADVANCED_SEGMENT_TREE_H
#define ALGORITHM_ADVANCED_SEGMENT_TREE_H

// 区域和检索 - 数组可修改
class NumArray {
private:
    vector<int> segmentTree;
    int n;

    void build(int i, int l, int r, vector<int> &nums) {
        if (l == e) {
            segmentTree[i] = nums[l];
            return;
        }
        int m = l + (e - l) / 2;
        build(i * 2 + 1, l, m, nums);
        build(i * 2 + 2, m + 1, r, nums);
        segmentTree[i] = segmentTree[i * 2 + 1] + segmentTree[i * 2 + 2];
    }

    void change(int index, int val, int node, int s, int e) {
        if (s == e) {
            segmentTree[node] = val;
            return;
        }
        int m = s + (e - s) / 2;
        if (index <= m) {
            change(index, val, node * 2 + 1, s, m);
        } else {
            change(index, val, node * 2 + 2, m + 1, e);
        }
        segmentTree[node] = segmentTree[node * 2 + 1] + segmentTree[node * 2 + 2];
    }

    int range(int left, int right, int node, int s, int e) {
        if (left == s && right == e) {
            return segmentTree[node];
        }
        int m = s + (e - s) / 2;
        if (right <= m) {
            return range(left, right, node * 2 + 1, s, m);
        } else if (left > m) {
            return range(left, right, node * 2 + 2, m + 1, e);
        } else {
            return range(left, m, node * 2 + 1, s, m) + range(m + 1, right, node * 2 + 2, m + 1, e);
        }
    }

public:
    NumArray(vector<int>& nums) : n(nums.size()), segmentTree(nums.size() * 4) {
        build(0, 0, n - 1, nums);
    }

    void update(int index, int val) {
        change(index, val, 0, 0, n - 1);
    }

    int sumRange(int left, int right) {
        return range(left, right, 0, 0, n - 1);
    }
};


//  1893检查是否区域内所有整数都被覆盖
bool isCovered(vector<vector<int>>& ranges, int left, int right) {
    vector<int> diff(52, 0);   // 差分数组
    for(const auto& range: ranges) {
        int l = range[0], r = range[1];
        ++diff[l];
        --diff[r+1];
    }
    // 前缀和
    int presum = 0;
    for (int i = 1; i <= 50; ++i) {
        presum += diff[i];
        if (i >= left && i <= right && presum <= 0) {
            return false;
        }
    }
    return true;
}




vector<int> d;


void build(int s, int t, int p) {
    if (s == t) {
        d[p] = a[s];
        return;
    }
    int m = s + ((t - s) >> 1);
    // 移位运算符的优先级小于加减法，所以加上括号
    // 如果写成 (s + t) >> 1 可能会超出 int 范围
    build(s, m, p * 2);
    build(m + 1, t, p * 2 + 1);
    // 递归对左右区间建树
    d[p] = d[p * 2] + d[(p * 2) + 1];
}

// [l, r] 为查询区间, [s, t] 为当前节点包含的区间, p 为当前节点的编号
int getsum(int l, int r, int s, int t, int p) {
    if (l <= s && t <= r)
        return d[p];  // 当前区间为询问区间的子集时直接返回当前区间的和
    int m = s + ((t - s) >> 1), sum = 0;
    if (l <= m) sum += getsum(l, r, s, m, p * 2);
    // 如果左儿子代表的区间 [s, m] 与询问区间有交集, 则递归查询左儿子
    if (r > m) sum += getsum(l, r, m + 1, t, p * 2 + 1);
    // 如果右儿子代表的区间 [m + 1, t] 与询问区间有交集, 则递归查询右儿子
    return sum;
}

// 积分排在第K位的猎头
// 基本需求
//  -根据猎头的ID快速查找、删除、更新
//  -获取区间的猎头ID列表
//  -按照积分高低，查找排名在K为的猎头ID
//  -按照积分高低，查询猎头ID排在第几位


// -https://blog.csdn.net/zearot/article/details/48299459


// 以组为单位订音乐会的门票 https://leetcode.cn/problems/booking-concert-tickets-in-groups/

// 我的日程安排表 III
// 线段树 懒标记标记区间 [l,r] 进行累加的次数，tree 记录区间 [l,r] 的最大值
class MyCalendarThree {
public:
    unordered_map<int, pair<int, int>> tree;

    MyCalendarThree() {}

    void update(int start, int end, int l, int r, int idx) {
        if (r < start || end < l) {
            return;
        }
        if (start <= l && r <= end) {
            tree[idx].first++;
            tree[idx].second++;
        } else {
            int mid = (l + r) >> 1;
            update(start, end, l, mid, 2 * idx);
            update(start, end, mid + 1, r, 2 * idx + 1);
            tree[idx].first = tree[idx].second + max(tree[2 * idx].first, tree[2 * idx + 1].first);
        }
    }

    int book(int start, int end) {
        update(start, end - 1, 0, 1e9, 1);
        return tree[1].first;
    }
};



#endif //ALGORITHM_ADVANCED_SEGMENT_TREE_H
