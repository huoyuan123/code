/*
 * 舍伍德算法 (Sherwood Algorithm)
 * -----------------------------------
 * 问题描述：消除或减少算法对特定输入的依赖，使算法性能与输入实例无关。
 * 即使对相同的输入，每次运行的时间和结果也可能不同，
 * 但算法总能给出正确结果（与蒙特卡罗不同）。
 *
 * 算法思想：
 *   舍伍德算法的核心是"随机化预处理"：
 *   在执行确定性算法前对输入进行随机排列，
 *   使最坏情况的发生概率变得极小。
 *
 * 典型应用：
 *   1. 随机化快速排序（见 06_快速排序.cpp）
 *   2. 随机化线性时间选择（通过随机选择 pivot 避免 O(n²)）
 *   3. 随机化二分搜索（先随机化排列再搜索）
 *
 * 与拉斯维加斯、蒙特卡罗的区别：
 *   - 舍伍德：总是正确，随机化用于平滑性能
 *   - 拉斯维加斯：总是正确，但运行时间随机
 *   - 蒙特卡罗：运行时间固定，但可能给出错误结果
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第7.3节
 */

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 确定性算法：在有序数组中找某个元素首次出现的位置
 * 对于已排序的数组，二分搜索很高效。
 * 但若数组未排序，需要 O(n) 搜索——舍伍德思想是
 * 先随机化，使搜索复杂度不依赖于输入。
 */

/**
 * 随机化选择第 k 小元素
 * 总是返回正确结果，但通过随机化 pivot 避免最坏情况 O(n²)
 */
int RandomizedSelect(vector<int>& a, int left, int right, int k) {
    if (left == right) return a[left];

    // 随机选择 pivot（舍伍德核心）
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(left, right);
    int pivotIdx = dist(rng);
    swap(a[left], a[pivotIdx]);

    // 划分
    int pivot = a[left];
    int i = left, j = right;
    while (i < j) {
        while (i < j && a[j] >= pivot) j--;
        a[i] = a[j];
        while (i < j && a[i] <= pivot) i++;
        a[j] = a[i];
    }
    a[i] = pivot;

    int rank = i - left + 1;
    if (k == rank) return a[i];
    else if (k < rank) return RandomizedSelect(a, left, i - 1, k);
    else return RandomizedSelect(a, i + 1, right, k - rank);
}

/**
 * 随机洗牌（Fisher-Yates / Knuth Shuffle）
 * 对任何输入序列进行随机化预处理
 */
void RandomShuffle(vector<int>& arr) {
    mt19937 rng(random_device{}());
    int n = arr.size();
    for (int i = n - 1; i > 0; i--) {
        uniform_int_distribution<int> dist(0, i);
        int j = dist(rng);
        swap(arr[i], arr[j]);
    }
}

int main() {
    cout << "=== 舍伍德算法 ===" << endl;

    // ---- 随机化选择 ----
    cout << "\n--- 随机化选择第 k 小 ---" << endl;
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10, 56, 31, 17,
                       55, 71, 2, 88, 44, 66, 19, 91, 8, 50};
    cout << "数组: ";
    for (int v : arr) cout << v << " ";
    cout << endl;

    for (int k : {1, 5, 10, 15, 20}) {
        vector<int> tmp = arr;
        int val = RandomizedSelect(tmp, 0, tmp.size() - 1, k);
        cout << "第 " << k << " 小: " << val << endl;
    }

    // ---- 随机洗牌 ----
    cout << "\n--- 随机洗牌（舍伍德预处理）---" << endl;
    vector<int> deck = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    RandomShuffle(deck);
    cout << "洗牌结果: ";
    for (int v : deck) cout << v << " ";
    cout << endl;

    return 0;
}
