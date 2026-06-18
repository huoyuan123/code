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
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：逐步确定邮票面值（或使用数量），每层决定一个面值后评估“能连续凑出的邮资区间”。
 *   - 限界剪枝：若在当前面值集合下，连续覆盖上界已经被限制住，继续扩展也不可能超过 best 则回溯。
 *   - 评估函数：计算“连续可达的最大邮资”时要小心循环边界与可达性初始化（常见做法是用 DP/可达数组）。
 *
 * 【实现提示】
 *   - 回溯核心：确定下一个面值范围 [x[k-1]+1, maxR+1]。
 *   - 剪枝核心：用 MaxContinuous 的结果估计上界，不可能超过 bestMax 时跳过。
 *   - 初始化：x[0] 必须为 1，否则无法连续覆盖从 1 开始的邮资。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.12节

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
        // 剩余 t = n-k-1 个面值，每增加一个面值最多将连续范围扩大 (m+1) 倍
        long long bound = newMaxR;
        for (int t = 0; t < n - k - 1; t++) bound *= (m + 1);
        if (bound <= bestMax) {  // [限界函数] 扩大上界估计：加剩余面值也不如最优，剪枝
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
