/*
 * 图的 m 着色问题 (Graph M-Coloring) - 回溯法
 * ------------------------------------------------
 * 问题描述：给定无向图 G=(V,E) 和 m 种颜色。
 * 为每个顶点着色，要求相邻顶点颜色不同。
 * 求所有可行的 m 着色方案及最少所需颜色数（色数）。
 *
 * 算法思想（回溯法）：
 *   解空间：完全 m 叉树（每个顶点有多种颜色选择）
 *   按顶点编号顺序着色，用当前所有相邻已着色顶点的颜色约束剪枝。
 *   约束函数：当前颜色不与任何相邻已着色顶点冲突。
 *
 * 时间复杂度：最坏 O(n·mⁿ)，实际上通过约束大幅降低
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：给每个顶点依次染色（每层决定一个顶点的颜色），这是典型的“m 叉树”搜索。
 *   - 约束剪枝：为顶点 v 赋色时，必须与所有已染色邻居颜色不同；冲突立即回溯。
 *   - 加速技巧：选择“度数更高/约束更强”的顶点先染色（启发式）通常能更早剪枝。
 *
 * 【实现提示】
 *   - 回溯核心：给顶点 v 依次尝试颜色 1..m，合法就递归到 v+1。
 *   - 剪枝核心：IsSafe 冲突则立即回溯，不再尝试更深层。
 *   - 状态撤销：回溯时清空 color[v]，避免影响后续分支。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.8节
 */

#include <iostream>
#include <vector>
#include "../encoding_fix.h"
using namespace std;

int n, m;                      // 顶点数、颜色数
vector<vector<bool>> adj;      // 邻接矩阵
vector<int> color;             // color[i] = 顶点 i 的颜色 (1..m)
int solutionCount = 0;         // 可行的着色方案数

/**
 * 检查顶点 v 是否能着颜色 c
 */
bool IsSafe(int v, int c) {
    for (int i = 0; i < n; i++) {
        if (adj[v][i] && color[i] == c)
            return false;  // 相邻顶点颜色冲突
    }
    return true;
}

/**
 * 图的 m 着色 - 回溯法
 * @param v 当前着色的顶点编号
 */
void Backtrack(int v) {
    if (v == n) {
        // 所有顶点已着色
        solutionCount++;
        if (solutionCount == 1) {
            cout << "  (首个可行方案) 着色: ";
            for (int i = 0; i < n; i++)
                cout << "v" << i << "=" << color[i] << " ";
            cout << endl;
        }
        return;
    }

    for (int c = 1; c <= m; c++) {
        if (IsSafe(v, c)) {
            color[v] = c;
            Backtrack(v + 1);
            color[v] = 0;  // 回溯
        }
    }
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    n = 5; m = 3;
    cout << "=== 图的 m 着色问题 ===" << endl;
    cout << "顶点数=" << n << ", 颜色数=" << m << endl;

    adj.assign(n, vector<bool>(n, false));
    vector<pair<int,int>> edges = {{0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{2,3},{2,4},{3,4}};
    cout << "边数=" << edges.size() << endl;
    for (auto [u, v] : edges) adj[u][v] = adj[v][u] = true;

    color.assign(n, 0);

    Backtrack(0);

    cout << "\n顶点数: " << n << ", 颜色数: " << m << endl;
    cout << "可行着色方案数: " << solutionCount << endl;

    return 0;
}
