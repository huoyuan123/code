/*
 * 布线问题 (Wiring Problem) - 分支限界法
 * ------------------------------------------
 * 问题描述：在 n×m 网格中，求从起点到终点的最短路径，
 * 避开障碍物（封锁的方格）。每次移动可到相邻的上/下/左/右方格。
 *
 * 算法思想（分支限界法 — FIFO 队列式 BFS）：
 *   从起点开始 BFS（本质是 FIFO 分支限界）。
 *   每个位置是活节点，扩展它的四个邻居。
 *   由于 BFS 按层遍历，首次到达终点的路径即最短路径。
 *   用 dist 记录距离，pre 记录前驱方向便于回溯路径。
 *
 * 时间复杂度：O(n×m)
 *
 *
 * 【阅读指引 / 易错点】
 *   - 活节点扩展：一次性产生某个节点的所有可行“下一步连接/放置”，并把它们加入活节点表。
 *   - 界函数：常见是“当前代价 + 未来最小可能代价”的下界；下界已不优于 best 时剪枝。
 *   - 状态表示：要能快速判断冲突（交叉/重叠/越界），状态设计直接决定搜索效率。
 *
 * 【实现提示】
 *   - 活节点表：这里用 FIFO 队列按层扩展，本质是最短路 BFS。
 *   - 界函数：层数（dist）就是当前下界，按层扩展可保证最优。
 *   - 状态记录：dist/prevDir 用于去重与回溯路径，不能漏更新。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.4节

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
#include "../encoding_fix.h"
using namespace std;

// 四个移动方向：上、下、左、右
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};
const char dirChar[4] = {'U', 'D', 'L', 'R'};

struct Position {
    int x, y;
    Position(int r, int c) : x(r), y(c) {}
};

/**
 * 布线问题 - 分支限界法 (BFS)
 * @param grid 网格，0=可通行，1=障碍
 * @param sx, sy 起点坐标
 * @param ex, ey 终点坐标
 * @param path 返回最短路径
 * @return 最短路径长度（不含起点），-1 表示不可达
 */
int Wiring_BFS(const vector<vector<int>>& grid,
               int sx, int sy, int ex, int ey,
               vector<pair<int,int>>& path) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> dist(n, vector<int>(m, -1));
    vector<vector<int>> prevDir(n, vector<int>(m, -1));

    queue<Position> q;
    q.push(Position(sx, sy));
    dist[sx][sy] = 0;

    while (!q.empty()) {
        Position cur = q.front(); q.pop();

        // 到达终点
        if (cur.x == ex && cur.y == ey) {
            // 回溯构造路径
            int len = dist[ex][ey];
            path.resize(len);
            int cx = ex, cy = ey;
            for (int i = len - 1; i >= 0; i--) {
                int d = prevDir[cx][cy];
                path[i] = {cx, cy};
                cx -= dx[d];  // 回退
                cy -= dy[d];
            }
            return len;
        }

        // 扩展四个方向
        for (int d = 0; d < 4; d++) {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                grid[nx][ny] == 0 && dist[nx][ny] == -1) {
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                prevDir[nx][ny] = d;
                q.push(Position(nx, ny));
            }
        }
    }

    return -1;  // 不可达
}

int main() {
    cout << "=== 布线问题 (分支限界法/BFS) ===" << endl;

    // 7×7 网格，0=可通行，1=障碍
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    int sx = 2, sy = 2;  // 起点
    int ex = 4, ey = 4;  // 终点

    cout << "网格 (" << grid.size() << "×" << grid[0].size() << "):" << endl;
    for (int i = 0; i < (int)grid.size(); i++) {
        for (int j = 0; j < (int)grid[0].size(); j++) {
            if (i == sx && j == sy) cout << "S ";
            else if (i == ex && j == ey) cout << "E ";
            else cout << (grid[i][j] ? "█ " : ". ");
        }
        cout << endl;
    }
    cout << "(S=起点, E=终点, █=障碍)" << endl;

    vector<pair<int,int>> path;
    int len = Wiring_BFS(grid, sx, sy, ex, ey, path);

    if (len >= 0) {
        cout << "\n最短路径长度: " << len << endl;
        cout << "路径: (" << sx << "," << sy << ")";
        for (auto& p : path) cout << " -> (" << p.first << "," << p.second << ")";
        cout << endl;
    } else {
        cout << "\n不可达！" << endl;
    }

    return 0;
}
