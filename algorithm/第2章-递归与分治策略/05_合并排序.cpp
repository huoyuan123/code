/*
 * 合并排序 (Merge Sort)
 * -------------------------
 * 问题描述：将 n 个元素按非递减顺序排列。
 *
 * 算法思想（分治法）：
 *   1. 分解：将待排序序列分成两个长度大致相等的子序列
 *   2. 求解：递归地对两个子序列进行合并排序
 *   3. 合并：将两个有序子序列合并为一个有序序列
 *
 * 时间复杂度：O(n log n) —— 每层合并 O(n)，共 log n 层
 * 空间复杂度：O(n)     —— 需要辅助数组
 * 稳定性：稳定排序
 *
 * 【阅读指引 / 易错点】
 *   - 分治三步：递归把区间拆到长度 1（天然有序）→ 合并两个有序子区间。
 *   - Merge 的边界：left..mid 与 mid+1..right 都是闭区间；循环条件和“复制剩余元素”最容易写错。
 *   - 空间开销：需要辅助数组 b 暂存合并结果；这是归并排序换取稳定性与 O(n log n) 的代价。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.7节

 *
 * 解空间树：递归树（归并排序递归树）
 * 递归方式：深度优先搜索(DFS)
 */

#include <iostream>
#include <vector>
#include "../encoding_fix.h"
using namespace std;

/**
 * 合并两个有序子数组
 * 将 a[left:mid] 和 a[mid+1:right] 合并到 b[left:right]
 * @param a     原始数组
 * @param b     辅助数组
 * @param left  左边界
 * @param mid   中间位置
 * @param right 右边界
 */
void Merge(const vector<int>& a, vector<int>& b, int left, int mid, int right) {
    int i = left;       // 左子数组指针
    int j = mid + 1;    // 右子数组指针
    int k = left;       // 合并后数组指针

    // 两个子数组都还有元素时，取较小者放入 b
    while (i <= mid && j <= right) {
        if (a[i] <= a[j])
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    }

    // 复制左子数组剩余元素
    while (i <= mid)
        b[k++] = a[i++];

    // 复制右子数组剩余元素
    while (j <= right)
        b[k++] = a[j++];
}

/**
 * 合并排序 - 递归实现
 * @param a     原始数组
 * @param b     辅助数组（用于暂存合并结果）
 * @param left  排序区间左边界
 * @param right 排序区间右边界
 */
void MergeSort(vector<int>& a, vector<int>& b, int left, int right) {
    if (left >= right) return;  // 递归基：区间长度 ≤ 1 时已经有序

    int mid = left + (right - left) / 2;  // 防溢出

    // 对左半部分排序
    MergeSort(a, b, left, mid);
    // 对右半部分排序
    MergeSort(a, b, mid + 1, right);

    // 合并两个有序子数组
    Merge(a, b, left, mid, right);

    // 将合并结果复制回原数组
    for (int i = left; i <= right; i++)
        a[i] = b[i];
}

int main() {
    cout << "=== 合并排序 (Merge Sort) ===" << endl;

    vector<int> arr = {38, 27, 43, 3, 9, 82, 10, 56, 31, 17};
    vector<int> tmp(arr.size());  // 辅助数组

    cout << "排序前: ";
    for (int v : arr) cout << v << " ";
    cout << endl;

    MergeSort(arr, tmp, 0, arr.size() - 1);

    cout << "排序后: ";
    for (int v : arr) cout << v << " ";
    cout << endl;

    return 0;
}
