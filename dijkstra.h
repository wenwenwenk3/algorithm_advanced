//
// Created by kai.chen on 2021/12/20.
//

//   Dijkstra 单源最短路径算法
//
//      1. 网络延迟时间
//      2. 最小体力消耗路径
//      3. 概率最大的路径

//   Floyd-Warshall 多源最短路径算法、A* 启发式搜索
//
//   Bellman-Ford 单源最短路径（可处理负权边）
//

// 请先复习图论基础：
//   0. 所有可能的路径
//   1. 环检测和拓扑排序，见算法基础：ke_cheng_biao.h
//   2. 二分图的判定，见算法基础：dfs_bfs.h
//   3. 二分图的最大匹配
//
#ifndef ALGORITHM_ADVANCED_DIJKSTRA_H
#define ALGORITHM_ADVANCED_DIJKSTRA_H
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 所有可能的路径
vector<vector<int>> ans;
vector<int> path;
void dfs(vector<vector<int>>& graph, int x, int n) {
    if (x == n) {
        ans.push_back(path);
        return;
    }
    for (auto& y : graph[x]) {
        path.push_back(y);
        dfs(graph, y, n);
        path.pop_back();
    }
}
vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
    path.push_back(0);
    dfs(graph, 0, graph.size() - 1);
    return ans;
}


// 作用于有向有权图
// 输入一幅图和一个起点 start，计算 start 到其他节点的最短距离
vector<int> dijkstra(int start, vector<list<int>>& graph);

/* ***** Dijkstra 基本框架 *******
struct State {
    int id; // 图节点的 id
    int dist; // 从 start 节点到当前节点的距离
    State(int _id, int _dist): id(_id), dist(_dist){}
}

// 返回节点 from 到节点 to 之间的边的权重
int weight(int from, int to);

// 输入节点 s 返回 s 的相邻节点
list<int> adj(int s);

// 输入一幅图和一个起点 start，计算 start 到其他节点的最短距离
vector<int> dijkstra(int start, vector<list<int>>& graph) {
    // 图中节点的个数
    int V = graph.size();
    // 记录最短路径的权重，可以理解为 dp table
    // 定义：distTo[i] 的值就是节点 start 到达节点 i 的最短路径权重
    vector<int> distTo(V, INT_MAX); // 求最小值，所以 dp table 初始化为正无穷

    // base case，start 到 start 的最短距离就是 0
    distTo[start] = 0;

    // 优先级队列，distFromStart 较小的排在前面
    priority_queue<State> pq ((a, b) -> {
        return a.dist - b.dist;
    });

    // 从起点 start 开始进行 BFS
    pq.push(State(start, 0));

    while (!pq.empty()) {
        State curState = pq.top();
        pq.pop();
        int curNodeID = curState.id;
        int curDistFromStart = curState.dist;
        //
        //         // 如果是只求从start到end的最短路径，在这里加一个判断就行了，其他代码不用改
        //        if (curNodeID == end) {
        //            return curDistFromStart;
        //        }
        //

        if (curDistFromStart > distTo[curNodeID]) {
            // 已经有一条更短的路径到达 curNode 节点了
            continue;
        }
        // 将 curNode 的相邻节点装入队列
        for (int nextNodeID : adj(curNodeID)) {
            // 看看从 start -> curNode -> nextNode 的距离是否会比 distTo[nextNodeID] 更短
            int distToNextNode = distTo[curNodeID] + weight(curNodeID, nextNodeID);
            if (distToNextNode < distTo[nextNodeID]) {
                // 更新 dp table
                distTo[nextNodeID] = distToNextNode;
                // 将这个节点以及距离放入队列
                pq.push(State(nextNodeID, distToNextNode));
            }
        }
    }
    return distTo;
}

// Q1. 没有visited集合记录已访问的节点，所以一个节点会被访问多次，会被多次加入队列，那会不会导致队列永远不为空，造成死循环？
 Answer: while循环每执行一次，都会往外拿一个元素，但想往队列里放元素，可就有很多限制了，必须满足下面这个条件：
// 看看从 curNode 达到 nextNode 的距离是否会更短
if (distTo[nextNodeID] > distToNextNode) {
    // 更新 dp table
    distTo[nextNodeID] = distToNextNode;
    pq.offer(new State(nextNodeID, distToNextNode));
}

 因为两个节点之间的最短距离（路径权重）肯定是一个确定的值，不可能无限减小下去，
 所以队列一定会空，队列空了之后，distTo数组中记录的就是从start到其他节点的最短距离。

// Q2. 为什么要用PriorityQueue而不是LinkedList实现的普通队列？
 Answer: 如果你非要用普通队列，其实也没问题的，你可以直接把PriorityQueue改成LinkedList，也能得到正确答案，但是效率会低很多。
 主要是为了效率上的优化，贪心的思路，更短的路径更可能

 // Q3. 时间复杂度O(ElogV)，其中E代表图中边的条数，V代表图中节点的个数。
 Answer: 因为理想情况下优先级队列中最多装V个节点，pq取数据添加数据操作都是logV，且对优先级队列的操作次数和E成正比

********/






// 1. 网络延迟时间
// 让你求所有节点都收到信号的时间，你把所谓的传递时间看做距离，实际上就是问你
// 「从节点k到其他所有节点的最短路径中，最长的那条最短路径距离是多少」，
// 说白了就是让你算从节点k出发到其他所有节点的最短路径，就是标准的 Dijkstra 算法。
struct State {
    int id; // 图节点的 id
    int distFromStart; // 从 start 节点到当前节点的距离
    State(int _id, int _dist): id(_id), distFromStart(_dist){}
};
struct cmp{
    bool operator()(State a, State b){
        return a.distFromStart > b.distFromStart;
    }
};
vector<int> dijkstra(int start, vector<list<pair<int, int>>>& graph) {
    // 图中节点的个数
    int V = graph.size();
    // 记录最短路径的权重，可以理解为 dp table
    // 定义：distTo[i] 的值就是节点 start 到达节点 i 的最短路径权重
    vector<int> distTo(V, INT_MAX); // 求最小值，所以 dp table 初始化为正无穷

    // base case，start 到 start 的最短距离就是 0
    distTo[start] = 0;

    // 优先级队列，distFromStart 较小的排在前面
    priority_queue<State, vector<State>, cmp> pq;
    // 从起点 start 开始进行 BFS
    pq.push(State(start, 0));

    while (!pq.empty()) {
        State curState = pq.top();
        pq.pop();
        int curNodeID = curState.id;
        int curDistFromStart = curState.distFromStart;

        if (curDistFromStart > distTo[curNodeID]) {
            continue;
        }

        // 将 curNode 的相邻节点装入队列
        for (auto neighbor : graph[curNodeID]) {
            int nextNodeID = neighbor.first;
            int distToNextNode = distTo[curNodeID] + neighbor.second;

            if (distTo[nextNodeID] > distToNextNode) {
                distTo[nextNodeID] = distToNextNode; // 更新 dp table
                pq.push(State(nextNodeID, distToNextNode));
            }
        }
    }
    return distTo;
}

int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    // 节点编号是从 1 开始的，所以要一个大小为 n + 1 的邻接表
    vector<list<pair<int, int>>> graph(n + 1);
    // 构造图
    for (auto edge : times) {
        int from = edge[0];
        int to = edge[1];
        int weight = edge[2];
        // from -> List<(to, weight)>
        // 邻接表存储图结构，同时存储权重信息
        graph[from].push_back(make_pair(to, weight));
    }
    // 启动 dijkstra 算法计算以节点 k 为起点到其他节点的最短路径
    vector<int> distTo = dijkstra(k, graph);

    // 找到最长的那一条最短路径
    int res = 0;
    for (int i = 1; i < distTo.size(); i++) {
        if (distTo[i] == INT_MAX) { // 有节点不可达，返回 -1
            return -1;
        }
        res = max(res, distTo[i]);
    }
    return res;
}


//  2. 最小体力消耗路径
// 这道题中评判一条路径是长还是短的标准不再是路径经过的权重总和，而是路径经过的权重最大值。

// 方向数组，上下左右的坐标偏移量
vector<vector<int>> dirs = {{0,1}, {1,0}, {0,-1}, {-1,0}};

// 返回坐标 (x, y) 的上下左右相邻坐标
list<pair<int, int>> adj(vector<vector<int>>& matrix, int x, int y) {
    int m = matrix.size(), n = matrix[0].size();
    // 存储相邻节点
    list<pair<int, int> neighbors;
    for (auto dir : dirs) {
        int nx = x + dir[0];
        int ny = y + dir[1];
        if (nx >= m || nx < 0 || ny >= n || ny < 0) {
            // 索引越界
            continue;
        }
        neighbors.push_back(make_pair(nx, ny));
    }
    return neighbors;
}

class State{
    // 矩阵中的一个位置
    int x, y;
    // 从起点 (0, 0) 到当前位置的最小体力消耗（距离）
    int effortFromStart;

    State(int x, int y, int effortFromStart) {
        this.x = x;
        this.y = y;
        this.effortFromStart = effortFromStart;
    }
}
struct cmp {
    bool operator()(State a, State b){
        return a.effortFromStart > b.effortFromStart;
    }
};
// Dijkstra 算法，计算 (0, 0) 到 (m - 1, n - 1) 的最小体力消耗
int minimumEffortPath(vector<vector<int>>& heights) {
    int m = heights.size(), n = heights[0].size();
    // 定义：从 (0, 0) 到 (i, j) 的最小体力消耗是 effortTo[i][j]
    vector<vector<int>> effortTo(m, vector<int>(n, INT_MAX); // dp table 初始化为正无穷

    // base case，起点到起点的最小消耗就是 0
    effortTo[0][0] = 0;

    // 优先级队列，effortFromStart 较小的排在前面
    priority_queue<State, vector<State>, cmp> pq;

    // 从起点 (0, 0) 开始进行 BFS
    pq.push(State(0, 0, 0));

    while (!pq.empty()) {
        State curState = pq.top();
        pq.pop();
        int curX = curState.x;
        int curY = curState.y;
        int curEffortFromStart = curState.effortFromStart;

        // 到达终点提前结束
        if (curX == m - 1 && curY == n - 1) {
            return curEffortFromStart;
        }

        if (curEffortFromStart > effortTo[curX][curY]) {
            continue;
        }
        // 将 (curX, curY) 的相邻坐标装入队列
        for (int[] neighbor : adj(heights, curX, curY)) {
            int nextX = neighbor[0];
            int nextY = neighbor[1];
            // 计算从 (curX, curY) 达到 (nextX, nextY) 的消耗
            int effortToNextNode = max(
                    effortTo[curX][curY],
                    Math.abs(heights[curX][curY] - heights[nextX][nextY])
            );
            // 更新 dp table
            if (effortTo[nextX][nextY] > effortToNextNode) {
                effortTo[nextX][nextY] = effortToNextNode;
                pq.offer(new State(nextX, nextY, effortToNextNode));
            }
        }
    }
    // 正常情况不会达到这个 return
    return -1;
}


// 3. 概率最大的路径
// Dijkstra 计算最短路径的正确性依赖一个前提：路径中每增加一条边，路径的总权重就会增加。

// 如果你想计算最长路径，路径中每增加一条边，路径的总权重就会减少，要是能够满足这个条件，也可以用 Dijkstra 算法。
// 看这道题是不是符合这个条件？边和边之间是乘法关系，每条边的概率都是小于 1 的，所以肯定会越乘越小。
// 只不过，这道题的解法要把优先级队列的排序顺序反过来，一些 if 大小判断也要反过来，我们直接看解法代码吧：
double maxProbability(int n, int[][] edges, double[] succProb, int start, int end) {
    List<double[]>[] graph = new LinkedList[n];
    for (int i = 0; i < n; i++) {
        graph[i] = new LinkedList<>();
    }
    // 构造邻接表结构表示图
    for (int i = 0; i < edges.length; i++) {
        int from = edges[i][0];
        int to = edges[i][1];
        double weight = succProb[i];
        // 无向图就是双向图；先把 int 统一转成 double，待会再转回来
        graph[from].add(new double[]{(double)to, weight});
        graph[to].add(new double[]{(double)from, weight});
    }

    return dijkstra(start, end, graph);
}

class State {
    // 图节点的 id
    int id;
    // 从 start 节点到达当前节点的概率
    double probFromStart;

    State(int id, double probFromStart) {
        this.id = id;
        this.probFromStart = probFromStart;
    }
}

double dijkstra(int start, int end, List<double[]>[] graph) {
    // 定义：probTo[i] 的值就是节点 start 到达节点 i 的最大概率
    double[] probTo = new double[graph.length];
    // dp table 初始化为一个取不到的最小值
    Arrays.fill(probTo, -1);
    // base case，start 到 start 的概率就是 1
    probTo[start] = 1;

    // 优先级队列，probFromStart 较大的排在前面
    Queue<State> pq = new PriorityQueue<>((a, b) -> {
        return Double.compare(b.probFromStart, a.probFromStart);
    });
    // 从起点 start 开始进行 BFS
    pq.offer(new State(start, 1));

    while (!pq.empty()) {
        State curState = pq.poll();
        int curNodeID = curState.id;
        double curProbFromStart = curState.probFromStart;

        // 遇到终点提前返回
        if (curNodeID == end) {
            return curProbFromStart;
        }

        if (curProbFromStart < probTo[curNodeID]) {
            // 已经有一条概率更大的路径到达 curNode 节点了
            continue;
        }
        // 将 curNode 的相邻节点装入队列
        for (double[] neighbor : graph[curNodeID]) {
            int nextNodeID = (int)neighbor[0];
            // 看看从 curNode 达到 nextNode 的概率是否会更大
            double probToNextNode = probTo[curNodeID] * neighbor[1];
            if (probTo[nextNodeID] < probToNextNode) {
                probTo[nextNodeID] = probToNextNode;
                pq.offer(new State(nextNodeID, probToNextNode));
            }
        }
    }
    // 如果到达这里，说明从 start 开始无法到达 end，返回 0
    return 0.0;
}



//
/* ***** Floyd-Warshall 基本框架 *******
 *
 // 用矩阵存储图，graph[i][j]表示从节点 i 到 j 的权重
 //  graph[i][i] = 0是自己的距离，graph[i][j] = INT_MAX 表示i,j不相连
 vector<vector<int>> graph;
 // dist 相当于dpTable存储定点之间的最短距离
 // 初始状态: 当 dist[i][j] = graph[i][j], 将通过迭代的方式更新最终的最短距离
 vector<vector<int>> dist;

 void floyd(){
    // 初始化 dist 表
    for(int i = 0; i < v; i++){
        for(int j = 0; j < v; j++){
            dist[i][j] = graph[i][j];
        }
    }

    // 逐次扫描，动态更新distance最短距离
    for(int k = 0; k < v; k++){
        for(int i = 0; i < v; i++){
            for(int j = 1; j < v; j++){
                if(dist[i][k]+dist[k][j] < dist[i][j]){
                    dist[i][j] = dist[i][k]+dist[k][j];
                }
            }
        }
    }

 }

 // Q1. floyd 时间复杂度是O(V^3), 不需要额外的空间。相比于 Dijkstra 有何优势？
    Answer: 如果用多次的Dijkstra算法求多源最短路径的话，显然复杂度是 V* ElogV
      对于稠密图，E接近V*(V-1), DJ 的总时间复杂度是 V^3 * logV, 显然Floyd更有优势
      对于稀疏图，E接近V，Dj的总时间复杂度是 V^2 * logV， 这种情况下，多次运行Dj更有优势
    除此之外，Dj需要pq，需要额外的内存消耗，编码复杂度也高很多

 * *****************************/

// A* 启发式搜索
//  其实就是BFS+贪心，虽然更快，但不一定最短
//  和Dj不同的是：
//     (1).优先级队列是根据f值（f(i) = g(i)+h(i)）来构建的，而Dj是根据dist值，也就是 g(i)
//      通过f值来判断哪个顶点该最先出队列，这样可以有效的避免跑偏，h(i)专业称呼是"启发函数"，f(i)是"估价函数"
//     (2).更新顶点的dist值时，也需要更新f值
//     (3).结束条件只要遍历到终点就结束，而Dj是终点出队列的时候才结束。
//

// Bellman-Ford
//  松弛：　　
//　　每次松弛操作实际上是对相邻节点的访问(相当于广度优先搜索)，第n次松弛操作保证了所有深度为n的路径最短。
//  负边权操作：
//　　   与迪科斯彻算法不同的是，迪科斯彻算法的基本操作“拓展”是在深度上寻路，而“松弛”操作则是在广度上寻路，
//    这就确定了贝尔曼-福特算法可以对负边进行操作而不会影响结果。
//  负权环判定：
//　　因为负权环可以无限制的降低总花费，所以如果发现第n次操作仍可降低花销，就一定存在负权环。
//      这一算法被认为在随机的稀疏图上表现出色，并且极其适合带有负边权的图

//  基本过程：
//      创建源顶点 v 到图中所有顶点的距离的集合 distSet，为图中的所有顶点指定一个距离值，初始均为 Infinite，源顶点距离为 0；
//计算最短路径，执行 V - 1 次遍历；
//对于图中的每条边：如果起点 u 的距离 d 加上边的权值 w 小于终点 v 的距离 d，则更新终点 v 的距离值 d；
//检测图中是否有负权边形成了环，遍历图中的所有边，计算 u 至 v 的距离，如果对于 v 存在更小的距离，则说明存在环(无向图不能用这种方法判断负环)
//
//      时间复杂度为 O(V*E)，其中 V 为顶点数量，E 为边的数量
// 基本框架：
///   n为顶点的个数，m为边的个数，edges数组储存了所有边，distance数组是源点到所有点的距离估计值，循环结束后就是最小值。
///  内层循环就是对源点与边的两个顶点进行松弛操作。
//  for (var i = 0; i < n - 1; i++) {
//    for (var j = 0; j < m; j++) {//对m条边进行循环
//      var edge = edges[j];
//      // 松弛操作
//      if (distance[edge.to] > distance[edge.from] + edge.weight ){
//        distance[edge.to] = distance[edge.from] + edge.weight;
//      }
//    }
//}


// SPFA
//  　基于Bellman-Ford之外，再可以确定，松弛操作必定只会发生在最短路径前导节点松弛成功过的节点上，用一个队列记录松弛过的节点，可以避免了冗余计算。
//

#endif //ALGORITHM_ADVANCED_DIJKSTRA_H

#if 0
树的BFS遍历框架
class State {
    // 记录 node 节点的深度
    int depth;
    TreeNode node;

    State(TreeNode node, int depth) {
        this.depth = depth;
        this.node = node;
    }
}

// 输入一棵二叉树的根节点，遍历这棵二叉树所有节点
void levelTraverse(TreeNode root) {
    if (root == null) return 0;
    Queue<State> q = new LinkedList<>();
    q.offer(new State(root, 1));

    // 遍历二叉树的每一个节点
    while (!q.isEmpty()) {
        State cur = q.poll();
        TreeNode cur_node = cur.node;
        int cur_depth = cur.depth;
        printf("节点 %s 在第 %s 层", cur_node, cur_depth);

        // 将子节点放入队列
        if (cur_node.left != null) {
            q.offer(new State(cur_node.left, cur_depth + 1));
        }
        if (cur_node.right != null) {
            q.offer(new State(cur_node.right, cur_depth + 1));
        }
    }
}
#endif
