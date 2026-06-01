/*
 * 电路板排列问题 (Circuit Board Permutation) - 回溯法
 * -------------------------------------------------------
 * 问题描述：n 块电路板，m 个连接块。用排列 π 表示电路板在机箱中的插入顺序。
 * 连接块中的电路板必须在连续的槽位中，跨过的槽位数称为密度。
 * 求使最大密度最小的排列方案。
 *
 * 算法思想（回溯法）：
 *   解空间：排列树
 *   设 now[j] 为连接块 j 中当前已排列的电路板数量，
 *   total[j] 为连接块 j 中的电路板总数。
 *   当 now[j] > 0 且 now[j] < total[j] 时，连接块 j 处于"打开"状态，计入密度。
 *   每次放入一块电路板后更新密度，若超过当前最优则剪枝。
 *
 * 时间复杂度：最坏 O(n!)
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：排列树/组合树（决定模块放置顺序或位置）；评价函数通常是总连线长/交叉数等。
 *   - 剪枝：用当前已放模块产生的最小可能代价作为下界；若下界已不优于 best 则回溯。
 *   - 状态恢复：放置/撤销时要同时恢复与该模块相关的边界、累计代价、可用位置集合。
 *
 * 【实现提示】
 *   - 回溯核心：每放一块板更新 now[] 与密度，再递归。
 *   - 剪枝核心：当前密度已 ≥ bestDensity 时直接回溯。
 *   - 状态撤销：回溯时要对 now[] 做对称撤销，避免影响兄弟分支。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.11节
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n, m;                       // 电路板数 n，连接块数 m
vector<vector<bool>> block;     // block[i][j] = 电路板 i 是否属于连接块 j
vector<int> total;              // total[j] = 连接块 j 中的电路板总数
vector<int> now;                // now[j] = 连接块 j 中已排列的电路板数
vector<int> bestOrder;          // 最优排列
vector<int> curOrder;           // 当前排列
vector<bool> used;              // 电路板是否已排列
int bestDensity = INT_MAX;      // 最小最大密度

/**
 * 电路板排列 - 回溯法
 * @param k       当前排列到第 k 个位置 (0-indexed)
 * @param curDens 当前密度
 */
void Backtrack(int k, int curDens) {
    if (k == n) {
        if (curDens < bestDensity) {
            bestDensity = curDens;
            bestOrder = curOrder;
        }
        return;
    }

    // 剪枝：当前密度已不低于最优值
    if (curDens >= bestDensity) return;

    for (int i = 0; i < n; i++) {
        if (used[i]) continue;

        used[i] = true;
        curOrder[k] = i;

        // 更新连接块状态
        int newDens = curDens;
        for (int j = 0; j < m; j++) {
            if (block[i][j]) {
                now[j]++;
                // 连接块 j 被部分占用时，每个部分占用计 1
                if (now[j] > 0 && now[j] < total[j]) {
                    // 第一次进入打开状态时密度+1
                }
                if (now[j] == 1 && total[j] > 1) newDens++;
            }
        }

        // 重新计算当前密度
        int actualDens = 0;
        for (int j = 0; j < m; j++) {
            if (now[j] > 0 && now[j] < total[j]) actualDens++;
        }

        Backtrack(k + 1, actualDens);

        // 回溯
        for (int j = 0; j < m; j++) {
            if (block[i][j]) now[j]--;
        }
        used[i] = false;
    }
}

int main() {
    cout << "=== 电路板排列问题 (回溯法) ===" << endl;

    // 示例：8 块电路板，5 个连接块
    n = 8; m = 5;

    block.assign(n, vector<bool>(m, false));
    // 连接块定义
    vector<vector<int>> blockDef = {
        {0, 0, 1, 0, 0},  // 电路板 0
        {0, 1, 0, 0, 0},  // 电路板 1
        {0, 0, 0, 1, 1},  // 电路板 2
        {1, 0, 0, 0, 0},  // 电路板 3
        {0, 0, 0, 1, 0},  // 电路板 4
        {0, 1, 0, 0, 0},  // 电路板 5
        {0, 0, 0, 0, 1},  // 电路板 6
        {0, 0, 1, 0, 0}   // 电路板 7
    };

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            block[i][j] = blockDef[i][j];

    // 统计每个连接块的电路板总数
    total.assign(m, 0);
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            if (block[i][j]) total[j]++;

    now.assign(m, 0);
    curOrder.assign(n, 0);
    used.assign(n, false);

    cout << "电路板数: " << n << ", 连接块数: " << m << endl;

    Backtrack(0, 0);

    cout << "最小最大密度: " << bestDensity << endl;
    cout << "最优排列: ";
    for (int v : bestOrder) cout << v << " ";
    cout << endl;

    return 0;
}
