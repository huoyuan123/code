// 6.1 旅游规划
// 需求：在有费用的无向图上，从 s 到 d 找到“路径长度最短”的路径；若有多条最短路径，选总费用最小的。
// 解法：Dijkstra（O(N^2) 实现即可，N≤500）。维护 dist[v] 为最短长度，cost[v] 为与该最短长度对应的最小费用。
// 边权：长度、费用均为非负整数（≤500）。

#include <stdio.h>
#include <stdlib.h>

#define INF 0x3f3f3f3f

int main(void) {
    int n, m, s, d;
    if (scanf("%d %d %d %d", &n, &m, &s, &d) != 4) return 0;
    // 使用邻接矩阵（n≤500，内存可承受），方便 O(N^2) Dijkstra
    int **len = (int **)malloc(sizeof(int*) * n);
    int **fee = (int **)malloc(sizeof(int*) * n);
    if (!len || !fee) return 0;
    for (int i = 0; i < n; ++i) {
        len[i] = (int*)malloc(sizeof(int) * n);
        fee[i] = (int*)malloc(sizeof(int) * n);
        for (int j = 0; j < n; ++j) {
            len[i][j] = (i == j) ? 0 : INF;
            fee[i][j] = (i == j) ? 0 : INF;
        }
    }
    for (int i = 0; i < m; ++i) {
        int a, b, L, C;
        if (scanf("%d %d %d %d", &a, &b, &L, &C) != 4) break;
        // 无向边；若存在多重边，保留“更短长度”，若长度相同则保留“更低费用”的边
        if (L < len[a][b] || (L == len[a][b] && C < fee[a][b])) {
            len[a][b] = len[b][a] = L;
            fee[a][b] = fee[b][a] = C;
        }
    }

    int *dist = (int*)malloc(sizeof(int) * n);
    int *cost = (int*)malloc(sizeof(int) * n);
    int *vis  = (int*)malloc(sizeof(int) * n);
    if (!dist || !cost || !vis) return 0;
    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        cost[i] = INF;
        vis[i] = 0;
    }
    dist[s] = 0;
    cost[s] = 0;

    for (int it = 0; it < n; ++it) {
        int u = -1;
        for (int v = 0; v < n; ++v) {
            if (!vis[v]) {
                if (u == -1 || dist[v] < dist[u] || (dist[v] == dist[u] && cost[v] < cost[u])) {
                    u = v;
                }
            }
        }
        if (u == -1) break; // 不连通；题目保证解存在，一般不会发生
        vis[u] = 1;
        if (u == d) break; // 已确定目的地最优
        for (int v = 0; v < n; ++v) {
            if (!vis[v] && len[u][v] < INF) {
                int nd = dist[u] + len[u][v];
                int nc = cost[u] + fee[u][v];
                if (nd < dist[v]) {
                    dist[v] = nd;
                    cost[v] = nc;
                } else if (nd == dist[v] && nc < cost[v]) {
                    cost[v] = nc;
                }
            }
        }
    }

    printf("%d %d\n", dist[d], cost[d]);

    for (int i = 0; i < n; ++i) { free(len[i]); free(fee[i]); }
    free(len); free(fee); free(dist); free(cost); free(vis);
    return 0;
}
