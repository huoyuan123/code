/*
 * 符号三角形问题 (Symbol Triangle) - 回溯法
 * ---------------------------------------------
 * 问题描述：第一行有 n 个符号（+ 或 -），之后每行比上一行少一个符号。
 * 规则：两个同号下为 +，异号下为 -。
 * 求使 + 和 - 个数相同的符号三角形个数。
 *
 * 算法思想（回溯法）：
 *   解空间：子集树（第一行确定后，所有行随之确定）
 *   确定第一行后即可计算整个三角形中 + 和 - 的数量。
 *   剪枝：当前 + 或 - 的数量已超过总数的一半。
 *
 * 时间复杂度：最坏 O(2ⁿ)，通过剪枝减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：对第一行符号做二进制选择（+/-），其余行由规则唯一推导出来，因此只需要搜索第一行。
 *   - 剪枝直觉：在推导过程中统计当前 + 与 - 的数量，若即使把剩余全部取某符号也无法满足目标则剪枝。
 *   - 边界：推导规则一般依赖相邻两符号，注意下标范围与是否覆盖最后一个位置。
 *
 * 【实现提示】
 *   - 回溯核心：先设定 tri[0][t]，再根据规则填充下层并统计 plusCount。
 *   - 剪枝核心：plusCount 或 minusCount 超过 half 时直接返回。
 *   - 状态撤销：返回前要撤销本次填充对 plusCount 的影响。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.4节

 *
 * 解空间树：子集树（Subset Tree）
 * 递归方式：深度优先搜索(DFS)

 *
 * ═══════════════════════════════════════════════════════════════
 * 【约束函数 vs 限界函数 — 区别说明】
 * ═══════════════════════════════════════════════════════════════
 * 约束函数 (Constraint)：检查部分解是否满足问题的硬性约束条件。
 *   不满足 → 立即剪枝（该分支不可能产生可行解，继续搜无意义）。
 *   例：装载重量≦容量、皇后是否冲突、顶点颜色是否冲突。
 *
 * 限界函数 (Bounding)：估算部分解可能达到的最优目标值。
 *   不可能优于当前最优 → 剪枝（该子树不可能有更优解）。
 *   例：当前重量+剩余≦bestW、价值上界≦bestValue、curDist≧best。
 *
 * 简单记法：约束 = "能不能"（可行性）， 限界 = "值不值"（最优性）。
 * ═══════════════════════════════════════════════════════════════
 */

#include <iostream>
#include <vector>
#include "../encoding_fix.h"
using namespace std;

int n;                   // 第一行符号数
int half;                // 三角形符号总数的一半
int totalCount = 0;      // 满足条件的三角形个数
vector<vector<int>> tri; // 符号三角形: 0=+, 1=-
int plusCount = 0;       // 当前 + 的个数

/**
 * 符号三角形 - 回溯法
 * @param t 当前填充到第一行的第几个位置 (0-indexed)
 */
void Backtrack(int t) {
    // 剪枝：+ 或 - 的数量超过半数
    if (t > 0) {
        if (plusCount > half) return;  // [约束函数] +号超半，不可能满足条件
        if (t * (t + 1) / 2 - plusCount > half) return;  // - 的数量也超过半数  // [约束函数] -号超半，不可能满足条件
    }

    if (t == n) {
        // 完整的第一行，检查是否满足条件
        if (plusCount == half) {
            totalCount++;
        }
        return;
    }

    // 尝试当前位置放 +
    tri[0][t] = 0;
    plusCount++;

    // 填充由这个选择确定的其他位置
    for (int i = 1; i <= t; i++) {
        // i-1 行 t-i 和 t-i+1 确定 i 行 t-i
        if (tri[i-1][t-i] == tri[i-1][t-i+1])
            tri[i][t-i] = 0;  // 同号为 +
        else
            tri[i][t-i] = 1;  // 异号为 -
        if (tri[i][t-i] == 0) plusCount++;
    }

    Backtrack(t + 1);

    // 回溯：撤销当前位置放 + 的状态
    for (int i = 1; i <= t; i++) {
        if (tri[i][t-i] == 0) plusCount--;
    }
    plusCount--;

    // 尝试当前位置放 -
    tri[0][t] = 1;
    // plusCount 不变（这只是第一行放 -）

    for (int i = 1; i <= t; i++) {
        if (tri[i-1][t-i] == tri[i-1][t-i+1])
            tri[i][t-i] = 0;
        else
            tri[i][t-i] = 1;
        if (tri[i][t-i] == 0) plusCount++;
    }

    Backtrack(t + 1);

    // 回溯：撤销
    for (int i = 1; i <= t; i++) {
        if (tri[i][t-i] == 0) plusCount--;
    }
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    n = 4;
    cout << "=== 符号三角形问题 ===" << endl;
    cout << "n = " << n << endl;

    int totalSymbols = n * (n + 1) / 2;  // 三角形总符号数

    if (totalSymbols % 2 != 0) {
        cout << "总符号数为奇数 " << totalSymbols << "，不可能 + 和 - 个数相等" << endl;
        cout << "满足条件的三角形数: 0" << endl;
        return 0;
    }

    half = totalSymbols / 2;
    tri.assign(n, vector<int>(n, 0));

    plusCount = 0;
    totalCount = 0;

    Backtrack(0);

    cout << "总符号数: " << totalSymbols << endl;
    cout << "+ 和 - 各需要: " << half << " 个" << endl;
    cout << "满足条件的符号三角形数: " << totalCount << endl;

    return 0;
}
