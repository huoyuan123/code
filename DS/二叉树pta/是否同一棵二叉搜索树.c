// PTA 7.2 是否同一棵二叉搜索树
// 构造参考 BST，再对每个查询序列构造 BST 比较结构是否一致

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *left;
    struct Node *right;
} Node;

static Node *new_node(int val) {
    Node *p = (Node *)malloc(sizeof(Node));
    if (!p) return NULL;
    p->val = val;
    p->left = p->right = NULL;
    return p;
}

static Node *insert(Node *root, int val) {
    if (!root) return new_node(val);
    if (val < root->val) {
        root->left = insert(root->left, val);
    } else {
        root->right = insert(root->right, val);
    }
    return root;
}

static int same_tree(Node *a, Node *b) {
    if (!a || !b) return a == b;
    if (a->val != b->val) return 0;
    return same_tree(a->left, b->left) && same_tree(a->right, b->right);
}

static void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    int N, L;
    while (scanf("%d", &N) == 1 && N) {
        if (scanf("%d", &L) != 1) break;

        Node *ref_root = NULL;
        for (int i = 0, x; i < N; ++i) {
            scanf("%d", &x);
            ref_root = insert(ref_root, x);
        }

        for (int i = 0; i < L; ++i) {
            Node *cur = NULL;
            for (int j = 0, x; j < N; ++j) {
                scanf("%d", &x);
                cur = insert(cur, x);
            }
            if (same_tree(ref_root, cur)) {
                printf("Yes\n");
            } else {
                printf("No\n");
            }
            free_tree(cur);
        }

        free_tree(ref_root);
    }
    return 0;
}
