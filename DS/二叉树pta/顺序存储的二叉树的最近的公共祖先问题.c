#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    
    int tree[1001];  // 用1~n索引存储节点值，方便对应节点编号
    for (int i = 1; i <= n; i++) {
        scanf("%d", &tree[i]);
    }
    
    int i, j;
    scanf("%d %d", &i, &j);
    
    // 检查节点是否为空
    if (tree[i] == 0) {
        printf("ERROR: T[%d] is NULL\n", i);
        return 0;
    }
    if (tree[j] == 0) {
        printf("ERROR: T[%d] is NULL\n", j);
        return 0;
    }
    
    // 寻找最近公共祖先
    int a = i, b = j;
    while (a != b) {
        if (a > b) {
            a /= 2;  // 编号大的节点上移到父节点
        } else {
            b /= 2;
        }
    }
    
    printf("%d %d\n", a, tree[a]);
    return 0;
}