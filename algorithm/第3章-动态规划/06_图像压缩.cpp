/*
 * 图像压缩 (Image Compression)
 * --------------------------------
 * 问题描述：将像素点序列 {p₁, p₂, ..., pₙ} 分割为连续的段。
 * 每段包含最多 256 个像素，段中所有像素用相同位数的定长编码。
 * 存储空间 = 11（段头：8位段长+3位位数）+ 段长 × 位数。
 * 求使总存储空间最小的分割方案。
 *
 * 算法思想（动态规划）：
 *   设 s[i] 为前 i 个像素的最优存储空间。
 *   递推公式：
 *     s[i] = min(s[k] + seg(k+1, i))  (0 ≤ k < i, i-k ≤ 256)
 *   其中 seg(k+1, i) = 11 + (i-k) × max_bits(p_{k+1}..p_i)
 *   max_bits = ⌈log₂(max_pixel_value + 1)⌉
 *
 * 时间复杂度：O(n × 256) = O(n)
 * 算法来源：《计算机算法设计与分析(第5版)》第3.7节
 */

 /*
 * 空间复杂度：O(n)
 *
 * 【阅读指引 / 易错点】
 *   - 常见建模：把图像按行/列分段，每段使用某种编码方式，代价=误差/压缩率的综合；DP 用来选择最优分段。
 *   - 初始化：不可达状态要用“无穷大”表示；转移时注意避免溢出（先判断前态是否为 INF）。
 *   - 方案输出：若需要输出每段边界，通常另设 pre[i] 记录最优切分点。
 */
#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

/**
 * 计算表示数值需要的二进制位数
 */
int BitsNeeded(int value) {
    if (value == 0) return 1;
    int bits = 0;
    while (value > 0) {
        bits++;
        value >>= 1;
    }
    return bits;
}

/**
 * 图像压缩 - 动态规划
 * @param pix   像素灰度值序列
 * @param segs  返回分段的起始位置（用于重建方案）
 * @return      最小存储空间
 */
int ImageCompress(const vector<int>& pix, vector<int>& segStart) {
    int n = pix.size();
    const int MAX_SEG = 256;     // 每段最多 256 个像素
    const int HEADER = 11;       // 段头开销

    vector<int> dp(n + 1, INT_MAX);
    vector<int> cut(n + 1, -1);  // cut[i] = 分段点位置

    dp[0] = 0;

    // 预处理每段的最大所需位数
    for (int i = 1; i <= n; i++) {
        int maxVal = 0;
        // 向前扫描最多 256 个像素
        for (int k = i; k >= max(1, i - MAX_SEG + 1); k--) {
            maxVal = max(maxVal, pix[k - 1]);
            int bits = BitsNeeded(maxVal);
            int segLen = i - k + 1;
            int cost = dp[k - 1] + HEADER + segLen * bits;

            if (cost < dp[i]) {
                dp[i] = cost;
                cut[i] = k - 1;  // 记录分段点
            }
        }
    }

    // 回溯构造分段方案
    int pos = n;
    while (pos > 0) {
        segStart.push_back(cut[pos] + 1);  // 转为 0-index
        pos = cut[pos];
    }
    reverse(segStart.begin(), segStart.end());

    return dp[n];
}

int main() {
    cout << "=== 图像压缩 ===" << endl;

    // 示例像素灰度值序列
    vector<int> pix = {10, 12, 15, 255, 1, 2, 1, 1, 2, 2, 1, 1};

    cout << "像素序列 (" << pix.size() << " 个像素): ";
    for (int v : pix) cout << v << " ";
    cout << endl;

    vector<int> segs;
    int totalSpace = ImageCompress(pix, segs);

    cout << "最小存储空间: " << totalSpace << " bits" << endl;
    cout << "分段起止位置: " << endl;
    for (size_t i = 0; i < segs.size(); i++) {
        int start = segs[i];
        int end = (i + 1 < segs.size()) ? segs[i+1] - 1 : (int)pix.size() - 1;
        cout << "  段" << (i+1) << ": 像素[" << start << ".." << end << "]";
        cout << " (" << (end - start + 1) << " 个像素)" << endl;
    }

    return 0;
}
