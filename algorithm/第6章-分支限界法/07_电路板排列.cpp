/*
 * 电路板排列问题 - 分支限界法
 * ---------------------------------
 * 问题描述：同第5.11节。求使最大密度最小的电路板排列。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   每个节点代表部分排列（前 k 块电路板已放置）。
 *   优先队列按当前密度升序（最小优先队列）。
 *   上界/下界：当前密度 + 剩余可能的密度增加量。
 *
 * 时间复杂度：最坏 O(n!)
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.8节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n, m;
vector<vector<bool>> block;
vector<int> total;
int bestDensity = INT_MAX;
vector<int> bestOrder;

struct BBNode {
    vector<int> order;      // 当前排列（前 k 个位置）
    vector<bool> used;      // 电路板是否已用
    vector<int> now;        // 每个连接块已排列数
    int curDensity;         // 当前密度
    int level;              // 当前层级

    BBNode(const vector<int>& o, const vector<bool>& u,
           const vector<int>& nw, int cd, int lv)
        : order(o), used(u), now(nw), curDensity(cd), level(lv) {}

    bool operator<(const BBNode& other) const {
        return curDensity > other.curDensity; // 最小密度优先
    }
};

void CircuitBoard_BB() {
    priority_queue<BBNode> pq;

    vector<int> initOrder(n, -1);
    vector<bool> initUsed(n, false);
    vector<int> initNow(m, 0);
    pq.push(BBNode(initOrder, initUsed, initNow, 0, 0));

    while (!pq.empty()) {
        BBNode cur = pq.top(); pq.pop();
        int k = cur.level;

        if (k == n) {
            if (cur.curDensity < bestDensity) {
                bestDensity = cur.curDensity;
                bestOrder = cur.order;
            }
            continue;
        }

        if (cur.curDensity >= bestDensity) continue;

        for (int i = 0; i < n; i++) {
            if (cur.used[i]) continue;

            vector<int> newOrder = cur.order;
            newOrder[k] = i;
            vector<bool> newUsed = cur.used;
            newUsed[i] = true;
            vector<int> newNow = cur.now;

            int newDens = 0;
            for (int j = 0; j < m; j++) {
                if (block[i][j]) newNow[j]++;
                if (newNow[j] > 0 && newNow[j] < total[j]) newDens++;
            }

            if (newDens < bestDensity) {
                pq.push(BBNode(newOrder, newUsed, newNow, newDens, k + 1));
            }
        }
    }
}

int main() {
    cout << "=== 电路板排列问题 (分支限界法) ===" << endl;

    n = 8; m = 5;
    block.assign(n, vector<bool>(m, false));
    vector<vector<int>> blockDef = {
        {0,0,1,0,0},{0,1,0,0,0},{0,0,0,1,1},{1,0,0,0,0},
        {0,0,0,1,0},{0,1,0,0,0},{0,0,0,0,1},{0,0,1,0,0}
    };
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            block[i][j] = blockDef[i][j];

    total.assign(m, 0);
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            if (block[i][j]) total[j]++;

    CircuitBoard_BB();

    cout << "最小最大密度: " << bestDensity << endl;
    cout << "最优排列: ";
    for (int v : bestOrder) cout << v << " ";
    cout << endl;

    return 0;
}
