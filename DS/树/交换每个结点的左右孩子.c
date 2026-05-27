/*
题目：交换二叉树中每个结点的左孩子和右孩子
要求：
  - 输入：先序序列（# 表示空结点），如 ABC##DE#G##F###
  - 输出：两行
	   第1行：原二叉树的中序遍历序列
	   第2行：交换左右孩子后的中序遍历序列
实现要点：
  - 以二叉链表存储
  - 从输入先序串递归建树
  - 中序遍历打印（无空格）
  - 递归镜像（交换左右孩子）
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct TNode {
	char data;
	struct TNode *lc;
	struct TNode *rc;
} TNode, *Tree;

static Tree new_node(char v) {
	TNode *p = (TNode *)malloc(sizeof(TNode));
	if (!p) return NULL;
	p->data = v;
	p->lc = p->rc = NULL;
	return p;
}

/* 从先序串按规则构造：
   约定：遇到 '#' 返回空；否则创建结点，再递归构造左、右子树。
   通过 pos 指针在字符串上“推进”。 */
static Tree build_from_preorder(const char *s, int *pos) {
	if (!s) return NULL;
	char ch = s[*pos];
	if (ch == '\0') return NULL;     // 输入耗尽
	(*pos)++;
	if (ch == '#') return NULL;       // 空结点
	Tree t = new_node(ch);
	if (!t) return NULL;              // 简化：忽略后续内存失败处理
	t->lc = build_from_preorder(s, pos);
	t->rc = build_from_preorder(s, pos);
	return t;
}

/* 中序遍历：左-根-右，不打印空格，仅输出字符 */
static void inorder(Tree t) {
	if (!t) return;
	inorder(t->lc);
	putchar(t->data);
	inorder(t->rc);
}

/* 镜像：交换每个结点的左右孩子 */
static void mirror(Tree t) {
	if (!t) return;
	TNode *tmp = t->lc;
	t->lc = t->rc;
	t->rc = tmp;
	mirror(t->lc);
	mirror(t->rc);
}

static void destroy(Tree t) {
	if (!t) return;
	destroy(t->lc);
	destroy(t->rc);
	free(t);
}

/* 读取标准输入中所有非空白字符，拼成一个连续的先序串 */
static int read_all_nonspace(char *buf, int cap) {
	int n = 0;
	int ch;
	while ((ch = getchar()) != EOF) {
		if (!isspace((unsigned char)ch)) {
			if (n + 1 < cap) buf[n++] = (char)ch; // 预留结尾'\0'
		}
	}
	buf[n] = '\0';
	return n;
}

int main(void) {
	/* 读入先序序列（可能没有换行），忽略空白 */
	char s[200005];
	int len = read_all_nonspace(s, (int)sizeof(s));
	(void)len; // 未直接使用，防编译告警

	int pos = 0;
	Tree root = build_from_preorder(s, &pos);

	/* 原树中序 */
	inorder(root);
	putchar('\n');

	/* 交换左右孩子后中序 */
	mirror(root);
	inorder(root);
	putchar('\n');

	destroy(root);
	return 0;
}

