/*
 * 0-1 背包问题 (0-1 Knapsack Problem) - 动态规划解法
 * -------------------------------------------------------
 * 问题描述：给定 n 个物品，物品 i 重量 wᵢ，价值 vᵢ，背包容量 C。
 * 每个物品要么全放入要么不放入（0-1 约束）。
 * 求总价值最大的装入方案。
 *
 * 算法思想（动态规划）：
 *   设 dp[i][j] = 前 i 个物品、容量 j 时的最大价值。
 *   递推公式：
 *     dp[i][j] = dp[i-1][j]                         (j < wᵢ，放不下)
 *     dp[i][j] = max(dp[i-1][j], dp[i-1][j-wᵢ]+vᵢ)  (j ≥ wᵢ)
 *
 * 空间优化：滚动数组降为 O(C)
 *   逆序更新：for j = C downto wᵢ
 *     dp[j] = max(dp[j], dp[j-wᵢ] + vᵢ)
 *
 * 时间复杂度：O(nC)
 * 空间复杂度：O(C)（优化后）
 *
 * 注：贪心算法只适用于可分割的背包问题（第4章），
 *     0-1 背包问题需用动态规划或回溯/分支限界。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.10节
 */

 /*
 * 空间复杂度：O(nC)（可优化为 O(C)）
 *
 * 【阅读指引 / 易错点】
 *   - 状态 dp[i][j]：前 i 件物品在容量 j 下的最大价值（或最大重量）；转移为“不选 i”与“选 i”。
 *   - 初始化：dp[0][*]=0；若用 -INF 表示不可达，转移时要先判断前态。
 *   - 一维优化：若把 dp 压成一维，容量 j 必须逆序循环（从大到小），否则会把同一物品用多次。
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 0-1 背包 - 二维 DP
 * @return 最大总价值
 */
int Knapsack_2D(const vector<int>& w, const vector<int>& v, int C) {
    int n = w.size();
    vector<vector<int>> dp(n + 1, vector<int>(C + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= C; j++) {
            if (j < w[i-1]) {
                dp[i][j] = dp[i-1][j];
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i-1][j - w[i-1]] + v[i-1]);
            }
        }
    }
    return dp[n][C];
}

/**
 * 0-1 背包 - 空间优化（一维滚动数组）
 * @param selected 返回是否选中各物品（用于重建方案）
 */
int Knapsack_1D(const vector<int>& w, const vector<int>& v, int C,
                 vector<bool>& selected) {
    int n = w.size();
    vector<int> dp(C + 1, 0);
    // trace[i][j] 记录 dp[i][j] 是否选用了物品 i
    vector<vector<bool>> trace(n + 1, vector<bool>(C + 1, false));

    for (int i = 1; i <= n; i++) {
        // 必须逆序更新，确保每个物品只用一次
        for (int j = C; j >= w[i-1]; j--) {
            if (dp[j - w[i-1]] + v[i-1] > dp[j]) {
                dp[j] = dp[j - w[i-1]] + v[i-1];
                trace[i][j] = true;
            }
        }
    }

    // 回溯重建选择的物品
    int j = C;
    for (int i = n; i >= 1; i--) {
        if (trace[i][j]) {
            selected[i-1] = true;
            j -= w[i-1];
        }
    }

    return dp[C];
}

int main() {
    cout << "=== 0-1 背包问题 (动态规划) ===" << endl;

    vector<int> w = {2, 3, 4, 5};   // 重量
    vector<int> v = {3, 4, 5, 6};   // 价值
    int C = 8;                        // 背包容量

    cout << "物品 (重量, 价值):" << endl;
    for (size_t i = 0; i < w.size(); i++)
        cout << "  物品" << (i+1) << ": (" << w[i] << ", " << v[i] << ")" << endl;
    cout << "背包容量: " << C << endl;

    cout << "\n二维 DP 结果: " << Knapsack_2D(w, v, C) << endl;

    vector<bool> selected(w.size(), false);
    int maxV = Knapsack_1D(w, v, C, selected);
    cout << "一维 DP 结果: " << maxV << endl;
    cout << "选中物品: ";
    for (size_t i = 0; i < selected.size(); i++)
        if (selected[i]) cout << "物品" << (i+1) << " ";
    cout << endl;

    return 0;
}
