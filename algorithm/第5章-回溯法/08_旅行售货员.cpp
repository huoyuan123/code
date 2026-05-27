/*
 * 旅行售货员问题 (TSP) - 回溯法
 * -----------------------------------
 * 问题描述：给定 n 个城市及两两之间的距离（完全图）。
 * 求从城市 1 出发，经过每个城市恰好一次，
 * 最后回到城市 1 的最短回路（Hamilton 回路）。
 *
 * 算法思想（回溯法）：
 *   解空间：排列树
 *   用 x[1..n] 表示路径：x[i] 为第 i 步访问的城市。
 *   按层递归，记录当前路径长度。
 *   剪枝：当前部分路径长度 ≥ 当前最优解时剪枝。
 *   边界：到达叶节点时还需加上回到起点的距离。
 *
 * 时间复杂度：最坏 O(n!)，通过剪枝减少
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.9节
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

const int INF = INT_MAX;

int n;                          // 城市数
vector<vector<int>> dist;       // 距离矩阵
vector<int> bestPath;           // 最优路径
vector<int> curPath;            // 当前路径
vector<bool> visited;           // 城市是否已访问
int bestDist = INF;             // 最优路径长度

/**
 * TSP - 回溯法（排列树）
 * @param step    当前在第几步 (0-indexed，已访问 step 个城市)
 * @param curDist 当前路径长度
 */
void Backtrack(int step, int curDist) {
    if (step == n) {
        // 所有城市访问完毕，加上回到起点的距离
        int totalDist = curDist + dist[curPath[n-1]][curPath[0]];
        if (totalDist < bestDist) {
            bestDist = totalDist;
            bestPath = curPath;
        }
        return;
    }

    // 剪枝：当前路径已不优于最优解
    if (curDist >= bestDist) return;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            int prev = curPath[step - 1];
            if (dist[prev][i] == INF) continue;  // 无边

            visited[i] = true;
            curPath[step] = i;

            Backtrack(step + 1, curDist + dist[prev][i]);

            visited[i] = false;
        }
    }
}

int main() {
    cout << "=== 旅行售货员问题 (回溯法) ===" << endl;

    // 4 个城市的完全图
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

    curPath.assign(n, 0);
    visited.assign(n, false);

    // 从城市 0 出发
    visited[0] = true;
    curPath[0] = 0;

    Backtrack(1, 0);

    cout << "\n最短回路长度: " << bestDist << endl;
    cout << "最短路径: ";
    for (int v : bestPath) cout << v << " -> ";
    cout << bestPath[0] << endl;

    return 0;
}
