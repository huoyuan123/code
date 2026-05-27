/*
 * 批处理作业调度 - 分支限界法
 * ---------------------------------
 * 问题描述：同第5.3节。n 个作业在两台机器上加工。
 * 求使完成时间和最小的调度顺序。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   每个节点代表部分调度（前 k 个位置的作业已确定）。
 *   下界计算：已安排作业在 M₂ 上的完成时间和 +
 *   剩余作业在 M₂ 上的估计完成时间和。
 *   优先队列按下界升序（最小优先队列）。
 *
 * 时间复杂度：最坏 O(n!)
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.9节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n;
vector<int> a, b;
int bestTime = INT_MAX;
vector<int> bestOrder;

struct BBNode {
    vector<int> order;      // 当前部分调度
    vector<bool> used;
    int f1;                 // M₁ 完成时间
    int f2Prev;             // 上一个作业在 M₂ 上的完成时间
    int sumF2;              // M₂ 完成时间和
    int level;

    BBNode(const vector<int>& o, const vector<bool>& u,
           int f1v, int f2p, int s, int lv)
        : order(o), used(u), f1(f1v), f2Prev(f2p), sumF2(s), level(lv) {}

    bool operator<(const BBNode& other) const {
        return sumF2 > other.sumF2;  // 最小 sumF2 优先
    }
};

void JobSchedule_BB() {
    priority_queue<BBNode> pq;

    vector<int> initOrder(n, -1);
    vector<bool> initUsed(n, false);
    pq.push(BBNode(initOrder, initUsed, 0, 0, 0, 0));

    while (!pq.empty()) {
        BBNode cur = pq.top(); pq.pop();
        int k = cur.level;

        if (k == n) {
            if (cur.sumF2 < bestTime) {
                bestTime = cur.sumF2;
                bestOrder = cur.order;
            }
            continue;
        }

        if (cur.sumF2 >= bestTime) continue;

        for (int i = 0; i < n; i++) {
            if (cur.used[i]) continue;

            vector<int> newOrder = cur.order;
            newOrder[k] = i;
            vector<bool> newUsed = cur.used;
            newUsed[i] = true;

            int newF1 = cur.f1 + a[i];
            int newF2 = max(cur.f2Prev, newF1) + b[i];
            int newSumF2 = cur.sumF2 + newF2;

            if (newSumF2 < bestTime) {
                pq.push(BBNode(newOrder, newUsed, newF1, newF2, newSumF2, k + 1));
            }
        }
    }
}

int main() {
    cout << "=== 批处理作业调度 (分支限界法) ===" << endl;

    n = 3;
    a = {2, 3, 2};
    b = {1, 1, 3};

    cout << "作业信息 (M₁, M₂):" << endl;
    for (int i = 0; i < n; i++)
        cout << "  J" << (i+1) << ": (" << a[i] << ", " << b[i] << ")" << endl;

    JobSchedule_BB();

    cout << "\n最优调度顺序: ";
    for (int job : bestOrder) cout << "J" << (job + 1) << " ";
    cout << endl;
    cout << "最小完成时间和: " << bestTime << endl;

    return 0;
}
