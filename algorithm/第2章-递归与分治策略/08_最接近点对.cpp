/*
 * 最接近点对问题 (Closest Pair of Points)
 * -------------------------------------------
 * 问题描述：给定平面上 n 个点，找出其中距离最近的一对点。
 *
 * 算法思想（分治法）：
 *   1. 将所有点按 x 坐标排序
 *   2. 用一条垂直线 L: x=m 将点集分为大小大致相等的左右两半 S1 和 S2
 *   3. 递归求出 S1 和 S2 中的最小距离 d1, d2，设 d = min(d1, d2)
 *   4. 考虑跨中线附近的点对：只检查 x 坐标在 [m-d, m+d] 范围内的点，
 *      将其中按 y 排序，对每个点最多检查 6 个后续点
 *   5. 返回 min(d, 跨中线的最小距离)
 *
 * 时间复杂度：O(n log n)
 *   递推公式：T(n) = 2T(n/2) + O(n)
 *
 * 【阅读指引 / 易错点】
 *   - “带状区域 strip”是关键：只需要检查中线两侧 x 距离 < d 的点，其他跨区点对不可能更近。
 *   - 为什么每点只查很少的后继点：strip 按 y 排序后，几何性质保证最多检查常数个点（常见结论是 ≤ 6 或 ≤ 7）。
 *   - 实现细节：递归基（2/3 个点）要覆盖所有小规模；同时注意 double 比较与精度输出。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.10节
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <iomanip>
#include "../encoding_fix.h"
using namespace std;

struct Point {
    double x, y;
};

/**
 * 计算两点间欧几里得距离
 */
double Distance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

// 全局比较函数
bool cmpX(const Point& a, const Point& b) { return a.x < b.x; }
bool cmpY(const Point& a, const Point& b) { return a.y < b.y; }

/**
 * 在跨中线带区域内寻找更近的点对
 * @param strip 中线附近 d 范围内的点（已按 y 排序）
 * @param d     当前已知最小距离
 * @return      跨中线区域的最小距离
 */
double StripClosest(vector<Point>& strip, double d) {
    double minDist = d;

    // 对每个点，只需检查其后的至多 6 个点
    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; j < strip.size() &&
             (strip[j].y - strip[i].y) < minDist; j++) {
            double dist = Distance(strip[i], strip[j]);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }
    return minDist;
}

/**
 * 最近点对 - 分治递归
 * @param pts 按 x 排序的点数组
 * @param left  区间左边界
 * @param right 区间右边界
 * @return      区间内最近点对的距离
 */
double ClosestPair(vector<Point>& pts, int left, int right) {
    // 递归基：2 或 3 个点时直接计算
    if (right - left == 1) {
        return Distance(pts[left], pts[right]);
    }
    if (right - left == 2) {
        double d1 = Distance(pts[left], pts[left + 1]);
        double d2 = Distance(pts[left + 1], pts[right]);
        double d3 = Distance(pts[left], pts[right]);
        return min({d1, d2, d3});
    }

    int mid = left + (right - left) / 2;
    double midX = pts[mid].x;

    // 分治：求左右子问题的解
    double dLeft  = ClosestPair(pts, left, mid);
    double dRight = ClosestPair(pts, mid + 1, right);
    double d = min(dLeft, dRight);

    // 收集中线附近 d 范围内的点
    vector<Point> strip;
    for (int i = left; i <= right; i++) {
        if (fabs(pts[i].x - midX) < d) {
            strip.push_back(pts[i]);
        }
    }

    // 按 y 坐标排序
    sort(strip.begin(), strip.end(), cmpY);

    // 在带状区域找更近点对
    return min(d, StripClosest(strip, d));
}

int main() {
    cout << "=== 最接近点对问题 ===" << endl;

    vector<Point> pts = {
        {2.0, 3.0}, {12.0, 30.0}, {40.0, 50.0},
        {5.0, 1.0}, {12.0, 10.0}, {3.0, 4.0},
        {20.0, 25.0}, {30.0, 35.0}, {15.0, 20.0}
    };

    cout << "点集:" << endl;
    for (auto& p : pts) {
        cout << "  (" << p.x << ", " << p.y << ")" << endl;
    }

    // 按 x 坐标排序（分治法前提）
    sort(pts.begin(), pts.end(), cmpX);

    double minDist = ClosestPair(pts, 0, pts.size() - 1);

    cout << fixed << setprecision(4);
    cout << "\n最近点对距离: " << minDist << endl;

    return 0;
}
