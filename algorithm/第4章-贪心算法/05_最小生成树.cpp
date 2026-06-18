/*
 * 最小生成树 (Minimum Spanning Tree)
 * -------------------------------------
 * 问题描述：给定连通无向图 G=(V,E)，每条边有权值 w。
 * 求一棵包含所有顶点的生成树，且边的总权值最小。
 *
 * 本文件包含两种经典贪心算法：
 *
 * 【Prim 算法】
 *   贪心策略：从任一顶点开始，每次选择连接"树内"和"树外"
 *   且权值最小的边，将新顶点加入树中。
 *   维护 key[v]（顶点 v 到当前树的最小边权）和 mstSet（是否在树中）。
 *   时间复杂度：O(n²)（朴素）/ O((n+m) log n)（堆优化）
 *
 * 【Kruskal 算法】
 *   贪心策略：每次选择权值最小且不构成回路的边加入生成树。
 *   需用并查集维护连通性。
 *   时间复杂度：O(m log m) —— 排序主导
 *
 * 【阅读指引 / 易错点】
 *   - Prim vs Kruskal：Prim 更像“从点扩展”，适合稠密图；Kruskal 更像“按边选”，适合稀疏图。
 *   - 正确性直觉：切分性质——对任意割，跨割的最小边必属于某棵最小生成树；两种算法都在重复应用此性质。
 *   - 判环手段：Kruskal 必须用并查集；不要用 DFS 每次判环（会很慢）。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第4.6节

 *
 * 解空间树：递归树（最小生成树状态树）
 * 递归方式：深度优先搜索(DFS)
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

const int INF = INT_MAX;

// ==================== Prim 算法 ====================

/**
 * Prim 最小生成树 - O(n²) 实现
 * @param graph  邻接矩阵
 * @param parent 返回父节点数组（记录生成树边）
 * @return       最小生成树总权值
 */
int Prim(const vector<vector<int>>& graph, vector<int>& parent) {
    int n = graph.size();
    vector<int> key(n, INF);      // key[v] = v 到当前树的最小边权
    vector<bool> inMST(n, false); // 是否已在生成树中

    // 从顶点 0 开始
    key[0] = 0;
    parent[0] = -1;

    for (int i = 0; i < n; i++) {
        // 选 key 最小且不在树中的顶点 u（贪心选择）
        int u = -1, minKey = INF;
        for (int j = 0; j < n; j++) {
            if (!inMST[j] && key[j] < minKey) {
                minKey = key[j];
                u = j;
            }
        }

        if (u == -1) break;

        inMST[u] = true;  // 将 u 加入生成树

        // 更新 u 的邻接顶点的 key 值
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != INF && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }

    // 计算总权值
    int total = 0;
    for (int i = 1; i < n; i++) {
        if (parent[i] != -1) {
            total += graph[i][parent[i]];
        }
    }
    return total;
}

// ==================== Kruskal 算法 ====================

struct Edge {
    int u, v, weight;
};

/**
 * Union-Find 查找（含路径压缩）
 */
int Find(vector<int>& parent, int x) {
    if (parent[x] != x)
        parent[x] = Find(parent, parent[x]);  // 路径压缩
    return parent[x];
}

/**
 * Union-Find 合并
 */
void Union(vector<int>& parent, vector<int>& rank, int x, int y) {
    int rx = Find(parent, x);
    int ry = Find(parent, y);
    if (rx == ry) return;

    // 按秩合并
    if (rank[rx] < rank[ry])
        parent[rx] = ry;
    else if (rank[rx] > rank[ry])
        parent[ry] = rx;
    else {
        parent[ry] = rx;
        rank[rx]++;
    }
}

/**
 * Kruskal 最小生成树
 * @param edges     边列表
 * @param n         顶点数
 * @param mstEdges  返回 MST 中的边
 * @return          MST 总权值
 */
int Kruskal(vector<Edge>& edges, int n, vector<Edge>& mstEdges) {
    // 按权值升序排列（贪心选择）
    sort(edges.begin(), edges.end(),
         [](const Edge& a, const Edge& b) {
             return a.weight < b.weight;
         });

    // 初始化并查集
    vector<int> parent(n), rank(n, 0);
    for (int i = 0; i < n; i++) parent[i] = i;

    int totalWeight = 0;

    for (Edge& e : edges) {
        if (Find(parent, e.u) != Find(parent, e.v)) {
            // 不构成回路，加入 MST
            Union(parent, rank, e.u, e.v);
            mstEdges.push_back(e);
            totalWeight += e.weight;

            // 已选 n-1 条边，MST 完成
            if ((int)mstEdges.size() == n - 1) break;
        }
    }

    return totalWeight;
}

int main() {
    cout << "=== 最小生成树 ===" << endl;

    // 图例：5 个顶点
    int n = 5;
    vector<vector<int>> graph = {
        {0,   2, INF,   6, INF},
        {2,   0,   3,   8,   5},
        {INF, 3,   0, INF,   7},
        {6,   8, INF,   0,   9},
        {INF, 5,   7,   9,   0}
    };

    // --- Prim 算法 ---
    vector<int> parent(n, -1);
    int primTotal = Prim(graph, parent);

    cout << "Prim 算法 MST 总权值: " << primTotal << endl;
    cout << "MST 边 (parent -> vertex):" << endl;
    for (int i = 1; i < n; i++)
        cout << "  " << parent[i] << " -- " << i << " (权值 " << graph[i][parent[i]] << ")" << endl;

    // --- Kruskal 算法 ---
    vector<Edge> edges = {
        {0, 1, 2}, {0, 3, 6}, {1, 2, 3},
        {1, 3, 8}, {1, 4, 5}, {2, 4, 7}, {3, 4, 9}
    };
    vector<Edge> mstEdges;
    int kruskalTotal = Kruskal(edges, n, mstEdges);

    cout << "\nKruskal 算法 MST 总权值: " << kruskalTotal << endl;
    cout << "MST 边:" << endl;
    for (Edge& e : mstEdges)
        cout << "  " << e.u << " -- " << e.v << " (权值 " << e.weight << ")" << endl;

    return 0;
}
