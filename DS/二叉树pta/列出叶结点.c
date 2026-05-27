#include <stdio.h>
#include <stdlib.h>

#define MAXN 10

typedef struct {
    int left, right;
} Node;

int main() {
    int n;
    Node tree[MAXN];
    int isChild[MAXN] = {0}; // 标记是否为孩子节点，用于找根
    int queue[MAXN], front = 0, rear = 0;

    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        char lstr[3], rstr[3];
        scanf("%s %s", lstr, rstr);
        if (lstr[0] == '-') {
            tree[i].left = -1;
        } else {
            tree[i].left = atoi(lstr);
            isChild[tree[i].left] = 1;
        }
        if (rstr[0] == '-') {
            tree[i].right = -1;
        } else {
            tree[i].right = atoi(rstr);
            isChild[tree[i].right] = 1;
        }
    }

    int root = -1;
    for (int i = 0; i < n; i++) {
        if (!isChild[i]) {
            root = i;
            break;
        }
    }

    // 层序遍历
    queue[rear++] = root;
    int firstLeaf = 1;
    while (front < rear) {
        int u = queue[front++];
        if (tree[u].left == -1 && tree[u].right == -1) {
            // 叶节点
            if (firstLeaf) {
                printf("%d", u);
                firstLeaf = 0;
            } else {
                printf(" %d", u);
            }
        }
        if (tree[u].left != -1) queue[rear++] = tree[u].left;
        if (tree[u].right != -1) queue[rear++] = tree[u].right;
    }

    return 0;
}
