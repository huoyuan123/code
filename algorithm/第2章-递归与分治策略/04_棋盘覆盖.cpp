/*
 * 棋盘覆盖 (Chessboard Coverage)
 * ----------------------------------
 * 问题描述：在一个 2^k × 2^k 的棋盘中，恰好有一个方格残缺。
 * 用 L 形骨牌（覆盖 3 个方格）完全覆盖棋盘中除残缺方格外的所有方格。
 *
 * 算法思想（分治法）：
 *   - 将棋盘分为 4 个 2^(k-1) × 2^(k-1) 的子棋盘
 *   - 残缺方格必位于其中一个子棋盘，在其他三个子棋盘相邻处
 *     各放置一个 L 形骨牌，使它们也各有一个"残缺"方格
 *   - 对四个子棋盘递归执行此过程
 *
 * 递推公式：T(k) = 4T(k-1) + O(1)
 * L 形骨牌总数 = (4^k - 1) / 3
 *
 * 【阅读指引 / 易错点】
 *   - “中心放一块 L” 是关键：在四象限交界处放置一块 L，使得除真实残缺象限外，其余三个象限各“人为制造”一个残缺格。
 *   - 递归参数含义：tr/tc 是当前子棋盘左上角；dr/dc 是该子棋盘的残缺格坐标；size 每次减半。
 *   - 输出表示：board[][] 里存的是骨牌编号（同一编号对应同一块 L 骨牌覆盖的三个格），便于可视化调试。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.6节
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include "../encoding_fix.h"
using namespace std;

int board[64][64];  // 棋盘（支持到 2^6 = 64）
int tile = 0;       // 骨牌编号（全局递增）

/**
 * 棋盘覆盖 - 分治递归
 * @param tr  棋盘左上角行号
 * @param tc  棋盘左上角列号
 * @param dr  残缺方格行号
 * @param dc  残缺方格列号
 * @param size 棋盘大小（2 的幂）
 */
void ChessBoard(int tr, int tc, int dr, int dc, int size) {
    if (size == 1) return;  // 递归基：1×1 无需覆盖

    int t = ++tile;         // 当前使用的骨牌编号
    int s = size / 2;       // 子棋盘大小

    // ---- 左上角子棋盘 ----
    if (dr < tr + s && dc < tc + s) {
        // 残缺方格在此子棋盘中，直接递归
        ChessBoard(tr, tc, dr, dc, s);
    } else {
        // 残缺方格不在此处，用骨牌覆盖本子棋盘的右下角
        board[tr + s - 1][tc + s - 1] = t;
        ChessBoard(tr, tc, tr + s - 1, tc + s - 1, s);
    }

    // ---- 右上角子棋盘 ----
    if (dr < tr + s && dc >= tc + s) {
        ChessBoard(tr, tc + s, dr, dc, s);
    } else {
        board[tr + s - 1][tc + s] = t;
        ChessBoard(tr, tc + s, tr + s - 1, tc + s, s);
    }

    // ---- 左下角子棋盘 ----
    if (dr >= tr + s && dc < tc + s) {
        ChessBoard(tr + s, tc, dr, dc, s);
    } else {
        board[tr + s][tc + s - 1] = t;
        ChessBoard(tr + s, tc, tr + s, tc + s - 1, s);
    }

    // ---- 右下角子棋盘 ----
    if (dr >= tr + s && dc >= tc + s) {
        ChessBoard(tr + s, tc + s, dr, dc, s);
    } else {
        board[tr + s][tc + s] = t;
        ChessBoard(tr + s, tc + s, tr + s, tc + s, s);
    }
}

int main() {
    // 预设测试数据（若需交互输入请用终端运行）
    int k = 3;
    int dr = 2, dc = 5;
    cout << "=== 棋盘覆盖问题 ===" << endl;
    cout << "k = " << k << " (棋盘大小=" << (1<<k) << "x" << (1<<k) << ")" << endl;
    cout << "残缺位置: (" << dr << ", " << dc << ")" << endl;

    int size = 1 << k;  // 2^k

    // 初始化棋盘（0 表示未覆盖）
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = 0;

    // 执行覆盖
    ChessBoard(0, 0, dr, dc, size);

    // 输出结果（相同编号组成一个 L 形骨牌）
    cout << "\n棋盘覆盖结果（0=残缺, 相同数字=同一骨牌）:" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << setw(3) << board[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
