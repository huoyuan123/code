/*
 * 循环赛日程表 (Round-Robin Schedule)
 * ---------------------------------------
 * 问题描述：设计 n=2^k 个选手的循环赛日程表，
 * 每个选手必须与其他 n-1 个选手各赛一次，
 * 每个选手每天只能赛一次，循环赛共进行 n-1 天。
 *
 * 算法思想（分治法）：
 *   将 n=2^k 个选手的日程表分成 4 个 n/2 × n/2 的子表：
 *     - 左上角：n/2 个选手前 n/2 天的安排（递归求解）
 *     - 左下角 = 左上角的值 + n/2
 *     - 右上角 = 左下角
 *     - 右下角 = 左上角
 *   从而由 2 个选手的日程表可递推出 4, 8, 16,... 个选手的日程表
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.11节
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "../encoding_fix.h"
using namespace std;

/**
 * 循环赛日程表 - 分治递归
 * @param table 日程表 (n×n)，table[i][j] 表示选手 i 在第 j 天的对手
 * @param r     当前子表起始行
 * @param c     当前子表起始列
 * @param size  当前子表大小
 */
void Schedule(vector<vector<int>>& table, int r, int c, int size) {
    if (size == 1) return;  // 递归基

    int half = size / 2;

    // 左上角子表递归求解
    Schedule(table, r, c, half);

    // 左下角 = 左上角 + half
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            table[r + half + i][c + j] = table[r + i][c + j] + half;
        }
    }

    // 右上角 = 左下角（选手对应关系不变，天数偏移）
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            table[r + i][c + half + j] = table[r + half + i][c + j];
        }
    }

    // 右下角 = 左上角
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            table[r + half + i][c + half + j] = table[r + i][c + j];
        }
    }
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    int k = 3;
    cout << "=== 循环赛日程表 ===" << endl;
    cout << "k = " << k << " (选手数=" << (1<<k) << ")" << endl;

    int n = 1 << k;  // 2^k
    vector<vector<int>> table(n, vector<int>(n, 0));

    // 初始化：选手 0 的日程表（第 i 天对战选手 i+1）
    for (int i = 0; i < n; i++) {
        table[0][i] = i + 1;  // 选手编号从 1 开始
    }

    Schedule(table, 0, 0, n);

    // 输出日程表
    cout << "\n循环赛日程表 (行=选手, 列=第几天):" << endl << endl;

    cout << setw(6) << "选手";
    for (int j = 0; j < n - 1; j++) {
        cout << setw(4) << "D" + to_string(j + 1);
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        cout << setw(6) << (i + 1);
        for (int j = 0; j < n - 1; j++) {
            cout << setw(4) << table[i][j];
        }
        cout << endl;
    }

    return 0;
}
