/*
 * 多边形游戏 (Polygon Game)
 * -----------------------------
 * 问题描述：给定一个由 n 个顶点构成的多边形，每个顶点有一个整数值，
 * 每条边有一个运算符（+ 或 *）。游戏过程：
 *   1. 删除一条边
 *   2. 重复以下步骤 n-2 次：选择一条边 E 及其两端顶点 u, v，
 *      用新顶点 (u op v) 替代，删除边 E
 *   3. 最后剩下的顶点值为游戏得分
 * 求最大可能得分及首次删除哪条边可获得该得分。
 *
 * 算法思想（动态规划）：
 *   与矩阵连乘类似，但乘法涉及负数，需要同时维护最大值和最小值：
 *     dpm[i][j] = 从顶点 i 开始的 j+1 个顶点的最小值
 *     dpM[i][j] = 从顶点 i 开始的 j+1 个顶点的最大值
 *   合并时考虑四种情况：max*max, max*min, min*max, min*min
 *
 * -----------------------------
 * 【动态规划 - 最小子问题】
 *   链长为 1（len=1）：dpm[i][0] = dpM[i][0] = v[i]
 *   即只有一个顶点、无需任何运算时，最大值和最小值都等于该顶点的值。
 *   这是 DP 递推的边界（base case），所有更长链的结果都由它逐层合并得到。
 *
 * 【时间复杂度分析】
 *   PolygonGame()：O(n³)
 *     三层循环：外层 len(2..n)=O(n)，中层 i(0..n-1)=O(n)，
 *             内层 s(1..len-1)=O(n)
 *     乘积为 O(n³)，内部 min/max 在 4 个元素中取值，为 O(1)
 *   main()：O(n³)
 *     调用 PolygonGame() 为 O(n³)，输出部分为 O(n)，
 *     整体由 O(n³) 主导
 * -----------------------------
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.6节
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 多边形游戏 - 动态规划
 * @param v     顶点值数组
 * @param op    运算符数组（'t'=+, 'x'=*）
 * @param maxV  返回的最大得分
 */
int PolygonGame(const vector<int>& v, const vector<char>& op) {
    int n = v.size();

    // m[i][j][0]=min, m[i][j][1]=max：从 i 起 j 个顶点的链
    vector<vector<int>> dpm(n, vector<int>(n, 0));   // 最小值
    vector<vector<int>> dpM(n, vector<int>(n, 0));   // 最大值

    // 初始化：长度为 1 的链
    for (int i = 0; i < n; i++) {
        dpm[i][0] = v[i];
        dpM[i][0] = v[i];
    }

    // len 为链长（2 到 n）
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i < n; i++) {
            dpm[i][len-1] = INT_MAX;
            dpM[i][len-1] = INT_MIN;

            // 枚举断开位置 s（1 到 len-1）
            for (int s = 1; s < len; s++) {
                int j = (i + s) % n;  // 断开位置对应的实际顶点

                int a = dpm[i][s-1];           // 左侧最小
                int b = dpM[i][s-1];           // 左侧最大
                int c = dpm[j][len-s-1];       // 右侧最小
                int d = dpM[j][len-s-1];       // 右侧最大

                vector<int> candidates;
                if (op[j] == 't') {
                    // 加法
                    candidates = {a + c, a + d, b + c, b + d};
                } else {
                    // 乘法：四个组合（考虑负数*负数得正数的情况）
                    candidates = {a * c, a * d, b * c, b * d};
                }

                int localMin = *min_element(candidates.begin(), candidates.end());
                int localMax = *max_element(candidates.begin(), candidates.end());

                dpm[i][len-1] = min(dpm[i][len-1], localMin);
                dpM[i][len-1] = max(dpM[i][len-1], localMax);
            }
        }
    }

    // 找全局最大
    int ans = INT_MIN;
    for (int i = 0; i < n; i++) {
        ans = max(ans, dpM[i][n-1]);
    }
    return ans;
}

int main() {
    cout << "=== 多边形游戏 ===" << endl;

    // 示例：4 个顶点的多边形，运算符 t=加法, x=乘法
    vector<int> v = {2, -1, 5, 3};
    vector<char> op = {'t', 'x', 't', 'x'};
    // 拓扑：v0 -t- v1 -x- v2 -t- v3 -x- (回到 v0)

    cout << "顶点值: ";
    for (int val : v) cout << val << " ";
    cout << endl;
    cout << "运算符号: ";
    for (char c : op) cout << c << " ";
    cout << endl;

    cout << "最大得分: " << PolygonGame(v, op) << endl;

    return 0;
}
