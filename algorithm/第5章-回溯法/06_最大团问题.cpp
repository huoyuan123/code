/*
 * 最大团问题 (Maximum Clique) - 回溯法
 * ----------------------------------------
 * 问题描述：给定无向图 G=(V,E)，求最大完全子图（团），
 * 即最大顶点子集使得其中每对顶点之间都有边相连。
 *
 * 算法思想（回溯法）：
 *   解空间：子集树
 *   设 current_clique 为当前团，candidate 为候选顶点集。
 *   每次从候选集中选一个顶点加入团，更新候选集为该顶点的邻居交集。
 *   剪枝策略：当前团大小 + 剩余候选 ≤ 最优团大小时剪枝。
 *
 * 时间复杂度：最坏 O(n·2ⁿ)，通过剪枝减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：子集树（顶点取/不取）；取顶点时必须与当前团内所有顶点相邻。
 *   - 剪枝：若“当前团大小 + 剩余可选顶点数”仍 ≤ best，则不可能更优，直接回溯。
 *   - 图表示：用邻接矩阵检查相邻是 O(1) 但占内存；用邻接表节省内存但检查可能变慢。
 *
 * 【实现提示】
 *   - 回溯核心：加入顶点后，候选集必须变成“与该顶点相邻的交集”。
 *   - 剪枝核心：curClique.size() + candidates.size() ≤ bestSize 直接返回。
 *   - 状态撤销：回溯时要弹出顶点，避免影响兄弟分支。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.7节

 *
 * 解空间树：子集树（Subset Tree）
 * 递归方式：深度优先搜索(DFS)

 *
 * ═══════════════════════════════════════════════════════════════
 * 【约束函数 vs 限界函数 — 区别说明】
 * ═══════════════════════════════════════════════════════════════
 * 约束函数 (Constraint)：检查部分解是否满足问题的硬性约束条件。
 *   不满足 → 立即剪枝（该分支不可能产生可行解，继续搜无意义）。
 *   例：装载重量≦容量、皇后是否冲突、顶点颜色是否冲突。
 *
 * 限界函数 (Bounding)：估算部分解可能达到的最优目标值。
 *   不可能优于当前最优 → 剪枝（该子树不可能有更优解）。
 *   例：当前重量+剩余≦bestW、价值上界≦bestValue、curDist≧best。
 *
 * 简单记法：约束 = "能不能"（可行性）， 限界 = "值不值"（最优性）。
 * ═══════════════════════════════════════════════════════════════
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n;                           // 顶点数
vector<vector<bool>> adj;        // 邻接矩阵
vector<int> bestClique;          // 最优团顶点集
vector<int> curClique;           // 当前团顶点集
int bestSize = 0;                // 最优团大小

/**
 * 最大团 - 回溯法
 * @param candidates 候选顶点集（每个顶点与当前团所有顶点相邻）
 */
void Backtrack(vector<int>& candidates) {
    if (candidates.empty()) {
        // 无更多候选，更新最优解
        if ((int)curClique.size() > bestSize) {
            bestSize = curClique.size();
            bestClique = curClique;
        }
        return;
    }

    // 剪枝：即使把所有候选都加入也不如最优
    if (curClique.size() + candidates.size() <= bestSize) return;  // [限界函数] 最大可能团也不如最优，剪枝

    while (!candidates.empty()) {
        int v = candidates.back();
        candidates.pop_back();

        // 将 v 加入当前团
        curClique.push_back(v);

        // 更新候选集为 v 的邻居与原候选集的交集
        vector<int> newCandidates;
        for (int u : candidates) {
            if (adj[v][u]) {  // [约束函数] 候选顶点必须与团内所有顶点相邻
                newCandidates.push_back(u);
            }
        }

        Backtrack(newCandidates);

        // 回溯
        curClique.pop_back();
    }
}

int main() {
    cout << "=== 最大团问题 (回溯法) ===" << endl;

    // 5 个顶点的示例图
    n = 5;
    adj.assign(n, vector<bool>(n, false));

    // 边集：0-1, 0-2, 0-3, 1-2, 1-3, 2-3, 2-4, 3-4
    vector<pair<int,int>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3},{2,4},{3,4}};
    for (auto [u, v] : edges) {
        adj[u][v] = adj[v][u] = true;
    }

    cout << "图的邻接矩阵:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << adj[i][j] << " ";
        cout << endl;
    }

    vector<int> candidates;
    for (int i = 0; i < n; i++) candidates.push_back(i);
    reverse(candidates.begin(), candidates.end());  // 使 pop_back 按 0..n-1

    Backtrack(candidates);

    cout << "\n最大团大小: " << bestSize << endl;
    cout << "最大团顶点: ";
    for (int v : bestClique) cout << v << " ";
    cout << endl;

    return 0;
}
