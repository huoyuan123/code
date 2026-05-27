/*
 * 0-1 背包问题 (回溯法)
 * ------------------------
 * 问题描述：同第3.10节。n 个物品，重量 wᵢ，价值 vᵢ，背包容量 C。
 * 求总价值最大的装入方案。
 *
 * 算法思想（回溯法）：
 *   解空间：子集树（每件物品选/不选）
 *   约束函数：当前重量 + wᵢ ≤ C
 *   限界函数：用贪心（可分割的背包）求当前状态的上界
 *     将剩余物品按单位价值降序，依次装入直到重量达到限制。
 *     若上界 ≤ 当前最优价值，剪枝。
 *
 * 对比：动态规划 O(nC)、回溯法最坏 O(2ⁿ)、分支限界法第6章
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.6节
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

struct Item {
    int id;
    int weight;
    int value;
    double ratio;  // 单位重量价值
};

int n, C;
vector<Item> items;       // 物品（按单位价值降序排列）
int bestValue = 0;        // 最优价值
vector<bool> bestX, curX; // 最优解 / 当前解

/**
 * 计算当前状态的上界（限界函数）
 * 贪心策略：尽可能装单位价值高的物品（可分割假设）
 * @param i   当前考察的物品索引
 * @param curW 当前重量
 * @param curV 当前价值
 * @return    上界值
 */
double UpperBound(int i, int curW, int curV) {
    double bound = curV;
    int remaining = C - curW;

    // 从第 i 个物品开始贪心装入
    while (i < n && items[i].weight <= remaining) {
        bound += items[i].value;
        remaining -= items[i].weight;
        i++;
    }

    // 最后一件物品只装入一部分（上界估计）
    if (i < n) {
        bound += (double)remaining * items[i].ratio;
    }

    return bound;
}

/**
 * 0-1 背包 - 回溯法
 * @param i    当前考察的物品索引
 * @param curW 当前总重量
 * @param curV 当前总价值
 */
void Backtrack(int i, int curW, int curV) {
    if (i >= n) {
        // 到达叶节点
        if (curV > bestValue) {
            bestValue = curV;
            bestX = curX;
        }
        return;
    }

    // 搜索左子树：装入物品 i
    if (curW + items[i].weight <= C) {
        curX[i] = true;
        Backtrack(i + 1, curW + items[i].weight, curV + items[i].value);
    }

    // 搜索右子树：不装物品 i（需要限界剪枝）
    if (UpperBound(i + 1, curW, curV) > bestValue) {
        curX[i] = false;
        Backtrack(i + 1, curW, curV);
    }
}

int main() {
    cout << "=== 0-1 背包问题 (回溯法) ===" << endl;

    n = 4, C = 7;
    vector<int> w = {3, 5, 2, 1};
    vector<int> v = {9, 10, 7, 4};

    // 构建物品列表
    for (int i = 0; i < n; i++) {
        items.push_back({i, w[i], v[i], (double)v[i] / w[i]});
    }

    // 按单位重量价值降序排列（提高限界精度）
    sort(items.begin(), items.end(),
         [](const Item& a, const Item& b) { return a.ratio > b.ratio; });

    cout << "物品 (重量, 价值, 单位价值):" << endl;
    for (auto& it : items)
        cout << "  物品" << (it.id+1) << ": (" << it.weight << ", "
             << it.value << ", " << it.ratio << ")" << endl;
    cout << "背包容量: " << C << endl;

    curX.assign(n, false);
    bestX.assign(n, false);

    Backtrack(0, 0, 0);

    cout << "\n最优价值: " << bestValue << endl;
    cout << "选中的物品: ";
    for (int i = 0; i < n; i++)
        if (bestX[i]) cout << "物品" << (items[i].id+1) << " ";
    cout << endl;

    return 0;
}
