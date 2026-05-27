// 图的存储结构与基本操作（邻接表）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVEX 100     // 图允许的最大顶点数量（数组上限）
#define INFINITY 65535 // 表示“不可达/无穷大”的权值，占位用（无边或未知距离）
// 邻接矩阵大小（仅用于打印/矩阵演示）
#define MAXMAT 100

// 时间复杂度: O(V^2), V 为顶点数
// 构造邻接矩阵（无向图）
void CreateAdjMatrix(int n, int m, int edges[][3], int matrix[MAXMAT][MAXMAT])
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            matrix[i][j] = (i == j) ? 0 : INFINITY;
    for (int k = 0; k < m; ++k)
    {
        int u = edges[k][0], v = edges[k][1], w = edges[k][2];
        matrix[u][v] = w;
        matrix[v][u] = w; // 无向图
    }
}

// 时间复杂度: O(V^2), V 为顶点数
// 打印邻接矩阵
void PrintAdjMatrix(int n, int matrix[MAXMAT][MAXMAT])
{
    printf("邻接矩阵:\n");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (matrix[i][j] == INFINITY)
                printf("INF ");
            else
                printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// 时间复杂度: O(V^2), V 为顶点数。适用于稠密图
// Prim（邻接矩阵实现，适用于无向连通带权图）适合稠密图
void MiniSpanTree_Prim_Matrix(int n, int matrix[MAXMAT][MAXMAT], char labels[])
{
    int lowcost[MAXMAT];  // 到生成树的最小成本
    int adjvex[MAXMAT];   // 记录对应的生成树顶点是通过哪一个顶点实现的
    int selected[MAXMAT]; // 是否已加入生成树
    // lowcost 帮助我们每一步都做出贪心选择，而 adjvex 则默默记下了这些选择是如何构成的
    for (int i = 0; i < n; ++i)
    {
        lowcost[i] = matrix[0][i]; // 数组存储各个顶点到 *当前已生成的树* 的最短距离
        adjvex[i] = 0;             // 数组记录 `lowcost` 中每个距离对应的树内顶点。
        selected[i] = 0;
    }
    selected[0] = 1;
    lowcost[0] = 0;

    printf("最小生成树(Prim - 矩阵):\n");
    for (int k = 1; k < n; ++k)
    {
        /*每一次循环（k 的一次递增）代表什么？
        寻找一条新边：在当前所有连接“树内顶点”和“树外顶点”的候补边中，找到那条权值最小的（贪心选择）。
        添加一个新顶点：将这条最小权值边所连接的那个“树外顶点”加入到生成树的集合中。
        构建一棵更大的树：完成这一步后，生成树就多了一个顶点和一条边。*/
        int min = INFINITY, u = -1;
        for (int i = 0; i < n; ++i)
        {
            if (!selected[i] && lowcost[i] < min)
            {
                min = lowcost[i];
                u = i;
            }
        }
        if (u == -1)
            break; // 不可达或已完成
        printf("(%c, %c) %d\n", labels[adjvex[u]], labels[u], min);
        // labels为顶点标签数组
        /*在函数开始处 adjvex[i] 初始被设为 0（并且 lowcost[i]=matrix[0][i]），selected[0]=1。
        因此第一次选出的 u（最小的直接边从 0 到某顶点）会打印 (labels[0], labels[u])，例如 ('a', 'c') 2。
        随后当 adjvex[v] 被更新为 u 时，后续输出会打印 (labels[u], labels[v]) 表示用 u 连接 v 的边被选中。*/
        selected[u] = 1;
        // 用新加入的顶点 u 更新 lowcost
        for (int v = 0; v < n; ++v)
        {
            if (!selected[v] && matrix[u][v] < lowcost[v])
            {
                lowcost[v] = matrix[u][v];
                adjvex[v] = u;
            }
        }
    }
    /*{ 'a' }
    lowcost: 初始化为顶点 'a' 到其他所有顶点的直接距离。
    { 0, 5, 2, ∞, 4, ∞ }
    adjvex: 因为 lowcost 中的所有距离都来自 'a'，所以 adjvex 全部记为 0。
    { 0, 0, 0, 0, 0, 0 }
    第 1 轮循环
    选择: 在 lowcost 中找到未选顶点的最小值是 2，对应顶点 'c' (下标 2)。
    输出: 边 (adjvex[2], 2) 即 (0, 2)，输出 (a, c) 2。
    更新: 将 'c' 加入集合 U，现在 U = { 'a', 'c' }。检查从 'c' 出发是否能提供更短的路径。
    发现从 'c' 到 'd' 的距离是 2，比原来的 ∞ 要短。
    lowcost 变化: { 0, 5, 0, 2, 4, ∞ } (下标3的值从∞变为2)
    adjvex 变化: { 0, 0, 0, 2, 0, 0 } (下标3的值从0变为2，表示到'd'的最短路径现在来自'c')
    第 2 轮循环
    选择: 在 lowcost 中找到未选顶点的最小值是 2，对应顶点 'd' (下标 3)。
    输出: 边 (adjvex[3], 3) 即 (2, 3)，输出 (c, d) 2。
    更新: 将 'd' 加入集合 U，现在 U = { 'a', 'c', 'd' }。检查从 'd' 出发是否能提供更短的路径。
    到 'b' 的距离：从 'd' 到 'b' 是 3，比 lowcost[1] 的 5 要短。
    到 'f' 的距离：从 'd' 到 'f' 是 3，比 lowcost[5] 的 ∞ 要短。
    lowcost 变化: { 0, 3, 0, 0, 4, 3 } (下标1从5->3, 下标5从∞->3)
    adjvex 变化: { 0, 3, 0, 2, 0, 3 } (下标1从0->3, 下标5从0->3)
    第 3 轮循环
    选择: 在 lowcost 中找到未选顶点的最小值是 3。有两个：'b' 和 'f'。代码实现会先找到下标较小的 'b' (下标 1)。
    输出: 边 (adjvex[1], 1) 即 (3, 1)，输出 (d, b) 3。
    更新: 将 'b' 加入集合 U，现在 U = { 'a', 'c', 'd', 'b' }。检查从 'b' 出发，没有发现更短的路径。
    lowcost 变化: { 0, 0, 0, 0, 4, 3 } (下标1变为0)
    adjvex 变化: 无变化。
    第 4 轮循环
    选择: 在 lowcost 中找到未选顶点的最小值是 3，对应顶点 'f' (下标 5)。
    输出: 边 (adjvex[5], 5) 即 (3, 5)，输出 (d, f) 3。
    更新: 将 'f' 加入集合 U，现在 U = { 'a', 'c', 'd', 'b', 'f' }。检查从 'f' 出发，没有发现更短的路径。
    lowcost 变化: { 0, 0, 0, 0, 4, 0 } (下标5变为0)
    adjvex 变化: 无变化。
    第 5 轮循环
    选择: 在 lowcost 中找到未选顶点的最小值是 4，对应顶点 'e' (下标 4)。
    输出: 边 (adjvex[4], 4) 即 (0, 4)，输出 (a, e) 4。
    更新: 将 'e' 加入集合 U。
    lowcost 变化: { 0, 0, 0, 0, 0, 0 } (下标4变为0)
    adjvex 变化: 无变化。*/
}

typedef char VertexType; // 顶点的数据类型：此处用字符（如 'A','B'）表示顶点标签
typedef int EdgeType;    // 边的权值类型：此处用整型权值（距离、代价等）

// 邻接表边结点
// 邻接表中的“边（弧）结点”结构：链接到一个相邻顶点
typedef struct EdgeNode
{
    int adjvex;            // 邻接点的数组下标（指向哪个顶点）
    EdgeType weight;       // 该边的权值（无权图可统一为 1 或不使用）
    struct EdgeNode *next; // 同一顶点的下一条边（单链表指针）
} EdgeNode;

// 顶点结点
// 邻接表中的“顶点结点”结构：保存顶点数据与其边链表的表头
typedef struct VertexNode
{
    VertexType data; // 顶点的数据值（标签/名称）
    EdgeNode *first; // 指向该顶点的第一条边（边链表头指针）
} VertexNode;

// 邻接表图
// 邻接表图整体结构
typedef struct
{
    VertexNode adjList[MAXVEX]; // 顶点数组，每个元素含顶点数据与其边链表
    int numVertexes, numEdges;  // 当前图的顶点数与边数（输入/构造时赋值）
    int directed;               // 图的类型：0 表示无向图；1 表示有向图
} ALGraph;
/*逐个字段把这条链 a(0): (e,4) -> (c,2) -> (b,5) -> NULL 映射到代码里的具体变量与类型。

顶点定位
a(0): adjList[0] 这个数组元素（类型 VertexNode）。顶点结点
adjList[0].data: 顶点标签字符 'a'。
adjList[0].first: 指向该顶点的“第一条边”的指针（类型 EdgeNode*），也就是链表的表头。

第一条边结点
(e,4): 一个 EdgeNode 结构体实例。
adjvex=4: 邻接点下标 4（对应顶点 e）。
weight=4: 边权值 4。
next: 指向下一条边结点（这里指向表示 (c,2) 的那个 EdgeNode）。

第二条边结点
......
adjList[0].first → 边结点1 (e,4) 的地址。
边结点1 next → 边结点2 (c,2) 的地址。
边结点2 next → 边结点3 (b,5) 的地址。
边结点3 next → NULL。
类型对应

adjList: VertexNode adjList[MAXVEX]（数组）
adjList[0]: VertexNode（结构体）
adjList[0].first: EdgeNode*（指向边链表头）
每个括号项 (X,w): 一个 EdgeNode，用其字段 adjvex 和 weight 表示邻接点与权值
每个 ->: 通过 EdgeNode.next 串联的单链表连接*/

// 时间复杂度: O(V+E), V 为顶点数, E 为边数
// 创建无向网图（邻接表）
void CreateALGraph(ALGraph *G)
{
    int i; // 顶点循环计数器
    printf("输入顶点数、边数、是否有向(0无向/1有向):\n");
    scanf("%d %d %d", &G->numVertexes, &G->numEdges, &G->directed);

    // 顶点信息
    printf("输入顶点信息(连续输入字符):\n");
    char ch; // 读取并清理行尾的换行符/残留字符
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    // EOF 的类型是 int（通常值为 -1），用来表示流已经到达末尾或发生了错误。
    // 函数如 getchar()、fgetc()、fread 等在到达末尾或出错时会返回 EOF，所以接收返回值的变量必须是 int，而不是 char。
    for (i = 0; i < G->numVertexes; ++i)
    {
        scanf("%c", &G->adjList[i].data);
        G->adjList[i].first = NULL;
    }
    // 初始化后: adjList[i].first == NULL (所有边链表为空)

    // 边信息：每条边 i j w
    printf("输入每条边: 顶点下标i j 以及权值w:\n");
    for (int k = 0; k < G->numEdges; ++k)
    {                // 逐条读取边
        int u, v, w; // u=起点索引 v=终点索引 w=边权值
        scanf("%d %d %d", &u, &v, &w);
        EdgeNode *e1 = (EdgeNode *)malloc(sizeof(EdgeNode)); // 插入 u->v 的边结点
        e1->adjvex = v;
        e1->weight = w;
        e1->next = G->adjList[u].first; // 头插: 新结点 next 指向原表头（可能为 NULL 或已有结点）
        G->adjList[u].first = e1;
        /*作用: 将顶点 u 的边链表表头更新为新建的边结点 e1，使 e1 成为链表的第一个结点。
        背景: 头插法两步操作配合：
        e1->next = G->adjList[u].first：让新结点的 next 指向原来的表头（可能是 NULL 或已有结点）。
        G->adjList[u].first = e1：把表头指针移动到新结点，从而完成“把新边插到最前面”的动作。*/
        if (!G->directed)
        {
            EdgeNode *e2 = (EdgeNode *)malloc(sizeof(EdgeNode)); // 无向图需要对称插入 v->u
            e2->adjvex = u;
            e2->weight = w;
            e2->next = G->adjList[v].first;
            G->adjList[v].first = e2;
        }
    }
}

// DFS
/*深度优先搜索（DFS）是一种“尽量向深处走”的图遍历策略。从某个顶点出发，优先沿着一条路径持续前进，直到没有未访问的邻接点，再逐层回溯，继续探索其他分支。*/
int visited[MAXVEX]; // 访问标记数组：1 表示已访问，0 表示未访问（遍历用）

void DFS(ALGraph G, int i)
{
    visited[i] = 1;
    printf("%c ", G.adjList[i].data);
    for (EdgeNode *p = G.adjList[i].first; p; p = p->next)
    {
        if (!visited[p->adjvex])
            DFS(G, p->adjvex);
        // adjvex为邻接点的数组下标（指向哪个顶点），类型为int
    }
}

// 时间复杂度: O(V+E), V 为顶点数, E 为边数 (对于邻接表)
// DFSTraverse 负责“全局初始化 + 顶点级调度”，DFS 负责“从某顶点开始的深度探索”。两者配合才能覆盖不连通图。
void DFSTraverse(ALGraph G)
{
    for (int i = 0; i < G.numVertexes; ++i)
        visited[i] = 0; // 初始化访问标记
    printf("深度优先遍历结果: ");
    for (int i = 0; i < G.numVertexes; ++i)
        if (!visited[i])
            DFS(G, i);
    printf("\n");
}

// BFS
/* 广度优先搜索（BFS）是一种按“层次”逐步扩展的图遍历算法。从起始顶点出发，先访问它的所有直接邻接点，再访问这些邻接点的邻接点，以此类推，像水波一圈一圈向外扩散。
核心结构: 队列。用队列保证“先入先出”，从而实现按层次逐步访问。*/
void BFSTraverse(ALGraph G)
{
    int q[MAXVEX], front = 0, rear = 0; // 简易队列：q 为存储，front/rear 为头尾指针
    /*工作方式：
    起点入队：q[rear++] = i;
    每次从队头取出一个顶点 u = q[front++]，访问其所有未访问的邻接点 v，并将这些 v 入队：q[rear++] = v;
    直到队列为空（front == rear），即所有可达顶点都被访问。
    队列的本质：保证先入队的顶点先被扩展，体现“广度优先”，即先访问距离起点近的，再访问远的。
    没有这个队列，BFS就无法实现“按层次”推进，只能像DFS那样一条路走到底。*/
    for (int i = 0; i < G.numVertexes; ++i)
        visited[i] = 0; // 初始化访问标记
    printf("广度优先遍历结果: ");
    for (int i = 0; i < G.numVertexes; ++i)
        if (!visited[i])
        {
            visited[i] = 1; // 标记当前起始顶点已访问
            printf("%c ", G.adjList[i].data);
            q[rear++] = i; // 入队起始顶点
            while (front != rear)
            {
                int u = q[front++]; // 出队一个顶点 u
                for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
                {
                    int v = p->adjvex; // u 的邻接顶点 v
                    if (!visited[v])
                    {
                        visited[v] = 1;
                        printf("%c ", G.adjList[v].data);
                        q[rear++] = v;
                    }
                }
            }
        }
    printf("\n");
}

/*邻接表（头插法后）：

a(0): (e,4) → (c,2) → (b,5) → NULL
b(1): (d,3) → (a,5) → NULL
c(2): (d,2) → (a,2) → NULL
d(3): (f,3) → (b,3) → (c,2) → NULL
e(4): (f,5) → (a,4) → NULL
f(5): (e,5) → (d,3) → NULL
BFS遍历详细过程：

初始化

visited[0..5]=0
打印“广度优先遍历结果: ”
队列 q 为空，front=0, rear=0
外层循环 i=0

visited[0]==0，标记 visited[0]=1，打印 a，q[rear++]=0（rear=1）
进入 while(front!=rear)
front=0, rear=1
u=q[front++]=0（front=1）
遍历 a 的邻接链：(e,4),(c,2),(b,5)
v=4，visited[4]==0，标记1，打印 e，q[rear++]=4（rear=2）
v=2，visited[2]==0，标记1，打印 c，q[rear++]=2（rear=3）
v=1，visited[1]==0，标记1，打印 b，q[rear++]=1（rear=4）
front=1, rear=4
u=q[front++]=4（front=2）
遍历 e 的邻接链：(f,5),(a,4)
v=5，visited[5]==0，标记1，打印 f，q[rear++]=5（rear=5）
v=0，已访问，跳过
front=2, rear=5
u=q[front++]=2（front=3）
遍历 c 的邻接链：(d,2),(a,2)
v=3，visited[3]==0，标记1，打印 d，q[rear++]=3（rear=6）
v=0，已访问，跳过
front=3, rear=6
u=q[front++]=1（front=4）
遍历 b 的邻接链：(d,3),(a,5)
v=3，已访问，跳过
v=0，已访问，跳过
front=4, rear=6
u=q[front++]=5（front=5）
遍历 f 的邻接链：(e,5),(d,3)
v=4，已访问，跳过
v=3，已访问，跳过
front=5, rear=6
u=q[front++]=3（front=6）
遍历 d 的邻接链：(f,3),(b,3),(c,2)
v=5，已访问，跳过
v=1，已访问，跳过
v=2，已访问，跳过
front=6, rear=6，while 结束
外层循环 i=1..5

这些顶点都已访问，跳过
打印换行

最终输出顺序：广度优先遍历结果: a e c b f d
*/

// 时间复杂度: O(V^2), V 为顶点数。若使用优先队列优化可达 O(E log V)
// Prim（邻接表，适用于无向连通带权图）
void MiniSpanTree_Prim(ALGraph G)
{
    if (G.directed)
    {
        printf("Prim 仅用于无向图\n");
        return;
    }
    int n = G.numVertexes; // 顶点数量
    int lowcost[MAXVEX];   // 当前每个顶点到生成树的最小边权（0 表示已纳入生成树）
    int adjvex[MAXVEX];    // 记录使 lowcost[v] 最小的树内顶点（边的另一端）
    for (int i = 0; i < n; ++i)
    {
        lowcost[i] = INFINITY;
        adjvex[i] = -1;
    }
    // 从0开始
    lowcost[0] = 0;
    adjvex[0] = 0;
    for (EdgeNode *p = G.adjList[0].first; p; p = p->next)
    {
        lowcost[p->adjvex] = p->weight;
        adjvex[p->adjvex] = 0;
    }

    printf("最小生成树(Prim):\n");
    for (int k = 1; k < n; ++k)
    {
        int min = INFINITY, u = -1; // 选择尚未纳入树且代价最小的顶点 u
        for (int i = 0; i < n; ++i)
            if (lowcost[i] != 0 && lowcost[i] < min)
            {
                min = lowcost[i];
                u = i;
            }
        if (u == -1)
            break;
        printf("(%c, %c) %d\n", G.adjList[adjvex[u]].data, G.adjList[u].data, min);
        lowcost[u] = 0; // 加入树
        // 用 u 更新
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
        {
            int v = p->adjvex; // u 的邻接顶点 v
            if (lowcost[v] != 0 && p->weight < lowcost[v])
            {
                lowcost[v] = p->weight;
                adjvex[v] = u;
            }
        }
    }
}

// Kruskal（邻接表→边集）适用于稀疏图
typedef struct
{
    int begin, end, weight;
} Edge; // 边集元素：起点、终点与权值（用于 Kruskal 排序）

int Find(int *parent, int f)
{
    while (parent[f] > 0)
        f = parent[f];
    return f;
} // 并查集查找：找到 f 所属集合代表

// 时间复杂度: O(E log E), E 为边数, 主要开销在排序
void MiniSpanTree_Kruskal(ALGraph G)
{
    if (G.directed)
    {
        printf("Kruskal 仅用于无向图\n");
        return;
    }
    Edge edges[MAXVEX * MAXVEX];
    int k = 0; // 边集数组与当前边计数 k

    // 步骤 1: 提取所有边
    // 遍历邻接表，将图的所有边存储到 `edges` 数组中。
    // 去重条件：if (u < v) 只在 u 的编号小于 v 时收集这条边。
    // 因为无向图中同一条边会在两个顶点的链表中各出现一次（一次是 u->v，一次是 v->u），用 u < v 可确保只加入一次，避免重复。
    for (int u = 0; u < G.numVertexes; ++u)
    {
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
        {
            int v = p->adjvex;
            /*在 a(0): (e,4) -> (c,2) -> (b,5) 中，首个边结点的 adjvex=4 表示邻接顶点是 e（索引 4），而不是数组里的“下一个顶点 1”。*/
            if (u < v)
            {
                edges[k++] = (Edge){u, v, p->weight};
            }
            // 外层顶点索引 u → 作为 begin边结点 p->adjvex（即邻接点 v） → 作为 end
            /*(Edge){u, v, p->weight} 构造一个临时的 Edge 结构体值，然后赋给数组 edges[k++]。
            字段顺序: 采用位置初始化，按 Edge 声明的字段顺序赋值，等价写法: 可以改成指定初始化更直观：(Edge){ .begin = u, .end = v, .weight = p->weight }。*/
        }
    }

    // 步骤 2: 对所有边按权重进行排序
    // 这里使用的是冒泡排序，时间复杂度为 O(E^2)。
    // 在标准实现中，通常使用更高效的排序算法（如快速排序 qsort），其时间复杂度为 O(E log E)。
    // 排序是 Kruskal 算法的核心开销。
    for (int i = 0; i < k; ++i)
        for (int j = i + 1; j < k; ++j)
            if (edges[i].weight > edges[j].weight)
            {
                Edge t = edges[i];
                edges[i] = edges[j];
                edges[j] = t;
            }

    // 步骤 3: 使用并查集构建最小生成树
    // 初始化并查集，每个顶点自成一个集合。
    // 在 Kruskal 算法里，用并查集动态维护“连通分量”，用来快速判断一条候选边是否会形成环。
    // 如果边的两个端点已在同一集合中，就跳过；否则选用这条边并把两个集合合并。
    int parent[MAXVEX];
    for (int i = 0; i < G.numVertexes; ++i)
        parent[i] = 0; // 并查集父亲数组：0 表示自成集合
    printf("最小生成树(Kruskal):\n");

    /*对每条按权重升序的边 (u,v)，计算 n = Find(parent, u)、m = Find(parent, v)。若 n != m，则输出这条边、并执行“合并”：parent[n] = m；若 n == m，说明已连通，加入会成环，跳过。
    Find 的含义：Find(int *parent, int f) 一直顺着 parent 指针往上跳，直到遇到根结点并返回它。此实现把“根”的标记设为 parent[root] == 0，所以条件写成 while (parent[f] > 0) f = parent[f];。
    parent 的约定：初始化 parent[i] = 0 表示每个点自成一个集合（自己是根）；非根结点存放的是其父结点的下标（正数）。*/
    // 遍历排好序的边
    for (int i = 0; i < k; ++i)
    {
        // 查找边两端顶点所在的集合（根节点）
        int n = Find(parent, edges[i].begin);
        int m = Find(parent, edges[i].end);

        // 如果两个顶点不在同一个集合中，说明这条边不会形成环路
        /*若 Find(u) == Find(v)：u、v 已在同一棵树里，树内本就存在从 u 到 v 的路径，再加一条 (u,v) 必定闭合成环，所以要跳过。
        若 Find(u) != Find(v)：u、v 分属两棵不同的树，跨树连一条边不会形成环，只会把两棵树合成更大的一棵树（森林里的树数 −1），所以应当选用。*/
        if (n != m)
        {
            // 合并两个集合
            parent[n] = m;
            // 将这条边加入最小生成树，并打印
            printf("(%c, %c) %d\n", G.adjList[edges[i].begin].data, G.adjList[edges[i].end].data, edges[i].weight);
        }
    }
    /*
    定义: 根（代表元）是并查集中某个集合的代表结点。每个集合恰有一个根；任一结点沿着父指针一直往上走，最终都会到达这个根。
    本实现中的判定: 在你的 Find() 里，根满足 parent[root] == 0。函数用 while (parent[f] > 0) f = parent[f];
    不断上跳，直到遇到 parent[f] 不是正数（即为 0）时返回，这个返回值就是“根”（集合代表）。

    作用:
    判环/同集合: 若 Find(u) == Find(v)，说明在同一集合；否则在不同集合。
    合并: 若根不同，则执行 parent[n] = m，把根 n 挂到根 m 下，两个集合并为一体。

    顶点映射: a=0, b=1, c=2, d=3, e=4, f=5。
    初始并查集: parent[0..5] = 0（每个顶点自成集合，根的判定是 parent[root] == 0）。
    排序后的边（按权值升序）: (a,c,2) → (c,d,2) → (b,d,3) → (d,f,3) → (a,e,4) → (a,b,5) → (e,f,5)。
    逐步执行

    (a,c,2) = (0,2)
    n = Find(0) = 0（parent[0]=0，直接返回）
    m = Find(2) = 2（parent[2]=0，直接返回）
    合并: parent[n] = m → parent[0] = 2
    parent 现状: [2,0,0,0,0,0]
    输出: (a, c) 2
    (c,d,2) = (2,3)
    n = Find(2) = 2（parent[2]=0）
    m = Find(3) = 3（parent[3]=0）
    合并: parent[2] = 3
    parent: [2,0,3,0,0,0]（链：0→2→3）

    “链”是怎么来的: 指的是并查集里的父指针链（不是图中的边）
    父指针链是并查集中由 parent[] 指向关系串起来的一条“向上”链。
    对任意结点 x，不断用 x = parent[x] 往上走，直到走到根（代表元）为止，这条从起点到根的序列就是它的父指针链。

    输出: (c, d) 2
    (b,d,3) = (1,3)
    n = Find(1) = 1（parent[1]=0）
    m = Find(3) = 3（parent[3]=0）
    合并: parent[1] = 3
    parent: [2,3,3,0,0,0]（0→2→3，1→3）
    出现两条父子链是因为并查集是森林。这些“链”是同一棵树的不同分支，并不是两个集合；它们在根 3 汇合，表示 0、1、2、3 已经属于同一集合。
    输出: (b, d) 3
    (d,f,3) = (3,5)
    n = Find(3) = 3（parent[3]=0）
    m = Find(5) = 5（parent[5]=0）
    合并: parent[3] = 5
    parent: [2,3,3,5,0,0]（0→2→3→5，1→3→5）
    输出: (d, f) 3
    (a,e,4) = (0,4)
    n = Find(0)：
    内部过程: 0→parent[0]=2→parent[2]=3→parent[3]=5→parent[5]=0（根）
    返回 n = 5
    m = Find(4) = 4（parent[4]=0）
    合并: parent[5] = 4
    parent: [2,3,3,5,0,4]（0→2→3→5→4，1→3→5→4，5→4 为根 4）
    输出: (a, e) 4
    (a,b,5) = (0,1)
    n = Find(0)：0→2→3→5→4 → 返回 4
    m = Find(1)：1→3→5→4 → 返回 4
    判定: n == m（同一集合，加入会成环）→ 跳过
    parent 不变: [2,3,3,5,0,4]
    (e,f,5) = (4,5)
    n = Find(4) = 4
    m = Find(5)：5→4 → 返回 4
    判定: n == m → 跳过
    parent 不变: [2,3,3,5,0,4]

    初始: parent[0..5]=0，六个单点集合，各自为根。
    选 (a,c): 设置 parent[0]=2。此时集合 {a,c} 的根是 2（Find(0)=2, Find(2)=2）。
    选 (c,d): 设置 parent[2]=3。集合 {a,c,d} 的根是 3（0→2→3，Find(0)=3，Find(2)=3，Find(3)=3）。
    选 (d,f): 设置 parent[3]=5。集合 {a,c,d,f} 的根是 5（0→2→3→5）。
    选 (b,d): 设置 parent[1]=3，再经过上一步关系，Find(1)=5，于是 {a,b,c,d,f} 的根仍是 5。
    选 (a,e): 这一步 Find(0)=5、Find(4)=4，合并为 parent[5]=4。现在整个 {a,b,c,d,e,f} 的根是 4（5→4）。
    */
}

// 时间复杂度: O(V+E), V 为顶点数, E 为边数
// 拓扑排序（邻接表，有向图）
int TopologicalSort(ALGraph G)
{
    if (!G.directed)
    {
        printf("拓扑排序需有向图\n");
        return 0;
    }
    int indeg[MAXVEX] = {0}; // 入度数组：记录每个顶点被指向的次数
    for (int u = 0; u < G.numVertexes; ++u)
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
            indeg[p->adjvex]++;

    int stack[MAXVEX], top = 0, count = 0; // 辅助栈保存当前入度为 0 的顶点；count 统计输出数量
    for (int i = 0; i < G.numVertexes; ++i)
        if (indeg[i] == 0)
            stack[top++] = i;

    printf("拓扑排序结果: ");
    while (top)
    {
        int u = stack[--top]; // 取出一个入度为 0 的顶点 u
        printf("%c ", G.adjList[u].data);
        count++;
        /*
        作用一（判环）: 循环结束后检查 count == G.numVertexes。为真→所有顶点都被处理，图为 DAG；为假→还有顶点未被处理，必存在环。
        作用二（校验完成度）: 也可视为“已生成的拓扑序长度”。若小于顶点数，说明只得到部分序列（因有环卡住）。
        为什么能判环: 有环时，环上顶点的入度永远无法减到 0，栈/队列会提前空，导致 count < V。
        */
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
        {
            int v = p->adjvex; // u 指向的邻接顶点 v
            // 如果顶点 v 的入度减一后变为 0，则将 v 压入栈顶。
            // 这通常用于拓扑排序算法中，表示 v 的所有前驱节点已处理完毕，可以进行后续处理。
            if (--indeg[v] == 0)
                stack[top++] = v;
        }
    }
    printf("\n");
    return count == G.numVertexes;
}

// 关键路径（AOE网，邻接表）
/*关键路径是在求整个项目（或网络）完成所需的最短总工期：
关键路径上的每个活动，其最早开始时间和最晚开始时间相等（即etv=ltv），任何一个环节延误都会导致整个项目延误。*/
int etv[MAXVEX], ltv[MAXVEX], topoStack[MAXVEX], topoTop; // etv:事件最早发生时间 ltv:事件最晚发生时间 topoStack:拓扑序栈 topoTop:栈顶

/*TopologicalSort(ALGraph G)用于一般有向图的拓扑排序与“是否为 DAG”的判定与输出；TopoForCP(ALGraph G)用于关键路径（AOE 网）的准备工作，边遍历时同步计算事件的最早发生时间 etv[]，并记录拓扑序到 topoStack 供后续 CriticalPath() 使用。
输出/副作用: TopologicalSort()打印拓扑序并返回是否成功（1/0），不保留序列；TopoForCP()不打印，返回是否成功（1/0），但会填充全局的 etv[]、topoStack[]、topoTop（以及后续由 CriticalPath()计算 ltv[]）。*/

// 时间复杂度: O(V+E), V 为顶点数, E 为边数
int TopoForCP(ALGraph G)
{
    int indeg[MAXVEX] = {0}; // 入度数组（AOE 网络）
    for (int i = 0; i < G.numVertexes; ++i)
    {
        etv[i] = 0;
    }
    for (int u = 0; u < G.numVertexes; ++u)
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
            indeg[p->adjvex]++;
    int st[MAXVEX], sp = 0, cnt = 0; // st:入度为 0 的顶点栈 sp:栈指针 cnt:输出计数
    for (int i = 0; i < G.numVertexes; ++i)
        if (indeg[i] == 0)
            st[sp++] = i;
    topoTop = 0;
    while (sp)
    {
        int u = st[--sp];
        topoStack[topoTop++] = u;
        cnt++;
        /*在 TopoForCP 里，topoStack[topoTop++] = u; 每弹出一个入度为 0 的顶点 u，就把它顺序压入 topoStack。
        循环结束后，topoStack 里存放的就是完整的拓扑序列（从 0 到 topoTop-1）*/
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
        {
            int v = p->adjvex;
            if (--indeg[v] == 0)
                st[sp++] = v;
            if (etv[u] + p->weight > etv[v])
                etv[v] = etv[u] + p->weight;
            // 作用: 在遍历弧 u->v 时，将 etv[v] 更新为通过 u 到达 v 的最大时间（etv[u] + p->weight），这就是“事件最早发生时间”的前向传递。
            /*顶点: 0,1,2,3,4,5
            弧与历时: 0→1(3), 0→2(2), 1→3(2), 1→4(3), 2→3(4), 2→5(3), 3→5(2), 4→5(1)
            初始化: 所有 etv[i]=0，入度根据弧计算。
            推导过程

            处理 0:
            更新: etv[1] = max(0, etv[0]+3=3) = 3，etv[2] = max(0, etv[0]+2=2) = 2
            触发行: 当遍历 0→1、0→2 时执行更新行
            处理 1:
            更新: etv[4] = max(0, 3+3=6) = 6，etv[3] = max(0, 3+2=5) = 5
            触发行: 遍历 1→4、1→3 时
            处理 2:
            更新: etv[5] = max(0, 2+3=5) = 5，etv[3] = max(5, 2+4=6) = 6（覆盖了上一步的 5）
            触发行: 遍历 2→5、2→3 时
            处理 3:
            更新: etv[5] = max(5, 6+2=8) = 8（再次覆盖为 8）
            触发行: 遍历 3→5 时
            处理 4:
            更新: etv[5] = max(8, 6+1=7) = 8（保持不变）
            触发行: 遍历 4→5 时
            处理 5: 无后继，不更新
            最终结果：etv = [0, 3, 2, 6, 6, 8]
            解释: 例如事件 5 的最早发生时间为 8，是多条前驱路径中“最晚完成”的那条决定的：0→1→3→5 (3+2+2=7) vs 0→2→3→5 (2+4+2=8) vs 0→1→4→5 (3+3+1=7) vs 0→2→5 (2+3=5)，最大值为 8。
            */
        }
    }
    return cnt == G.numVertexes;
}

void CriticalPath(ALGraph G)
{
    if (!G.directed)
    {
        printf("关键路径需有向图(AOE)\n");
        return;
    }
    if (!TopoForCP(G))
    {
        printf("网中存在环路，无法求关键路径\n");
        return;
    }
    int sink = topoStack[topoTop - 1]; // 拓扑序最后一个顶点视为汇点
    int maxEt = etv[sink];             // 汇点的最早发生时间作为初始 ltv
    // maxEt 是“最大事件最早发生时间”，也就是整个 AOE 网（项目）完成的最早时间。
    for (int i = 0; i < G.numVertexes; ++i)
        ltv[i] = maxEt;

    // 逆拓扑求 ltv
    while (topoTop)
    {
        int u = topoStack[--topoTop]; // 按逆拓扑序回溯计算 ltv
        // 遍历所有入边 x->u：需要从所有顶点找指向 u 的边
        for (int x = 0; x < G.numVertexes; ++x)
        {
            for (EdgeNode *p = G.adjList[x].first; p; p = p->next)
            {
                if (p->adjvex == u)
                {
                    if (ltv[x] > ltv[u] - p->weight)
                        ltv[x] = ltv[u] - p->weight; // 取两条路径的最小值。
                    /*逆拓扑序遍历（5,4,3,2,1,0）：
                    顶点5：无出边，ltv[5]=9
                    顶点4：有一条 4→5(1)
                    ltv[4]=min(ltv[4], ltv[5]-1)=min(9,9-1)=8
                    顶点3：有一条 3→5(2)
                    ltv[3]=min(ltv[3], ltv[5]-2)=min(9,9-2)=7
                    顶点2：有两条 2→3(4), 2→5(3)
                    ltv[2]=min(ltv[2], ltv[3]-4)=min(9,7-4)=3
                    ltv[2]=min(3, ltv[5]-3)=min(3,9-3)=3
                    顶点1：有两条 1→3(2), 1→4(3)
                    ltv[1]=min(ltv[1], ltv[3]-2)=min(9,7-2)=5
                    ltv[1]=min(5, ltv[4]-3)=min(5,8-3)=5
                    顶点0：有两条 0→1(3), 0→2(2)
                    ltv[0]=min(ltv[0], ltv[1]-3)=min(9,5-3)=2
                    ltv[0]=min(2, ltv[2]-2)=min(2,3-2)=1*/
                }
            }
        }
    }

    printf("关键路径(活动):\n");
    for (int u = 0; u < G.numVertexes; ++u)
    {
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
        {
            int v = p->adjvex;            // 边 u->v
            int ete = etv[u];             // 活动最早开始时间 = 事件 u 的 etv
            int lte = ltv[v] - p->weight; // 活动最晚开始时间 = 事件 v 的 ltv - 活动历时
            if (ete == lte)
            {
                printf("<%c, %c> length: %d\n", G.adjList[u].data, G.adjList[v].data, p->weight);
            }
        }
    }
}

// 时间复杂度: O(V^2), V 为顶点数。若使用优先队列优化可达 O(E log V)
// Dijkstra（邻接表）——作用：计算“单源最短路径”（源点到所有顶点的最短距离），要求边权非负；
// 输出每个顶点的最短距离，并通过 prev[] 回溯重建实际路径。
void ShortestPath_Dijkstra(ALGraph G, int src)
//只计算一个固定源点到其他所有顶点的最短路径（比如 “从顶点 a 出发，到 b、c、d 的最短路径”）。
{
    // src 是 source（源点）的缩写：表示 Dijkstra 算法的起始顶点索引（整型下标），即要从哪个顶点开始计算单源最短路径
    if (src < 0 || src >= G.numVertexes)
    {
        printf("源点非法\n");
        return;
    }
    int n = G.numVertexes;                       // 顶点数量
    int dist[MAXVEX], vis[MAXVEX], prev[MAXVEX]; // dist:最短距离 vis:已确定最短路标记 prev:前驱顶点
    for (int i = 0; i < n; ++i)
    {
        dist[i] = INFINITY;
        vis[i] = 0;
        prev[i] = -1;
        /*prev[src] 永远是 -1（因为没有顶点能作为源点的前驱）；
        dist[src] 永远是 0（源点到自己的距离）。*/
    }
    dist[src] = 0; // 初始化：源点距离为 0，其余为“无穷”

    for (int k = 0; k < n; ++k)
    {
        int u = -1, min = INFINITY; // 选取当前“未确定且距离最小”的顶点 u（贪心选择）
        //u: 当前未确定且 dist 最小的顶点。
        //min: 用于寻找 u 的临时最小值。
        for (int i = 0; i < n; ++i)
            if (!vis[i] && dist[i] < min)
            {
                min = dist[i];
                u = i;
            }
        /*循环作用：对 i 从 0 到 n-1 遍历，检查未被确定的顶点（!vis[i]），找出当前 dist[i] 最小的顶点，把其下标赋给 u，对应最小距离存入 min。
        语义：u 将成为“当前与源点距离最小且尚未确定最短路”的顶点（贪心选择），随后会执行 vis[u]=1 并对 u 的邻接点做松弛（relaxation）。*/
        if (u == -1)
            break;  // 若找不到：说明剩余顶点不可达或已全部确定
        vis[u] = 1; // 将 u 的最短路“确定下来”（不再更新）
        for (EdgeNode *p = G.adjList[u].first; p; p = p->next)
        {
            int v = p->adjvex;
            // 松弛（relaxation）：若通过 u 到 v 的距离更短，则更新 dist[v] 与其前驱 prev[v]
            /*松弛条件：dist[u]+w(u,v)<dist[v]
            如果条件成立，则更新：dist[v]=dist[u]+w(u,v)*/
            if (!vis[v] && dist[u] + p->weight < dist[v])
            {
                dist[v] = dist[u] + p->weight;
                prev[v] = u;
            }
        }
    }

    printf("单源最短路径 (源点 %c):\n", G.adjList[src].data);
    for (int i = 0; i < n; ++i)
    {
        if (dist[i] == INFINITY)
        {
            printf("%c: 不可达\n", G.adjList[i].data);
            continue;
        }
        printf("%c: 距离=%d 路径=", G.adjList[i].data, dist[i]);
        int stk[MAXVEX], tp = 0, p = i; // 路径重建：沿 prev[] 回溯到源点，压栈再逆序打印
        //第一轮循环时，p 就是起点 i，所以 stk[tp++] = p 实际上就是 stk[tp++] = i，把起点压入栈，为后续遍历做准备。
        while (p != -1)
        {
            stk[tp++] = p;
            p = prev[p];
        }
        /*路径打印的核心是while (p != -1) 循环 ——无论i是哪个顶点，回溯都会在prev[p]=-1时停止，而只有src的prev是 - 1，我们分两种情况看：
        情况 1：i = src（打印源点到自己的路径）
        初始p = i = src；
        进入while (p != -1)：把src压入栈 → p = prev[src] = -1 → 循环终止；
        栈里只有src，打印出来就是src本身（比如src=0时打印a）。
        情况 2：i ≠ src（打印源点到其他顶点的路径）
        以i=5（顶点d，src=0）为例：
        初始p = 5 → 压栈 → p = prev[5] = 3（e）；
        p=3 → 压栈 → p = prev[3] = 0（a，即src）；
        p=0 → 压栈 → p = prev[0] = -1 → 循环终止；
        栈里是[5,3,0]，逆序打印就是0→3→5（a→e→d）。*/
        for (int t = tp - 1; t >= 0; --t)
        {
            printf("%c", G.adjList[stk[t]].data);
            if (t)
                printf("->");
        }
        printf("\n");
    }
    printf("\n");
}

// 时间复杂度: O(V^3), V 为顶点数
// Floyd（邻接表 → 初始化矩阵后执行）——作用：计算“任意两点最短路径”（All-Pairs Shortest Paths），
// 通过三重循环动态规划更新 dist[i][j]，并用 nextv[i][j] 记录从 i 前往 j 的下一跳以便路径重建。
// 可处理负权边但需无负环（本示例边权皆非负）。
//依次将每个点作为中间点去更新
void Floyd(ALGraph G)
//计算所有顶点之间的最短路径（比如 “a 到 b、a 到 c、b 到 a、b 到 c……” 的所有组合）。
{
    int n = G.numVertexes;
    int dist[MAXVEX][MAXVEX], nextv[MAXVEX][MAXVEX]; //dist:两点最短距离矩阵 nextv:路径重建的下一跳矩阵
    /*设有n 个顶点，下一跳矩阵通常用 next[i][j] 或 path[i][j] 表示，是一个 n×n 的二维数组。
    next[i][j] 的含义是：从i 出发，沿最短路径到j，第一步应该走到哪个顶点。
    如果 i 直接有边到 j，则 next[i][j] = j。如果 i 需要经过 k 到 j，则 next[i][j] = next[i][k]，即第一步先到 k。
    例如有路径：0→2→3则 next[0][3] = 2，表示从 0 到 3，第一步先到 2。*/
    /*对于任意两个顶点i和j，如果你要从i走最短路径到j，你第一步应该走向哪个顶点，这个顶点就叫“下一跳”。*/
    // 初始化距离与下一跳：先设自环为 0，其余为 INF，再据邻接表填入直接边
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            dist[i][j] = (i == j) ? 0 : INFINITY; // 自己到自己距离 0，其他未知设为无穷
            nextv[i][j] = -1;                     // -1 表示目前未知下一跳
        }
        for (EdgeNode *p = G.adjList[i].first; p; p = p->next)
        {
            int j = p->adjvex;
            dist[i][j] = p->weight; // 直接边权
            nextv[i][j] = j;        // 直接到达：i 的下一跳就是 j
        }
        /*例如i=0(a)的邻接边：a→b(19)、a→e(14)、a→g(18)，因此：
        dist[0][1]=19，nextv[0][1]=1；
        dist[0][4]=14，nextv[0][4]=4；
        dist[0][6]=18，nextv[0][6]=6；*/
    }
    // 动态规划：尝试经由中转点 k 改善 i→j 的最短距离
    //公式为dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])。
    for (int k = 0; k < n; ++k) // 以 k 为中间点尝试优化 i->j
        for (int i = 0; i < n; ++i)
        {
            if (dist[i][k] == INFINITY)
                continue; // i 无法到 k，跳过
            for (int j = 0; j < n; ++j)
            {
                if (dist[k][j] == INFINITY)
                    continue; // k 无法到 j，跳过
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j]; // 通过 k 改善 i→j 的距离
                    nextv[i][j] = nextv[i][k];            // 更新路径的下一跳：沿用 i→k 的下一跳
                }
            }
        }
    printf("Floyd 距离矩阵:\n");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (dist[i][j] == INFINITY)
                printf("INF ");
            else
                printf("%3d ", dist[i][j]);
        }
        printf("\n");
    }
    printf("\nFloyd 任意两点路径示例:\n");
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            if (i == j || dist[i][j] == INFINITY)
                continue;
            printf("%c->%c 距离=%d 路径=", G.adjList[i].data, G.adjList[j].data, dist[i][j]);
            int u = i;
            printf("%c", G.adjList[u].data); // 依据 nextv 逐跳打印 i→…→j 的路径
            while (u != j)
            {
                u = nextv[u][j];
                if (u == -1)
                {
                    printf("(路径重建失败)");
                    break;
                }
                printf("->%c", G.adjList[u].data);
            }
            /*例如：a->f 距离=7 路径=a->c->d->f
            1. 初始化阶段
            假设顶点编号：a=0, c=2, d=3, f=5
            初始时，nextv[i][j] 如果有直接边，则为 j，否则为 -1 或不可达。
            2. 弗洛伊德算法更新阶段
            每次发现经过某个中间点 k 能让 i 到 j 更短时，更新：
            dist[i][j] = dist[i][k] + dist[k][j]
            nextv[i][j] = nextv[i][k]（即从 i 到 j 的第一步，应该先到 nextv[i][k]）
            3. 输出路径过程
            假设你要输出 a 到 f 的路径：

            步骤一：起点 a，终点 f
            当前：u = a (0)，v = f (5)
            nextv[0][5] 是什么？
            如果 a 到 f 的最短路径第一步是到 c，那么 nextv[0][5] = 2（c）
            步骤二：a → c
            输出 a
            更新：u = nextv[0][5] = 2（c）
            步骤三：c → d
            现在 u = 2（c），v = 5（f）
            如果 c 到 f 的最短路径第一步是到 d，那么 nextv[2][5] = 3（d）
            输出 c
            更新：u = nextv[2][5] = 3（d）
            步骤四：d → f
            现在 u = 3（d），v = 5（f）
            如果 d 到 f 的最短路径第一步就是 f，那么 nextv[3][5] = 5（f）
            输出 d
            更新：u = nextv[3][5] = 5（f）
            步骤五：到达终点
            现在 u = 5（f），等于终点，输出 f，结束。*/
            printf("\n");
        }
    printf("\n");
}

// 测试图：无向带权（用于 Prim/Kruskal/DFS/BFS/Dijkstra/Floyd）
void CreateTestGraph(ALGraph *G)
{
    // 构造 6 顶点无向带权图，使邻接表为：
    // e(4): (f,5) -> (a,4) -> NULL
    // f(5): (e,5) -> (d,3) -> NULL
    // d(3): (f,3) -> (b,3) -> (c,2) -> NULL
    // b(1): (d,3) -> (a,5) -> NULL
    // c(2): (d,2) -> (a,2) -> NULL
    // a(0): (e,4) -> (c,2) -> (b,5) -> NULL
    G->numVertexes = 6;
    G->numEdges = 7;
    G->directed = 0;
    G->adjList[0].data = 'a';
    G->adjList[1].data = 'b';
    G->adjList[2].data = 'c';
    G->adjList[3].data = 'd';
    G->adjList[4].data = 'e';
    G->adjList[5].data = 'f';
    for (int i = 0; i < G->numVertexes; ++i)
        G->adjList[i].first = NULL;
    // 选取插入顺序以配合“头插法”得到上述链表次序
    int edges[][3] = {
        {0, 1, 5}, // a-b：让 b 的 (a,5) 在后（先插入）
        {0, 2, 2}, // a-c：让 c 的 (a,2) 在后（先插入）
        {2, 3, 2}, // c-d：让 d 的 (c,2) 在后（先插入）
        {1, 3, 3}, // b-d：让 d 的 (b,3) 在 (c,2) 前（后插入）
        {3, 5, 3}, // d-f：让 d 的 (f,3) 在最前（最后插入到 d）
        {0, 4, 4}, // a-e：让 e 的 (a,4) 在后（先插入到 e）
        {4, 5, 5}  // e-f：让 e 的 (f,5) 在最前、f 的 (e,5) 在最前（最后插入）
    };
    for (int k = 0; k < G->numEdges; ++k)
    {
        int u = edges[k][0], v = edges[k][1], w = edges[k][2];
        EdgeNode *e1 = (EdgeNode *)malloc(sizeof(EdgeNode));
        e1->adjvex = v;
        e1->weight = w;
        e1->next = G->adjList[u].first;
        G->adjList[u].first = e1;
        EdgeNode *e2 = (EdgeNode *)malloc(sizeof(EdgeNode));
        e2->adjvex = u;
        e2->weight = w;
        e2->next = G->adjList[v].first;
        G->adjList[v].first = e2;
    }
}

// 打印邻接表，便于直观看到当前链表顺序
void PrintAdjList(ALGraph G)
{
    printf("邻接表:\n");
    for (int i = 0; i < G.numVertexes; ++i)
    {
        printf("%c(%d): ", G.adjList[i].data, i);
        for (EdgeNode *p = G.adjList[i].first; p; p = p->next)
        {
            printf("(%c,%d)", G.adjList[p->adjvex].data, p->weight);
            if (p->next)
                printf(" -> ");
        }
        printf(" -> NULL\n");
    }
    printf("\n");
}

// 有向无环图（用于拓扑排序）
void CreateDAG(ALGraph *G)
{
    G->numVertexes = 4;
    G->numEdges = 4;
    G->directed = 1;
    G->adjList[0].data = 'A';
    G->adjList[1].data = 'B';
    G->adjList[2].data = 'C';
    G->adjList[3].data = 'D';
    for (int i = 0; i < G->numVertexes; ++i)
        G->adjList[i].first = NULL;
    int edges[][3] = {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1}};
    for (int k = 0; k < 4; ++k)
    {
        int u = edges[k][0], v = edges[k][1], w = edges[k][2];
        EdgeNode *e = (EdgeNode *)malloc(sizeof(EdgeNode));
        e->adjvex = v;
        e->weight = w;
        e->next = G->adjList[u].first;
        G->adjList[u].first = e;
    }
}

// AOE 网（用于关键路径）
void CreateAOE(ALGraph *G)
{
    G->numVertexes = 6;
    G->numEdges = 8;
    G->directed = 1;
    for (int i = 0; i < G->numVertexes; ++i)
    {
        G->adjList[i].data = '0' + i;
        G->adjList[i].first = NULL;
    }
    int edges[][3] = {
        {0, 1, 3}, {0, 2, 2}, {1, 3, 2}, {1, 4, 3}, {2, 3, 4}, {2, 5, 3}, {3, 5, 2}, {4, 5, 1}};
    for (int k = 0; k < 8; ++k)
    {
        int u = edges[k][0], v = edges[k][1], w = edges[k][2];
        EdgeNode *e = (EdgeNode *)malloc(sizeof(EdgeNode));
        e->adjvex = v;
        e->weight = w;
        e->next = G->adjList[u].first;
        G->adjList[u].first = e;
    }
}

int main()
{
    ALGraph G;
    CreateTestGraph(&G);
    printf("图的顶点数: %d, 边数: %d\n", G.numVertexes, G.numEdges);
    PrintAdjList(G);
    DFSTraverse(G);
    BFSTraverse(G);
    MiniSpanTree_Prim(G);
    MiniSpanTree_Kruskal(G);

    // 邻接矩阵测试
    int matrix[MAXMAT][MAXMAT];
    // 与CreateTestGraph一致的边
    int edges[7][3] = {
        {0, 1, 5}, {0, 2, 2}, {2, 3, 2}, {1, 3, 3}, {3, 5, 3}, {0, 4, 4}, {4, 5, 5}};
    CreateAdjMatrix(6, 7, edges, matrix);
    PrintAdjMatrix(6, matrix);
    // 用字符标签调用矩阵版 Prim
    char labels[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    MiniSpanTree_Prim_Matrix(6, matrix, labels);

    ALGraph DAG;
    CreateDAG(&DAG);
    TopologicalSort(DAG);

    ALGraph AOE;
    CreateAOE(&AOE);
    CriticalPath(AOE);

    ShortestPath_Dijkstra(G, 0);
    Floyd(G);
    return 0;
}
