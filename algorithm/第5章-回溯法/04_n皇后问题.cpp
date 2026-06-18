/*
 * n 皇后问题 (N-Queens) - 回溯法
 * ---------------------------------
 * 问题描述：在 n×n 棋盘放置 n 个皇后，要求互不攻击
 * （即任意两个皇后不在同一行、同一列、同一对角线）。
 * 求所有可行摆放方案。
 *
 * 算法思想（回溯法）：
 *   解空间：排列树。
 *   每行放一个皇后，x[i] 表示第 i 行皇后放在第 x[i] 列。
 *   约束条件：
 *     - 不同列：x[i] ≠ x[j]
 *     - 不同对角线：|i - j| ≠ |x[i] - x[j]|
 *   按行逐层搜索，一旦冲突立即回溯。
 *
 * 时间复杂度：最坏 O(n!)，通过剪枝大幅减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：排列树（每行放 1 个皇后，列不能重复）；冲突检测再加上两条对角线约束。
 *   - O(1) 冲突检测：colUsed/diag1/diag2 这种标记数组能把 IsSafe 从 O(n) 降到 O(1)，回溯效率差异很大。
 *   - 对角线下标：常用 row+col 与 row-col+n-1 做索引；最容易错的是偏移量和数组长度。
 *
 * 【实现提示】
 *   - 回溯核心：每层放一行，尝试所有列；放置与撤销要同步更新三类标记数组。
 *   - 剪枝核心：一旦列/对角线冲突立即回溯，不再深入。
 *   - 输出映射：若需要输出棋盘，注意行列是 0/1 基。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.5节

 *
 * 解空间树：排列树（Permutation Tree）
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
#include <cmath>
#include "../encoding_fix.h"
using namespace std;

int n;                      // 棋盘大小
vector<int> x;              // x[i] = 第 i 行皇后所在的列
vector<bool> colUsed;       // 列占用标记
vector<bool> diag1;         // 主对角线占用 (row+col 为常量)
vector<bool> diag2;         // 副对角线占用 (row-col 为常量)
int solutionCount = 0;

/**
 * 检查当前位置是否安全（不使用全局标记的版本）
 */
bool IsSafe(int row, int col) {
    for (int i = 0; i < row; i++) {
        // 同列 或 同对角线
        if (x[i] == col || abs(row - i) == abs(col - x[i]))  // [约束函数] 检查同列/同对角线冲突
            return false;
    }
    return true;
}

/**
 * n 皇后 - 回溯法（使用标记数组加速，O(1) 检查冲突）
 * @param row 当前放置的行号 (0-indexed)
 */
void Backtrack_Fast(int row) {
    if (row == n) {
        solutionCount++;
        // 输出第一个解
        if (solutionCount == 1) {
            cout << "  (首个解) 皇后位置 (行,列): ";
            for (int i = 0; i < n; i++)
                cout << "(" << i+1 << "," << x[i]+1 << ") ";
            cout << endl;
        }
        return;
    }

    for (int col = 0; col < n; col++) {
        int d1 = row + col;
        int d2 = row - col + n - 1;  // 偏移使索引非负

        if (!colUsed[col] && !diag1[d1] && !diag2[d2]) {  // [约束函数] O(1)检查列和对角线冲突
            // 放置皇后
            x[row] = col;
            colUsed[col] = diag1[d1] = diag2[d2] = true;

            Backtrack_Fast(row + 1);

            // 回溯
            colUsed[col] = diag1[d1] = diag2[d2] = false;
        }
    }
}

/**
 * 打印棋盘
 */
void PrintBoard(const vector<int>& solution) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << (solution[i] == j ? " Q" : " .");
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    n = 8;
    cout << "=== n 皇后问题 ===" << endl;
    cout << "棋盘大小 n = " << n << endl;

    x.assign(n, 0);
    colUsed.assign(n, false);
    diag1.assign(2 * n - 1, false);
    diag2.assign(2 * n - 1, false);

    Backtrack_Fast(0);

    cout << "\nn=" << n << " 时共有 " << solutionCount << " 种解法" << endl;

    return 0;
}
