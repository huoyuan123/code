/*
 * 蒙特卡罗算法 (Monte Carlo Algorithm)
 * ---------------------------------------
 * 问题描述：通过随机采样获得近似解，解可能不正确，
 * 但错误概率随采样数增加而减小。
 *
 * 算法思想：
 *   蒙特卡罗算法的特点：
 *     - 运行时间固定或可预期
 *     - 可能返回不正确的解（与拉斯维加斯相反）
 *     - 错误概率可通过增加采样次数控制
 *     - 适用于求数值近似解、组合计数等
 *
 * 典型应用：
 *   1. 主元素问题 (Majority Element)
 *   2. 素性测试 (Miller-Rabin)
 *   3. 字符串匹配（指纹/哈希方法）
 *   4. 图连通性测试
 *
 * 与拉斯维加斯的区别：
 *   - 蒙特卡罗：时间固定，结果可能错 — 牺牲正确性换时间
 *   - 拉斯维加斯：结果一定对，时间随机 — 牺牲时间换正确性
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第7.5节
 */

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 蒙特卡罗主元素检测 (Majority Element)
 *
 * 问题：判断数组中是否存在出现次数 > n/2 的主元素。
 * 确定性算法需要 O(n) 时间（Boyer-Moore 投票）。
 * 蒙特卡罗方法：随机采样少量元素，统计频率大致估计。
 *
 * @param arr  数组
 * @param samples 采样次数
 * @return 主元素值，不存在则返回 -1（以一定概率错误）
 */
int MonteCarloMajority(const vector<int>& arr, int samples) {
    int n = arr.size();
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, n - 1);

    // 随机采样，统计各元素出现频率
    vector<int> freq(1001, 0);  // 假设元素值 ≤ 1000
    for (int s = 0; s < samples; s++) {
        int idx = dist(rng);
        freq[arr[idx]]++;
    }

    // 看采样中哪个元素占多数
    int threshold = samples / 2;
    for (int val = 0; val <= 1000; val++) {
        if (freq[val] > threshold) {
            return val;           // 很可能就是主元素
        }
    }

    return -1;  // 未检测到主元素（可能错误）
}

/**
 * 蒙特卡罗素性测试 (简化版 Fermat 测试)
 *
 * 费马小定理：若 n 是素数，对任意 a ∈ [1, n-1]，有 a^(n-1) ≡ 1 (mod n)
 * 若存在 a 使 a^(n-1) ≢ 1 (mod n)，则 n 一定是合数。
 * 若所有选中的 a 都满足同余，则 n 很可能是素数。
 *
 * @param n 待测整数
 * @param k 测试次数（越大越可靠）
 * @return 素数返回 true，合数返回 false
 */
bool FermatPrimalityTest(long long n, int k = 10) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    mt19937 rng(random_device{}());
    uniform_int_distribution<long long> dist(2, n - 2);

    // 快速幂取模：计算 base^exp mod mod
    auto modPow = [](long long base, long long exp, long long mod) -> long long {
        long long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    };

    for (int i = 0; i < k; i++) {
        long long a = dist(rng);
        // 计算 a^(n-1) mod n
        if (modPow(a, n - 1, n) != 1) {
            return false;  // 一定是合数
        }
    }

    return true;  // 很可能是素数（错误概率 ≤ (1/2)^k）
}

int main() {
    cout << "=== 蒙特卡罗算法 ===" << endl;

    // ---- 1. 主元素检测 ----
    cout << "\n--- 蒙特卡罗主元素检测 ---" << endl;
    vector<int> arr = {5, 5, 3, 5, 5, 5, 2, 5, 5, 7, 5, 5};
    cout << "数组: ";
    for (int v : arr) cout << v << " ";
    cout << endl;

    for (int s : {5, 10, 20, 50}) {
        int result = MonteCarloMajority(arr, s);
        cout << "  采样 " << s << " 次: 主元素 = " << result
             << (result == 5 ? " (正确!)" : " (错误)") << endl;
    }

    // ---- 2. 素性测试 ----
    cout << "\n--- 蒙特卡罗素性测试 (Fermat) ---" << endl;
    vector<long long> testNums = {17, 561, 1105, 7919, 104729};

    for (long long num : testNums) {
        bool isPrime = FermatPrimalityTest(num, 20);
        cout << "  " << num << " : " << (isPrime ? "可能素数" : "合数");

        // 验证（小数字手动验证）
        cout << " [";
        bool actual = true;
        if (num <= 1) actual = false;
        for (long long d = 2; d * d <= num; d++) {
            if (num % d == 0) { actual = false; break; }
        }
        cout << (actual ? "实际: 素数" : "实际: 合数");
        cout << (isPrime == actual ? " ✓" : " ✗") << "]" << endl;
    }

    return 0;
}
