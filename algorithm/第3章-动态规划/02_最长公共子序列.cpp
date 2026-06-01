/*
 * 最长公共子序列 (Longest Common Subsequence, LCS)
 * --------------------------------------------------
 * 问题描述：给定两个序列 X={x₁,...,xₘ} 和 Y={y₁,...,yₙ}，
 * 找出 X 和 Y 的最长公共子序列。
 *
 * 算法思想（动态规划）：
 *   设 c[i][j] 为 X[1..i] 和 Y[1..j] 的 LCS 长度。
 *   递推公式：
 *     c[i][j] = 0                          (i=0 或 j=0)
 *     c[i][j] = c[i-1][j-1] + 1            (xᵢ = yⱼ)
 *     c[i][j] = max(c[i-1][j], c[i][j-1])   (xᵢ ≠ yⱼ)
 *
 * 时间复杂度：O(mn)
 * 空间复杂度：O(mn)（可优化为 O(min(m,n))）
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.3节
 */

 * 空间复杂度：O(mn)
 *
 * 【阅读指引 / 易错点】
 *   - 状态 dp[i][j]：常用定义是 X 的前 i 个字符与 Y 的前 j 个字符的 LCS 长度（i/j 取 0..m/0..n）。
 *   - 转移：若末字符相等取 dp[i-1][j-1]+1，否则取 max(dp[i-1][j], dp[i][j-1])。
 *   - 方案还原：若代码有记录方向数组（如 b/trace），从 (m,n) 反向走即可输出一个 LCS。
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * LCS 长度计算
 * @param X  序列 X
 * @param Y  序列 Y
 * @param c  c[i][j] 记录 LCS 长度
 * @param b  b[i][j] 记录方向：1=↖, 2=↑, 3=←
 */
void LCSLength(const string& X, const string& Y,
               vector<vector<int>>& c,
               vector<vector<int>>& b) {
    int m = X.length();
    int n = Y.length();

    // 初始化边界
    for (int i = 0; i <= m; i++) c[i][0] = 0;
    for (int j = 0; j <= n; j++) c[0][j] = 0;

    // 递推填充
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i-1] == Y[j-1]) {
                // 字符匹配：从左上角转移
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 1;  // ↖ 方向
            } else if (c[i-1][j] >= c[i][j-1]) {
                // 取上方较大值
                c[i][j] = c[i-1][j];
                b[i][j] = 2;  // ↑ 方向
            } else {
                // 取左边较大值
                c[i][j] = c[i][j-1];
                b[i][j] = 3;  // ← 方向
            }
        }
    }
}

/**
 * 回溯构造 LCS
 * @param b 方向数组
 * @param X 原始序列 X
 * @param i 当前行索引
 * @param j 当前列索引
 */
void PrintLCS(const vector<vector<int>>& b, const string& X, int i, int j) {
    if (i == 0 || j == 0) return;  // 递归基

    if (b[i][j] == 1) {
        // ↖：当前字符属于 LCS
        PrintLCS(b, X, i - 1, j - 1);
        cout << X[i-1];
    } else if (b[i][j] == 2) {
        // ↑：向上回溯
        PrintLCS(b, X, i - 1, j);
    } else {
        // ←：向左回溯
        PrintLCS(b, X, i, j - 1);
    }
}

int main() {
    cout << "=== 最长公共子序列 (LCS) ===" << endl;

    string X = "ABCBDAB";
    string Y = "BDCABA";

    int m = X.length(), n = Y.length();
    vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));
    vector<vector<int>> b(m + 1, vector<int>(n + 1, 0));

    LCSLength(X, Y, c, b);

    cout << "序列 X: " << X << endl;
    cout << "序列 Y: " << Y << endl;
    cout << "LCS 长度: " << c[m][n] << endl;
    cout << "LCS 内容: ";
    PrintLCS(b, X, m, n);
    cout << endl;

    return 0;
}
