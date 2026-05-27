/*
 * 单源最短路径 - 分支限界法
 * ------------------------------
 * 问题描述：同第4.5节。求从源点到其他各顶点的最短路径。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   与 Dijkstra 算法原理相同，以优先队列（最小堆）方式组织活节点。
 *   每个节点存储：(顶点编号, 当前最短距离)。
 *   每次从优先队列取距离最小的节点进行扩展（贪心选择），
 *   对其每个邻接边进行松弛操作。
 *   这是 Dijkstra 算法的分支限界表述。
 *
 * 与回溯法的区别：分支限界用广搜/优搜并一次性产生所有儿子；
 *                回溯法用深搜并逐个产生儿子。
 *
 * 时间复杂度：O((n+m) log n) —— 优先队列版本
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.2节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

const int INF = INT_MAX;

struct Node {
    int vertex;      // 顶点编号
    int distance;    // 从源点到当前顶点的最短路径长度

    Node(int v, int d) : vertex(v), distance(d) {}

    // 优先队列按距离升序（小顶堆）
    bool operator<(const Node& other) const {
        return distance > other.distance;  // C++ 默认大顶堆，取反
    }
};

/**
 * 单源最短路径 - 分支限界法（优先队列实现）
 * @param graph 邻接表 {{邻接顶点, 边权}, ...}
 * @param src   源点
 * @param dist  返回最短距离
 * @param prev  返回前驱节点
 */
void ShortestPath_BB(const vector<vector<pair<int,int>>>& graph,
                     int src, vector<int>& dist, vector<int>& prev) {
    int n = graph.size();
    dist.assign(n, INF);
    prev.assign(n, -1);

    priority_queue<Node> pq;
    dist[src] = 0;
    pq.push(Node(src, 0));

    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();

        int u = cur.vertex;
        // 跳过已过时的节点（之前入队后被更优距离更新）
        if (cur.distance > dist[u]) continue;

        // 扩展 u 的所有邻接边（一次性产生所有儿子）
        for (auto& edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;

            // 松弛操作
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push(Node(v, dist[v]));
            }
        }
    }
}

/**
 * 输出路径
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
    cout << "=== 单源最短路径 (分支限界法) ===" << endl;

    int n = 5;
    // 邻接表表示
    vector<vector<pair<int,int>>> graph(n);
    graph[0] = {{1, 10}, {3, 30}, {4, 100}};
    graph[1] = {{2, 50}};
    graph[2] = {{4, 10}};
    graph[3] = {{2, 20}, {4, 60}};
    // 顶点 4 没有出边

    cout << "图的邻接表:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "  " << i << " -> ";
        for (auto& e : graph[i])
            cout << e.first << "(权=" << e.second << ") ";
        cout << endl;
    }

    vector<int> dist, prev;
    int src = 0;
    ShortestPath_BB(graph, src, dist, prev);

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
