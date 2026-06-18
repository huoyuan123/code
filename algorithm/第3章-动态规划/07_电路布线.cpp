/*
 * 电路布线 (Circuit Wiring)
 * -----------------------------
 * 问题描述：在一块电路板的上方有 n 个接线柱，下方也有 n 个接线柱。
 * 连接 (i, π(i)) 表示上方接线柱 i 与下方接线柱 π(i) 相连。
 * 当且仅当 i<j 且 π(i)>π(j) 时两条连线相交。
 * 求最大不相交子集（即最多不交叉连线数）。
 *
 * 算法思想（动态规划）：
 *   问题等价于求排列 π 的最长递增子序列 (LIS)。
 *   设 dp[i] 为以 π(i) 结尾的最长递增子序列长度。
 *   递推公式：
 *     dp[i] = max(dp[j] + 1)  (0 ≤ j < i, π[j] < π[i])
 *     dp[i] = 1               (前面没有更小的值)
 *   最终答案 = max(dp[0..n-1])
 *
 *   也可转化为 LCS：将下方接线柱按 1..n 排序，上方对应变为 π⁻¹，
 *   求 π⁻¹ 与 1..n 的 LCS。
 *
 * 时间复杂度：O(n²)（朴素）或 O(n log n)（LIS 优化）
 *   LIS O(n log n) 解法：维护 tails 数组，tails[l] 存储长度为 l 的
 *   递增子序列的最小末尾值。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.8节
 */

 /*
 * 空间复杂度：O(n)
 *
 * 【阅读指引 / 易错点】
 *   - 这类“布线/连线”DP常可转化为序列问题（如最长上升子序列/LCS 的变体）：关键是把“无交叉/不冲突”转换成序关系。
 *   - 状态定义一定要写清：dp[i] 表示以第 i 根线为结尾的最优值，还是 dp[i][j] 表示两个端点的最优值。
 *   - 若要还原具体连线方案：额外记录前驱数组 prev，然后从最优末端回溯。
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 电路布线 —— O(n²) 动态规划
 */
int CircuitWiring_DP(const vector<int>& pi) {
    int n = pi.size();
    vector<int> dp(n, 1);  // 每个元素自身构成长度为 1 的 LIS

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (pi[j] < pi[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }

    return *max_element(dp.begin(), dp.end());
}

/**
 * 电路布线 —— O(n log n) 优化（贪心+二分）
 * tails[l] = 长度为 l 的递增子序列的最小末尾值
 */
int CircuitWiring_Fast(const vector<int>& pi) {
    vector<int> tails;  // tails 始终保持有序

    for (int val : pi) {
        // 二分搜索：找 tails 中第一个 ≥ val 的位置
        auto it = lower_bound(tails.begin(), tails.end(), val);
        if (it == tails.end()) {
            tails.push_back(val);  // 扩展 LIS 长度
        } else {
            *it = val;             // 替换，保持末尾值尽可能小
        }
    }

    return tails.size();
}

int main() {
    cout << "=== 电路布线 (最大不相交子集) ===" << endl;

    // 上接线柱 i:   1  2  3  4  5  6  7  8  9  10
    // 下接线柱 π(i): 8  7  4  2  5  1  9  3  10  6
    vector<int> pi = {8, 7, 4, 2, 5, 1, 9, 3, 10, 6};

    cout << "连线排列 π: ";
    for (int v : pi) cout << v << " ";
    cout << endl;

    cout << "最大不相交连线数 (DP O(n²)):  " << CircuitWiring_DP(pi) << endl;
    cout << "最大不相交连线数 (Fast O(nlogn)): " << CircuitWiring_Fast(pi) << endl;

    return 0;
}
