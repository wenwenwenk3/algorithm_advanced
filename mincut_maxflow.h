//
// Created by kai.chen on 2022/1/13.
//
//      1. Hungarian 匈牙利算法： 二分图的最大匹配数
//
//
//              最大流问题中，使从起点s到终点t的流量达到最大
//          残存网络描述了图中各边的剩余容量以及可以通过“回流”删除的流量大小。
//          在Ford-Fulkerson方法中，正是通过在残存网络中寻找一条从s到t的增广路径，并对应这条路径上的各边对流网络中的各边的流进行修改。
//          如果路径上的一条边存在于流网络中，那么对该边的流增加，否则对其反向边的流减少。增加或减少的值是确定的，就是该增广路径上值最小的边。
///
///     2. Ford-Fulkerson
///     定理：当残存网络中不存在一条从s到t的增广路径，那么该图已经达到最大流。

//      3. Tarjan, Kosaraju 算法 求强连通分量
//
#ifndef ALGORITHM_ADVANCED_MINCUT_MAXFLOW_H
#define ALGORITHM_ADVANCED_MINCUT_MAXFLOW_H

// 1. Hungarian 匈牙利算法： 二分图的最大匹配数
//    -- （König定理）求最小点覆盖数：一个二分图中的最大匹配数等于这个图中的最小点覆盖数。https://zhuanlan.zhihu.com/p/96229700
int M, N;            //M, N分别表示左、右侧集合的元素数量
int Map[MAXM][MAXN]; //邻接矩阵存图
int p[MAXN];         //记录当前右侧元素所对应的左侧元素
bool vis[MAXN];      //记录右侧元素是否已被访问过
bool match(int i){
    for (int j = 1; j <= N; ++j)
        if (Map[i][j] && !vis[j]){ //有边且未访问
            vis[j] = true;                 //记录状态为访问过
            if (p[j] == 0 || match(p[j])){ //如果暂无匹配，或者原来匹配的左侧元素可以找到新的匹配
                p[j] = i;    //当前左侧元素成为当前右侧元素的新匹配
                return true; //返回匹配成功
            }
        }
    return false; //循环结束，仍未找到匹配，返回匹配失败
}
int Hungarian(){
    int cnt = 0;
    for (int i = 1; i <= M; ++i){
        memset(vis, 0, sizeof(vis)); //重置vis数组
        if (match(i))
            cnt++;
    }
    return cnt;
}


// Ford-Fulkerson



#endif //ALGORITHM_ADVANCED_MINCUT_MAXFLOW_H
