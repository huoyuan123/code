/*
 * 装载问题 (Loading Problem) - 分支限界法
 * -------------------------------------------
 * 问题描述：同第5.2节。n 个集装箱 + 两艘船。
 * 求将尽可能多的重量装入第一艘船（≤ c₁ 前提下）。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   每个节点代表部分装载方案（部分物品已决定装/不装）。
 *   用优先队列维护活节点，按限界值（上界）降序排列。
 *   限界函数（上界）：当前重量 + 剩余可装重量 = curW + sumRemaining
 *   每次取上界最大的节点扩展（最大优先队列）。
 *
 * 时间复杂度：最坏 O(2ⁿ)，通过限界剪枝减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 与回溯法区别：回溯通常深搜逐个生成儿子；分支限界往往用队列/优先队列，一次性生成儿子并按界排序扩展。
 *   - 活节点：节点里一般要保存当前层 i、当前装载量/剩余容量，以及用于估计的上界（bound）。
 *   - 易错点：界函数要和目标一致（最大化装载量或最小化剩余）；界过松会导致搜索爆炸。
 *
 * 【实现提示】
 *   - 活节点表：用优先队列按 bound 从大到小扩展，优先探索更有希望的节点。
 *   - 界函数：必须是“上界”（乐观估计），否则可能错剪最优解。
 *   - 状态拷贝：节点中 x/curW/level 要独立保存，避免相互影响。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.3节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

// 变量说明：n = 集装箱数量；c1 = 第一艘船载重；w = 集装箱重量列表；bestW = 当前最优装载重量；bestX = 最优方案（哪些集装箱装入）
int n, c1;
vector<int> w;
int bestW = 0;
vector<bool> bestX;
// 节点结构：level = 当前层级（已考虑的集装箱数）；curW = 当前装载重量；bound = 上界；x = 当前部分解（哪些集装箱装入）
struct BBNode {
    int level;          // 当前层级（已考虑的集装箱数）
    int curW;           // 当前装载重量
    double bound;       // 上界（优先队列排序依据）,上界是当前重量 + 剩余可装重量
    vector<bool> x;     // 当前部分解，x[i] = true 表示集装箱 i 已装入

    BBNode(int l, int cw, double b, const vector<bool>& sol)
        : level(l), curW(cw), bound(b), x(sol) {}

    // 最大优先队列（上界大者优先）
    bool operator<(const BBNode& other) const {
        return bound < other.bound;
    }
};

/**
 * 计算节点的上界
 * 上界 = 当前重量 + 剩余所有集装箱重量
 */
double CalcBound(int curW, int level, int totalW) {
    return curW + (totalW - curW);  // 简化上界：当前重 + 所有剩余
}

/**
 * 装载问题 - 分支限界法（最大优先队列）
 */
void MaxLoading() {
    int totalW = 0;
    for (int weight : w) totalW += weight;

    priority_queue<BBNode> pq;

    vector<bool> initX(n, false);
    double bound = CalcBound(0, 0, totalW);
    pq.push(BBNode(0, 0, bound, initX));

    while (!pq.empty()) {
        BBNode cur = pq.top(); pq.pop();

        int i = cur.level;
        if (i == n) {
            // 叶节点——完整解
            if (cur.curW > bestW) {
                bestW = cur.curW;
                bestX = cur.x;
            }
            continue;
        }

        // 限界剪枝：上界 ≤ 当前最优则跳过
        if (cur.bound <= bestW) continue;

        // 扩展左儿子：装入集装箱 i
        if (cur.curW + w[i] <= c1) {
            vector<bool> leftX = cur.x;
            leftX[i] = true;
            double leftBound = cur.curW + w[i];
            // 计算更精确的上界
            int remaining = 0;
            for (int j = i + 1; j < n; j++) remaining += w[j];
            leftBound = cur.curW + w[i] + remaining;

            pq.push(BBNode(i + 1, cur.curW + w[i], leftBound, leftX));
        }

        // 扩展右儿子：不装集装箱 i
        {
            vector<bool> rightX = cur.x;
            rightX[i] = false;
            int remaining = 0;
            for (int j = i + 1; j < n; j++) remaining += w[j];
            double rightBound = cur.curW + remaining;

            if (rightBound > bestW) {
                pq.push(BBNode(i + 1, cur.curW, rightBound, rightX));
            }
        }
    }
}

int main() {
    cout << "=== 装载问题 (分支限界法) ===" << endl;

    n = 5;
    w = {30, 40, 50, 20, 10};
    c1 = 80;

    cout << "集装箱重量: ";
    for (int weight : w) cout << weight << " ";
    cout << endl;
    cout << "第一艘船载重 c1=" << c1 << endl;

    bestX.assign(n, false);
    MaxLoading();

    cout << "第一艘船最优装载重量: " << bestW << endl;
    cout << "装入的集装箱: ";
    int sum = 0;
    for (int i = 0; i < n; i++)
        if (bestX[i]) {
            cout << "集装箱" << (i+1) << "(重" << w[i] << ") ";
            sum += w[i];
        }
    cout << endl;
    cout << "验证总重: " << sum << endl;

    return 0;
}
