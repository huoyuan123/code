// 一个二叉树，如果每一个层的结点数都达到最大值，则这个二叉树就是完美二叉树。对于深度为 D 的，有 N 个结点的二叉树，若其结点对应于相同深度完美二叉树的层序遍历的前 N 个结点，这样的树就是完全二叉树。

// 给定一棵完全二叉树的后序遍历，请你给出这棵树的层序遍历结果。

// 输入格式：
// 输入在第一行中给出正整数 N（≤30），即树中结点个数。第二行给出后序遍历序列，为 N 个不超过 100 的正整数。同一行中所有数字都以空格分隔。

// 输出格式：
// 在一行中输出该树的层序遍历序列。所有数字都以 1 个空格分隔，行首尾不得有多余空格。

// 输入样例：
// 8
// 91 71 2 34 10 15 55 18
// 输出样例：
// 18 34 55 71 2 10 15 91
#include <stdio.h>
#include <stdlib.h>
#define MAXN 30
int tree[MAXN + 1]; // 使用数组存储完全二叉树，索引从1开始
void postOrderToLevelOrder(int n, int postOrder[]) {
    // 将后序遍历结果存入数组
    for (int i = 0; i < n; i++) {
        tree[i + 1] = postOrder[i];
    }
    // 输出层序遍历结果
    for (int i = 1; i <= n; i++) {
        printf("%d", tree[i]);
        if (i < n) printf(" ");
    }
    printf("\n");
}
int main() {
    int n;
    scanf("%d", &n);
    int postOrder[MAXN];
    for (int i = 0; i < n; i++) {
        scanf("%d", &postOrder[i]);
    }
    postOrderToLevelOrder(n, postOrder);
    return 0;
}
