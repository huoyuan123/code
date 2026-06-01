/*
 * 活动安排问题 (Activity Arrangement)
 * ---------------------------------------
 * 问题描述：给定 n 个活动，每个活动 i 有起始时间 sᵢ 和结束时间 fᵢ。
 * 若两个活动 [sᵢ, fᵢ) 和 [sⱼ, fⱼ) 不重叠（即 fᵢ ≤ sⱼ 或 fⱼ ≤ sᵢ），则可以兼容。
 * 求最大兼容活动集合。
 *
 * 算法思想（贪心算法）：
 *   贪心策略：每次选择结束时间最早且与已选活动兼容的活动。
 *   正确性证明：贪心选择性质 + 最优子结构性质
 *     设活动按 f 升序排列，活动 1 结束最早。
 *     存在某个最优解包含活动 1（否则可用活动 1 替换第一个活动）。
 *
 * 时间复杂度：O(n log n) —— 排序主导
 *
 * 【阅读指引 / 易错点】
 *   - 选择规则：按结束时间 f 升序排序，每次选“结束最早且与已选兼容”的活动。
 *   - 正确性直觉：交换论证——任取最优解，把其中第一个活动替换为“结束最早活动”不会减少可选数量。
 *   - 冲突判定：本文件用半开区间 [s,f)（允许 f 相等首尾相接）；若改为闭区间要同步调整兼容条件。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第4.1节
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

struct Activity {
    int id;     // 活动编号
    int start;  // 起始时间
    int finish; // 结束时间
};

/**
 * 活动安排 - 贪心算法
 * @param acts      活动列表（按结束时间排序）
 * @param selected  返回选中的活动编号
 * @return          最大活动数
 */
int GreedyActivitySelector(vector<Activity>& acts, vector<int>& selected) {
    // 按结束时间升序排序（贪心策略的核心）
    sort(acts.begin(), acts.end(),
         [](const Activity& a, const Activity& b) {
             return a.finish < b.finish;
         });

    selected.push_back(acts[0].id);  // 选择第一个活动（结束最早）
    int lastFinish = acts[0].finish; // 上一个选中活动的结束时间
    int count = 1;

    for (size_t i = 1; i < acts.size(); i++) {
        if (acts[i].start >= lastFinish) {
            // 当前活动与已选活动兼容
            selected.push_back(acts[i].id);
            lastFinish = acts[i].finish;
            count++;
        }
    }

    return count;
}

int main() {
    cout << "=== 活动安排问题 ===" << endl;

    // 活动 (起始时间, 结束时间)
    vector<Activity> acts = {
        {1, 1, 4},
        {2, 3, 5},
        {3, 0, 6},
        {4, 5, 7},
        {5, 3, 9},
        {6, 6, 10},
        {7, 8, 11},
        {8, 8, 12},
        {9, 2, 13},
        {10, 12, 14}
    };

    cout << "所有活动 (起始, 结束):" << endl;
    for (auto& a : acts)
        cout << "  活动" << a.id << ": [" << a.start << ", " << a.finish << ")" << endl;

    vector<int> selected;
    int maxCount = GreedyActivitySelector(acts, selected);

    cout << "\n最大兼容活动数: " << maxCount << endl;
    cout << "选中的活动: ";
    for (int id : selected) cout << "活动" << id << " ";
    cout << endl;

    return 0;
}
