/*
 * 多机调度问题 (Multi-machine Scheduling)
 * -------------------------------------------
 * 问题描述：设有 n 个独立的作业 {J₁, ..., Jₙ}，由 m 台相同的机器加工。
 * 作业 Jᵢ 需要处理时间 tᵢ。约定每个作业可在任何一台机器上加工，
 * 但不可拆分。求使完成时间最早（makespan 最小）的调度方案。
 *
 * 算法思想（贪心算法）：
 *   NP 难问题，使用贪心近似算法 —— LPT (Longest Processing Time first)。
 *   策略：将作业按处理时间降序排列，
 *   依次将每个作业分配给当前总负载最小的机器。
 *
 * 近似比：LPT 的 makespan ≤ (4/3 - 1/(3m)) × OPT
 *
 * 时间复杂度：O(n log n + n log m) —— 排序 + 优先队列
 *
 * 【阅读指引 / 易错点】
 *   - LPT 策略：先把作业按处理时间降序，再依次放到“当前总负载最小”的机器上（用最小堆维护负载）。
 *   - 为什么要先降序：把长作业尽早分散到不同机器上，通常能显著降低最终最大负载。
 *   - 实现细节：堆元素是 (负载, 机器编号)；每分配一次要 pop→更新负载→push 回去。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第4.7节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

struct Job {
    int id;
    int time;  // 处理时间
};

/**
 * 多机调度 - LPT 贪心算法
 * @param jobs      作业列表
 * @param m         机器数
 * @param schedule  返回每台机器的作业分配 (machine -> job ids)
 * @return          makespan (最大完成时间)
 */
int MultiMachineSchedule(vector<Job>& jobs, int m,
                         vector<vector<int>>& schedule) {
    // 步骤1：按处理时间降序排列（LPT 规则）
    sort(jobs.begin(), jobs.end(),
         [](const Job& a, const Job& b) {
             return a.time > b.time;
         });

    // 优先队列：{当前负载, 机器编号}，最小堆
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> pq;

    schedule.assign(m, vector<int>());

    // 初始化所有机器负载为 0
    for (int i = 0; i < m; i++) {
        pq.push({0, i});
    }

    // 依次将每个作业分配给当前负载最小的机器
    for (auto& job : jobs) {
        auto [load, machineId] = pq.top();
        pq.pop();

        schedule[machineId].push_back(job.id);
        load += job.time;
        pq.push({load, machineId});
    }

    // 找最大负载即为 makespan
    int makespan = 0;
    while (!pq.empty()) {
        makespan = max(makespan, pq.top().first);
        pq.pop();
    }

    return makespan;
}

int main() {
    cout << "=== 多机调度问题 (LPT 算法) ===" << endl;

    // 7 个作业，处理时间
    vector<Job> jobs = {
        {1, 2}, {2, 14}, {3, 4}, {4, 16}, {5, 6}, {6, 5}, {7, 3}
    };
    int m = 3;  // 3 台机器

    cout << "作业处理时间: ";
    for (auto& j : jobs) cout << "J" << j.id << "=" << j.time << " ";
    cout << endl;
    cout << "机器数: " << m << endl;

    vector<vector<int>> schedule;
    int makespan = MultiMachineSchedule(jobs, m, schedule);

    cout << "\n调度方案 (makespan = " << makespan << "):" << endl;
    for (int i = 0; i < m; i++) {
        cout << "  机器 " << (i+1) << ": ";
        int load = 0;
        for (int jobId : schedule[i]) {
            // 找回该作业的处理时间
            auto it = find_if(jobs.begin(), jobs.end(),
                [jobId](const Job& j) { return j.id == jobId; });
            int t = (it != jobs.end()) ? it->time : 0;
            cout << "J" << jobId << "(" << t << ") ";
            load += t;
        }
        cout << "  [总负载=" << load << "]" << endl;
    }

    return 0;
}
