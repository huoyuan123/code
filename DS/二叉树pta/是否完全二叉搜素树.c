// 4.2 是否完全二叉搜索树（左大右小的 BST）
// 需求：顺序插入构建 BST（左子树键值大，右子树键值小），
//       输出层序遍历结果；并判断是否完全二叉树（YES/NO）。

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int key;
	struct Node *left;
	struct Node *right;
} Node;

static Node *new_node(int key) {
	Node *p = (Node *)malloc(sizeof(Node));
	if (!p) return NULL;
	p->key = key;
	p->left = p->right = NULL;
	return p;
}

// 按题意插入：左子树键值大，右子树键值小（与常规 BST 相反）
static Node *bst_insert(Node *root, int key) {
	if (!root) return new_node(key);
	Node *cur = root;
	while (1) {
		if (key > cur->key) {
			if (cur->left) cur = cur->left; else { cur->left = new_node(key); break; }
		} else { // key < cur->key（输入保证互不相同）
			if (cur->right) cur = cur->right; else { cur->right = new_node(key); break; }
		}
	}
	return root;
}

static void destroy(Node *t) {
	if (!t) return;
	destroy(t->left);
	destroy(t->right);
	free(t);
}

int main(void) {
	int N;
	if (scanf("%d", &N) != 1) return 0;
	Node *root = NULL;
	for (int i = 0; i < N; ++i) {
		int x;
		if (scanf("%d", &x) != 1) return 0;
		root = bst_insert(root, x);
	}

	if (!root) {
		// 按题意 N>=1，一般不会到这里；稳妥处理
		printf("\nYES\n");
		return 0;
	}

	// 层序遍历 + 完全二叉树判定
	// 判定方法：BFS 时把空指针也入队；一旦弹出过 NULL，之后若再弹出非 NULL，则不是完全二叉树。
	// 同时收集非空结点的层序输出。
	int cap = 4 * N + 8; // 足够的队列容量（含 NULL 占位）
	Node **q = (Node **)malloc(sizeof(Node *) * cap);
	int head = 0, tail = 0;
	q[tail++] = root;
	int encounteredNull = 0;
	int isComplete = 1;

	int *out = (int *)malloc(sizeof(int) * N);
	int outLen = 0;

	while (head < tail) {
		Node *p = q[head++];
		if (p == NULL) {
			encounteredNull = 1;
			continue;
		}
		// 记录层序输出（仅非空）
		if (outLen < N) out[outLen++] = p->key;
		// 完全性检查逻辑
		if (encounteredNull) {
			isComplete = 0; // 之前出现了空位，现在又出现非空结点
			// 仍把孩子入队也可以，但没有必要；直接把逻辑跑完
		}
		// 将左右孩子（可为 NULL）入队
		if (tail + 2 <= cap) {
			q[tail++] = p->left;
			q[tail++] = p->right;
		}
	}

	// 输出层序序列（首尾无多余空格）
	for (int i = 0; i < outLen; ++i) {
		if (i) putchar(' ');
		printf("%d", out[i]);
	}
	putchar('\n');

	printf(isComplete ? "YES\n" : "NO\n");

	free(q);
	free(out);
	destroy(root);
	return 0;
}

