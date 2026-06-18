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
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：子集树（选/不选第 i 件物品）；cur 记录当前重量/价值，best 记录目前最优。
 *   - 常见限界函数：用“分数背包（可拆分）”的贪心上界估计当前节点能达到的最大价值，从而剪枝。
 *   - 物品排序：若使用分数上界，通常按价值密度排序能让上界更紧、剪枝更有效。
 *
 * 【实现提示】
 *   - 回溯核心：先尝试“选第 i 件”，再尝试“不选第 i 件”。
 *   - 剪枝核心：不选分支需用 UpperBound 判断是否仍有超过 bestValue 的可能。
 *   - 排序依赖：上界计算依赖 ratio 排序，修改排序时需同步调整映射/输出。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.6节

 *
 * 解空间树：子集树（Subset Tree）
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
    if (curW + items[i].weight <= C) {  // [约束函数] 检查装入后是否超重
        curX[i] = true;
        Backtrack(i + 1, curW + items[i].weight, curV + items[i].value);
    }

    // 搜索右子树：不装物品 i（需要限界剪枝）
    if (UpperBound(i + 1, curW, curV) > bestValue) {  // [限界函数] 贪心上界：不装是否可能更优
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
