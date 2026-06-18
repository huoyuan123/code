/*
 * 旅行售货员问题 (TSP) - 回溯法
 * -----------------------------------
 * 问题描述：给定 n 个城市及两两之间的距离（完全图）。
 * 求从城市 1 出发，经过每个城市恰好一次，
 * 最后回到城市 1 的最短回路（Hamilton 回路）。
 *
 * 算法思想（回溯法）：
 *   解空间：排列树
 *   用 x[1..n] 表示路径：x[i] 为第 i 步访问的城市。
 *   按层递归，记录当前路径长度。
 *   剪枝：当前部分路径长度 ≥ 当前最优解时剪枝。
 *   边界：到达叶节点时还需加上回到起点的距离。
 *
 * 时间复杂度：最坏 O(n!)，通过剪枝减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：排列树（决定访问城市顺序）；当前路径 cost 累加，bestCost 记录最优回路。
 *   - 常见限界：用“当前已走成本 + 每个未访问点的最小出边/入边估计”形成下界，若下界 ≥ bestCost 则剪枝。
 *   - 闭环处理：到达叶子后还要加上“回到起点”的边；很多实现漏掉这一步导致答案偏小。
 *
 * 【实现提示】
 *   - 回溯核心：每层选择下一个城市，累计 curDist。
 *   - 剪枝核心：curDist 已 ≥ bestDist 时停止扩展。
 *   - 终止处理：叶节点必须加上“回到起点”的距离。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.9节

 *
 * 解空间树：排列树（Permutation Tree）
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
#include <climits>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

const int INF = INT_MAX;

int n;                          // 城市数
vector<vector<int>> dist;       // 距离矩阵，dist[i][j] = 从城市 i 到城市 j 的距离
vector<int> bestPath;           // 最优路径
vector<int> curPath;            // 当前路径
vector<bool> visited;           // 城市是否已访问
int bestDist = INF;             // 最优路径长度

/**
 * TSP - 回溯法（排列树）
 * @param step    当前在第几步 (0-indexed，已访问 step 个城市)
 * @param curDist 当前路径长度
 */
void Backtrack(int step, int curDist) {
    if (step == n) {
        // 所有城市访问完毕，加上回到起点的距离
        int totalDist = curDist + dist[curPath[n-1]][curPath[0]];
        //已走路程    从最后一个城市 回到 起点城市 的距离
        if (totalDist < bestDist) {
            bestDist = totalDist;
            bestPath = curPath;
        }
        return;
    }

    // 剪枝：当前路径已不优于最优解
    if (curDist >= bestDist) return;  // [限界函数] 当前路径已不优于最优，剪枝

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            int prev = curPath[step - 1];
            if (dist[prev][i] == INF) continue;  // 无边  // [约束函数] 检查两城市间是否有边

            visited[i] = true;
            curPath[step] = i;  // 记录：第 step 步走到了城市 i

            Backtrack(step + 1, curDist + dist[prev][i]);

            visited[i] = false;

            //回溯，这里 visited[i] = false 不是"标记去过"，而是撤销标记——在尝试完城市 i 之后，把它恢复为"未访问"状态。
        }
    }
}

int main() {
    cout << "=== 旅行售货员问题 (回溯法) ===" << endl;

    // 4 个城市的完全图
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

    curPath.assign(n, 0);
    visited.assign(n, false);

    // 从城市 0 出发
    visited[0] = true;
    curPath[0] = 0;

    Backtrack(1, 0);

    cout << "\n最短回路长度: " << bestDist << endl;
    cout << "最短路径: ";
    for (int v : bestPath) cout << v << " -> ";
    cout << bestPath[0] << endl;

    return 0;
}
/*Backtrack(step=1, curDist=0)           ← 从城市0出发，准备选第1站
│
├─ i=0: visited[0]=true → 跳过
│
├─ i=1: !visited[1] ✓
│   visited[1] = true               visited = [T, T, F, F]
│   curPath[1] = 1                  curPath = [0, 1, ?, ?]
│   │
│   │  Backtrack(step=2, curDist=30)  ← 0→1 路程=30
│   │  │
│   │  ├─ i=0: visited → 跳过
│   │  ├─ i=1: visited → 跳过
│   │  ├─ i=2: !visited[2] ✓
│   │  │   visited[2] = true        visited = [T, T, T, F]
│   │  │   curPath[2] = 2           curPath = [0, 1, 2, ?]
│   │  │   │
│   │  │   │  Backtrack(step=3, curDist=35)  ← 0→1→2 路程=30+5=35
│   │  │   │  │
│   │  │   │  └─ i=3: !visited[3] ✓
│   │  │   │      visited[3] = true  visited = [T, T, T, T]
│   │  │   │      curPath[3] = 3    curPath = [0, 1, 2, 3]
│   │  │   │      │
│   │  │   │      │  Backtrack(step=4, curDist=55)  ← 0→1→2→3 路程=35+20=55
│   │  │   │      │  step==n → 计算回路: 55 + dist[3][0] = 55+4 = 59
│   │  │   │      │  bestDist = 59
│   │  │   │      │
│   │  │   │      visited[3] = false ← 撤销！visited = [T, T, T, F]
│   │  │   │      return
│   │  │   │
│   │  │   visited[2] = false ← 撤销！visited = [T, T, F, F]
│   │  │
│   │  └─ i=3: !visited[3] ✓
│   │      visited[3] = true        visited = [T, T, F, T]
│   │      curPath[2] = 3           curPath = [0, 1, 3, ?]
│   │      │
│   │      │  Backtrack(step=3, curDist=40)  ← 0→1→3 路程=30+10=40
│   │      │  │
│   │      │  └─ i=2: !visited[2] ✓   ← 关键！city2没被标记，可以走
│   │      │      ...
│   │      │
│   │      visited[3] = false ← 撤销！
│   │
│   visited[1] = false ← 撤销！visited = [T, F, F, F]
│
├─ i=2: ... 现在 visited[1] 是 false，可以自由选择城市1 ...
│
└─ i=3: ... 同样不会受之前探索的影响 ...
关键点：visited[3] = false 的意义
看上面 i=1→i=2→i=3 那条路走完后：


不走 visited[3]=false:
  接下来试 i=1→i=3→? 时，visited = [T, T, T, T]
  没有城市可选了，路径 0→1→3→? 被错误跳过！

走了 visited[3]=false:
  接下来试 i=1→i=3→? 时，visited = [T, T, F, F]
  还能选城市2，正确生成 0→1→3→2→0 ✓*/