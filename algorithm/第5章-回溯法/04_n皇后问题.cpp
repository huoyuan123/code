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
 * 算法来源：《计算机算法设计与分析(第5版)》第5.5节
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
        if (x[i] == col || abs(row - i) == abs(col - x[i]))
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

        if (!colUsed[col] && !diag1[d1] && !diag2[d2]) {
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
