/*
 * 单源最短路径 - Dijkstra 算法
 * ---------------------------------
 * 问题描述：给定带权有向图 G=(V,E)，求从源点 v₀ 到其他各顶点的最短路径。
 * 所有边的权值非负。
 *
 * 算法思想（贪心算法）：
 *   设置顶点集合 S，初始 S 只含源点。dist[v] 为当前从源点到 v 的最短距离。
 *   每次从 V-S 中选择 dist 值最小的顶点 u 加入 S，
 *   并用 u 更新其所有邻接点的 dist 值（松弛操作）：
 *     if dist[u] + w(u,v) < dist[v]: dist[v] = dist[u] + w(u,v)
 *
 * 时间复杂度：
 *   - 朴素实现：O(n²)（邻接矩阵 + 线性查找最小 dist）
 *   - 堆优化：O((n+m) log n)
 *
 * 【阅读指引 / 易错点】
 *   - 选择规则（Dijkstra）：每轮选取 V-S 中 dist 最小的顶点 u，把 dist[u] 视为最终最短距离。
 *   - 正确性前提：边权必须非负；若存在负权边，贪心选择性质不成立，需要 Bellman-Ford 等算法。
 *   - 不可达处理：当找不到新的 u（u=-1）时即可提前结束；输出时用 INF/∞ 表示不可达。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第4.5节

 *
 * 解空间树：递归树（最短路径状态树）
 * 递归方式：深度优先搜索(DFS)
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

const int INF = INT_MAX;

/**
 * Dijkstra 算法 - 邻接矩阵 O(n²) 实现
 * @param graph  邻接矩阵，graph[i][j] 为边 (i,j) 的权值（INF 表示无边）
 * @param src    源点编号
 * @param dist   返回最短距离
 * @param prev   返回前驱节点（用于路径重建）
 */
void Dijkstra(const vector<vector<int>>& graph, int src,
              vector<int>& dist, vector<int>& prev) {
    int n = graph.size();
    vector<bool> visited(n, false);  // S 集合：已确定最短路径的顶点

    dist.assign(n, INF);
    prev.assign(n, -1);

    dist[src] = 0;

    for (int i = 0; i < n; i++) {
        // 在 V-S 中找 dist 最小的顶点 u（贪心选择）
        int u = -1, minDist = INF;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;  // 所有剩余顶点都不可达
        visited[u] = true;    // 将 u 加入 S

        // 松弛操作：用 u 更新所有邻接点
        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] != INF &&
                dist[u] != INF &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }
}

/**
 * 输出从 src 到 v 的路径
 */
void PrintPath(const vector<int>& prev, int v) {
    if (prev[v] == -1) {
        cout << v;
        return;
    }
    PrintPath(prev, prev[v]);
    cout << " -> " << v;
}

int main() {
    cout << "=== Dijkstra 单源最短路径 ===" << endl;

    // 5 个顶点的有向图（邻接矩阵）
    int n = 5;
    vector<vector<int>> graph = {
        {  0,  10, INF,  30, 100},
        {INF,   0,  50, INF, INF},
        {INF, INF,   0, INF,  10},
        {INF, INF,  20,   0,  60},
        {INF, INF, INF, INF,   0}
    };

    cout << "图的邻接矩阵 (INF=∞):" << endl;
    for (auto& row : graph) {
        for (int v : row) {
            if (v == INF) cout << " ∞\t";
            else cout << " " << v << "\t";
        }
        cout << endl;
    }

    vector<int> dist, prev;
    int src = 0;  // 源点

    Dijkstra(graph, src, dist, prev);

    cout << "\n从源点 " << src << " 出发的最短路径:" << endl;
    for (int v = 0; v < n; v++) {
        cout << "  到 " << v << ": 距离=";
        if (dist[v] == INF) cout << "∞";
        else {
            cout << dist[v] << "  路径: ";
            PrintPath(prev, v);
        }
        cout << endl;
    }

    return 0;
}
