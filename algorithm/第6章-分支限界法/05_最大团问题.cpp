/*
 * 最大团问题 - 分支限界法
 * ---------------------------
 * 问题描述：同第5.7节。求无向图的最大完全子图（团）。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   每个节点代表一个部分团（已选了某些顶点）。
 *   优先队列按团的潜在大小（上界）降序排列。
 *   上界 = 当前团大小 + 候选顶点数（可能再加入的最多顶点）
 *   每次取上界最大的节点扩展。
 *
 * 时间复杂度：最坏 O(n·2ⁿ)，通过限界减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 界函数常用“剩余可选顶点数”或更强的图上界（如染色上界）估计最大可能团大小。
 *   - 搜索策略：分支限界往往比纯回溯更偏向“优先扩展最有希望的节点”，更快找到较大 best 从而加速剪枝。
 *   - 易错点：节点扩展要确保加入顶点后仍是团（与当前团内所有点相邻）。
 *
 * 【实现提示】
 *   - 活节点表：按 bound 最大优先扩展，尽快更新 bestSize。
 *   - 界函数：bound = 当前团大小 + 候选数，是一个乐观上界。
 *   - 状态拷贝：扩展节点时要复制 clique/candidates，避免破坏原节点。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.6节

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
#include <queue>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n;
vector<vector<bool>> adj;
vector<int> bestClique;
int bestSize = 0;

struct BBNode {
    vector<int> clique;     // 当前团（顶点集）
    vector<int> candidates; // 候选顶点（与团中所有顶点相邻）
    int bound;              // 上界（当前大小 + 候选数）

    BBNode(const vector<int>& c, const vector<int>& cand)
        : clique(c), candidates(cand), bound(c.size() + cand.size()) {}

    bool operator<(const BBNode& other) const {
        return bound < other.bound;  // 最大优先队列
    }
};

void MaxClique_BB() {
    priority_queue<BBNode> pq;

    vector<int> allVertices;
    for (int i = 0; i < n; i++) allVertices.push_back(i);
    reverse(allVertices.begin(), allVertices.end());

    pq.push(BBNode({}, allVertices));

    while (!pq.empty()) {
        BBNode cur = pq.top(); pq.pop();

        // 记录最优解
        if (cur.clique.size() > bestSize) {
            bestSize = cur.clique.size();
            bestClique = cur.clique;
        }

        // 限界剪枝
        if (cur.bound <= bestSize) continue;  // [限界函数] 上界不优于最优，剪枝

        if (cur.candidates.empty()) continue;

        while (!cur.candidates.empty()) {
            int v = cur.candidates.back();
            cur.candidates.pop_back();

            // 加入 v 扩展团
            vector<int> newClique = cur.clique;
            newClique.push_back(v);

            // 新候选集 = v 的邻居 ∩ 原候选集
            vector<int> newCandidates;
            for (int u : cur.candidates) {
                if (adj[v][u]) {
                    newCandidates.push_back(u);
                }
            }

            if (newClique.size() + newCandidates.size() > bestSize) {
                pq.push(BBNode(newClique, newCandidates));
            }
        }
    }
}

int main() {
    cout << "=== 最大团问题 (分支限界法) ===" << endl;

    n = 5;
    adj.assign(n, vector<bool>(n, false));
    vector<pair<int,int>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3},{2,4},{3,4}};
    for (auto [u, v] : edges)
        adj[u][v] = adj[v][u] = true;

    cout << "图的邻接矩阵:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << adj[i][j] << " ";
        cout << endl;
    }

    MaxClique_BB();

    cout << "\n最大团大小: " << bestSize << endl;
    cout << "最大团顶点: ";
    sort(bestClique.begin(), bestClique.end());
    for (int v : bestClique) cout << v << " ";
    cout << endl;

    return 0;
}
