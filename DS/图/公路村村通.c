// 公路村村通 - 最低成本使所有城镇连通 (MST)
// 输入: n m, 后跟 m 行: u v w (无向边)
// 要求: 若能使 1..n 全连通, 输出最小总成本; 否则输出 -1
// 约束: n ≤ 1000, m ≤ 3n (≤3000). 用 Kruskal + 并查集即可.
// 时间复杂度: O(m log m) 排序 + 近似 O(m α(n)) 并查集.
// 注意: 成本可能累加较大, 使用 long long.

#include <stdio.h>
#include <stdlib.h>

typedef struct { int u, v; int w; } Edge;

static int *parent, *rankv; // 并查集

static void dsu_init(int n){
    parent = (int*)malloc(sizeof(int)*(n+1));
    rankv = (int*)malloc(sizeof(int)*(n+1));
    for(int i=1;i<=n;++i){ parent[i]=i; rankv[i]=0; }
}
static int dsu_find(int x){
    while(parent[x]!=x){ parent[x]=parent[parent[x]]; x=parent[x]; }
    return x;
}
static int dsu_union(int a,int b){
    int pa=dsu_find(a), pb=dsu_find(b);
    if(pa==pb) return 0;
    if(rankv[pa]<rankv[pb]) parent[pa]=pb;
    else if(rankv[pb]<rankv[pa]) parent[pb]=pa;
    else { parent[pb]=pa; rankv[pa]++; }
    return 1;
}

static int cmp_edge(const void *a,const void *b){
    const Edge *ea=(const Edge*)a, *eb=(const Edge*)b;
    return ea->w - eb->w; // 成本升序
}

int main(void){
    int n,m;
    if(scanf("%d %d", &n, &m)!=2) return 0;
    if(n<=0){ printf("-1\n"); return 0; }
    Edge *edges = (Edge*)malloc(sizeof(Edge)*m);
    for(int i=0;i<m;++i){
        int u,v,w; if(scanf("%d %d %d", &u,&v,&w)!=3){ m=i; break; }
        edges[i].u=u; edges[i].v=v; edges[i].w=w;
    }
    qsort(edges, m, sizeof(Edge), cmp_edge);
    dsu_init(n);
    long long total=0; int used=0;
    for(int i=0;i<m && used < n-1; ++i){
        if(dsu_union(edges[i].u, edges[i].v)){
            total += edges[i].w;
            used++;
        }
    }
    if(used == n-1) printf("%lld\n", total); else printf("-1\n");
    free(edges); free(parent); free(rankv);
    return 0;
}
