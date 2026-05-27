/*
 * 旅行售货员问题 (TSP) - 分支限界法
 * --------------------------------------
 * 问题描述：同第5.9节。求最短 Hamilton 回路。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   每个节点表示一条部分路径（已访问部分城市）。
 *   优先队列按下界（当前路径长度 + 估计剩余路径长度）升序排列。
 *   下界计算方法：
 *     下界 = 当前路径长度 + 每个未访问城市的最小出边 + 回到起点的最小边
 *   或更简单的下界：当前路径长度 × 剩余城市比例
 *   每次取下界最小的节点扩展。
 *
 * 时间复杂度：最坏 O(n!)
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.7节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

const int INF = INT_MAX;

int n;
vector<vector<int>> dist;
int bestDist = INF;
vector<int> bestPath;

struct BBNode {
    vector<int> path;    // 已访问的城市顺序
    vector<bool> visited; // 各城市是否已访问
    int curDist;         // 当前路径长度
    double lowerBound;   // 下界估计

    BBNode(const vector<int>& p, const vector<bool>& v, int cd, double lb)
        : path(p), visited(v), curDist(cd), lowerBound(lb) {}

    // 最小优先队列（下界小者优先）
    bool operator<(const BBNode& other) const {
        return lowerBound > other.lowerBound;
    }
};

/**
 * 计算下界
 * 策略：已走距离 + 每个未访问城市的最小出边之和
 */
double CalcLowerBound(const vector<int>& path, const vector<bool>& visited,
                      int curDist) {
    double bound = curDist;
    int lastCity = path.back();

    // 对于每个未访问城市，加上其最小出边（不包括回到起点的边）
    int minRemaining = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            int minEdge = INF;
            for (int j = 0; j < n; j++) {
                if (i != j) minEdge = min(minEdge, dist[i][j]);
            }
            if (minEdge != INF) minRemaining += minEdge;
        }
    }

    // 加上从最后一个城市到未访问城市的最小边
    int minFromLast = INF;
    for (int j = 0; j < n; j++) {
        if (!visited[j]) {
            minFromLast = min(minFromLast, dist[lastCity][j]);
        }
    }

    bound += max(minFromLast, minRemaining > 0 ? 1 : 0);
    return bound;
}

/**
 * TSP - 分支限界法
 */
void TSP_BB() {
    priority_queue<BBNode> pq;

    vector<int> startPath = {0};
    vector<bool> startVisited(n, false);
    startVisited[0] = true;

    double startLB = CalcLowerBound(startPath, startVisited, 0);
    pq.push(BBNode(startPath, startVisited, 0, startLB));

    while (!pq.empty()) {
        BBNode cur = pq.top(); pq.pop();

        // 限界剪枝
        if (cur.lowerBound >= bestDist) continue;

        // 已访问所有城市
        if (cur.path.size() == (size_t)n) {
            int lastCity = cur.path.back();
            int totalDist = cur.curDist + dist[lastCity][0];  // 回到起点
            if (totalDist < bestDist) {
                bestDist = totalDist;
                bestPath = cur.path;
            }
            continue;
        }

        int lastCity = cur.path.back();

        // 扩展下一个城市
        for (int v = 0; v < n; v++) {
            if (!cur.visited[v] && dist[lastCity][v] != INF) {
                vector<int> newPath = cur.path;
                newPath.push_back(v);
                vector<bool> newVisited = cur.visited;
                newVisited[v] = true;
                int newDist = cur.curDist + dist[lastCity][v];

                double newLB = CalcLowerBound(newPath, newVisited, newDist);
                if (newLB < bestDist) {
                    pq.push(BBNode(newPath, newVisited, newDist, newLB));
                }
            }
        }
    }
}

int main() {
    cout << "=== 旅行售货员问题 (分支限界法) ===" << endl;

    n = 4;
    dist = {
        {  0, 30,  6,  4},
        { 30,  0,  5, 10},
        {  6,  5,  0, 20},
        {  4, 10, 20,  0}
    };

    cout << "城市距离矩阵:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << dist[i][j] << "\t";
        cout << endl;
    }

    TSP_BB();

    cout << "\n最短回路长度: " << bestDist << endl;
    cout << "最短路径: ";
    for (int v : bestPath) cout << v << " -> ";
    cout << bestPath[0] << endl;

    return 0;
}
