/*
 * 最优装载 (Optimal Loading)
 * ------------------------------
 * 问题描述：n 个集装箱，集装箱 i 重量为 wᵢ。轮船载重为 C。
 * 求在不超过载重的前提下，装载尽可能多的集装箱。
 *
 * 算法思想（贪心算法）：
 *   贪心策略：重量最轻的优先装船。
 *   将所有集装箱按重量升序排列，依次装入直到装不下为止。
 *
 * 时间复杂度：O(n log n) —— 排序主导
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第4.3节
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

struct Container {
    int id;     // 编号
    int weight; // 重量
};

/**
 * 最优装载 - 贪心算法
 * @param containers 集装箱列表
 * @param C          轮船载重
 * @param selected   返回选中集装箱编号
 * @return           最大装载数量
 */
int OptimalLoading(vector<Container>& containers, int C,
                   vector<int>& selected) {
    // 按重量升序排序（最轻优先）
    sort(containers.begin(), containers.end(),
         [](const Container& a, const Container& b) {
             return a.weight < b.weight;
         });

    int totalWeight = 0;
    int count = 0;

    for (auto& c : containers) {
        if (totalWeight + c.weight <= C) {
            selected.push_back(c.id);
            totalWeight += c.weight;
            count++;
        } else {
            break;  // 之后的更重，更放不下
        }
    }

    return count;
}

int main() {
    cout << "=== 最优装载 ===" << endl;

    vector<Container> containers = {
        {1, 10}, {2, 40}, {3, 30}, {4, 20}, {5, 50}, {6, 25}
    };
    int C = 80;  // 轮船载重

    cout << "集装箱重量: ";
    for (auto& c : containers)
        cout << "[" << c.id << "]=" << c.weight << " ";
    cout << endl;
    cout << "轮船载重: " << C << endl;

    vector<int> selected;
    int maxCount = OptimalLoading(containers, C, selected);

    cout << "最多装载: " << maxCount << " 个集装箱" << endl;
    cout << "选中的集装箱: ";
    for (int id : selected) cout << id << " ";
    cout << endl;

    return 0;
}
