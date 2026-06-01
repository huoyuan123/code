/*
 * 随机数生成 (Random Number Generation)
 * ----------------------------------------
 * 问题描述：生成满足特定分布的随机数序列。
 * 这是所有随机化算法的基础。
 *
 * 算法思想：
 *   1. 线性同余法 (Linear Congruential Generator, LCG)：
 *      X_{n+1} = (a * X_n + c) mod m
 *      选取合适的参数 a, c, m 获得长周期的伪随机序列
 *   2. 平方取中法：取中间几位再平方再取中
 *   3. 利用 C++ 标准库 <random> 生成各种分布的随机数
 *
 * 本文件演示用 C++11 的随机数库生成：
 *   - 均匀分布 (Uniform Distribution)
 *   - 正态分布 (Normal Distribution)
 *   - 随机排列 (Random Shuffle)
 *
 *
 * 【阅读指引 / 易错点】
 *   - 随机数质量：伪随机发生器需要“种子”；同一种子会产生同序列（便于复现），不同种子用于真实随机。
 *   - 分布要匹配：生成整数/浮点、区间端点是否包含、是否均匀分布，都是最常见的坑。
 *   - 工程建议：优先使用 C++11 的 <random>（mt19937 + uniform_*_distribution），避免 rand() 的低质量与取模偏差。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第7.1节
 */

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include "../encoding_fix.h"
using namespace std;

int main() {
    cout << "=== 随机数生成 ===" << endl;

    // 使用 Mersenne Twister 引擎（高质量伪随机数）
    mt19937 rng(time(nullptr));  // 用当前时间作为种子

    // ---- 1. 均匀分布 ----
    cout << "\n1. 均匀分布 [0, 100):" << endl;
    uniform_int_distribution<int> uniDist(0, 99);
    cout << "  随机整数: ";
    for (int i = 0; i < 10; i++)
        cout << uniDist(rng) << " ";
    cout << endl;

    // 均匀分布浮点数 [0.0, 1.0)
    uniform_real_distribution<double> realDist(0.0, 1.0);
    cout << "  随机浮点数: ";
    for (int i = 0; i < 5; i++)
        cout << realDist(rng) << " ";
    cout << endl;

    // ---- 2. 正态分布 ----
    cout << "\n2. 正态分布 N(0, 1):" << endl;
    normal_distribution<double> normalDist(0.0, 1.0);
    cout << "  随机样本: ";
    for (int i = 0; i < 10; i++)
        cout << normalDist(rng) << " ";
    cout << endl;

    // ---- 3. 随机排列 ----
    cout << "\n3. 随机排列 (Fisher-Yates 洗牌):" << endl;
    vector<int> deck = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    shuffle(deck.begin(), deck.end(), rng);
    cout << "  洗牌后: ";
    for (int v : deck) cout << v << " ";
    cout << endl;

    // ---- 4. 伯努利分布（coin flip） ----
    cout << "\n4. 伯努利分布 p=0.5:" << endl;
    bernoulli_distribution coin(0.5);
    int heads = 0, tails = 0;
    for (int i = 0; i < 1000; i++) {
        if (coin(rng)) heads++;
        else tails++;
    }
    cout << "  1000 次掷币: 正面=" << heads << ", 反面=" << tails << endl;

    return 0;
}
