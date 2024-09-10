//
// Created by kai.chen on 2021/12/23.
//      并查集
//
//      1. 岛屿数量问题（替换XO）
//      2. 等式方程的可满足性
//      3. 以图判树
//
//
#ifndef ALGORITHM_ADVANCED_UNION_FIND_H
#define ALGORITHM_ADVANCED_UNION_FIND_H
#include <vector>

using namespace std;

// 基本思想
//class UnionFind {
//    typedef Node int
//public:
//    // 将p q连通
//    void union(Node p, Node q);
//    // 判断p q是否连通
//    bool connected(Node p, Node q);
//    // 返回连通分量个数
//    int count();
//};

// 具体实现
//  (1) parent数组记录每一个节点的父节点
//  (2) size记录重量 (可选优化) 带权可以解决更高阶的问题
//  (3) find要路径压缩，保证高度维持在常数
class UnionFind{
private:
    int _count;
    vector<int> _parent; // 数组实现森林，基于每个节点只有一个父节点指向
    vector<int> _size;
public:
    // 构造函数
    explicit UnionFind(int n){
        // 一开始互不连通
        _count = n;
        _parent.resize(n);
        _size.resize(n);
        // 父节点初始指向自己
        for (int i = 0; i < n; ++i) {
            _parent[i] = i;
            _size[i] = 1;
        }
    }
    // find的功能是：向上找到树根，即x的根节点。时间复杂度是树的高度，最差O(N)
    int find_basic(int x){
        // 只有根节点的parent[x] = x
        while(_parent[x] != x){
            x = _parent[x];
        }
        return x;
    }
    void union_basic(int p, int q){
        // 如果两个节点被连通，只要让其中一个节点的根节点接到另一个节点的根节点上就行
        int rootP = find(p);
        int rootQ = find(q);
        // 如果有相同的根节点说明pq连通
        if(rootP == rootQ) return ;
        // 合并p q两棵树
        _parent[rootP] = rootQ; // 或者可以_parent[rootQ] = rootP
        _count--;
    }

    // 优化1：为了降低find的时间复杂度最差O(N),需要让树尽量平衡，不要退化成链表。
    // 上面只是 简单的让 p接在了q下面,很可能出现"头重脚轻"的情况。
    //  所以希望将小一些的树接到大一些的树下面,这样find可以达到log(N)
    void Union(int p, int q){
        int rootP = find(p);
        int rootQ = find(q);
        // 如果有相同的根节点说明pq连通
        if(rootP == rootQ) return ;
        // 小树接到大树下面
        if(_size[rootP] > _size[rootQ]){
            _parent[rootQ] = rootP;
            _size[rootP] += _size[rootQ];
        } else{
            _parent[rootP] = rootQ;
            _size[rootQ] += _size[rootP];
        }
        _count--;
    }
    // 优化2：调用find 每次遍历根节点同时，顺手将树的高度缩短，使树的高度永远不会超过3。超过了就被缩短。
    int find(int x){
        // 只有根节点的parent[x] = x
        while(_parent[x] != x){
            _parent[x] = _parent[_parent[x]];
            x = _parent[x];
        }
        return x;
    }

    int count(){
        return _count;
    }
    bool connected(int p, int q){
        int rootP = find(p);
        int rootQ = find(q);
        return rootP == rootQ;
    }
}; // 总结，union-find的时间复杂度：构造初始化需要O(N), union/connected/count的时间复杂度都是O(1)
// (其实是反阿克尔曼函数，增长速度趋近于O(1)，find复杂度公式比较难推倒，但基本上趋近logn)



// 1. 替换XO
// 被围绕的区域(岛屿填充) ，
//      X能把O围了，但任何边界上的 O 都不会被填充为 X
//
void solveXO(vector<vector<char>>& board){
    if (board.empty()) return;
    int m = board.size();
    int n = board[0].size();

    UnionFind uf(m * n + 1); // 给dummy留一个位置 m*n
    int dummy = m*n;

    // 将四周的'O'和dummy连通
    for(int i = 0; i < n; i++){
        if(board[i][0] == 'O'){ // 第一列
            uf.Union(i*n, dummy);
        }
        if(board[i][n-1] == 'O'){ // 最后一列 (a[i][j] ==> a[i*n+j])
            uf.Union(i*n + n-1, dummy);
        }
    }
    for(int j = 0; j < n; j++){
        if(board[0][j] == 'O'){ // 第一行
            uf.Union(j, dummy);
        }
        if(board[m-1][j] == 'O'){ // 最后一行
            uf.Union((m-1)*n + j, dummy);
        }
    }

    // 方向数组；搜索上下左右的常用伎俩
    int d[4][2] = {{1,0}, {0,1},{0,-1},{-1,0}}; // 定义四个方向向量

    for(int i = 1; i < m-1; i++){
        for(int j = 1; j < n-1; j++){
            if(board[i][j] == 'O'){
                // 将四个方向的'O'连通
                for(int k = 0; k < 4; k++){
                    int x = i+d[k][0], y = j+d[k][1];
                    if(board[x][y] == 'O'){
                        uf.Union(x*n + y, i*n + j); // 连通(xy),(ij)
                    }
                }
            }
        }
    }

    // 到这里，union-find集就已经建好了。dummy连通着所有没有被包围的O，剩下的O都可以被替换
    for(int i = 1; i < m-1; i++){
        for(int j = 1; j < n-1; j++){
            if(!uf.connected(dummy, i*n + j)){
                board[i][j] = 'X';
            }
        }
    }
}


// 2. 等式方程的可满足性
// 题目描述：字符串数组equations
//    例如：输入：["a==b", "b!=c", "c==a"]  输出：false 解释：三个等式不可能同时满足
// 思路：
//      这是经典的动态连通性问题，具有：自反性、传递性、对称性
//    做法：将算式分成==和!=两部分，先处理==算式 建立并查集，再依次判断!=是否破坏关系
class UnionFindEQ {
private:
    int _count;
    vector<int> _parent;
public:
    explicit UnionFindEQ(int n) {
        _count = n;
        _parent.resize(n);
    }

    int find(int x){
        // 只有根节点的parent[x] = x
        while(_parent[x] != x){
            _parent[x] = _parent[_parent[x]];
            x = _parent[x];
        }
        return x;
    }

    void Union(int p, int q) {
        int rootP = find(p), rootQ = find(q);
        if(rootP == rootQ) return ;
        // 合并p q两棵树
        _parent[rootP] = rootQ;
        _count--;
    }

    int count(){
        return _count;
    }

    bool connected(int p, int q){
        int rootP = find(p);
        int rootQ = find(q);
        return rootP == rootQ;
    }
};

bool equationsPossible(vector<string>& equations) {
    UnionFindEQ uf(26);
    // 用相等的字母 建立UnionFind集
    for (const string& str: equations) {
        if (str[1] == '=') {
            int p = str[0] - 'a';
            int q = str[3] - 'a';
            uf.Union(p, q);
        }
    }
    // 检查不等关系的成立
    for (const string& str: equations) {
        if (str[1] == '!') {
            int x = str[0];
            int y = str[3];
            // 如果已经有相等关系，存在冲突
            if (uf.connected(x - 'a', y - 'a')) {
                return false;
            }
        }
    }
    return true;
}

// 3. 以图判树
// 题目描述：给你输入编号从0到n - 1的n个结点，和一个无向边列表edges（每条边用节点二元组表示），
//  请你判断输入的这些边组成的结构是否是一棵树。
//  例如：输入：n = 5 edges = [[0,1], [0,2], [0,3], [1,4]] 输出：true  解释：这些边构成的是一棵树
//
// 思路：思考一下，什么情况下加入一条边会使得树变成图？
// 分析：对于添加的这条边，如果该边的两个节点本来就在同一连通分量里，那么添加这条边会产生环；
//  反之，如果该边的两个节点不在同一连通分量里，则添加这条边不会产生环。
// 关键就是判断 是否连通，不就是UnionFind的拿手好戏

// 判断输入的若干条边是否能构造出一棵树结构
bool validTree(int n, vector<vector<int>> edges) {
    UnionFind uf(n); // 初始化 0...n-1 共 n 个节点
    // 遍历所有边，将组成边的两个节点进行连接
    for (auto edge : edges) {
        int u = edge[0];
        int v = edge[1];
        // 若两个节点已经在同一连通分量中，会产生环
        if (uf.connected(u, v)) {
            return false;
        }
        // 这条边不会产生环，可以是树的一部分
        uf.Union(u, v);
    }
    // 要保证最后只形成了一棵树，即只有一个连通分量
    return uf.count() == 1;
}

#endif //ALGORITHM_ADVANCED_UNION_FIND_H
// 参考资料：https://www.bilibili.com/video/BV1BJ41137qp?p=1&vd_source=0b16c952d9a60a452d6365bb2d227f4a
