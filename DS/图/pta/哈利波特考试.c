#include <stdio.h>

#define INF 1000000000

int main(void) {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    if (n <= 0) { puts("0"); return 0; }

    static int dist[105][105];
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            dist[i][j] = (i == j) ? 0 : INF;
        }
    }

    for (int i = 0; i < m; ++i) {
        int a, b, w;
        if (scanf("%d %d %d", &a, &b, &w) != 3) return 0;
        if (a >= 1 && a <= n && b >= 1 && b <= n) {
            if (w < dist[a][b]) dist[a][b] = w;
            if (w < dist[b][a]) dist[b][a] = w; // 可逆，视为无向边
        }
    }

    // Floyd-Warshall 全源最短路
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            if (dist[i][k] == INF) continue;
            for (int j = 1; j <= n; ++j) {
                if (dist[k][j] == INF) continue;
                int nd = dist[i][k] + dist[k][j];
                if (nd < dist[i][j]) dist[i][j] = nd;
            }
        }
    }

    int bestIdx = -1;
    int bestMax = INF;
    for (int i = 1; i <= n; ++i) {
        int mx = 0;
        for (int j = 1; j <= n; ++j) {
            if (dist[i][j] == INF) { mx = INF; break; }
            if (dist[i][j] > mx) mx = dist[i][j];
        }
        if (mx < bestMax) { bestMax = mx; bestIdx = i; }
    }

    if (bestIdx == -1 || bestMax == INF) {
        puts("0");
    } else {
        printf("%d %d\n", bestIdx, bestMax);
    }
    return 0;
}
