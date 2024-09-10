//
// Created by kai.chen on 2022/1/13.
//

#ifndef ALGORITHM_ADVANCED_HUFFMAN_GREY_CODE_H
#define ALGORITHM_ADVANCED_HUFFMAN_GREY_CODE_H
using namespace std;
#include <vector>


// 1. 格雷编码
// 题目描述：n 位格雷码序列 是一个由 2n 个整数组成的序列，其中：
//      每对 相邻 整数的二进制表示 恰好一位不同 ，且第一个 和 最后一个 整数的二进制表示 恰好一位不同
//   给你一个整数 n ，返回任一有效的 n 位格雷码序列 。
//  输入：n = 2
//  输出：[0,1,3,2]
//  解释：
//[0,1,3,2] 的二进制表示是 [00,01,11,10] 。
//- 00 和 01 有一位不同
//- 01 和 11 有一位不同
//- 11 和 10 有一位不同
//- 10 和 00 有一位不同

// 思路：
// 一位格雷码只有两个元素，【1， 0】
//因为格雷码 n 每增加1，包含的数字会翻倍，这里我们设n位格雷码包含c个数，前一个n为n'，所以c = 2c'
//所以这时n中的前c'个数是n'中的所有数字前面补0，相当于全部都是n`中的数字
//n = 2  [ 00,  01,  11,  10]
//n = 3  [000, 001, 011, 010] (前四个数)
//这时n中的后c'个数是n'中的所有数字前面补1，然后变为逆序
//n = 2  [ 00,  01,  11,  10]
//补   1 [100, 101, 111, 110]
//逆  序 [110, 111, 101, 100] （后四个数）
//结果拼接
// n = 3  [000, 001, 011, 010, 110, 111, 101, 100]
vector<int> grayCode(int n) {
    vector<int> res;
    // res.reserve(1 << n);// 指定数组的capacity为 2^n
    res.push_back(0); // 第一个数0

    for (int i = 1; i <= n; i++) {
        int m = res.size();
        // 对于已知n-1位格雷码结果，来求n位格雷码结果，需要在后面push_back 进去 m 位（数字前面补1，然后变为逆序）
        for (int j = m - 1; j >= 0; j--) {
            res.push_back(res[j] | (1 << (i - 1)));
        }
    }
    return res;
}
// 时间复杂度：O(2^n)。递推过程的时间复杂度为 sum(i=1..n) 2^{i-1} = O(2^n)
// 空间复杂度：O(1)。注意返回值不计入空间复杂度。




#endif //ALGORITHM_ADVANCED_HUFFMAN_GREY_CODE_H
