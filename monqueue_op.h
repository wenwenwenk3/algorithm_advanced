//
// Created by kai.chen on 2021/12/30.
//
//  单调队列
//    就是一个队列，先进先出，使用巧妙的方式 让队列中的元素都是单调递增/递减的。
//      1. 滑动窗口最大值
//      2. 环形子数组的最大和
//
#ifndef ALGORITHM_ADVANCED_MONQUEUE_OP_H
#define ALGORITHM_ADVANCED_MONQUEUE_OP_H
#include <queue>
#include <deque>
#include <vector>
using namespace std;

// 1. 滑动窗口最大值
// 题目描述：输入数组nums 和k，一个大小为k的子数组在nums滑动。请输出每次滑动窗口的最大值
//      例如：输入：[1,3,-1,-3,5,3,6,7], k =3  输出：[3,3,5,5,6,7]
//  思路：
//      单调队列：一堆数字中，已知最值为A, 那么每次新加一个数字B，只需要比较一下A和B的关系就可以更新新的最值
//
// 单调队列数据结构的实现
class MontonicQueue {
public:
    // 在队尾添加元素val，将队列前面小于val的元素全部删除
    void push(int val){ //
        while(!q.empty() && q.back() < val){
            q.pop_back();
        }
        q.push_back(val);
    }
    // 队头删除元素
    void pop(int val){ // 一堆数字中，已知最值max，减去一个值，若这个值恰好为max才需要更新最值
        if(val == max()){
            q.pop_front();
        }
    }
    int max(){ // 始终保持队列的首部就是最大值   // 5->4->3->2
        return q.front();
    }
private:
    deque<int> q;
};

vector<int> maxSlidingWindow(vector<int>& nums, int k){
    MontonicQueue window;
    vector<int> res;
    for(int i = 0; i < nums.size(); i++){
        if(i < k-1){ // 先起码填满window元素个数到k-1个
            window.push(nums[i]);
        }
        else{
            // 窗口向前滑动，添加新数字
            window.push(nums[i]);
            // 记录窗口最大值
            res.push_back(window.max());
            // 弹出旧数字
            int left = i -k+1;
            window.pop(nums[left]);
        }
    }
    return res;
}

/*
func maxSlidingWindow(nums []int, k int) []int {
    ans := make([]int, 0, len(nums)-k+1)
    q := []int{} // 优先级队列，保存的是下标，最大的元素一直在前面
    for i, x := range nums {
        for len(q) > 0 && nums[q[len(q)-1]] <= x { // 新进来的元素比队列尾部的元素大，pop back尾部元素
            q = q[:len(q)-1]
        }
        q = append(q, i)
        if i - q[0] >= k {
            q = q[1:]
        }
        if i >= k-1 {
            ans = append(ans, nums[q[0]])
        }
    }
    return ans
}*/

void testmaxSlidingWindow(){
    int a[] = {1,3,1,2,0,5};
    vector<int> nums(a, a+sizeof(a)/sizeof(a[0]));
    vector<int> res = maxSlidingWindow(nums, 3);
    for(auto i : res){
        cout<< i<<",";
    }
    cout<<endl;
}


// 2. 环形子数组的最大和
// 思路： 数组翻倍，先计算 A[0..2n-1] 的前缀和 sums[0..2n] ，
//      枚举每个前缀和的下标j = [1,2n]，现在要找 j-i <= n 时最大的 sums[j] - sums[i]，即每次找j的左边最小的 sums[i]。
//      要想O(1) 地找到每个 j 的左侧最值及其下标 i，其中 i 的范围需要满足某些条件(这里是 j - i <= N)。这是典型的单调队列的场景：即滑动窗口的最值问题
// 工业级写法
typedef pair<int, int> Node; // idx,val
class MontonicQueueMin{
public:
    // 在队尾添加元素val，将队列前面大于等于val的元素全部删除
    void push(pair<int, int> val){ //
        while(!q.empty() && q.back().second >= val.second){
            q.pop_back();
        }
        q.push_back(val);
    }
    // 队头删除元素
    void pop(pair<int, int> val){ // 一堆数字中，已知最值max，减去一个值，若这个值恰好为max才需要更新最值
        if(val == min()){
            q.pop_front();
        }
    }
    pair<int, int> min(){ // 始终保持队列的首部就是最小值   // 5->4->3->2
        return q.front();
    }
private:
    deque<Node> q;
};
int maxSubarraySumCircular(vector<int>& nums) {
    int n = nums.size();
    if(n == 1) return nums[0];
    nums.insert(nums.end(), nums.begin(), nums.end());
    MontonicQueueMin segment;

    vector<int> sums(n * 2 + 1, 0);
    for(int i = 0; i < n * 2; ++i) {
        sums[i + 1] = nums[i] + sums[i];
    }

    int ans = INT_MIN;
    segment.push(make_pair(0, sums[0]));
    for(int j = 1; j <= n * 2; ++j){
        int pj = sums[j];
        int pi = segment.min().second;
        ans = max(ans, pj - pi);

        segment.push(make_pair(j, sums[j]));
        if(j - segment.min().first == n){
            segment.pop(segment.min());
        }
    }
    return ans;
}
void testmaxSubarraySumCircular(){
    int a[] = {-3,-2,-3};
    vector<int> nums(a, a+sizeof(a)/sizeof(a[0]));
    cout<<"max sum:"<<maxSubarraySumCircular(nums)<<endl;
}



// 野生写法
int maxSubarraySumCircular_v1(vector<int>& nums) {
    int n = nums.size();
    if(n == 1) return nums[0];
    nums.insert(nums.end(), nums.begin(), nums.end());
    deque<int> deq;

    vector<int> sums(n * 2 + 1, 0);
    for(int i = 0; i < n * 2; ++i) {
        sums[i + 1] = nums[i] + sums[i];
    }

    int ans = INT_MIN;
    deq.push_back(0);
    for(int j = 1; j <= n * 2; ++j){
        int pj = sums[j];
        int pi = sums[deq.front()];
        ans = max(ans, pj - pi);
        while(!deq.empty() && sums[deq.back()] >= pj)
            deq.pop_back();
        deq.push_back(j);
        if(j - deq.front() == n)
            deq.pop_front();
    }
    return ans;
}


#endif //ALGORITHM_ADVANCED_MONQUEUE_OP_H
