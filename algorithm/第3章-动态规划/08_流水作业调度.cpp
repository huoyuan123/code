/*
 * 流水作业调度 (Flow Shop Scheduling)
 * ---------------------------------------
 * 问题描述：n 个作业 {J₁, ..., Jₙ} 在 2 台机器 M₁ 和 M₂ 上加工。
 * 每个作业先经 M₁ 加工 aᵢ 时间，再经 M₂ 加工 bᵢ 时间。
 * 求最优作业顺序使完成时间最早。
 *
 * 算法思想（Johnson 法则）：
 *   令 N₁ = {i | aᵢ < bᵢ}（M₁ 耗时短的作业）
 *   令 N₂ = {i | aᵢ ≥ bᵢ}（M₂ 耗时短的作业）
 *   对 N₁ 按 aᵢ 升序排列
 *   对 N₂ 按 bᵢ 降序排列
 *   N₁ 在前、N₂ 在后拼接即得最优顺序
 *
 * 时间复杂度：O(n log n) —— 排序开销
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.9节
 */

/*
 * 空间复杂度：O(n)
 *
 * 【阅读指引 / 易错点】
 *   - 调度类题的核心是”状态/排序规则/代价计算”：先确认目标函数（如最小完工时间 makespan）。
 *   - 若实现采用 DP：要注意状态规模是否会爆炸；常见做法是用排序/构造降低为多项式。
 *   - 结果验证：建议同时打印每台机器的完工时间序列，能快速定位代价计算是否写错。
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

struct Job {
    int id;     // 作业编号
    int a;      // M₁ 加工时间
    int b;      // M₂ 加工时间
};

/**
 * Johnson 法则排序比较器
 * 将作业分为两类：a < b（归 N₁），a ≥ b（归 N₂）
 */
bool JohnsonCmp(const Job& x, const Job& y) {
    // 按 Johnson 法则分类：N₁ (a < b) 排在 N₂ (a >= b) 前面
    bool xN1 = (x.a < x.b);
    bool yN1 = (y.a < y.b);

    // 不同类：N₁ 全部排在 N₂ 前面
    if (xN1 != yN1) return xN1;

    // 同一类内排序：N₁ 按 a 升序，N₂ 按 b 降序
    if (xN1)
        return x.a < y.a;       // N₁: a 升序
    else
        return x.b > y.b;       // N₂: b 降序
}

/**
 * 流水作业调度 - Johnson 算法
 * @param jobs 作业列表（会排序后返回最优顺序）
 * @return     (总完成时间，最优作业顺序)
 */
int FlowShopSchedule(vector<Job>& jobs, vector<int>& order) {
    // 按 Johnson 法则排序
    sort(jobs.begin(), jobs.end(), JohnsonCmp);

    for (auto& j : jobs) order.push_back(j.id);

    // 计算总完成时间
    int n = jobs.size();
    vector<int> finishM1(n), finishM2(n);

    finishM1[0] = jobs[0].a;
    finishM2[0] = finishM1[0] + jobs[0].b;

    for (int i = 1; i < n; i++) {
        finishM1[i] = finishM1[i-1] + jobs[i].a;
        finishM2[i] = max(finishM2[i-1], finishM1[i]) + jobs[i].b;
    }

    return finishM2[n-1];
}

int main() {
    cout << "=== 流水作业调度 (Johnson 法则) ===" << endl;

    // 4 个作业：(a, b) = (3,8), (4,5), (2,9), (5,2)
    vector<Job> jobs = {
        {1, 3, 8},
        {2, 4, 5},
        {3, 2, 9},
        {4, 5, 2}
    };

    cout << "作业 (M₁, M₂):" << endl;
    for (auto& j : jobs)
        cout << "  J" << j.id << ": (" << j.a << ", " << j.b << ")" << endl;

    vector<int> order;
    int totalTime = FlowShopSchedule(jobs, order);

    cout << "\n最优调度顺序: ";
    for (int id : order) cout << "J" << id << " ";
    cout << endl;
    cout << "总完成时间: " << totalTime << endl;

    return 0;
}
