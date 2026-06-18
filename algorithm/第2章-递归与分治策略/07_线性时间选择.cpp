/*
 * 线性时间选择 (Linear Time Selection)
 * ----------------------------------------
 * 问题描述：在 n 个元素组成的无序集合中，找到第 k (1≤k≤n) 小的元素。
 *
 * 算法思想（分治法）：
 *   1. 将 n 个元素分成 ⌈n/5⌉ 组，每组 5 个元素
 *   2. 找每组的中位数
 *   3. 递归找出中位数的中位数 x
 *   4. 以 x 为基准划分数组
 *   5. 根据 k 与基准位置的关系决定在哪一侧递归查找
 *
 * 时间复杂度：O(n) —— 每组最多 5 个元素保证中位数的中位数不会太偏
 *   递推公式：T(n) ≤ T(⌈n/5⌉) + T(3n/4) + O(n)
 *
 * 【阅读指引 / 易错点】
 *   - “5 分组”是为了保证 pivot 不会太偏：中位数的中位数至少能丢弃固定比例元素，从而得到线性时间上界。
 *   - k 的含义：这里的 k 是当前子区间内的第 k 小（1-indexed），递归到右侧时要减去 rank。
 *   - 小规模优化：当区间元素较少时直接排序取第 k 小，减少常数开销（阈值可按需要调整）。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.9节

 *
 * 解空间树：递归树（选择算法递归树）
 * 递归方式：深度优先搜索(DFS)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 交换两个元素
 */
void Swap(int& a, int& b) {
    int t = a; a = b; b = t;
}

/**
 * 划分函数（与快速排序相同）
 * @return 基准元素最终位置
 */
int Partition(vector<int>& a, int left, int right, int pivotVal) {
    // 找到 pivotVal 并放到最左边
    for (int i = left; i <= right; i++) {
        if (a[i] == pivotVal) {
            Swap(a[left], a[i]);
            break;
        }
    }

    int pivot = a[left];
    while (left < right) {
        while (left < right && a[right] >= pivot) right--;
        a[left] = a[right];
        while (left < right && a[left] <= pivot) left++;
        a[right] = a[left];
    }
    a[left] = pivot;
    return left;
}

/**
 * 线性时间选择
 * @param a     数组
 * @param left  区间左边界
 * @param right 区间右边界
 * @param k     要找的第 k 小 (1-indexed within this segment)
 * @return      第 k 小的元素值
 */
int Select(vector<int>& a, int left, int right, int k) {
    if (right - left < 75) {
        // 元素较少时直接排序取第 k 小
        sort(a.begin() + left, a.begin() + right + 1);
        return a[left + k - 1];
    }

    // 步骤1-3：找中位数的中位数
    for (int i = 0; i <= (right - left) / 5; i++) {
        // 每组 5 个元素排序，取中位数放到前面
        int subLeft = left + i * 5;
        int subRight = min(subLeft + 4, right);
        sort(a.begin() + subLeft, a.begin() + subRight + 1);
        int medianIdx = subLeft + (subRight - subLeft) / 2;
        Swap(a[left + i], a[medianIdx]);
    }

    // 递归求中位数的中位数
    int numMedians = (right - left + 1) / 5;
    if ((right - left + 1) % 5 != 0) numMedians++;
    int pivotVal = Select(a, left, left + numMedians - 1, (numMedians + 1) / 2);

    // 步骤4：划分
    int pivotPos = Partition(a, left, right, pivotVal);
    int rank = pivotPos - left + 1;  // pivot 在当前区间内的排名

    // 步骤5：递归查找
    if (k == rank) {
        return a[pivotPos];
    } else if (k < rank) {
        return Select(a, left, pivotPos - 1, k);
    } else {
        return Select(a, pivotPos + 1, right, k - rank);
    }
}

int main() {
    cout << "=== 线性时间选择 ===" << endl;

    vector<int> arr = {38, 27, 43, 3, 9, 82, 10, 56, 31, 17,
                       55, 71, 2, 88, 44, 66, 19, 91, 8, 50};

    cout << "数组: ";
    for (int v : arr) cout << v << " ";
    cout << endl;

    for (int k = 1; k <= arr.size(); k += 5) {
        int val = Select(arr, 0, arr.size() - 1, k);
        cout << "第 " << k << " 小的元素: " << val << endl;
    }

    // 验证：排序后对照
    sort(arr.begin(), arr.end());
    cout << "\n排序后 (验证): ";
    for (int v : arr) cout << v << " ";
    cout << endl;

    return 0;
}
