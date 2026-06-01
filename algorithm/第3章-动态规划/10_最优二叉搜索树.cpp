/*
 * 最优二叉搜索树 (Optimal Binary Search Tree)
 * ---------------------------------------------
 * 问题描述：给定 n 个关键字 k₁<k₂<...<kₙ 及其搜索概率 pᵢ，
 * 和 n+1 个伪关键字（空子树）的搜索概率 qⱼ。
 * 求一棵二叉搜索树使得搜索的期望代价最小。
 *
 * 算法思想（动态规划）：
 *   设 e[i][j] = 包含关键字 kᵢ...kⱼ 的最优 BST 的期望搜索代价。
 *   设 w[i][j] = Σ pₖ + Σ qₘ （区间 [i,j] 的概率之和）
 *   递推公式：
 *     e[i][j] = min(e[i][r-1] + e[r+1][j] + w[i][j])  (i ≤ r ≤ j)
 *     其中 root[i][j] 记录最优根节点 r
 *
 *   边界：e[i][i-1] = qᵢ₋₁ （只含伪关键字 dᵢ₋₁）
 *
 * 时间复杂度：O(n³)（可用 Knuth 优化降为 O(n²)）
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.11节
 */

 * 空间复杂度：O(n²)
 *
 * 【阅读指引 / 易错点】
 *   - 概念区分：p_i 是关键字 k_i 的成功搜索概率；q_i 是“落在 k_i 与 k_{i+1} 之间”的失败搜索概率。
 *   - 典型三表：e[i][j]（期望代价）、w[i][j]（概率和）、root[i][j]（最优根），都是区间 DP。
 *   - 下标最易错：很多教材用 1..n 的关键字下标，但 q 需要 0..n；实现时要先把下标约定写死再填表。
#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
#include "../encoding_fix.h"
using namespace std;

/**
 * 最优二叉搜索树 - 动态规划
 * @param p    关键字搜索概率 p[1..n]
 * @param q    伪关键字搜索概率 q[0..n]
 * @param e    e[i][j] 期望搜索代价
 * @param root root[i][j] 最优根节点
 */
void OptimalBST(const vector<double>& p, const vector<double>& q,
                vector<vector<double>>& e,
                vector<vector<double>>& w,
                vector<vector<int>>& root) {
    int n = p.size() - 1;

    // 初始化：w[i][i-1] = q[i-1], e[i][i-1] = q[i-1]
    for (int i = 1; i <= n + 1; i++) {
        e[i][i-1] = q[i-1];
        w[i][i-1] = q[i-1];
    }

    // l = 区间长度 (1 到 n)
    for (int l = 1; l <= n; l++) {
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            e[i][j] = 1e18;  // 初始化为极大值
            w[i][j] = w[i][j-1] + p[j] + q[j];

            // 枚举根节点 r
            for (int r = i; r <= j; r++) {
                double cost = e[i][r-1] + e[r+1][j] + w[i][j];
                if (cost < e[i][j]) {
                    e[i][j] = cost;
                    root[i][j] = r;  // 记录最优根
                }
            }
        }
    }
}

/**
 * 输出 BST 结构（先序遍历格式）
 */
void PrintBST(const vector<vector<int>>& root, int i, int j, int depth = 0) {
    if (i > j) {
        cout << string(depth * 4, ' ') << "d" << (i-1) << " (伪关键字)" << endl;
        return;
    }
    int r = root[i][j];
    cout << string(depth * 4, ' ') << "k" << r << " (根)" << endl;
    PrintBST(root, i, r - 1, depth + 1);    // 左子树
    PrintBST(root, r + 1, j, depth + 1);    // 右子树
}

int main() {
    cout << "=== 最优二叉搜索树 ===" << endl;

    // 示例：5 个关键字的搜索概率
    vector<double> p = {0, 0.15, 0.10, 0.05, 0.10, 0.20};  // p[1..5]
    vector<double> q = {0.05, 0.10, 0.05, 0.05, 0.05, 0.10}; // q[0..5]

    int n = p.size() - 1;

    // 分配 (n+2) × (n+1) 的表，下标 [0..n+1]
    vector<vector<double>> e(n + 2, vector<double>(n + 1, 0));
    vector<vector<double>> w(n + 2, vector<double>(n + 1, 0));
    vector<vector<int>> root(n + 2, vector<int>(n + 1, 0));

    OptimalBST(p, q, e, w, root);

    cout << fixed << setprecision(4);
    cout << "最小期望搜索代价: " << e[1][n] << endl;

    cout << "\n最优 BST 结构:" << endl;
    PrintBST(root, 1, n);

    // 输出 e 表（可选）
    cout << "\ne[i][j] 表:" << endl;
    for (int i = 1; i <= n + 1; i++) {
        for (int j = 0; j <= n; j++) {
            if (i > j + 1)
                cout << setw(10) << "-";
            else
                cout << setw(10) << e[i][j];
        }
        cout << endl;
    }

    return 0;
}
