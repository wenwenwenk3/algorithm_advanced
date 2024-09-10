//
// Created by kai.chen on 2021/12/20.
//
//  1. 单调栈
//      问题1.1. 下一个更大的元素
//      变体1.2. 更暖和的气温需要多少天
//      问题1.2. 下一个更大元素II (可循环的原数组中找)
//      问题1.3. 下一个更大的元素I（另一个数组中找）
//      问题1.4. 接雨水
//
//      问题1.5. 柱状图的最大矩形
//      问题1.6. 移掉k位数字使剩下的数字最小
//
//      问题1.7. 去除重复字母让返回结果字典序最小

//      1.8. 区间最小值乘区间和 的最大值 - lc又名巫师力量和

#ifndef ALGORITHM_ADVANCED_MONSTACK_OP_H
#define ALGORITHM_ADVANCED_MONSTACK_OP_H
#include <vector>
#include <stack>
#include <unordered_map>
#include <queue>
using namespace std;

//  1. 单调栈
// 概述：栈是个先进后出的结构。单调栈就是通过巧妙的逻辑，使得每次新添加元素后 栈的元素都是保持单调递增/递减的。
//  常常用来处理一类典型的问题："Next Greater Element" 下一个更大的元素 // 三水小姐姐卡常小技巧，可以用两个元素的数组模拟栈

// 问题1.1. 下一个更大的元素（原数组中找）
// 题目描述：输入一个数组，要求返回等长数组，对应的索引存储着下一个更大的元素，没有更大存-1
//      例如：输入: [2,1,2,4,3]  返回: [4,2,4,-1,-1]
// 思路：
//    (1)暴力解法：直接对每个元素后面进行扫描，找到第一个更大的元素。时间O(N^2)
//    (2)单调栈：想象成人的身高，这些人站成一列。每个人能看到的比他高的第一个人就是目标
//       具体做法：for循环从后往前扫描，用一个栈里面存的是每次元素的下一个更大值，如果为空表示没有。个数也最多为1
//       时间复杂度：for循环是O(N)的，push\pop 都最多1次，所以整体的复杂度是O(N)
vector<int> nextGreaterElement(vector<int>& nums){
    vector<int> res(nums.size());
    stack<int> stk; // 栈里面存的是每次元素的下一个更大值，如果为空表示没有。个数也最多为1
    // for循环从后往前扫描
    for(int i = nums.size()-1; i >= 0; i--){
        // 判定个子高矮，比当前元素矮的个子出列，被挡住了。
        while(!stk.empty() && stk.top() <= nums[i]){
            stk.pop();
        }
        // 填充 当前扫描元素的next greater elem
        res[i] = stk.empty() ? -1: stk.top(); // 如果栈不为空，即为栈顶。为空说明没有next greater elem, 填-1
        stk.push(nums[i]); // 把当前扫描元素放进栈里
    }
    return res;
}

// 变体1.2 更暖和的气温需要多少天
// 题目描述： 温度数组c存放着每天的温度，求对于每一天需要等多少天才能找到更暖和的气温。等不到填0
//      例如：c = [73,74,75,71,69,72,76,73]  返回：[1,1,4,2,1,1,0,0]
// 思路： 和上题一样，只不过这次是求到next greater ele的索引距离而已，只需要让stk存索引，而不是值。
//      具体做法：for循环从后往前扫描，用一个栈里面存的是每次元素的下一个更大值的索引，如果为空表示没有。个数也最多为1
vector<int> dailyTemperatures(vector<int>& nums){
    vector<int> res(nums.size());
    stack<int> stk; // 栈里面存的是每次元素的下一个更大值的索引
    // for循环从后往前扫描
    for(int i = nums.size()-1; i >= 0; i--){
        // 判定个子高矮，比当前元素矮的个子出列，被挡住了。
        while(!stk.empty() && nums[stk.top()]  <= nums[i]){
            stk.pop();
        }
        // 填充 当前扫描元素到next greater elem的距离
        res[i] = stk.empty() ? 0: (stk.top() - i); // 如果栈不为空，即为栈顶。为空说明没有next greater elem, 填-1
        stk.push(i); // 把当前扫描元素索引放进栈里
    }
    return res;
}
/* --go version--
func dailyTemperatures(temperatures []int) []int {
	n := len(temperatures)
	stk := make([]int, 0, n)
	res := make([]int, n)
	for i := n - 1; i >= 0; i-- {
		for len(stk) > 0 && temperatures[stk[len(stk)-1]] <= temperatures[i] {
			stk = stk[:len(stk)-1]
		}
		if len(stk) == 0 {
			res[i] = 0
		} else {
			res[i] = stk[len(stk)-1] - i
		}
		stk = append(stk, i)
	}
	return res
}
 */

// 1.2 股票的价格跨度
//   输入：["StockSpanner","next","next","next","next","next","next","next"], [[],[100],[80],[60],[70],[60],[75],[85]]
//输出：[null,1,1,1,2,1,4,6]
class StockSpanner {
private:
    stack<pair<int,int> > monoStack; // 单调栈,<price 价格, weight 宽度>
public:
    StockSpanner() {
    }
// 第一步：100入栈，此时100的跨度仅仅包含自身，跨度为1；
//第二步：80入栈，100 > 80，80的跨度仅仅包含自身，跨度为1；
//第三步：60入栈，80 > 60，60的跨度仅仅包含自身，跨度为1；
//第四步：70入栈，60 < 70，80 > 70，70的跨度包含60和自身，跨度为2（将60的跨度累加起来），此时60应该出栈，因为70后面的元素，如果大于等于70，那么肯定大于60，所以60已经没有意义了；
//第五步：60入栈，此时60的跨度仅仅包含自身，跨度为1；
//第六步：75入栈，此时60和70都应该出栈，道理和第四步一样，75的跨度为4，分别累加了70的跨度2和60的跨度1，再加上自身1，所以跨度是4；
//第七步：85入栈，此时75和80都应该出栈，道理和第四步一样，75的跨度为4，80的跨度为1，再加上自身，所以跨度是6。
    int next(int price) {
        pair<int,int> newprice(price,1);
        while(!monoStack.empty() && monoStack.top().first <= price){ // 矮个子出栈
            auto tmp = monoStack.top();
            newprice.second += tmp.second; //
            monoStack.pop();
        }
        monoStack.push(newprice);
        return newprice.second;
    }
};


// 问题1.2. 下一个更大元素 II (可循环的原数组中找)
// 题目描述：循环数组（最后一个元素的下一个元素是数组的第一个元素）
// 思路：
//      难点在于处理循环数组，在计算机里内存都是线性的。循环往往就是加上了%运算而已
//     做法：可以在原数组后面再接一个原数组，这样就可以用原来比身高的方式比了
//     至于怎么接呢？当然可以直接构造一个，然后套用上面的框架就可以。但其实这里可以不用接新数组，利用下标%技巧假装翻倍了就可以
vector<int> nextGreaterElementsCircle(vector<int>& nums){
    vector<int> res(nums.size());
    stack<int> stk;
    int n = nums.size();
    // for循环从后往前扫描, 假装数组翻倍了，长度为 2n-1;
    for(int i = 2*n-1; i >= 0; i--){
        // 判定个子高矮，比当前元素矮的个子出列，被挡住了。
        while(!stk.empty() && stk.top() <= nums[i]){
            stk.pop();
        }
        // 填充 当前扫描元素的next greater elem
        res[i%n] = stk.empty() ? -1: stk.top(); // 如果栈不为空，即为栈顶。为空说明没有next greater elem, 填-1
        stk.push(nums[i%n]); // 把当前扫描元素索引放进栈里
    }
    return res;
}
void testnextGreaterElementsCircle(){
    int a[] = {1,2,1};
    vector<int> nums(a, a+sizeof(a)/sizeof(a[0]));
    vector<int> res = nextGreaterElementsCircle(nums);
    for(auto i : res){
        cout<<i<<",";
    }
    cout<<endl;
}
// 双倍数组写法(取余技巧写法虽好但写的人多，常规翻倍写法更清晰)
vector<int> nextGreaterElements(vector<int>& nums) {
    int n = (int)nums.size();
    vector<int> dnums(n*2);
    for(int i = 0; i < n; i++){
        dnums[i] = nums[i];
        dnums[i+n] = nums[i];
    }

    stack<int> stk;
    vector<int> res(n);
    // for循环从后往前扫描, 数组翻倍了，长度为 2n-1;
    for(int i = 2*n-1; i >= 0; i--){
        // 判定个子高矮，比当前元素矮的个子出列，被挡住了。
        while(!stk.empty() && stk.top() <= dnums[i]){
            stk.pop();
        }
        // 填充 当前扫描元素的next greater elem
        res[i%n] = stk.empty() ? -1: stk.top(); // 如果栈不为空，即为栈顶。为空说明没有next greater elem, 填-1
        stk.push(dnums[i]); // 把当前扫描元素索引放进栈里
    }
    return res;
}


// 问题1.3. 下一个更大的元素I（另一个数组中找）
// 题目描述：nums1 中数字x 的下一个更大元素是指x在nums2中对应位置右侧的第一个比x 更大的元素。nums1 中的所有整数同样出现在 nums2 中
//      例如：输入：nums1 = [4,1,2], nums2 = [1,3,4,2].  输出：[-1,3,-1] // 2出现在nums2最后面
// 思路：
//   暴力解法：直接根据题意进行模拟，先找到nums1[i] 在nums2 的位置 j，
//          然后接着往后找到最近一个比其大的数，如果 j 走到结尾尚未出现合法的ans[i]，则是 -1 。时间复杂度 O(N1*N2)
//   单调栈+哈希：
//      具体做法： 由于我们目标是找到某个数其在nums2 的右边中第一个比其大的数，因此我们可以对 nums2 进行倒序扫描
//          用map来存放nums2 中每一个元素的下一个更大值 映射. 然后再遍历nums1 来map里面查就好了。时间复杂度 O(N1+N2)
vector<int> nextGreaterElementI(vector<int>& nums1, vector<int>& nums2){
    unordered_map<int,int> map;
    stack<int> stk; // 栈里面存的是每次元素的下一个更大值
    // for循环从后往前扫描nums2
    for(int i = nums2.size()-1; i >= 0; i--){
        int temp = nums2[i];
        while(!stk.empty() && nums2[stk.top()] <= temp){  // 故意漏掉写成小于，debug等于的情况
           stk.pop(); // 矮个子都不要
        }
        map[temp] = stk.empty() ? -1: stk.top();
        stk.push(temp); // // 把当前扫描元素索引放进栈里
    }
    vector<int> res(nums1.size());
    for(int i=0; i < nums1.size(); i++){
        res[i] = map[nums1[i]];  // nums1 中的所有整数同样出现在 nums2 中
    }
    return res;
}

void testnextGreaterElementI(){
    int a[] = {4,1,2};
    int b[] = {1,3,4,2};
    vector<int> nums1(a, a+sizeof(a)/sizeof(a[0]));
    vector<int> nums2(b, b+sizeof(b)/sizeof(b[0]));
    vector<int> res = nextGreaterElementI(nums1, nums2);
    for(auto i : res){
        cout<<i<<",";
    }
    cout<<endl;
}

int numberOfDays(int year, int month){
    vector<int> day = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(month != 2){
        return day[month];
    }
    // 2月
    if(year % 400 == 0 || (year%4 == 0 && year%100 != 0)){ // 闰年
        return day[2]+1;
    }
    return day[2]; // 非闰年
}

// 1.4. 接雨水
// 题目描述：给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
// 思路：
//      (1) 暴力解法: 对于每根柱子而言，我们只需要找出「其左边最高的柱子」和「其右边最高的柱子」。
//              对左右的最高柱子取一个最小值，再和当前柱子的高度做一个比较，即可得出当前位置可以接下的雨水。边缘的柱子不可能接到雨水（某一侧没有柱子）
//              整体时间复杂度为 O(n^2), 空间O(1)
//      (2) 单调栈: 可以把问题转化成，给定一个数组，如何求得任意位置的左半边的最大值和右半边的最大值。
//              时间复杂度O(N), 每个元素最多进栈和出栈一次。复杂度为 O(n), 空间O(N)
int trap_v1(vector<int>& height) {
    int n = height.size();
    int ans = 0;
    for (int i = 1; i < n - 1; i++) { // i从[1,n-1],因为边缘的柱子不可能接到雨水（某一侧没有柱子）
        int cur = height[i];

        // 获取当前位置的左边最大值
        int l = INT_MIN;
        for (int j = i - 1; j >= 0; j--) {
            l = max(l, height[j]);
        }
        if (l <= cur) continue; // 左边没有更大的柱子，没法接水

        // 获取当前位置的右边边最大值
        int r = INT_MIN;
        for (int j = i + 1; j < n; j++) {
            r = max(r, height[j]);
        }
        if (r <= cur) continue; // 右边没有更大的柱子，没法接水

        // 计算当前位置可接的雨水
        ans += min(l, r) - cur;
    }
    return ans;
}

int trap_v2(vector<int>& height) {
    int res = 0;
    stack<int> stk;
    int n = height.size();
    // for 依次扫描
    for (int i = 0; i < n; ++i) { // 3 2 1 2 3
        while (!stk.empty() && height[i] > height[stk.top()]) {
            int top = stk.top();
            stk.pop();

            if (stk.empty()) { // 如果栈内没有元素，说明当前位置左边没有比其高的柱子，跳过
                break;
            }
            // 窗口左右位置，并由左右位置得出「宽度」和「高度」
            int left = stk.top(), right = i;
            int currWidth = right - left - 1;
            int currHeight = min(height[left] - height[top], height[right] - height[top]);
            res += currWidth * currHeight;
        }
        stk.push(i);
    }
    return res;
}

// 只用两个指针竖着算，空间复杂度降到 O1
int trapV2pointer(vector<int>& height) {
    if(height.empty()) return 0;
    int n = height.size();
    int sum = 0;

    int lmax = height.front();
    int rmax = height.back();

    // 竖着考虑每一 柱子 能接多少水
    int left = 0, right = n - 1;
    while(left <= right){
        lmax = max(height[left], lmax);
        rmax = max(height[right], rmax);

        if(lmax > rmax){ // 左边最大值 大于 右边最大值，用低一点的右边作为高度接水
            sum += rmax - height[right];
            right--;
        }else{
            sum += lmax - height[left];
            left++;
        }
    }
    return sum;
}

void test_trap(){
    vector<int> h = {0,1,0,2,1,0,1,3,2,1,2,1};
    cout<<trap_v2(h)<<endl;
}

// 进阶：接雨水II，问题升级为三维
//  首先，最外面一层，外面是没有方块的，是无法接水的，与最外一层相邻方块，也被最外一层的“影响”，和“短板效应”的思想有点相似，因此我们可以利用最外一层作为突破口，然后往里面找。
//  我们可以用最短路算法Dijkstra算法解决该问题，DijkstraDijkstra利用堆(优先队列)维护了一个有序结构，每次拿出最短的边，我们可以利用最短路思想，把边迁移为“方块的高度”，进而解决我们这个问题
//
// 最短路找的过程有几个注意点：
//      *优先从矮的的方块开始找，一个格子周围有四个方块，但最终的盛水量，取决于最矮一个。
//      *计算完当前方块盛水量，要更新当前点的高度，h[i][j] = max(heightMap[i][j], min(h[i−1][j],h[i+1][j],h[i][j−1],h[i][j+1]) )。
priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> > > > pq;
bool vis[201][201];
int dirx[4] = {0, 0, 1, -1};
int diry[4] = {1, -1, 0, 0};
int trapRainWaterII(vector<vector<int>>& heightMap) {
    int n = heightMap.size(), m = heightMap[0].size(), ans = 0;

    // 处理最边缘的方块，将其push入pq
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
                pq.push(make_pair(heightMap[i][j], make_pair(i, j)));
                vis[i][j] = true;
            }
        }
    }

    while (!pq.empty()) {
        int h = pq.top().first, x = pq.top().second.first, y = pq.top().second.second;
        pq.pop();
        for (int i = 0; i < 4; i++) {
            int nx = dirx[i] + x, ny = diry[i] + y;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && (!vis[nx][ny])) {
                if (heightMap[nx][ny] < h) {
                    ans += h - heightMap[nx][ny];
                }
                vis[nx][ny] = true;
                pq.push(make_pair(max(heightMap[nx][ny], h), make_pair(nx, ny)));
            }
        }
    }
    return ans;
}



// 1.5. 柱状图的最大矩形
// 在确定一个柱形的面积的时候，除了右边要比当前严格小，其实还蕴含了一个条件，那就是左边也要比当前高度严格小。
//思路：
// (1)对于一个高度，如果能得到向左和向右的边界
// (2)那么就能对每个高度求一次面积
// (3)遍历所有高度，即可得出最大面积
// 当确定了一个柱形的高度的时候，我们就将它从栈顶移出，所有的柱形在栈中进栈一次，出栈一次，
// 一开始栈为空，最后也一定要让栈为空，表示这个高度数组里所有的元素都考虑完了。
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    vector<int> left(n), right(n);

    // 找每一个h[i]的left边界
    stack<int> mono_stack;
    for (int i = 0; i < n; ++i) {
        while (!mono_stack.empty() && heights[mono_stack.top()] >= heights[i]) {
            mono_stack.pop();
        }
        left[i] = (mono_stack.empty() ? -1 : mono_stack.top());
        mono_stack.push(i);
    }

    // 找每一个h[i]的right边界
    mono_stack = stack<int>();
    for (int i = n - 1; i >= 0; --i) {
        while (!mono_stack.empty() && heights[mono_stack.top()] >= heights[i]) {
            mono_stack.pop();
        }
        right[i] = (mono_stack.empty() ? n : mono_stack.top());
        mono_stack.push(i);
    }

    int ans = 0;
    for (int i = 0; i < n; ++i) {
        ans = max(ans, (right[i] - left[i] - 1) * heights[i]);
    }
    return ans;
}// 时间复杂度O(N),空间O(N). (主要优化就是单调栈找左右边界从暴力的O(N)优化成了O(1))


// 1.6. 移掉k位数字使剩下的数字最小
// 题目描述：给你一个以字符串表示的非负整数 num 和一个整数 k ，移除这个数中的 k 位数字，使得剩下的数字最小。请你以字符串形式返回这个最小的数字。
//      ( num 的长度小于 10002 且  ≥ k。num 不会包含任何前导零。)
// 例如:    输入: num = "1432219", k = 3
//         输出: "1219"
// 思路：单调栈
//从左至右扫描，当前扫描的数还不确定要不要删，入栈暂存。
//123531这样「高位递增」的数，肯定不会想删高位，会尽量删低位。
//432135这样「高位递减」的数，会想干掉高位，直接让高位变小，效果好。
//所以，如果当前遍历的数比栈顶大，符合递增，是满意的，让它入栈。
//如果当前遍历的数比栈顶小，栈顶立刻出栈，不管后面有没有更大的 (因为栈顶的数属于高位，删掉它，小的顶上，高位变小，效果好于低位变小。)
//
// "1432219"  k = 3
//bottom[1       ]top		1入
//bottom[1 4     ]top		4入
//bottom[1 3     ]top	4出	3入
//bottom[1 2     ]top	3出	2入
//bottom[1 2 2   ]top		2入
//bottom[1 2 1   ]top	2出	1入	出栈满3个，停止出栈
//bottom[1 2 1 9 ]top		9入
string removeKdigits(string num, int k) {
    stack<char> monstk;
    for(char & c : num){
        while(k > 0 && !monstk.empty() && monstk.top() > c){
            monstk.pop();
            k--;
        }
        monstk.push(c);
    }
    while(k--){
        monstk.pop();
    }
    string str = "";
    while(!monstk.empty()){
        str += monstk.top();
        monstk.pop();
    }

    reverse(str.begin(), str.end());
    bool iscontinous0 = true;
    string res;
    for(char c : str){
        if(c == '0' && iscontinous0){
            continue;
        }
        iscontinous0 = false;
        res+=c;
    }

    return res=="" ? "0":res;
}

// 问题1.7. 去除重复字母让返回结果字典序最小
//题目描述
// 要求一、要去重。
// 要求二、去重字符串中的字符顺序不能打乱 s 中字符出现的相对顺序。
// 要求三、在所有符合上一条要求的去重字符串中，字典序最小的作为最终结果。
//https://leetcode-cn.com/problems/remove-duplicate-letters/solution/you-qian-ru-shen-dan-diao-zhan-si-lu-qu-chu-zhong-/
string removeDuplicateLetters(string s) {
    stack<char> stk;
    bool instack[256];
    memset(instack, 0, sizeof(instack));

    vector<int> count(256, 0);
    for (int i = 0; i < s.length(); i++) {
        count[s[i]-'0']++;
    }

    for(const auto& c : s){
        count[c-'0']--;
        if(instack[c-'0'] == false){
            while(!stk.empty() && c < stk.top()){
                if(count[stk.top()-'0'] > 0) {
                    instack[stk.top()-'0'] = false;
                    stk.pop();
                } else break;
            }
            stk.push(c);
            instack[c-'0'] = true;
        }
    }
    string res;
    while(!stk.empty()){
        res += stk.top();
        stk.pop();
    }

    reverse(res.begin(), res.end());
    return res;
}


// 1.8. 区间最小值乘区间和 的最大值
// 暴力解法：题目是找max(区间和 * 区间最小值)，枚举的每个元素（设为x）作为区间最小值，
//          往左右扩展直到x不再是最小值。并同时记录区间和，的最小. 时间复杂度 N^2
// 最小栈： 方法一中找每个元素左右边界的复杂度是O(N)，通过单调栈的数据结构可以将其优化为O(1)。
//         整体时间复杂度 O(N)
using LL = long long;
static constexpr int mod = 1000000007;
int maxSumMinProduct(vector<int>& nums) {
    int n = nums.size();
    // 数组 left 初始化为 0，数组 right 初始化为 n-1
    // 设置为元素不存在时的特殊值
    vector<int> left(n), right(n, n - 1);
    // 单调栈
    stack<int> s;
    for (int i = 0; i < n; ++i) {
        while (!s.empty() && nums[s.top()] >= nums[i]) {
            // 这里的 right 是非严格定义的，right[i] 是右侧最近的小于等于 nums[i] 的元素下标
            right[s.top()] = i - 1;
            s.pop();
        }
        if (!s.empty()) {
            // 这里的 left 是严格定义的，left[i] 是左侧最近的严格小于 nums[i] 的元素下标
            left[i] = s.top() + 1;
        }
        s.push(i);
    }

    // 前缀和
    vector<LL> pre(n + 1);
    for (int i = 1; i <= n; ++i) {
        pre[i] = pre[i - 1] + nums[i - 1];
    }

    LL best = 0;
    for (int i = 0; i < n; ++i) {
        best = max(best, (pre[right[i] + 1] - pre[left[i]]) * nums[i]);
    }
    return best % mod;
}




#endif //ALGORITHM_ADVANCED_MONSTACK_OP_H
