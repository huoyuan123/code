#include <stdio.h>

/*
判定任务调度是否可行：判断依赖图是否有环。
输入：
  n (1..n 为任务编号)
  接着 n 行：每行先给依赖数 k，再给 k 个依赖任务编号（表示这些任务必须先于当前任务完成）。
思路：
  将每个依赖 b -> 当前任务 i 建边，统计入度，对整图做 Kahn 拓扑排序。
  若能弹出 n 个节点，则无环，输出 1；否则有环，输出 0。
*/

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n <= 0) { printf("1\n"); return 0; }

    int indeg[105] = {0};
    int g[105][105] = {0};

    for (int i = 1; i <= n; ++i) {
        int k; 
        if (scanf("%d", &k) != 1) k = 0;
        for (int j = 0; j < k; ++j) {
            int pre;
            scanf("%d", &pre);
            if (pre >= 1 && pre <= n && !g[pre][i]) {
                g[pre][i] = 1;
                ++indeg[i];
            }
        }
    }

    int q[105];
    int head = 0, tail = 0;
    for (int i = 1; i <= n; ++i) if (indeg[i] == 0) q[tail++] = i;

    int cnt = 0;
    while (head < tail) {
        int u = q[head++];
        ++cnt;
        for (int v = 1; v <= n; ++v) {
            if (g[u][v]) {
                if (--indeg[v] == 0) q[tail++] = v;
            }
        }
    }

    printf(cnt == n ? "1\n" : "0\n");
    return 0;
}
