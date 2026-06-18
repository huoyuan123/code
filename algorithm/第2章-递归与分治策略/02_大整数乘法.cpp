/*
 * 大整数乘法 (Large Integer Multiplication)
 * ---------------------------------------------
 * 问题描述：计算两个 n 位大整数的乘积。传统乘法需要 O(n^2) 次位乘操作。
 * 算法思想（分治法）：
 *   将 X 和 Y 分别拆分为高位和低位两部分：
 *     X = A * 10^(n/2) + B
 *     Y = C * 10^(n/2) + D
 *   则：XY = AC * 10^n + ((A+B)(C+D) - AC - BD) * 10^(n/2) + BD
 *   只需计算 3 次 n/2 位的乘法（AC, BD, (A+B)(C+D)），而非 4 次。
 * 时间复杂度：T(n) = 3T(n/2) + O(n) => O(n^log₂3) ≈ O(n^1.59)
 *
 * 【阅读指引 / 易错点】
 *   - 核心在“减少一次乘法”：把 4 次子乘（AC, AD, BC, BD）改成 3 次子乘（AC, BD, (A+B)(C+D)）。
 *   - 递归基要足够小：当位数很小时直接用普通乘法，避免递归开销。
 *   - 本实现用 long long + pow(10, k) 演示思想：pow 是浮点运算，位数大时会有精度/溢出问题；工程上应使用大整数类与整型幂。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.4节
 *
 * 说明：本实现使用 C++ 的 long long 模拟，实际应用需配合大数类使用。

 *
 * 解空间树：递归树（分治乘法递归树）
 * 递归方式：深度优先搜索(DFS)
 */

#include <iostream>
#include <cmath>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

typedef long long ll;

/**
 * 计算整数的位数
 */
int GetDigitCount(ll num) {
    if (num == 0) return 1;
    int cnt = 0;
    while (num > 0) {
        cnt++;
        num /= 10;
    }
    return cnt;
}

/**
 * 大整数乘法 - 分治实现
 * @param x  乘数
 * @param y  被乘数
 * @return   x * y 的结果
 */
ll LargeIntMul(ll x, ll y) {
    // 递归基：当数字较小时直接用普通乘法
    if (x < 10 || y < 10) {
        return x * y;
    }

    // 计算位数（取两者中较大位数）
    int n = max(GetDigitCount(x), GetDigitCount(y));
    int half = n / 2;
    ll power = (ll)pow(10, half);

    // 拆分 X = A * 10^half + B, Y = C * 10^half + D
    ll A = x / power;
    ll B = x % power;
    ll C = y / power;
    ll D = y % power;

    // 三次递归乘法（核心：减少一次乘法）
    ll AC = LargeIntMul(A, C);           // 计算 AC
    ll BD = LargeIntMul(B, D);           // 计算 BD
    ll ABCD = LargeIntMul(A + B, C + D); // 计算 (A+B)(C+D)

    // 组合结果：XY = AC*10^(2*half) + ((A+B)(C+D)-AC-BD)*10^half + BD
    return AC * (ll)pow(10, 2 * half) + (ABCD - AC - BD) * power + BD;
}

int main() {
    cout << "=== 大整数乘法（分治法）===" << endl;

    // 测试样例
    ll a = 12345678;
    ll b = 87654321;

    cout << "计算 " << a << " × " << b << endl;
    cout << "分治结果: " << LargeIntMul(a, b) << endl;
    cout << "直接运算: " << a * b << "  (验证)" << endl;

    return 0;
}
