/*
 * 凸多边形最优三角剖分 (Optimal Convex Polygon Triangulation)
 * ---------------------------------------------------------------
 * 问题描述：给定凸多边形 P = {v₀, v₁, ..., vₙ₋₁} 及定义在三角形上的权函数 w，
 * 求将 P 划分为 n-2 个互不相交的三角形，且使得各三角形权值之和最小。
 *
 * 算法思想（动态规划）：
 *   问题结构与矩阵连乘完全相同。定义 t[i][j] 为多边形 {vᵢ₋₁, vᵢ, ..., vⱼ}
 *   的最优三角剖分值（1 ≤ i < j ≤ n）。
 *   递推公式：
 *     t[i][j] = 0                                              (j - i = 1)
 *     t[i][j] = min(t[i][k] + t[k+1][j] + w(vᵢ₋₁, vₖ, vⱼ))   (i ≤ k < j)
 *
 *   权函数 w(vᵢ₋₁, vₖ, vⱼ) 常取三角形周长、弦长等。
 *
 * 时间复杂度：O(n³)
 * 空间复杂度：O(n²)
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.5节
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include "../encoding_fix.h"
using namespace std;

// 二维点结构
struct Point { double x, y; };

/**
 * 计算两点距离
 */
double Dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

/**
 * 权函数：三角形三边之和（周长）
 * 三角形顶点为 (vᵢ₋₁, vₖ, vⱼ)
 */
double Weight(const vector<Point>& v, int i, int k, int j) {
    return Dist(v[i-1], v[k]) + Dist(v[k], v[j]) + Dist(v[i-1], v[j]);
}

/**
 * 凸多边形最优三角剖分
 * @param v 凸多边形顶点（按顺时针或逆时针排列）
 * @param t t[i][j] 记录最小权值和
 * @param s s[i][j] 记录最优中间点
 */
void MinWeightTriangulation(const vector<Point>& v,
                            vector<vector<double>>& t,
                            vector<vector<int>>& s) {
    int n = v.size() - 1;  // 顶点数（v 下标 0..n 共 n+1 个）

    // 初始化：相邻两点（j-i=1）无法构成三角形，权值为 0
    for (int i = 1; i <= n; i++)
        t[i][i] = 0;

    // r = j - i，即子多边形边数
    for (int r = 2; r <= n; r++) {
        for (int i = 1; i <= n - r + 1; i++) {
            int j = i + r - 1;
            t[i][j] = 1e18;  // 初始化为极大值

            for (int k = i; k < j; k++) {
                double cost = t[i][k] + t[k+1][j] + Weight(v, i, k, j);
                if (cost < t[i][j]) {
                    t[i][j] = cost;
                    s[i][j] = k;  // 记录最优 k
                }
            }
        }
    }
}

int main() {
    cout << "=== 凸多边形最优三角剖分 ===" << endl;

    // 定义凸多边形顶点（v₀ 到 v₅）
    vector<Point> v = {
        {0, 0}, {2, 0}, {3, 2}, {2, 4}, {0, 4}, {-1, 2}
    };
    int n = v.size();  // 6 个顶点

    // 分配 n×n 表（行 1..n-1, 列 1..n-1 使用）
    vector<vector<double>> t(n, vector<double>(n, 0));
    vector<vector<int>> s(n, vector<int>(n, 0));

    // 第一个点和最后一个点是同一个（多边形闭合），传入 n-1 个点
    // 实际上 t 的下标含义：顶点从 v₁ 到 vₙ，v₀ 作为第 0 个参考点
    MinWeightTriangulation(v, t, s);

    cout << "凸多边形顶点: " << n << endl;
    cout << "最小三角剖分权值和: " << t[1][n-1] << endl;

    return 0;
}
