// 5.2 六度空间
// 题意：给定无向图（1..N），对每个结点计算距离<=6（含自身）可达的结点占总数百分比，保留两位小数。
// 方法：对每个源点做一次限定深度 BFS（剪枝：深度>=6不再展开）；统计访问数。
// 复杂度：最坏 O(N*(N+M))，N≤1000，M≤33N，仍可接受（约 34,000,000 级别）。

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int N, M;
    if (scanf("%d %d", &N, &M) != 2) return 0;
    if (N <= 1) { // N>1 按题意；若意外给1，百分比对唯一节点即100%
        if (N == 1) printf("1: 100.00%\n");
        return 0;
    }
    // 统计度数以便一次性分配邻接表
    int *deg = (int *)calloc(N+1, sizeof(int));
    int *eu = (int *)malloc(sizeof(int)*M);
    int *ev = (int *)malloc(sizeof(int)*M);
    if (!deg || !eu || !ev) return 0;
    for (int i=0;i<M;++i) {
        int u,v; if (scanf("%d %d", &u, &v)!=2){ M=i; break; }
        eu[i]=u; ev[i]=v;
        if (u>=1 && u<=N) ++deg[u];
        if (v>=1 && v<=N) ++deg[v];
    }
    int **adj = (int **)calloc(N+1, sizeof(int*));
    if (!adj) return 0;
    for (int i=1;i<=N;++i) {
        if (deg[i]>0) adj[i]=(int*)malloc(sizeof(int)*deg[i]); else adj[i]=NULL;
        deg[i]=0; // 重置写入指针
    }
    for (int i=0;i<M;++i) {
        int u=eu[i], v=ev[i];
        if (u>=1 && u<=N) adj[u][deg[u]++]=v;
        if (v>=1 && v<=N) adj[v][deg[v]++]=u;
    }
    free(eu); free(ev);

    int *queue = (int*)malloc(sizeof(int)*N); // BFS 队列（至多 N 个不同结点）
    int *depth = (int*)malloc(sizeof(int)*N);
    int *vis = (int*)malloc(sizeof(int)*(N+1));
    if (!queue || !depth || !vis) return 0;

    for (int src=1; src<=N; ++src) {
        // 初始化访问标记
        for (int i=1;i<=N;++i) vis[i]=0;
        int head=0, tail=0;
        queue[tail]=src; depth[tail]=0; tail++; vis[src]=1;
        int count=1; // 包含自身
        while (head<tail) {
            int u = queue[head];
            int d = depth[head];
            head++;
            if (d>=6) continue; // 深度达到6，子节点不再扩展
            for (int k=0;k<deg[u];++k) {
                int v=adj[u][k];
                if (!vis[v]) {
                    vis[v]=1;
                    queue[tail]=v; depth[tail]=d+1; tail++;
                    count++;
                }
            }
        }
        double percent = (double)count * 100.0 / (double)N;
        printf("%d: %.2f%%\n", src, percent);
    }

    // 释放资源
    for (int i=1;i<=N;++i) free(adj[i]);
    free(adj); free(deg); free(queue); free(depth); free(vis);
    return 0;
}
