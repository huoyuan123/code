/*
 * 二分搜索 (Binary Search)
 * --------------------------
 * 问题描述：在已排序的数组 a[0:n-1] 中搜索特定元素 x。
 * 算法思想：
 *   每次将搜索范围缩小一半。将 x 与数组中间元素比较，
 *   若相等则找到；若 x 小于中间元素，则在左半部分继续搜索；
 *   否则在右半部分继续搜索。
 * 时间复杂度：O(log n) —— 每次比较将问题规模减半
 * 适用条件：数组必须有序（通常为升序）
 * 算法来源：《计算机算法设计与分析(第5版)》第2.3节
 */

#include <iostream>
#include <vector>
#include "../encoding_fix.h"
using namespace std;

/**
 * 二分搜索 - 非递归实现
 * @param a   已排序的数组（升序）
 * @param n   数组长度
 * @param x   待搜索的目标值
 * @return    目标值的索引，若未找到则返回 -1
 */
int BinarySearch(const vector<int>& a, int n, int x) {
    int left = 0;           // 搜索区间左边界
    int right = n - 1;      // 搜索区间右边界

    while (left <= right) {
        int mid = left + (right - left) / 2;  // 防溢出写法，等价于 (left+right)/2

        if (x == a[mid]) {
            return mid;           // 找到目标，返回索引
        } else if (x < a[mid]) {
            right = mid - 1;      // 目标在左半部分，收缩右边界
        } else {
            left = mid + 1;       // 目标在右半部分，收缩左边界
        }
    }

    return -1;  // 未找到
}

/**
 * 二分搜索 - 递归实现
 * @param a     已排序的数组
 * @param left  搜索区间左边界
 * @param right 搜索区间右边界
 * @param x     待搜索的目标值
 * @return      目标值的索引，若未找到则返回 -1
 */
int BinarySearchRecursive(const vector<int>& a, int left, int right, int x) {
    if (left > right) {
        return -1;  // 递归基：区间为空，未找到
    }

    int mid = left + (right - left) / 2;

    if (x == a[mid]) {
        return mid;
    } else if (x < a[mid]) {
        return BinarySearchRecursive(a, left, mid - 1, x);
    } else {
        return BinarySearchRecursive(a, mid + 1, right, x);
    }
}

int main() {
    vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int n = arr.size();

    cout << "数组: ";
    for (int v : arr) cout << v << " ";
    cout << endl;

    // 测试1：查找存在的元素
    int x1 = 7;
    int idx1 = BinarySearch(arr, n, x1);
    cout << "二分搜索 " << x1 << " (非递归): 索引 " << idx1 << endl;

    // 测试2：查找不存在的元素
    int x2 = 8;
    int idx2 = BinarySearch(arr, n, x2);
    cout << "二分搜索 " << x2 << " (非递归): 索引 " << idx2 << endl;

    // 测试3：递归版本
    int x3 = 15;
    int idx3 = BinarySearchRecursive(arr, 0, n - 1, x3);
    cout << "二分搜索 " << x3 << " (递归):   索引 " << idx3 << endl;

    return 0;
}
