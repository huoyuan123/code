/*
 * 批处理作业调度 (Batch Job Scheduling) - 回溯法
 * -------------------------------------------------
 * 问题描述：n 个作业在 2 台机器上加工，每个作业先 M₁ 后 M₂，时间分别为 aᵢ, bᵢ。
 * 求使完成时间和（sum of completion times）最小的调度顺序。
 *
 * 算法思想（回溯法）：
 *   解空间：排列树（n! 个排列）
 *   设当前调度为 x[1..n]，f1 = 当前作业在 M₁ 上的完成时间，
 *   f2[i] = 第 i 个作业在 M₂ 上的完成时间：
 *     f1 += a[x[i]]
 *     f2[i] = max(f2[i-1], f1) + b[x[i]]
 *   剪枝：当前部分调度的 f2 前缀和 ≥ 已知最优值时剪枝
 *
 * 时间复杂度：最坏 O(n!)（通过剪枝减少）
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：排列树（作业的加工顺序/分配方案逐步确定）；每层通常表示“决定第 i 个位置放哪个作业”。
 *   - 剪枝思路：用当前部分调度的完工时间作为下界，若已不可能优于 best 则回溯。
 *   - 状态撤销：回溯返回上一层时要恢复机器/作业的时间累计状态，否则会把上一分支的影响带入下一分支。
 *
 * 【实现提示】
 *   - 回溯核心：先放置作业、更新 f1/f2/sum，再递归；回溯时恢复 used/curOrder。
 *   - 剪枝核心：sumF2 是下界，一旦 ≥ bestTime 就停止扩展。
 *   - 变量含义：注意 f1 是 M1 完成时间，f2Prev 是 M2 上一个作业完成时间。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.3节
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n;                           // 作业数
vector<int> a, b;                // 作业在 M₁, M₂ 上的加工时间
vector<int> bestOrder;           // 最优排序
vector<int> curOrder;            // 当前排序
vector<bool> used;               // 作业是否已分配
int bestTime = INT_MAX;          // 最优完成时间和

/**
 * 批处理作业调度 - 回溯法（排列树）
 * @param k     当前安排到第几个位置 (0-indexed)
 * @param f1    当前作业在 M₁ 上的完成时间
 * @param f2Prev 上一个作业在 M₂ 上的完成时间
 * @param sumF2 当前已安排的作业在 M₂ 上的完成时间之和
 */
void Backtrack(int k, int f1, int f2Prev, int sumF2) {
    if (k == n) {
        // 完整排列，更新最优解
        if (sumF2 < bestTime) {
            bestTime = sumF2;
            bestOrder = curOrder;
        }
        return;
    }

    // 剪枝：当前部分和已经 ≥ 最优值
    if (sumF2 >= bestTime) return;

    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = true;
            curOrder[k] = i;

            int newF1 = f1 + a[i];                        // M₁ 完成时间
            int newF2 = max(f2Prev, newF1) + b[i];         // M₂ 完成时间
            int newSumF2 = sumF2 + newF2;                  // 新的累计完成时间和

            Backtrack(k + 1, newF1, newF2, newSumF2);

            used[i] = false;
        }
    }
}

int main() {
    cout << "=== 批处理作业调度 (回溯法) ===" << endl;

    n = 3;
    a = {2, 3, 2};  // M₁ 加工时间
    b = {1, 1, 3};  // M₂ 加工时间

    cout << "作业信息 (M₁, M₂):" << endl;
    for (int i = 0; i < n; i++)
        cout << "  J" << (i+1) << ": (" << a[i] << ", " << b[i] << ")" << endl;

    used.assign(n, false);
    curOrder.assign(n, 0);

    Backtrack(0, 0, 0, 0);

    cout << "\n最优调度顺序: ";
    for (int job : bestOrder) cout << "J" << (job + 1) << " ";
    cout << endl;
    cout << "最小完成时间和: " << bestTime << endl;

    return 0;
}
