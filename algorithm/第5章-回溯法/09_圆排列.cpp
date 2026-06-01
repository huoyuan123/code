/*
 * 圆排列问题 (Circle Permutation) - 回溯法
 * --------------------------------------------
 * 问题描述：给定 n 个大小不等的圆（半径 r₁, r₂, ..., rₙ），
 * 将它们在一条直线上一字排列且彼此相切。
 * 求最短的排列长度（即第一个圆到最后一个圆的水平距离最短）。
 *
 * 算法思想（回溯法）：
 *   解空间：排列树
 *   按顺序放圆。当放置第 k 个圆时，计算它与前 k-1 个圆相切时的中心位置：
 *     center[k] = max(center[i] + 2*sqrt(r[i]*r[k]))，对所有 i < k
 *   排列长度 = max(center[i] + r[i]) + r[k]
 *   剪枝：当前部分排列的 ≥ 已知最优值时剪枝
 *
 * 时间复杂度：最坏 O(n!)
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：排列树（圆的放置顺序）；每一步把一个圆放到当前序列末尾并计算新的边界/长度。
 *   - 剪枝：若当前部分排列得到的下界已经不可能优于 best，则停止向下搜索。
 *   - 对称性：圆排列常存在旋转/镜像等对称冗余；固定第一个圆或限定顺序可减少重复搜索。
 *
 * 【实现提示】
 *   - 回溯核心：放第 k 个圆时，计算与已放圆相切的最大中心位置。
 *   - 剪枝核心：若当前最右边界已 ≥ bestLen，则不再深入。
 *   - 状态撤销：回溯时仅需恢复 used/curOrder（center 可直接覆盖）。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.10节
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n;                        // 圆的数量
vector<double> r;             // 圆半径
vector<double> center;        // 圆心位置
vector<double> bestCenter;    // 最优排列的圆心位置
vector<int> bestOrder;        // 最优排列顺序
vector<int> curOrder;         // 当前排列顺序
vector<bool> used;            // 圆是否已排列
double bestLen = DBL_MAX;     // 最优排列长度

/**
 * 计算在当前位置放入第 k 个圆后的排列长度
 * @param k 当前放入的圆的索引（在 curOrder 中的位置）
 */
double ComputeLength(int k) {
    double maxRight = 0;
    for (int i = 0; i <= k; i++) {
        int idx = curOrder[i];
        maxRight = max(maxRight, center[idx] + r[idx]);
    }
    double minLeft = DBL_MAX;
    for (int i = 0; i <= k; i++) {
        int idx = curOrder[i];
        minLeft = min(minLeft, center[idx] - r[idx]);
    }
    return maxRight - minLeft;
}

/**
 * 圆排列 - 回溯法
 * @param k 当前放入第几个圆 (0-indexed)
 */
void Backtrack(int k) {
    if (k == n) {
        double len = ComputeLength(n - 1);
        if (len < bestLen) {
            bestLen = len;
            bestCenter = center;
            bestOrder = curOrder;
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (used[i]) continue;

        // 计算圆 i 在当前排列中的最佳圆心位置
        double cx = r[i];  // 至少紧贴 y 轴
        for (int j = 0; j < k; j++) {
            int prevIdx = curOrder[j];
            cx = max(cx, center[prevIdx] + 2 * sqrt(r[prevIdx] * r[i]));
        }

        used[i] = true;
        curOrder[k] = i;
        center[i] = cx;

        // 剪枝检查：当前部分排列是否可能比最优解更优
        if (k < n - 1 && cx + r[i] < bestLen) {
            Backtrack(k + 1);
        } else if (k == n - 1) {
            Backtrack(k + 1);
        }

        used[i] = false;
    }
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    n = 3;
    cout << "=== 圆排列问题 (回溯法) ===" << endl;
    cout << "圆的数量=" << n << endl;

    r = {1.0, 2.0, 3.0};
    cout << "半径: ";
    for (double v : r) cout << v << " ";
    cout << endl;

    center.resize(n);
    curOrder.resize(n);
    bestCenter.resize(n);
    bestOrder.resize(n);
    used.assign(n, false);

    Backtrack(0);

    cout << "\n圆半径: ";
    for (double rad : r) cout << rad << " ";
    cout << endl;
    cout << "最优排列顺序: ";
    for (int idx : bestOrder) cout << idx << "(r=" << r[idx] << ") ";
    cout << endl;
    cout << "最短排列长度: " << bestLen << endl;

    return 0;
}
