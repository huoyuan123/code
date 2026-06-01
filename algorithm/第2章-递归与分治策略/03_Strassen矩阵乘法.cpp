/*
 * Strassen 矩阵乘法
 * --------------------
 * 问题描述：计算两个 n×n 矩阵的乘积 C = A × B。
 * 传统方法需要 O(n³) 次标量乘法。
 *
 * 算法思想（分治法）：
 *   将 A、B 各分为 4 个 n/2 × n/2 的子矩阵，则
 *   C 可通过 7 次子矩阵乘法和 18 次子矩阵加/减法得到
 *   （普通分治需要 8 次乘法）。
 *   7 个中间矩阵：
 *     M1 = A11(B12 - B22)    M5 = (A11 + A22)(B11 + B22)
 *     M2 = (A11 + A12)B22    M6 = (A12 - A22)(B21 + B22)
 *     M3 = (A21 + A22)B11    M7 = (A11 - A21)(B11 + B12)
 *     M4 = A22(B21 - B11)
 *   结果：
 *     C11 = M5 + M4 - M2 + M6    C12 = M1 + M2
 *     C21 = M3 + M4               C22 = M5 + M1 - M3 - M7
 *
 * 时间复杂度：T(n) = 7T(n/2) + O(n²) => O(n^log₂7) ≈ O(n^2.81)
 *
 * 【阅读指引 / 易错点】
 *   - 分治结构：先把矩阵拆成 4 块子矩阵（A11/A12/A21/A22 等），再按公式计算 7 个 M 矩阵，最后拼回 C 的 4 个块。
 *   - 规模约束：实现里通常假设 n 为 2 的幂；真实数据需要“补零填充”到最近的 2 的幂后再计算。
 *   - 性能直觉：减少了乘法次数但增加了加减次数与内存分配；小规模时可能比普通 O(n^3) 更慢。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第2.5节
 */

#include <iostream>
#include <vector>
#include "../encoding_fix.h"
using namespace std;

typedef vector<vector<int>> Matrix;

/**
 * 矩阵加法：C = A + B
 */
Matrix Add(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

/**
 * 矩阵减法：C = A - B
 */
Matrix Sub(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

/**
 * 拆分子矩阵：从原矩阵中取子块
 * @param src 源矩阵
 * @param row 起始行
 * @param col 起始列
 * @param sz  子矩阵大小
 */
Matrix GetSubMatrix(const Matrix& src, int row, int col, int sz) {
    Matrix sub(sz, vector<int>(sz, 0));
    for (int i = 0; i < sz; i++)
        for (int j = 0; j < sz; j++)
            sub[i][j] = src[row + i][col + j];
    return sub;
}

/**
 * 合并子矩阵：将子矩阵写入目标矩阵指定位置
 */
void SetSubMatrix(Matrix& dst, const Matrix& src, int row, int col) {
    int sz = src.size();
    for (int i = 0; i < sz; i++)
        for (int j = 0; j < sz; j++)
            dst[row + i][col + j] = src[i][j];
}

/**
 * Strassen 矩阵乘法
 * 约束：n 必须是 2 的幂（便于二分），实际应用需填充补齐
 */
Matrix Strassen(const Matrix& A, const Matrix& B) {
    int n = A.size();

    // 递归基：1×1 矩阵直接相乘
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    int half = n / 2;

    // 分解 A、B 为 4 个子矩阵
    Matrix A11 = GetSubMatrix(A, 0, 0, half);
    Matrix A12 = GetSubMatrix(A, 0, half, half);
    Matrix A21 = GetSubMatrix(A, half, 0, half);
    Matrix A22 = GetSubMatrix(A, half, half, half);

    Matrix B11 = GetSubMatrix(B, 0, 0, half);
    Matrix B12 = GetSubMatrix(B, 0, half, half);
    Matrix B21 = GetSubMatrix(B, half, 0, half);
    Matrix B22 = GetSubMatrix(B, half, half, half);

    // 计算 7 个中间矩阵（7 次递归乘法，替代 8 次）
    Matrix M1 = Strassen(A11, Sub(B12, B22));
    Matrix M2 = Strassen(Add(A11, A12), B22);
    Matrix M3 = Strassen(Add(A21, A22), B11);
    Matrix M4 = Strassen(A22, Sub(B21, B11));
    Matrix M5 = Strassen(Add(A11, A22), Add(B11, B22));
    Matrix M6 = Strassen(Sub(A12, A22), Add(B21, B22));
    Matrix M7 = Strassen(Sub(A11, A21), Add(B11, B12));

    // 组合得到 C 的四个子矩阵
    Matrix C11 = Add(Sub(Add(M5, M4), M2), M6);
    Matrix C12 = Add(M1, M2);
    Matrix C21 = Add(M3, M4);
    Matrix C22 = Sub(Sub(Add(M5, M1), M3), M7);

    // 合并为结果矩阵
    Matrix C(n, vector<int>(n, 0));
    SetSubMatrix(C, C11, 0, 0);
    SetSubMatrix(C, C12, 0, half);
    SetSubMatrix(C, C21, half, 0);
    SetSubMatrix(C, C22, half, half);

    return C;
}

/**
 * 朴素矩阵乘法（用于验证结果正确性）
 */
Matrix NaiveMul(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

int main() {
    cout << "=== Strassen 矩阵乘法 ===" << endl;

    // 4×4 矩阵示例
    Matrix A = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 1, 2, 3},
        {4, 5, 6, 7}
    };

    Matrix B = {
        {8, 7, 6, 5},
        {4, 3, 2, 1},
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    };

    Matrix C_strassen = Strassen(A, B);
    Matrix C_naive = NaiveMul(A, B);

    cout << "矩阵 A:" << endl;
    for (auto& row : A) {
        for (int v : row) cout << v << "\t";
        cout << endl;
    }

    cout << "\n矩阵 B:" << endl;
    for (auto& row : B) {
        for (int v : row) cout << v << "\t";
        cout << endl;
    }

    cout << "\nStrassen 结果 C = A×B:" << endl;
    for (auto& row : C_strassen) {
        for (int v : row) cout << v << "\t";
        cout << endl;
    }

    cout << "\n朴素乘法结果 (验证):" << endl;
    for (auto& row : C_naive) {
        for (int v : row) cout << v << "\t";
        cout << endl;
    }

    return 0;
}
