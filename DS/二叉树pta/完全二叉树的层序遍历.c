#include <stdio.h>

int n;
int post[35];
int level[35];

// 由后序序列构建完全二叉树的层序序列
// 参数：后序序列起始下标l、结束下标r，当前子树根在层序数组中的下标root
void build(int l, int r, int root) {
    if (l > r) return;
    if (root >= n) return;           // 防越界
    int k = r - l + 1;

    // 当前子树根是后序的最后一个元素
    level[root] = post[r];
    if (k == 1) return;              // 叶子，直接返回

    // 计算左子树节点个数（完全二叉树）
    int h = 0;                       // 能容纳 k 个节点的最小高度（从 1 开始计层）
    while ((1 << h) - 1 < k) h++;
    int full_above_last = (1 << (h - 1)) - 1;          // 除最后一层外的满层节点数
    int last_level_nodes = k - full_above_last;        // 最后一层实际节点数
    int left_max_at_last = (h >= 2) ? (1 << (h - 2)) : 0; // 最后一层左子树最多可容纳的节点数
    int left_last = (last_level_nodes < left_max_at_last) ? last_level_nodes : left_max_at_last;
    int leftSize = ((h >= 2) ? ((1 << (h - 2)) - 1) : 0) + left_last;

    // 递归构建左右子树
    build(l, l + leftSize - 1, 2 * root + 1);       // 左子树
    build(l + leftSize, r - 1, 2 * root + 2);       // 右子树
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &post[i]);
    build(0, n - 1, 0);
    // 输出层序遍历
    for (int i = 0; i < n; i++) {
        if (i) printf(" ");
        printf("%d", level[i]);
    }
    return 0;
}

