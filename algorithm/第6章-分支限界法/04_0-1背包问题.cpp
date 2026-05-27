/*
 * 0-1 背包问题 - 分支限界法
 * -----------------------------
 * 问题描述：同第3.10节和第5.6节。
 *
 * 算法思想（分支限界法 — 最大优先队列式）：
 *   每个节点代表部分决策（前几个物品装/不装）。
 *   用优先队列（最大堆）维护活节点，按价值上界排序。
 *   上界计算：同回溯法，用贪心（可分割背包）计算。
 *   每次取上界最大的节点扩展，产生两个儿子（装/不装）。
 *
 * 与回溯法对比：回溯法深搜+剪枝；分支限界法广搜/优搜+限界。
 * 分支限界常用于求最优解，而回溯可用于求所有解。
 *
 * 时间复杂度：最坏 O(2ⁿ)，通过限界剪枝
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.5节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

struct Item {
    int weight, value;
    double ratio;
    int origIdx;
};

int n, C;
vector<Item> items;
int bestValue = 0;
vector<bool> bestX;

struct BBNode {
    int level;          // 当前层级
    int curW;           // 当前重量
    int curV;           // 当前价值
    double bound;       // 价值上界
    vector<bool> x;     // 当前部分解

    BBNode(int l, int w, int v, double b, const vector<bool>& sol)
        : level(l), curW(w), curV(v), bound(b), x(sol) {}

    // 最大优先队列【上界大者优先】
    bool operator<(const BBNode& other) const {
        return bound < other.bound;
    }
};

/**
 * 计算当前状态的价值上界（贪心 + 部分物品）
 */
double CalcBound(int level, int curW, int curV) {
    if (curW >= C) return 0;

    double bound = curV;
    int remaining = C - curW;

    for (int i = level; i < n; i++) {
        if (items[i].weight <= remaining) {
            bound += items[i].value;
            remaining -= items[i].weight;
        } else {
            bound += items[i].ratio * remaining;
            break;
        }
    }

    return bound;
}

/**
 * 0-1 背包 - 分支限界法
 */
void Knapsack_BB() {
    priority_queue<BBNode> pq;

    vector<bool> initX(n, false);
    double initBound = CalcBound(0, 0, 0);
    pq.push(BBNode(0, 0, 0, initBound, initX));

    while (!pq.empty()) {
        BBNode cur = pq.top(); pq.pop();

        int i = cur.level;

        // 叶节点
        if (i == n) {
            if (cur.curV > bestValue) {
                bestValue = cur.curV;
                bestX = cur.x;
            }
            continue;
        }

        // 限界剪枝
        if (cur.bound <= bestValue) continue;

        // 扩展左儿子：装入物品 i
        if (cur.curW + items[i].weight <= C) {
            vector<bool> leftX = cur.x;
            leftX[i] = true;
            double leftBound = CalcBound(i + 1, cur.curW + items[i].weight,
                                         cur.curV + items[i].value);
            if (leftBound > bestValue) {
                pq.push(BBNode(i + 1, cur.curW + items[i].weight,
                               cur.curV + items[i].value, leftBound, leftX));
            }
        }

        // 扩展右儿子：不装物品 i
        {
            vector<bool> rightX = cur.x;
            rightX[i] = false;
            double rightBound = CalcBound(i + 1, cur.curW, cur.curV);
            if (rightBound > bestValue) {
                pq.push(BBNode(i + 1, cur.curW, cur.curV, rightBound, rightX));
            }
        }
    }
}

int main() {
    cout << "=== 0-1 背包问题 (分支限界法) ===" << endl;

    n = 4, C = 7;
    vector<int> w_orig = {3, 5, 2, 1};
    vector<int> v_orig = {9, 10, 7, 4};

    for (int i = 0; i < n; i++)
        items.push_back({w_orig[i], v_orig[i], (double)v_orig[i]/w_orig[i], i});
    sort(items.begin(), items.end(),
         [](const Item& a, const Item& b) { return a.ratio > b.ratio; });

    cout << "物品 (重量, 价值, 单位价值):" << endl;
    for (auto& it : items)
        cout << "  物品" << (it.origIdx+1) << ": (" << it.weight << ", "
             << it.value << ", " << it.ratio << ")" << endl;
    cout << "背包容量: " << C << endl;

    bestX.assign(n, false);
    Knapsack_BB();

    cout << "\n最优价值: " << bestValue << endl;
    cout << "选中的物品: ";
    for (int i = 0; i < n; i++)
        if (bestX[i]) cout << "物品" << (items[i].origIdx+1) << " ";
    cout << endl;

    return 0;
}
