/*
 * 装载问题 (Loading Problem) - 回溯法
 * ---------------------------------------
 * 问题描述：n 个集装箱，重量分别为 wᵢ。有两艘船，载重分别为 c₁ 和 c₂。
 * 问能否将所有集装箱装上两艘船，并给出装载方案。
 *
 * 算法思想（回溯法）：
 *   等价于：求子集使得其重量和在不超过 c₁ 的前提下最大化，且剩余总重 ≤ c₂。
 *   转化为 0-1 背包的变体：使第一艘船的装载量最大。
 *   解空间：子集树（每个集装箱装或不装到第一艘船）
 *   剪枝策略：
 *     约束函数：当前装载量 + wᵢ ≤ c₁
 *     限界函数：当前装载量 + 剩余总重 ≤ best（已经不优于当前最优则剪枝）
 *
 * 时间复杂度：最坏 O(2ⁿ)，实际通过剪枝大幅减少
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.2节
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n;                    // 集装箱数
vector<int> w;            // 重量
int c1, c2;               // 两艘船的载重
int bestW = 0;            // 第一艘船的最优装载量
vector<bool> bestX;       // 最优解（第一艘船装哪些）
vector<bool> curX;        // 当前解
int totalW = 0;           // 所有集装箱总重
vector<int> suffixSum;    // 后缀和：从 i 到 n-1 的重量和

/**
 * 装载问题 - 回溯法（子集树）
 * @param i    当前考察的集装箱编号
 * @param curW 当前第一艘船的装载重量
 */
void Backtrack(int i, int curW) {
    if (i >= n) {
        // 到达叶节点，更新最优解
        if (curW > bestW) {
            bestW = curW;
            bestX = curX;
        }
        return;
    }

    // 剩余全部装上也不如最优，剪枝
    if (curW + suffixSum[i] <= bestW) return;

    int remainW = totalW - (curW + suffixSum[i]);  // 不装 i 的话第一艘船的剩余可能

    // 搜索左子树：装入集装箱 i
    if (curW + w[i] <= c1) {
        curX[i] = true;
        Backtrack(i + 1, curW + w[i]);
    }

    // 搜索右子树：不装集装箱 i
    curX[i] = false;
    Backtrack(i + 1, curW);
}

/**
 * 装载问题求解
 * @return 是否可行（两艘船能否装下所有集装箱）
 */
bool Loading() {
    totalW = accumulate(w.begin(), w.end(), 0);

    // 计算后缀和（按重量降序排序可以加速剪枝）
    suffixSum.resize(n + 1);
    suffixSum[n] = 0;
    for (int i = n - 1; i >= 0; i--) {
        suffixSum[i] = suffixSum[i + 1] + w[i];
    }

    bestW = 0;
    bestX.assign(n, false);
    curX.assign(n, false);

    Backtrack(0, 0);

    // 剩余重量是否能被第二艘船装下
    return (totalW - bestW) <= c2;
}

int main() {
    cout << "=== 装载问题 (回溯法) ===" << endl;

    n = 5;
    w = {30, 40, 50, 20, 10};
    c1 = 80;
    c2 = 90;

    cout << "集装箱重量: ";
    for (int weight : w) cout << weight << " ";
    cout << endl;
    cout << "第一艘船载重 c1=" << c1 << ", 第二艘船载重 c2=" << c2 << endl;

    // 按重量降序排列（提高剪枝效率）
    vector<int> origIdx(n);
    for (int i = 0; i < n; i++) origIdx[i] = i;
    sort(origIdx.begin(), origIdx.end(), [](int a, int b) { return w[a] > w[b]; });
    vector<int> sortedW(n);
    for (int i = 0; i < n; i++) sortedW[i] = w[origIdx[i]];
    w = sortedW;

    if (Loading()) {
        cout << "可行！第一艘船最优装载=" << bestW << ", 剩余=" << (totalW - bestW) << "≤c2=" << c2 << endl;
        cout << "第一艘船装的集装箱: ";
        for (int i = 0; i < n; i++)
            if (bestX[i]) cout << origIdx[i] + 1 << "(重量" << w[i] << ") ";
        cout << endl;
    } else {
        cout << "不可行，两艘船无法装下所有集装箱" << endl;
    }

    return 0;
}
