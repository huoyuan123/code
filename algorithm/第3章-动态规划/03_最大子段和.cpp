/*
 * 最大子段和 (Maximum Subarray Sum)
 * ------------------------------------
 * 问题描述：给定 n 个整数（可能为负）组成的序列 a₁, a₂, ..., aₙ，
 * 求形如 Σₖ₌ᵢʲ aₖ 的子段和的最大值。当所有整数均为负时，最大子段和为 0。
 *
 * 算法思想（动态规划）：
 *   设 b[j] 为以 a[j] 结尾的最大子段和。
 *   递推公式：
 *     b[j] = max(b[j-1] + a[j], a[j])
 *   最终答案 = max(b[1], b[2], ..., b[n])
 *
 * 直观含义：对于每个位置 j，决定是"接在前面的子段后"还是"另起一个子段"。
 *
 * 时间复杂度：O(n) —— 每个元素只需常数时间
 * 空间复杂度：O(1) —— 只需维护当前 b 值
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第3.4节
 */

 /*
 * 空间复杂度：O(1)
 *
 * 【阅读指引 / 易错点】
 *   - 常见 DP 定义：bestEndHere 表示“以当前位置结尾的最大子段和”，bestSoFar 表示全局最大。
 *   - 转移直觉：要么把当前位置接在之前的最优后面，要么从当前位置重新开始（比较 a[i] 与 bestEndHere+a[i]）。
 *   - 边界：若全为负数，答案是“最大的那个负数”；初始化时不要把 bestSoFar 直接设为 0。
 */
#include <iostream>
#include <vector>
#include <climits>
#include "../encoding_fix.h"
using namespace std;

/**
 * 最大子段和 - 动态规划 O(n) 解法
 * @param a     整数序列
 * @param start 返回最大子段的起始位置（0-indexed）
 * @param end   返回最大子段的结束位置（0-indexed）
 * @return      最大子段和
 */
int MaxSubarraySum(const vector<int>& a, int& start, int& end) {
    int maxSum = 0;     // 全局最大和
    int curSum = 0;     // 以当前元素结尾的最大子段和
    int tempStart = 0;  // 临时起始位置
    start = 0;
    end = -1;           // -1 表示子段为空

    for (int j = 0; j < (int)a.size(); j++) {
        if (curSum > 0) {
            // 接在前面子段后面更优
            curSum += a[j];
        } else {
            // 另起一个子段
            curSum = a[j];
            tempStart = j;
        }

        if (curSum > maxSum) {
            maxSum = curSum;
            start = tempStart;
            end = j;
        }
    }

    return maxSum;
}

int main() {
    cout << "=== 最大子段和 ===" << endl;

    vector<int> a = {-2, 11, -4, 13, -5, -2};

    cout << "序列: ";
    for (int v : a) cout << v << " ";
    cout << endl;

    int start, end;
    int result = MaxSubarraySum(a, start, end);

    cout << "最大子段和: " << result << endl;
    cout << "子段下标: [" << start << ", " << end << "]" << endl;
    cout << "子段内容: ";
    for (int i = start; i <= end; i++)
        cout << a[i] << " ";
    cout << endl;

    return 0;
}
