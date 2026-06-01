/*
 * 数值随机化算法 (Numerical Randomized Algorithm)
 * ----------------------------------------------------
 * 问题描述：用随机化方法解决数值计算问题，如 π 值计算、积分计算等。
 *
 * 算法思想：
 *   利用随机采样进行近似计算。精度随采样数增加而提高。
 *   典型应用：
 *     1. 蒙特卡罗方法求 π（投点法）
 *     2. 蒙特卡罗方法求定积分
 *
 * 特点：解总是可能不正确，但错误概率随迭代次数增加而减小。
 *
 *
 * 【阅读指引 / 易错点】
 *   - 数值随机化常见目的：用随机采样/随机扰动把“难输入”打散，使期望误差更小或期望时间更好。
 *   - 统计意义：单次随机结果波动较大时，通常要做多次独立试验取均值/中位数。
 *   - 易错点：不要把“随机一次的结果”当确定值；同时注意浮点误差与随机分布边界。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第7.2节
 */

#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>
#include <functional>
#include "../encoding_fix.h"
using namespace std;

/**
 * 蒙特卡罗方法计算 π 值（投点法）
 * 在单位正方形中随机投点，统计落在 1/4 单位圆内的比例。
 * 落在圆内的概率 = π/4，因此 π ≈ 4 × (圆内点数 / 总点数)
 *
 * @param points 投点数
 * @return π 的估计值
 */
double MonteCarloPi(int points) {
    mt19937 rng(42);  // 固定种子便于复现
    uniform_real_distribution<double> dist(0.0, 1.0);

    int inside = 0;
    for (int i = 0; i < points; i++) {
        double x = dist(rng);
        double y = dist(rng);
        // 检查是否在 1/4 圆内：x² + y² ≤ 1
        if (x * x + y * y <= 1.0) {
            inside++;
        }
    }

    return 4.0 * inside / points;
}

/**
 * 蒙特卡罗方法求定积分 ∫ₐᵇ f(x) dx
 * 思路：在矩形 [a,b]×[0,M] 中随机投点，
 * 统计落在函数曲线下方的点的比例。
 *
 * 积分 ≈ M × (b-a) × (在曲线下方的点数 / 总点数)
 *
 * @param f 被积函数
 * @param a 积分下限
 * @param b 积分上限
 * @param M 函数在 [a,b] 上的最大值（估计）
 * @param points 投点数
 */
double MonteCarloIntegral(function<double(double)> f,
                          double a, double b, double M, int points) {
    mt19937 rng(42);
    uniform_real_distribution<double> distX(a, b);
    uniform_real_distribution<double> distY(0.0, M);

    int under = 0;
    for (int i = 0; i < points; i++) {
        double x = distX(rng);
        double y = distY(rng);
        if (y <= f(x)) {  // 在曲线下方
            under++;
        }
    }

    double area = M * (b - a);
    return area * under / points;
}

int main() {
    cout << fixed << setprecision(10);
    cout << "=== 数值随机化算法 ===" << endl;

    // ---- 1. 蒙特卡罗求 π ----
    cout << "\n--- 蒙特卡罗方法求 π ---" << endl;
    cout << "真实值: " << M_PI << endl;

    for (int n : {1000, 10000, 100000, 1000000}) {
        double pi = MonteCarloPi(n);
        cout << "  投点 " << n << " 次: π ≈ " << pi
             << " (误差=" << fabs(pi - M_PI) << ")" << endl;
    }

    // ---- 2. 蒙特卡罗求定积分 ----
    cout << "\n--- 蒙特卡罗方法求定积分 ∫₀² x² dx ---" << endl;
    auto f = [](double x) { return x * x; };
    double trueIntegral = 8.0 / 3.0;  // [x³/3]₀²
    cout << "真实值: " << trueIntegral << endl;

    for (int n : {1000, 10000, 100000}) {
        double integral = MonteCarloIntegral(f, 0.0, 2.0, 4.0, n);
        cout << "  采样 " << n << " 次: ∫ ≈ " << integral
             << " (误差=" << fabs(integral - trueIntegral) << ")" << endl;
    }

    return 0;
}
