// 5.1 路径判断
// 输入：N E，接着 E 行无向边(u v)，最后一行两个顶点 i j。
// 输出：判断 i 与 j 间是否存在路径。
// 约束：0 < N ≤ 10；顶点编号 0..N-1；E 未给上限（很小）。
// 方法：构建邻接表 + BFS (或并查集)。此处用 BFS。
// 特殊：i==j 直接认为存在路径。

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int N, E;
    if (scanf("%d %d", &N, &E) != 2) return 0;
    if (N <= 0) { // 没有顶点，后续输入无意义
        return 0;
    }
    // 邻接表：每个顶点的动态数组存放邻接点
    int *deg = (int *)calloc(N, sizeof(int));
    int **adj = (int **)calloc(N, sizeof(int *));
    if (!deg || !adj) return 0;

    // 先统计度数以便一次性分配（两遍法）
    int *edges_u = (int *)malloc(sizeof(int) * E);
    int *edges_v = (int *)malloc(sizeof(int) * E);
    if (!edges_u || !edges_v) return 0;
    for (int e = 0; e < E; ++e) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) { E = e; break; }
        edges_u[e] = u; edges_v[e] = v;
        if (u >=0 && u < N) ++deg[u];
        if (v >=0 && v < N) ++deg[v];
    }
    for (int i = 0; i < N; ++i) {
        if (deg[i] > 0) {
            adj[i] = (int *)malloc(sizeof(int) * deg[i]);
        } else {
            adj[i] = NULL;
        }
        deg[i] = 0; // 重置用于写入
    }
    for (int e = 0; e < E; ++e) {
        int u = edges_u[e], v = edges_v[e];
        if (u>=0 && u<N) adj[u][deg[u]++] = v;
        if (v>=0 && v<N) adj[v][deg[v]++] = u;
    }
    free(edges_u); free(edges_v);

    int s, t;
    if (scanf("%d %d", &s, &t) != 2) {
        // 输入不完整
        for (int i=0;i<N;++i) free(adj[i]);
        free(adj); free(deg);
        return 0;
    }

    if (s == t) {
        printf("There is a path between %d and %d.\n", s, t);
        for (int i=0;i<N;++i) free(adj[i]);
        free(adj); free(deg);
        return 0;
    }

    // BFS
    int *vis = (int *)calloc(N, sizeof(int));
    int *queue = (int *)malloc(sizeof(int) * N);
    int head = 0, tail = 0;
    vis[s] = 1;
    queue[tail++] = s;
    int found = 0;
    while (head < tail) {
        int u = queue[head++];
        for (int k = 0; k < deg[u]; ++k) {
            int v = adj[u][k];
            if (!vis[v]) {
                vis[v] = 1;
                if (v == t) { found = 1; head = tail; break; }
                queue[tail++] = v;
            }
        }
    }

    if (found)
        printf("There is a path between %d and %d.\n", s, t);
    else
        printf("There is no path between %d and %d.\n", s, t);

    // 释放
    for (int i=0;i<N;++i) free(adj[i]);
    free(adj); free(deg); free(vis); free(queue);
    return 0;
}
