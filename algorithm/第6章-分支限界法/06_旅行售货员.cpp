/*
 * 旅行售货员问题 (TSP) - 分支限界法
 * --------------------------------------
 * 问题描述：同第5.9节。求最短 Hamilton 回路。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   每个节点表示一条部分路径（已访问部分城市）。
 *   优先队列按下界（当前路径长度 + 估计剩余路径长度）升序排列。
 *   下界计算方法：
 *     下界 = 当前路径长度 + 每个未访问城市的最小出边 + 回到起点的最小边
 *   或更简单的下界：当前路径长度 × 剩余城市比例
 *   每次取下界最小的节点扩展。
 *
 * 时间复杂度：最坏 O(n!)
 *
 *
 * 【阅读指引 / 易错点】
 *   - 节点表示：通常保存当前路径、已访问集合、当前花费 cost，以及下界 bound（例如 1-tree / 最小出入边估计）。
 *   - 活节点表：用优先队列按 bound 最小优先扩展（最小化问题），尽快逼近最优回路。
 *   - 易错点：叶子节点要补上“回到起点”的边；同时下界必须小于等于真实最优（否则会错剪）。
 *
 * 【实现提示】
 *   - 活节点表：按 lowerBound 最小优先扩展，尽早拿到较好 bestDist。
 *   - 界函数：CalcLowerBound 必须是下界（乐观估计），不能高估。
 *   - 完整性：到叶子节点时务必补上回到起点的边。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.7节

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
#include <queue>
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

const int INF = INT_MAX;

int n;
vector<vector<int>> dist;
int bestDist = INF;
vector<int> bestPath;

struct BBNode {
    vector<int> path;    // 已访问的城市顺序
    vector<bool> visited; // 各城市是否已访问
    int curDist;         // 当前路径长度
    double lowerBound;   // 下界估计

    BBNode(const vector<int>& p, const vector<bool>& v, int cd, double lb)
        : path(p), visited(v), curDist(cd), lowerBound(lb) {}

    // 最小优先队列（下界小者优先）
    bool operator<(const BBNode& other) const {
        return lowerBound > other.lowerBound;
        //other 是 operator< 的 右操作数，也就是 被比较的另一个 BBNode 对象。
    }
    /*通过故意反转 operator< 的比较逻辑，把 priority_queue 默认的大顶堆变成小顶堆，
    使得每次出队的都是 lowerBound 最小的节点，保证分支限界法优先探索最有希望的路径。*/
};

/**
 * 计算下界
 * 策略：已走距离 + 每个未访问城市的最小出边之和
 */
double CalcLowerBound(const vector<int>& path, const vector<bool>& visited,
                      int curDist) {
    double bound = curDist;
    //bound 是当前路径长度，后续要加上未访问城市的最小出边之和
    int lastCity = path.back();
    //path 最后一个元素，就是最后一个已访问城市	当前路径的终点，后续扩展要从这个城市出发


    // 对于每个未访问城市，加上其最小出边（不包括回到起点的边）
    int minRemaining = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i])  // ← 只遍历"未访问"城市
        {
            int minEdge = INF;
            for (int j = 0; j < n; j++) {
                if (i != j) minEdge = min(minEdge, dist[i][j]);
            }
            if (minEdge != INF) minRemaining += minEdge;
            // 从城市i"出去"的最少花费
        }
    }

    // 如果所有城市都已访问，下界 = 已走距离 + 回到起点的距离（完整回路）
    if (minRemaining == 0) {
        return bound + dist[lastCity][0];
    }

    // 加上从最后一个已访问城市到未访问城市的最小边
    int minFromLast = INF;
    for (int j = 0; j < n; j++) {
        if (!visited[j]) {
            minFromLast = min(minFromLast, dist[lastCity][j]);
        }
    }
    // 下界 = 已走距离 + 最后城市到下一站的最短边 + 每个未访问城市的最小出边之和
    bound += minFromLast + minRemaining;
    return bound;
}

/**
 * TSP - 分支限界法
 */
void TSP_BB() {
    // 最小优先队列：下界(lowerBound)越小的节点越优先出队
    priority_queue<BBNode> pq;

    // 起点固定为城市0
    vector<int> startPath = {0};
    // 初始访问标记：全false，只有城市0标记为true
    vector<bool> startVisited(n, false);
    startVisited[0] = true;             // 起点已访问

    // 计算根节点的下界，curDist=0因为还没开始走
    double startLB = CalcLowerBound(startPath, startVisited, 0);
    // 根节点入队：路径={0}, 已走距离=0, 下界=startLB
    pq.push(BBNode(startPath, startVisited, 0, startLB));

    while (!pq.empty()) {
        BBNode cur = pq.top(); pq.pop();
        /*
        priority_queue 的设计不允许直接取走元素，必须分两步：
        BBNode cur = pq.top();   // 第1步：拷贝堆顶元素到 cur
        pq.pop();       // 第2步：从队列中移除堆顶
        */

        // 限界剪枝
        if (cur.lowerBound >= bestDist) continue;  // [限界函数] 下界不优于最优，剪枝
        //continue 不执行的才是"通过"（活下来了），执行了才是"被剪"。
        //因为continue 是跳过当前循环剩余部分，直接进入下一轮循环，所以当 cur.lowerBound >= bestDist 时，当前节点被剪掉，不会执行后续扩展逻辑。

        // 已访问所有城市
        if (cur.path.size() == n) {
            int lastCity = cur.path.back();
            int totalDist = cur.curDist + dist[lastCity][0];  // 回到起点
            if (totalDist < bestDist) {
                bestDist = totalDist;
                bestPath = cur.path;
            }
            continue;
        }

        int lastCity = cur.path.back();

        // 扩展下一个城市
        for (int v = 0; v < n; v++) {
            if (!cur.visited[v] && dist[lastCity][v] != INF) {
                vector<int> newPath = cur.path;
                newPath.push_back(v);
                vector<bool> newVisited = cur.visited;
                //拷贝当前路径，把城市 v 追加到末尾。
                newVisited[v] = true;
                int newDist = cur.curDist + dist[lastCity][v];

                double newLB = CalcLowerBound(newPath, newVisited, newDist);
                if (newLB < bestDist) {
                    pq.push(BBNode(newPath, newVisited, newDist, newLB));
                }
            }
        }
        /*找出最优路径为25后pq优先队列里的结点为：
        ┌─────────────────────────────────┐
        │ {0,2,1,3}  lb=25                │
        │ {0,3,2}    lb=34                │
        │ {0,2,3}    lb=41                │
        │ {0,1}      lb=44                │
        └─────────────────────────────────┘
        队列不空，while 继续转！在后续循环中清空pq
        但是由于bestdist和bestpath已经保存所以即使被剪掉而不会影响*/
    }
}

int main() {
    cout << "=== 旅行售货员问题 (分支限界法) ===" << endl;

    n = 4;
    dist = {
        {  0, 30,  6,  4},
        { 30,  0,  5, 10},
        {  6,  5,  0, 20},
        {  4, 10, 20,  0}
    };

    cout << "城市距离矩阵:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << dist[i][j] << "\t";
        cout << endl;
    }

    TSP_BB();

    cout << "\n最短回路长度: " << bestDist << endl;
    cout << "最短路径: ";
    for (int v : bestPath) cout << v << " -> ";
    cout << bestPath[0] << endl;

    return 0;
}
