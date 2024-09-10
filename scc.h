//
// Created by kai.chen on 2022/8/20.
//

#ifndef ALGORITHM_ADVANCED_SCC_H
#define ALGORITHM_ADVANCED_SCC_H
#include <bits/stdc++.h>

using namespace std;

int map[511][511]; // 邻接矩阵
int nmap[511][511];
int visited[501];
stack<int> S;
int N;

int DFS1(int  v) {
    visited[v] = 1;
    for (int i = 1;i <= N;i++) {
        if (!visited[i] && map[v][i]){
            DFS1( i );
        }
    }
    S.push( v );
    return 0;
}

int DFS2( int v ){
    visited[v] = 1;
    for (int i = 1;i <= N;i++){
        if ( !visited[i] && nmap[v][i] ){
            DFS2( i );
        }
    }
    return 0;
}

int kosaraju() { /* Kosaraju求强连通分量 */
    while (!S.empty()) S.pop();
    memset(visited,0,sizeof(visited));
    for (int i = 1;i <= N;i++)
        if (!visited[i]) DFS1( i );
    int t = 0;
    memset(visited,0,sizeof(visited));
    while (!S.empty()){
        int v = S.top();
        S.pop();
        print f( "|%d|", v );
        if (!visited[v]){
            t++;
            DFS2( v );
        }
    }
    return t;
}

int main(){
    int M,s,e;
    scanf("%d%d",&N,&M);
    memset(map,0,sizeof(map) );
    memset(nmap,0,sizeof(nmap) );
    for (int i = 0; i < M; i++){
        scanf("%d%d",&s,&e);
        map[s][e] = 1;
        nmap[e][s] = 1;
    }
    printf("%d\n", kosaraju()); /* 输出连通分量个数 */
    return 0;
}

#endif //ALGORITHM_ADVANCED_SCC_H
