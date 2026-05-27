/*
 * 快速排序 (Quick Sort)
 * -------------------------
 * 问题描述：将 n 个元素按非递减顺序排列。
 *
 * 算法思想（分治法）：
 *   1. 选取一个基准元素 (pivot)
 *   2. 划分：将数组分为两部分，左边 ≤ pivot，右边 ≥ pivot
 *   3. 递归地对左右两部分执行快速排序
 *
 * 时间复杂度：平均 O(n log n)，最坏 O(n²)（当每次划分极不均匀时）
 * 空间复杂度：O(log n) —— 递归栈深度
 * 稳定性：不稳定排序
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.8节
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 划分函数
 * 以第一个元素 a[left] 为基准，将数组划分为两部分
 * @param a     数组
 * @param left  区间左边界
 * @param right 区间右边界
 * @return      基准元素最终位置
 */
int Partition(vector<int>& a, int left, int right) {
    int pivot = a[left];  // 选择第一个元素为基准

    while (left < right) {
        // 从右向左找第一个小于 pivot 的元素
        while (left < right && a[right] >= pivot)
            right--;
        a[left] = a[right];  // 将该元素移到左边

        // 从左向右找第一个大于 pivot 的元素
        while (left < right && a[left] <= pivot)
            left++;
        a[right] = a[left];  // 将该元素移到右边
    }

    a[left] = pivot;  // 基准元素归位
    return left;       // 返回基准位置
}

/**
 * 快速排序 - 递归实现
 * @param a     数组
 * @param left  排序区间左边界
 * @param right 排序区间右边界
 */
void QuickSort(vector<int>& a, int left, int right) {
    if (left >= right) return;  // 递归基

    int pivotPos = Partition(a, left, right);  // 划分
    QuickSort(a, left, pivotPos - 1);           // 递归排序左半部分
    QuickSort(a, pivotPos + 1, right);          // 递归排序右半部分
}

/**
 * 随机化划分函数（避免最坏情况）
 * 随机选择一个元素作为基准，与 a[left] 交换后再划分
 */
int RandomizedPartition(vector<int>& a, int left, int right) {
    int randomIdx = left + rand() % (right - left + 1);
    swap(a[left], a[randomIdx]);  // 随机选基准放到首位
    return Partition(a, left, right);
}

void RandomizedQuickSort(vector<int>& a, int left, int right) {
    if (left >= right) return;

    int pivotPos = RandomizedPartition(a, left, right);
    RandomizedQuickSort(a, left, pivotPos - 1);
    RandomizedQuickSort(a, pivotPos + 1, right);
}

int main() {
    cout << "=== 快速排序 (Quick Sort) ===" << endl;

    vector<int> arr1 = {49, 38, 65, 97, 76, 13, 27, 50};
    vector<int> arr2 = arr1;  // 复制一份用于随机化版本

    cout << "排序前: ";
    for (int v : arr1) cout << v << " ";
    cout << endl;

    QuickSort(arr1, 0, arr1.size() - 1);
    cout << "普通快排: ";
    for (int v : arr1) cout << v << " ";
    cout << endl;

    RandomizedQuickSort(arr2, 0, arr2.size() - 1);
    cout << "随机快排: ";
    for (int v : arr2) cout << v << " ";
    cout << endl;

    return 0;
}
