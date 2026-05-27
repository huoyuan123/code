/*
 * 矩阵连乘问题 (Matrix Chain Multiplication)
 * ---------------------------------------------
 * 问题描述：给定 n 个矩阵的链 A₁A₂...Aₙ，矩阵 Aᵢ 的维度为 pᵢ₋₁ × pᵢ。
 * 求完全括号化方式使得标量乘法次数最少。
 *
 * 算法思想（动态规划）：
 *   设 m[i][j] 为计算 Aᵢ...Aⱼ 所需的最少乘法次数。
 *   递推公式：
 *     m[i][j] = 0                              (i = j)
 *     m[i][j] = min(m[i][k] + m[k+1][j] + pᵢ₋₁pₖpⱼ)  (i ≤ k < j)
 *   用 s[i][j] 记录最优断开位置，便于构造最优解。
 *
 * 时间复杂度：O(n³) —— 三重循环
 * 空间复杂度：O(n²)
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.1节
 */

#include <iostream>
#include <vector>
#include <climits>
#include "../encoding_fix.h"
using namespace std;

/**
 * 矩阵连乘 - 动态规划
 * @param p 矩阵维度数组，p[0..n] 表示 A₁(p₀×p₁) ... Aₙ(pₙ₋₁×pₙ)
 * @param m m[i][j] 记录最少乘法次数
 * @param s s[i][j] 记录最优断开位置
 */
void MatrixChain(const vector<int>& p,
                 vector<vector<int>>& m,
                 vector<vector<int>>& s) {
    int n = p.size() - 1;  // 矩阵个数

    // 初始化：单个矩阵乘法次数为 0
    for (int i = 1; i <= n; i++)
        m[i][i] = 0;

    // r 为链长（从 2 到 n）
    for (int r = 2; r <= n; r++) {
        for (int i = 1; i <= n - r + 1; i++) {
            int j = i + r - 1;
            m[i][j] = INT_MAX;  // 初始化为无穷大

            // 枚举断开位置 k，i ≤ k < j
            for (int k = i; k < j; k++) {
                int cost = m[i][k] + m[k+1][j] + p[i-1] * p[k] * p[j];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = k;  // 记录最优断开点
                }
            }
        }
    }
}

/**
 * 根据 s 数组构造最优括号化方案（递归输出）
 */
void PrintOptimalParens(const vector<vector<int>>& s, int i, int j) {
    if (i == j) {
        cout << "A" << i;
    } else {
        cout << "(";
        PrintOptimalParens(s, i, s[i][j]);      // 左半部分
        PrintOptimalParens(s, s[i][j] + 1, j);   // 右半部分
        cout << ")";
    }
}

int main() {
    cout << "=== 矩阵连乘问题 ===" << endl;

    // 示例：A₁:30×35, A₂:35×15, A₃:15×5, A₄:5×10, A₅:10×20, A₆:20×25
    vector<int> p = {30, 35, 15, 5, 10, 20, 25};
    int n = p.size() - 1;

    // 下标从 1 开始使用，所以分配 n+1 大小
    vector<vector<int>> m(n + 1, vector<int>(n + 1, 0));
    vector<vector<int>> s(n + 1, vector<int>(n + 1, 0));

    MatrixChain(p, m, s);

    cout << "矩阵维度: ";
    for (int i = 1; i <= n; i++)
        cout << "A" << i << "(" << p[i-1] << "×" << p[i] << ") ";
    cout << endl;

    cout << "最少乘法次数: " << m[1][n] << endl;
    cout << "最优括号方案: ";
    PrintOptimalParens(s, 1, n);
    cout << endl;

    return 0;
}
