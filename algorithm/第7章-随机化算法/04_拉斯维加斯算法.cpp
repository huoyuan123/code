/*
 * 拉斯维加斯算法 (Las Vegas Algorithm)
 * ----------------------------------------
 * 问题描述：算法总能给出正确解，但运行时间具有随机性。
 *
 * 算法思想：
 *   以 n 皇后问题和随机快速排序为代表。
 *   拉斯维加斯算法的特点是：
 *     - 总能得到正确解
 *     - 运行时间是随机变量
 *     - 可能有概率很低的最坏运行时间
 *     - 期望运行时间通常很好
 *
 * 典型应用：
 *   1. 随机 n 皇后问题（拉斯维加斯版）
 *   2. 快速排序（随机选择 pivot）
 *   3. 大整数分解的 Pollard's Rho 算法
 *
 * 本文件演示：用拉斯维加斯算法求解 n 皇后问题。
 *   随机选择列的排列，检查是否满足对角线约束。
 *   若失败则重试（拉斯维加斯的特点）。
 *
 * 与舍伍德区别：
 *   - 拉斯维加斯：运行时间随机，解始终正确
 *   - 舍伍德：性能与输入无关（平滑化）
 *
 *
 * 【阅读指引 / 易错点】
 *   - 定义：拉斯维加斯算法“结果总正确”，但运行时间是随机变量（可能快也可能慢）。
 *   - 实践技巧：常用“重复试验 + 超时重启”把尾部慢情况压下去；期望时间仍保持良好。
 *   - 易错点：要区分“失败重试”与“允许错误”；拉斯维加斯允许重试但不允许输出错误答案。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第7.4节

 *
 * 解空间树：排列树（n皇后回溯部分）
 * 递归方式：深度优先搜索(DFS)
 */

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "../encoding_fix.h"
using namespace std;

int n;
mt19937 rng(random_device{}());

/**
 * 检查给定排列是否满足 n 皇后约束（对角线无冲突）
 */
bool CheckDiagonal(const vector<int>& cols) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (abs(i - j) == abs(cols[i] - cols[j]))
                return false;  // 对角线冲突
        }
    }
    return true;
}

/**
 * 拉斯维加斯算法求解 n 皇后问题
 * 每次随机生成一个列排列，检查是否满足条件
 * 期望尝试次数：对于小 n 很快找到解；对于大 n 需要更多尝试
 *
 * @param cols 返回皇后列位置
 * @return 尝试次数
 */
int LasVegasNQueens(vector<int>& cols) {
    vector<int> perm(n);
    for (int i = 0; i < n; i++) perm[i] = i;  // 每个皇后不同行不同列

    int attempts = 0;
    bool found = false;

    while (!found) {
        attempts++;
        shuffle(perm.begin(), perm.end(), rng);  // 随机排列
        if (CheckDiagonal(perm)) {
            cols = perm;
            found = true;
        }
    }

    return attempts;
}

/**
 * 混合策略：前几个皇后用拉斯维加斯随机放置，
 * 剩余用回溯法完成（提高成功率）
 */
bool BacktrackLimited(vector<int>& cols, int row) {
    if (row == n) return true;

    // 对候选列随机排序
    vector<int> candidates;
    for (int col = 0; col < n; col++) {
        bool safe = true;
        for (int i = 0; i < row; i++) {
            if (cols[i] == col || abs(row - i) == abs(col - cols[i])) {
                safe = false;
                break;
            }
        }
        if (safe) candidates.push_back(col);
    }

    shuffle(candidates.begin(), candidates.end(), rng);

    for (int col : candidates) {
        cols[row] = col;
        if (BacktrackLimited(cols, row + 1)) return true;
    }
    return false;
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    n = 8;
    cout << "=== 拉斯维加斯算法 ===" << endl;
    cout << "棋盘大小 n = " << n << endl;

    vector<int> cols(n);

    // 纯拉斯维加斯（小 n 有效）
    cout << "\n--- 纯拉斯维加斯 ---" << endl;
    if (n <= 12) {
        int attempts = LasVegasNQueens(cols);
        cout << "找到解，尝试了 " << attempts << " 次" << endl;
        cout << "皇后位置 (行,列): ";
        for (int i = 0; i < n; i++)
            cout << "(" << i+1 << "," << cols[i]+1 << ") ";
        cout << endl;
    } else {
        cout << "n=" << n << " 纯随机策略成功率过低，使用混合策略" << endl;
    }

    // 混合策略（任意 n 都有效）
    cout << "\n--- 混合策略（拉斯维加斯 + 回溯）---" << endl;
    cols.assign(n, -1);
    bool ok = BacktrackLimited(cols, 0);
    if (ok) {
        cout << "找到解！" << endl;
        cout << "皇后位置 (行,列): ";
        for (int i = 0; i < n; i++)
            cout << "(" << i+1 << "," << cols[i]+1 << ") ";
        cout << endl;
    } else {
        cout << "未找到解" << endl;
    }

    return 0;
}
