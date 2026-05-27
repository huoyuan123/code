/*
 * 连续邮资问题 (Continuous Postage) - 回溯法
 * ---------------------------------------------
 * 问题描述：给定面值种类数 n 和最大允许的张数 m。
 * 求 n 种不同面值邮票的设计方案，使得从 1 开始能连续表示的邮资最大。
 * 每封信最多贴 m 张。
 *
 * 算法思想（回溯法）：
 *   设 x[1..k] 为已选面值（x[1]=1 保证能表示 1）。
 *   r = 当前 x[1..k] 能连续表示的最大邮资值。
 *   下一面值 x[k+1] 的范围：[x[k]+1, r+1]
 *   （若超过 r+1，则 r+1 无法被表示，破坏了连续性）
 *
 *   用 DP 验证当前面值集合的可表示范围：
 *     dp[s] = 表示邮资 s 所需的最小张数
 *     dp[0] = 0, dp[s] = min(dp[s - x[i]]) + 1
 *
 * 时间复杂度：搜索树规模很大，通过上下界约束减少搜索
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.12节
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n, m;                      // n 种面值，最多贴 m 张
vector<int> bestX;             // 最优面值方案
int bestMax = 0;               // 最优连续邮资
vector<int> curX;              // 当前面值方案（x[0]=1）

/**
 * 用 DP 计算当前面值集合能连续表示的最大邮资
 * @param k 当前已选面值种类数
 * @return  从 1 开始连续可表示的最大邮资值
 */
int MaxContinuous(int k) {
    int maxVal = curX[k-1] * m;     // 最大可能邮资
    vector<int> dp(maxVal + 1, INT_MAX);
    dp[0] = 0;

    for (int i = 1; i <= maxVal; i++) {
        for (int j = 0; j < k; j++) {
            if (i >= curX[j] && dp[i - curX[j]] != INT_MAX) {
                dp[i] = min(dp[i], dp[i - curX[j]] + 1);
            }
        }
        // 一旦需要超过 m 张，停止
        if (dp[i] > m) {
            return i - 1;  // 上一个值就是最大连续范围
        }
    }
    return maxVal;
}

/**
 * 连续邮资 - 回溯法
 * @param k    当前已选面值种类数
 * @param maxR 当前 k 种面值能连续表示的最大值
 */
void Backtrack(int k, int maxR) {
    if (k == n) {
        if (maxR > bestMax) {
            bestMax = maxR;
            bestX = curX;
        }
        return;
    }

    // 尝试下一面值：范围 [curX[k-1]+1, maxR+1]
    for (int val = curX[k-1] + 1; val <= maxR + 1; val++) {
        curX[k] = val;
        int newMaxR = MaxContinuous(k + 1);

        // 剪枝：当前方案可能达到的最优上限
        // 剩余 (n-k-1) 个面值，保守估计每增加一个面值至少将连续范围扩大 m 倍
        if (newMaxR * (1 + m * (n - k - 1)) <= bestMax) {
            continue;  // 不可能超过最优
        }

        Backtrack(k + 1, newMaxR);
    }
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    n = 4; m = 5;
    cout << "=== 连续邮资问题 (回溯法) ===" << endl;
    cout << "面值种数=" << n << ", 最大张数=" << m << endl;

    curX.assign(n, 0);
    curX[0] = 1;  // 必须有种面值为 1
    bestX.assign(n, 0);

    int maxR = MaxContinuous(1);  // 只有面值 1 时最大连续 = m

    Backtrack(1, maxR);

    cout << "\n最优面值方案: ";
    for (int i = 0; i < n; i++) cout << bestX[i] << " ";
    cout << endl;
    cout << "最大连续邮资: 1 到 " << bestMax << endl;

    return 0;
}
