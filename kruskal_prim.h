//
// Created by kai.chen on 2022/1/9.
//
//  最小生成树
//      所谓最小生成树，就是图中若干边的集合（我们后文称这个集合为mst，最小生成树的英文缩写），你要保证这些边：
//          (1)、包含图中的所有节点。
//          (2)、形成的结构是树结构（即不存在环）。
//          (3)、权重和最小。
//
//      1. 最低成本连通所有城市
//      2. 连接所有点的最小费用

//      Kruskal 算法思路：
//          贪心的思想，从权重最小的边开始，选择合适的边加入mst集合，这样挑出来的边组成的树就是权重和最小的
//      将所有边按照权重从小到大排序，从权重最小的边开始遍历，
//      如果这条边和mst中的其它边不会形成环，则这条边是最小生成树的一部分，将它加入mst集合；
//          否则，这条边不是最小生成树的一部分，不要把它加入mst集合。环检测用UF
//      时间复杂度： O(m*log(m)) m边的条数

//
//      Prim 算法思路：
//          基于"切分定理"每次把权重最小的「横切边」拿出来加入最小生成树，直到把构成最小生成树的所有边都切出来为止。
//      总时间复杂度是O(ElogE)：复杂度主要在priority_queue的操作上 那么最多操作O(E)次pq。
//      每次操作优先级队列的时间复杂度取决于队列中的元素个数，取最坏情况就是O(logE)
#ifndef ALGORITHM_ADVANCED_KRUSKAL_PRIM_H
#define ALGORITHM_ADVANCED_KRUSKAL_PRIM_H
#include <list>
#include <algorithm>
#include <functional>
#include "union_find.h"
using namespace std;

// 1. 最低成本连通所有城市
// connections(city1, city2, cost) = [[1,2,5],[1,3,6],[2,3,1]]
bool compare(vector<int> a, vector<int> b){
    return a[2]<b[2];
}
int minimumCost(int n, vector<vector<int>> connections) {
    // 城市编号为 1...n，所以初始化大小为 n + 1
    UnionFind uf(n + 1);
    // 对所有边按照权重从小到大排序
    sort(connections.begin(), connections.end(), compare);
    // 记录最小生成树的权重之和
    int mst = 0;
    for (auto edge : connections) {
        int u = edge[0];
        int v = edge[1];
        int weight = edge[2];
        // 若这条边会产生环，则不能加入 mst
        if (uf.connected(u, v)) {
            continue;
        }
        // 若这条边不会产生环，则属于最小生成树
        mst += weight;
        uf.Union(u, v);
    }
    // 保证所有节点都被连通
    // 按理说 uf.count() == 1 说明所有节点被连通
    // 但因为节点 0 没有被使用，所以 0 会额外占用一个连通分量
    return uf.count() == 2 ? mst : -1;
}



// 2. 连接所有点的最小费用
// (xi,yi) -> (xj,yj) 的费用为 deltaX + deltaY
// 例如： 输入：points = [[0,0],[2,2],[3,10],[5,2],[7,0]] 输出：20
int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size();
    // 生成所有边及权重
    vector<vector<int>> edges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int xi = points[i][0], yi = points[i][1];
            int xj = points[j][0], yj = points[j][1];
            // 用坐标点在 points 中的索引表示坐标点
            edges.push_back(vector<int>{
                    i, j, abs(xi - xj) + abs(yi - yj)
            });
        }
    }
    // 将边按照权重从小到大排序
    sort(edges.begin(), edges.end(), compare);
    // 执行 Kruskal 算法
    int mst = 0;
    UnionFind uf(n);
    for (auto edge : edges) {
        int u = edge[0];
        int v = edge[1];
        int weight = edge[2];
        // 若这条边会产生环，则不能加入 mst
        if (uf.connected(u, v)) {
            continue;
        }
        // 若这条边不会产生环，则属于最小生成树
        mst += weight;
        uf.Union(u, v);
    }
    return mst;
}

void testminCostConnectPoints(){
    vector<vector<int>> points = {{0,0},{2,2},{3,10},{5,2},{7,0}};
    cout<< minCostConnectPoints(points)<<endl;
}


// *****************************************************************************************
//              Prim 算法
// *****************************************************************************************
struct Edge{
    int from;
    int to;
    int weight;
    Edge(int _from, int _to, int _weight): from(_from), to(_to), weight(_weight) {}
};


struct cmp {
    bool operator()(Edge a, Edge b){
        return a.weight > b.weight;
    }
};
class Prim {
private:
    priority_queue<Edge> pq; // 核心数据结构，存储「横切边」的优先级队列
    vector<bool> inMST; // 类似 visited 数组的作用，记录哪些节点已经成为最小生成树的一部分
    int weightSum = 0; // 记录最小生成树的权重和

    // graph 是用邻接表表示的一幅图，
    // graph[s] 记录节点 s 所有相邻的边，
    // 三元组 int[]{from, to, weight} 表示一条边
    vector<list<Edge>> graph;

public:
    Prim(vector<list<Edge>> Graph) {
        graph = Graph;
        priority_queue<Edge, vector<Edge>, cmp> pq;
        // 图中有 n 个节点
        int n = graph.size();
        inMST.resize(n);

        // 随便从一个点开始切分都可以，我们不妨从节点 0 开始
        inMST[0] = true;
        cut(0);
        // 不断进行切分，向最小生成树中添加边
        while (!pq.empty()) {
            Edge edge = pq.top();
            pq.pop();
            int to = edge.to;
            int weight = edge.weight;
            if (inMST[to]) {
                // 节点 to 已经在最小生成树中，跳过
                // 否则这条边会产生环
                continue;
            }
            // 将边 edge 加入最小生成树
            weightSum += weight;
            inMST[to] = true;
            // 节点 to 加入后，进行新一轮切分，会产生更多横切边
            cut(to);
        }
    }

    // 切分节点s，即将 s 的横切边加入优先队列
    void cut(int s) {
        // 遍历 s 的邻边
        for (auto edge : graph[s]) {
            int to = edge.to;
            if (inMST[to]) {
                // 相邻接点 to 已经在最小生成树中，跳过
                // 否则这条边会产生环
                continue;
            }
            // 加入横切边队列
            pq.push(edge);
        }
    }

    // 最小生成树的权重和
    int getWeightSum() {
        return weightSum;
    }

    // 判断最小生成树是否包含图中的所有节点
    bool allConnected() {
        for(int i = 0; i < inMST.size(); i++) {
            if (!inMST[i]) return false;
        }
        return true;
    }
};


// 用 Prim算法 重写：1. 最低成本连通所有城市     2. 连接所有点的最小费用
vector<list<Edge>> buildGraphByConnections(int n, vector<vector<int>>& points) {
    vector<list<Edge>> graph(n);
    for(int i = 0; i < n; i++){
        graph[i].push_back(Edge(points[i][0], points[i][1], points[i][2]));
    }
    return graph;
}
// Prim 求 最低成本连通所有城市
int minimumCost_vPrim(int n, vector<vector<int>>& connections) {
    // 转化成无向图邻接表的形式
    vector<list<Edge>> graph = buildGraphByConnections(n, connections);
    // 执行 Prim 算法
    Prim prim(graph);

    if(!prim.allConnected()) { // 当最小生成树无法覆盖所有节点
        return -1;
    }
    return prim.getWeightSum();
}

vector<list<Edge>> buildGraphByPoints(int n, vector<vector<int>>& points) {
    vector<list<Edge>> graph(n);
    // 生成所有边及权重
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int xi = points[i][0], yi = points[i][1];
            int xj = points[j][0], yj = points[j][1];
            int weight = abs(xi - xj) + abs(yi - yj);
            // 用 points 中的索引表示坐标点
            graph[i].push_back(Edge(i, j, weight));
            graph[j].push_back(Edge(j, i, weight));
        }
    }
    return graph;
}

// Prim 求 连接所有点的最小费用
int minCostConnectPoints_vPrim(vector<vector<int>>& points) {
    int n = points.size();
    vector<list<Edge>> graph = buildGraphByPoints(n, points);
    return Prim(graph).getWeightSum();
}




#endif //ALGORITHM_ADVANCED_KRUSKAL_PRIM_H
