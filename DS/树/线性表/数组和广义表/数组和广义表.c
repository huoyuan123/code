// 数组与广义表：多种特殊矩阵的紧凑表示与演示
// 本文件实现：对称矩阵、上三角矩阵、下三角矩阵、对角矩阵、稀疏矩阵（COO）
// 构造函数 + 取/设元素 + 打印还原

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef int ElemType; // 元素类型，可按需改为 double 等

// ========================= 广义表（Generalized List，头尾表示法） =========================
// 设计说明：
// - 采用“头尾表示”链式结构：
//   节点分为两类：原子（GL_ATOM）与子表（GL_LIST）。
//   若为 GL_LIST，则 u.ptr.hp 指向第一个元素（可为原子或子表），u.ptr.tp 指向“余表”（同一层的剩余元素链）。
// - 空表用空指针 NULL 表示；因此打印空表为 "()"。
// - 本实现的原子类型为单个字符（AtomType=char），便于直观演示与打印。

typedef char AtomType; // 广义表原子类型：用字符表示，如 'a','b','x' 等

typedef enum { GL_ATOM = 0, GL_LIST = 1 } GLTag;

typedef struct GLNode {
	GLTag tag; // 节点类型：原子 or 子表
	union {
		AtomType atom; // 原子结点存放字符
		struct {                // 子表结点：头指针 + 尾指针
			struct GLNode *hp;  // head pointer：本层第一个元素
			struct GLNode *tp;  // tail pointer：本层剩余元素所成的表
		} ptr;
	} u;
} GLNode, *GList;

// ---------- 基本构造与销毁 ----------
static GList GL_NewAtom(AtomType a) {
	GLNode *p = (GLNode *)malloc(sizeof(GLNode));
	if (!p) return NULL;
	p->tag = GL_ATOM; p->u.atom = a; return p;
}

static GList GL_NewList(GList hp, GList tp) {
	GLNode *p = (GLNode *)malloc(sizeof(GLNode));
	if (!p) return NULL;
	p->tag = GL_LIST; p->u.ptr.hp = hp; p->u.ptr.tp = tp; return p;
}

void GList_Destroy(GList L) {
	if (!L) return;
	if (L->tag == GL_ATOM) { free(L); return; }
	// 子表：先递归销毁 head 与 tail，再释放自身
	GList hp = L->u.ptr.hp, tp = L->u.ptr.tp;
	free(L);
	GList_Destroy(hp);
	GList_Destroy(tp);
}

// ---------- 判空/类型、长度与深度 ----------
int GList_IsEmpty(GList L) { return L == NULL; }
int GList_IsAtom(GList L)  { return L && L->tag == GL_ATOM; }

// 长度：顶层元素个数（若是原子，视为 1；空表为 0）
int GList_Length(GList L) {
	if (!L) return 0;
	if (L->tag == GL_ATOM) return 1;
	int n = 0; for (GList p = L; p; p = p->u.ptr.tp) ++n; return n;
}

// 深度：空表为 1；原子为 0；表为 (max(各元素深度) + 1)
int GList_Depth(GList L) {
	if (!L) return 1;
	if (L->tag == GL_ATOM) return 0;
	int maxd = 0;
	for (GList p = L; p; p = p->u.ptr.tp) {
		int d = GList_Depth(p->u.ptr.hp);
		if (d > maxd) maxd = d;
	}
	return maxd + 1;
}

// 取表头/表尾（若非表或空，返回 NULL）
GList GList_Head(GList L) { return (L && L->tag == GL_LIST) ? L->u.ptr.hp : NULL; }
GList GList_Tail(GList L) { return (L && L->tag == GL_LIST) ? L->u.ptr.tp : NULL; }

// ---------- 拷贝 ----------
GList GList_Copy(GList L) {
	if (!L) return NULL;
	if (L->tag == GL_ATOM) return GL_NewAtom(L->u.atom);
	return GL_NewList(GList_Copy(L->u.ptr.hp), GList_Copy(L->u.ptr.tp));
}

// ---------- 插入/删除：对“表”进行操作 ----------
// 若 *L 为原子，先包装为单元素表，再执行插入/删除，便于教学演示。
static void GList_WrapIfAtom(GList *L) {
	if (L && *L && (*L)->tag == GL_ATOM) {
		*L = GL_NewList(*L, NULL); // 把原子提升为一元表 (a)
	}
}

// 在表头插入一个元素 e（e 可以是原子或子表）
void GList_InsertFirst(GList *L, GList e) {
	if (!L) return;
	if (!*L) { *L = GL_NewList(e, NULL); return; }
	GList_WrapIfAtom(L);
	*L = GL_NewList(e, *L);
}

// 删除表头元素，返回被删元素到 *eOut；成功返回 1，失败返回 0
int GList_DeleteFirst(GList *L, GList *eOut) {
	if (!L || !*L) return 0;
	GList_WrapIfAtom(L);
	if (!*L || (*L)->tag != GL_LIST) return 0; // 防御
	GLNode *firstNode = *L;            // 头结点（代表第一个元素 + 余表）
	GList firstElem = firstNode->u.ptr.hp; // 第一个元素（可能是原子或子表）
	GList rest = firstNode->u.ptr.tp;  // 余表
	free(firstNode);                   // 释放头结点本身（不销毁元素）
	*L = rest;                         // 表指向余表
	if (eOut) *eOut = firstElem;       // 元素交给调用者管理
	return 1;
}

// ---------- 打印 ----------
static void GList_PrintRec(GList L) {
	if (!L) { printf("()"); return; }
	if (L->tag == GL_ATOM) { putchar(L->u.atom); return; }
	putchar('(');
	for (GList p = L; p; p = p->u.ptr.tp) {
		GList_PrintRec(p->u.ptr.hp);
		if (p->u.ptr.tp) printf(",");
	}
	putchar(')');
}

void GList_Print(GList L) { GList_PrintRec(L); }

// ---------- 从字符串构造（递归下降解析） ----------
// 语法（简化）：
//   G   -> ATOM | '(' ListOpt ')'
//   ListOpt -> ε | ElemList
//   ElemList -> G (',' G)*
// - ATOM 简化为“非空且不含 (),, 空白 的连续字符”，取首字符作为原子。
// - 允许任意空白。

typedef struct { const char *s; size_t i; } GLParser;

static void gl_skip_ws(GLParser *p) { while (p->s[p->i] && isspace((unsigned char)p->s[p->i])) ++p->i; }
static int  gl_peek(GLParser *p) { gl_skip_ws(p); return p->s[p->i]; }
static int  gl_get(GLParser *p)  { int ch = p->s[p->i]; if (ch) ++p->i; return ch; }

static GList gl_parse_any(GLParser *p); // 前向声明

static GList gl_parse_atom(GLParser *p) {
	gl_skip_ws(p);
	size_t start = p->i;
	while (p->s[p->i]) {
		char ch = p->s[p->i];
		if (ch == '(' || ch == ')' || ch == ',' || isspace((unsigned char)ch)) break;
		++p->i;
	}
	if (p->i == start) return NULL; // 空 -> 解析失败
	// 简化：取第一个字符作为原子值
	AtomType a = p->s[start];
	return GL_NewAtom(a);
}

static GList gl_parse_list(GLParser *p) {
	// 入口处当前字符应为 '('，调用前应已判断
	gl_get(p); // 消耗 '('
	if (gl_peek(p) == ')') { gl_get(p); return NULL; } // 空表 "()"

	// 解析第一个元素
	GList first = gl_parse_any(p);
	GList head = GL_NewList(first, NULL); // 建立第一个结点
	GList tail = head;

	// 继续解析 , G ... 直到遇到 ')'
	while (gl_peek(p) == ',') {
		gl_get(p); // 消耗 ','
		GList elem = gl_parse_any(p);
		GLNode *node = GL_NewList(elem, NULL);
		tail->u.ptr.tp = node; tail = node;
	}
	if (gl_peek(p) == ')') gl_get(p); // 消耗 ')'
	return head;
}

static GList gl_parse_any(GLParser *p) {
	int ch = gl_peek(p);
	if (ch == '\0') return NULL;            // 空串
	if (ch == '(') return gl_parse_list(p);  // 子表
	return gl_parse_atom(p);                  // 原子
}

GList GList_CreateFromString(const char *s) {
	if (!s) return NULL;
	GLParser P = { s, 0 };
	return gl_parse_any(&P);
}

// ========================= 对称矩阵（存储下三角含对角） =========================
typedef struct {
	int n;            // 阶数 n×n
	ElemType *data;   // 长度 n(n+1)/2，仅存储下三角含对角
} SymmetricMatrix;

/**
 * 计算下三角（含对角）压缩存储的一维下标。
 * 参数：i, j 为 0 基坐标，要求 i >= j。
 * 返回：在长度为 n(n+1)/2 的数组中的线性位置。
 * 复杂度：O(1)
 */
static size_t idxLower(int i, int j) { // 0 基；要求 i>=j
	return (size_t)i * (i + 1) / 2 + j;
}

/**
 * 对称矩阵压缩存储的通用下标映射（与课件公式一致）。
 * 0 基下：
 *   k = i*(i+1)/2 + j  (i>=j)
 *   k = j*(j+1)/2 + i  (i< j)
 * 等价于课件 1 基公式：
 *   k = i*(i-1)/2 + j - 1 (i>=j)
 *   k = j*(j-1)/2 + i - 1 (i< j)
 */
static size_t idxSymK(int n, int i, int j) {
	(void)n; // n 未直接使用，仅与文档一致
	if (i >= j) return (size_t)i * (i + 1) / 2 + j;
	else return (size_t)j * (j + 1) / 2 + i;
}

/**
 * 从完整 n×n 矩阵构造对称矩阵（只存下三角含对角）。
 * full 按行主序存放；只拷贝 i>=j 的元素。
 * 返回：动态分配的 SymmetricMatrix*，失败返回 NULL。
 */
SymmetricMatrix *Symmetric_CreateFromFull(int n, const ElemType *full) {
	if (n <= 0) return NULL;
	// 1) 分配结构体
	SymmetricMatrix *M = (SymmetricMatrix *)malloc(sizeof(SymmetricMatrix));
	if (!M) return NULL;
	M->n = n;
	// 2) 申请压缩存储数组：仅下三角含对角，共 n(n+1)/2 个
	size_t len = (size_t)n * (n + 1) / 2;
	M->data = (ElemType *)malloc(len * sizeof(ElemType));
	if (!M->data) { free(M); return NULL; }
	// 3) 只拷贝 i>=j 的部分（行主序读取原矩阵），下标映射与课件一致
	for (int i = 0; i < n; ++i)
		for (int j = 0; j <= i; ++j)
			M->data[idxSymK(n, i, j)] = full[i * n + j];
	return M;
}

/**
 * 释放对称矩阵存储。
 */
void Symmetric_Destroy(SymmetricMatrix *M) {
	if (!M) return; free(M->data); free(M);
}

/**
 * 获取对称矩阵元素（自动镜像到下三角读取）。
 * 越界返回 0。
 */
ElemType Symmetric_Get(const SymmetricMatrix *M, int i, int j) { // 0 基坐标
	if (!M || i < 0 || j < 0 || i >= M->n || j >= M->n) return 0;
	// 直接使用对称矩阵的通用映射公式计算 k
	return M->data[idxSymK(M->n, i, j)];
}

/**
 * 设置对称矩阵元素（写入下三角存储）。
 * 越界安全忽略。
 */
void Symmetric_Set(SymmetricMatrix *M, int i, int j, ElemType v) { // 写入两对称位置
	if (!M || i < 0 || j < 0 || i >= M->n || j >= M->n) return;
	// 使用对称矩阵的通用映射公式计算存储位置
	M->data[idxSymK(M->n, i, j)] = v;
}

/**
 * 以稠密形式打印对称矩阵。
 */
void Symmetric_PrintFull(const SymmetricMatrix *M) {
	if (!M) return;
	for (int i = 0; i < M->n; ++i) {
		for (int j = 0; j < M->n; ++j) {
			printf("%4d ", Symmetric_Get(M, i, j));
		}
		printf("\n");
	}
}

// ========================= 上三角矩阵（存储 i<=j） =========================
typedef struct {
	int n;
	ElemType *data; // 长度 n(n+1)/2，按行存储每行从对角到末列
} UpperTriMatrix;

/**
 * 计算上三角（含对角）压缩存储的一维下标。
 * 参数：n 阶数，i<=j。
 */
static size_t idxUpper(int n, int i, int j) { // 0 基；要求 i<=j
	// 行 i 之前共有 i 行：每行长度 (n - k)
	size_t before = (size_t)i * n - (size_t)i * (i - 1) / 2;
	return before + (j - i);
}

/** 从完整矩阵创建上三角压缩存储 */
UpperTriMatrix *Upper_CreateFromFull(int n, const ElemType *full) {
	if (n <= 0) return NULL;
	// 分配结构体与压缩数组
	UpperTriMatrix *M = (UpperTriMatrix *)malloc(sizeof(UpperTriMatrix));
	if (!M) return NULL; M->n = n;
	size_t len = (size_t)n * (n + 1) / 2;
	M->data = (ElemType *)malloc(len * sizeof(ElemType));
	if (!M->data) { free(M); return NULL; }
	// 只拷贝 i<=j 的部分
	for (int i = 0; i < n; ++i)
		for (int j = i; j < n; ++j)
			M->data[idxUpper(n, i, j)] = full[i * n + j];
	return M;
}

/** 销毁上三角矩阵 */
void Upper_Destroy(UpperTriMatrix *M) { if (!M) return; free(M->data); free(M); }

/** 获取上三角矩阵元素；下三角位置恒为 0 */
ElemType Upper_Get(const UpperTriMatrix *M, int i, int j) {
	if (!M || i < 0 || j < 0 || i >= M->n || j >= M->n) return 0;
	if (i <= j) return M->data[idxUpper(M->n, i, j)]; else return 0;
}

/** 稠密打印上三角矩阵 */
void Upper_PrintFull(const UpperTriMatrix *M) {
	if (!M) return;
	for (int i = 0; i < M->n; ++i) { for (int j = 0; j < M->n; ++j) printf("%4d ", Upper_Get(M,i,j)); printf("\n"); }
}

// ========================= 下三角矩阵（存储 i>=j） =========================
typedef struct {
	int n; ElemType *data; // 长度 n(n+1)/2
} LowerTriMatrix;

/** 从完整矩阵创建下三角压缩存储 */
LowerTriMatrix *Lower_CreateFromFull(int n, const ElemType *full) {
	if (n <= 0) return NULL;
	// 分配结构体与压缩数组
	LowerTriMatrix *M = (LowerTriMatrix *)malloc(sizeof(LowerTriMatrix));
	if (!M) return NULL; M->n = n;
	size_t len = (size_t)n * (n + 1) / 2;
	M->data = (ElemType *)malloc(len * sizeof(ElemType));
	if (!M->data) { free(M); return NULL; }
	// 只拷贝 i>=j 的部分
	for (int i = 0; i < n; ++i)
		for (int j = 0; j <= i; ++j)
			M->data[idxLower(i, j)] = full[i * n + j];
	return M;
}

/** 销毁下三角矩阵 */
void Lower_Destroy(LowerTriMatrix *M) { if (!M) return; free(M->data); free(M); }

/** 获取下三角矩阵元素；上三角位置恒为 0 */
ElemType Lower_Get(const LowerTriMatrix *M, int i, int j) {
	if (!M || i < 0 || j < 0 || i >= M->n || j >= M->n) return 0;
	if (i >= j) return M->data[idxLower(i, j)]; else return 0;
}

/** 稠密打印下三角矩阵 */
void Lower_PrintFull(const LowerTriMatrix *M) {
	if (!M) return;
	for (int i = 0; i < M->n; ++i) { for (int j = 0; j < M->n; ++j) printf("%4d ", Lower_Get(M,i,j)); printf("\n"); }
}

// ========================= 对角矩阵（仅存主对角） =========================
typedef struct { int n; ElemType *d; } DiagMatrix;

/** 从一维主对角数组创建对角矩阵 */
DiagMatrix *Diag_CreateFromArray(int n, const ElemType *diag) {
	if (n <= 0) return NULL;
	// 分配主对角数组并拷贝
	DiagMatrix *M = (DiagMatrix *)malloc(sizeof(DiagMatrix)); if (!M) return NULL; M->n = n;
	M->d = (ElemType *)malloc((size_t)n * sizeof(ElemType)); if (!M->d) { free(M); return NULL; }
	memcpy(M->d, diag, (size_t)n * sizeof(ElemType));
	return M;
}

/** 销毁对角矩阵 */
void Diag_Destroy(DiagMatrix *M) { if (!M) return; free(M->d); free(M); }

/** 读取对角矩阵元素；非对角恒 0 */
ElemType Diag_Get(const DiagMatrix *M, int i, int j) {
	if (!M || i < 0 || j < 0 || i >= M->n || j >= M->n) return 0; return (i==j)? M->d[i] : 0;
}

/** 稠密打印对角矩阵 */
void Diag_PrintFull(const DiagMatrix *M) {
	if (!M) return;
	for (int i = 0; i < M->n; ++i) { for (int j = 0; j < M->n; ++j) printf("%4d ", Diag_Get(M,i,j)); printf("\n"); }
}

// ========================= 稀疏矩阵（COO 三元组） =========================
typedef struct { int r, c; ElemType v; } Triple;//v 是当前扫描到的矩阵元素的数值。因为是行主序，二维下标 (i,j) 在线性数组的下标是 i*cols + j。
typedef struct { int rows, cols, nnz; Triple *a; } SparseCOO;

/**
 * 从完整矩阵构建稀疏三元组（COO）。
 * 只记录非零项 (r,c,v)。
 */
SparseCOO *Sparse_FromFull(int rows, int cols, const ElemType *full) {                 // 将稠密矩阵 full 转为 COO 三元组
	if (rows <= 0 || cols <= 0) return NULL;                                           // 参数校验：行或列非正，视为无效
	// 1) 统计非零数
	int count = 0;                                                                       // nnz 计数器
	for (int i = 0; i < rows * cols; ++i)                                                // 一维下标扫描所有元素
		if (full[i] != 0) ++count;                                                       // 遇到非零则增加计数
	// 2) 分配结构体与三元组数组
	SparseCOO *S = (SparseCOO *)malloc(sizeof(SparseCOO));                               // 申请稀疏矩阵描述结构
	if (!S) return NULL;                                                                 // 分配失败
	S->rows = rows; S->cols = cols; S->nnz = count;                                      // 写入规模与非零数
	S->a = (Triple *)malloc((size_t)count * sizeof(Triple));                             // 为 nnz 个三元组分配空间
	if (!S->a) { free(S); return NULL; }                                                 // 分配失败：释放结构体并返回
	// 3) 填充三元组（按行列顺序）
	int k = 0;                                                                           // 三元组写入位置指针
	for (int i = 0; i < rows; ++i) {                                                     // 外层：逐行
		for (int j = 0; j < cols; ++j) {                                                 // 内层：逐列
			ElemType v = full[i * cols + j];                                             // 取稠密矩阵元素 a[i][j]
			if (v != 0) {                                                                // 仅记录非零
				S->a[k++] = (Triple){ i, j, v };                                         // 生成一条 {行,列,值} 记录，并推进 k
			}
		}
	}
	return S;                                                                            // 返回构造完成的 COO 稀疏矩阵
}

/** 释放 COO 稀疏矩阵 */
void Sparse_Destroy(SparseCOO *S) { if (!S) return; free(S->a); free(S); }

/** 读取 COO 元素，顺序扫描（O(nnz)） */
ElemType Sparse_Get(const SparseCOO *S, int i, int j) {
	if (!S || i < 0 || j < 0 || i >= S->rows || j >= S->cols) return 0;
	// 顺序扫描查找匹配的 (i,j)
	for (int k = 0; k < S->nnz; ++k) if (S->a[k].r == i && S->a[k].c == j) return S->a[k].v; return 0;
}

/** 稠密打印 COO 稀疏矩阵（通过 Sparse_Get 重建） */
void Sparse_PrintFull(const SparseCOO *S) {
	if (!S) return;
	for (int i = 0; i < S->rows; ++i) {
		for (int j = 0; j < S->cols; ++j) printf("%4d ", Sparse_Get(S,i,j));
		printf("\n");
	}
}

// ========================= 稀疏矩阵（十字交叉链表 Orthogonal List） =========================
// 结点按行通过 right 链接，按列通过 down 链接；行头、列头数组保存各自首结点。
typedef struct OLNode {
	int i, j;           // 0 基行列下标
	ElemType e;         // 元素值
	struct OLNode *right; // 同一行下一个非零
	struct OLNode *down;  // 同一列下一个非零
} OLNode;

typedef struct {
	int rows, cols, nnz; // 规模与非零数
	OLNode **rhead;      // 行头指针数组，长度 rows
	OLNode **chead;      // 列头指针数组，长度 cols
} CrossList;

/** 创建一个十字链表结点 */
static OLNode *OLNode_new(int i, int j, ElemType e) {
	OLNode *p = (OLNode *)malloc(sizeof(OLNode));
	if (!p) return NULL; p->i = i; p->j = j; p->e = e; p->right = p->down = NULL; return p;
}

// 根据三元组构造十字链表；若 indexBaseIsOne=1，则输入三元组的 r/c 按 1 基转换为 0 基。
// 要求输入位置唯一（不含重复 (r,c)）。
/**
 * Cross_FromTriples：由稀疏三元组构造十字交叉链表。
 * - 行头 rhead[i] 按列序连接该行非零（j 递增）。
 * - 列头 chead[j] 按行序连接该列非零（i 递增）。
 * 遇到同位置重复时采用覆盖策略。
 */
CrossList *Cross_FromTriples(int rows, int cols, int nnz, const Triple *a, int indexBaseIsOne) {
	if (rows <= 0 || cols <= 0) return NULL;
	CrossList *C = (CrossList *)malloc(sizeof(CrossList)); if (!C) return NULL;
	C->rows = rows; C->cols = cols; C->nnz = 0;
	// 行/列头数组初始化为 NULL
	C->rhead = (OLNode **)calloc((size_t)rows, sizeof(OLNode *));
	C->chead = (OLNode **)calloc((size_t)cols, sizeof(OLNode *));
	if (!C->rhead || !C->chead) { free(C->rhead); free(C->chead); free(C); return NULL; }

	for (int k = 0; k < nnz; ++k) {
		int i = a[k].r, j = a[k].c; ElemType e = a[k].v;
		// 1) 统一索引为 0 基
		if (indexBaseIsOne) { --i; --j; }
		// 2) 越界与 0 值直接跳过
		if (i < 0 || j < 0 || i >= rows || j >= cols || e == 0) continue;
		// 3) 在第 i 行按 j 升序找到插入位置
		OLNode *prevR = NULL, *curR = C->rhead[i];
		while (curR && curR->j < j) { prevR = curR; curR = curR->right; }
		if (curR && curR->j == j) {
			// 3a) 已存在该位置：覆盖数值（列链无需改动）
			curR->e = e;
		} else {
			// 3b) 新建结点并链接到行链
			OLNode *node = OLNode_new(i, j, e);
			if (!node) { /* 分配失败则跳过该元素 */ continue; }
			node->right = curR;
			if (prevR) prevR->right = node; else C->rhead[i] = node;
			// 4) 插入到列链（第 j 列，按 i 升序）
			OLNode *prevC = NULL, *curC = C->chead[j];
			while (curC && curC->i < i) { prevC = curC; curC = curC->down; }
			node->down = curC;
			if (prevC) prevC->down = node; else C->chead[j] = node;
			C->nnz++;
		}
	}
	return C;
}

/** 释放十字链表的全部结点与头数组 */
void Cross_Destroy(CrossList *C) {
	if (!C) return;
	// 逐行遍历并释放行链上的所有结点
	for (int i = 0; i < C->rows; ++i) {
		OLNode *p = C->rhead[i];
		while (p) { OLNode *nxt = p->right; free(p); p = nxt; }
	}
	free(C->rhead); free(C->chead); free(C);
}

/** 在十字链表中查找元素（按行链），不存在返回 0。复杂度：O(该行非零数) */
ElemType Cross_Get(const CrossList *C, int i, int j) {
	if (!C || i < 0 || j < 0 || i >= C->rows || j >= C->cols) return 0;
	// 在第 i 行顺着 right 指针找到列 j 或更大者
	OLNode *p = C->rhead[i];
	while (p && p->j < j) p = p->right;
	return (p && p->j == j) ? p->e : 0;
}

/** 稠密打印十字链表重建的矩阵 */
void Cross_PrintFull(const CrossList *C) {
	if (!C) return;
	for (int i = 0; i < C->rows; ++i) {
		for (int j = 0; j < C->cols; ++j) printf("%4d ", Cross_Get(C, i, j));
		printf("\n");
	}
}

/** 打印各行链的 (列,值) 序列 */
void Cross_PrintRows(const CrossList *C) {
	if (!C) return; printf("(Row lists)\n");
	for (int i = 0; i < C->rows; ++i) {
		printf("r%-2d:", i);
		for (OLNode *p = C->rhead[i]; p; p = p->right) printf(" (c=%d,v=%d)", p->j, p->e);
		printf("\n");
	}
}

/** 打印各列链的 (行,值) 序列 */
void Cross_PrintCols(const CrossList *C) {
	if (!C) return; printf("(Col lists)\n");
	for (int j = 0; j < C->cols; ++j) {
		printf("c%-2d:", j);
		for (OLNode *p = C->chead[j]; p; p = p->down) printf(" (r=%d,v=%d)", p->i, p->e);
		printf("\n");
	}
}

// ========================= 通用稠密矩阵（double）及运算 =========================
// 说明：为实现转置/求逆/加减/乘除等通用运算，单独提供一个 double 精度的通用矩阵类型。
typedef struct { int rows, cols; double *a; } Matrix;

/** 创建 r×c 稠密矩阵并置零 */
static Matrix *Mat_create(int r, int c) {
	if (r <= 0 || c <= 0) return NULL;
	// 分配结构体与连续数据区，并将元素置零
	Matrix *M = (Matrix *)malloc(sizeof(Matrix)); if (!M) return NULL;
	M->rows = r; M->cols = c; M->a = (double *)calloc((size_t)r * c, sizeof(double));
	if (!M->a) { free(M); return NULL; } return M;
}

/** 从 double 数组拷贝创建稠密矩阵（行主序） */
static Matrix *Mat_fromArray(int r, int c, const double *arr) {
	Matrix *M = Mat_create(r, c); if (!M) return NULL;
	memcpy(M->a, arr, (size_t)r * c * sizeof(double)); return M;
}

/** 释放稠密矩阵 */
static void Mat_destroy(Matrix *M) { if (!M) return; free(M->a); free(M); }

/** 打印稠密矩阵；可选标题 */
static void Mat_print(const Matrix *M, const char *title) {
	if (title) printf("%s\n", title);
	if (!M) { printf("(null)\n"); return; }
	for (int i = 0; i < M->rows; ++i) {
		for (int j = 0; j < M->cols; ++j) printf("%8.3f ", M->a[i * M->cols + j]);
		printf("\n");
	}
}

/** 矩阵转置 */
static Matrix *Mat_transpose(const Matrix *A) {
	if (!A) return NULL; Matrix *T = Mat_create(A->cols, A->rows); if (!T) return NULL;
	// 交换行列，将 A(i,j) 写到 T(j,i)
	for (int i = 0; i < A->rows; ++i)
		for (int j = 0; j < A->cols; ++j)
			T->a[j * T->cols + i] = A->a[i * A->cols + j];
	return T;
}

/** 同型矩阵逐元素加法 */
static Matrix *Mat_add(const Matrix *A, const Matrix *B) {
	if (!A || !B || A->rows != B->rows || A->cols != B->cols) return NULL;
	Matrix *C = Mat_create(A->rows, A->cols); if (!C) return NULL;
	for (int i = 0, n = A->rows * A->cols; i < n; ++i) C->a[i] = A->a[i] + B->a[i];
	return C;
}

/** 同型矩阵逐元素减法 */
static Matrix *Mat_sub(const Matrix *A, const Matrix *B) {
	if (!A || !B || A->rows != B->rows || A->cols != B->cols) return NULL;
	Matrix *C = Mat_create(A->rows, A->cols); if (!C) return NULL;
	for (int i = 0, n = A->rows * A->cols; i < n; ++i) C->a[i] = A->a[i] - B->a[i];
	return C;
}

/** 经典矩阵乘法（i-k-j 三重循环），含稀疏跳零的小优化 */
static Matrix *Mat_mul(const Matrix *A, const Matrix *B) {
	if (!A || !B || A->cols != B->rows) return NULL;
	Matrix *C = Mat_create(A->rows, B->cols); if (!C) return NULL;
	// i-k-j 循环，按行累加；若 A(i,k)==0 可跳过一列乘加
	for (int i = 0; i < A->rows; ++i) {
		for (int k = 0; k < A->cols; ++k) {
			double aik = A->a[i * A->cols + k];
			if (aik == 0.0) continue;
			for (int j = 0; j < B->cols; ++j)
				C->a[i * C->cols + j] += aik * B->a[k * B->cols + j];
		}
	}
	return C;
}

/** 逐元素除法（Hadamard 除），同型要求；除数为 0 处结果置 0 */
static Matrix *Mat_hadamard_div(const Matrix *A, const Matrix *B) { // 逐元素除法（同型）
	if (!A || !B || A->rows != B->rows || A->cols != B->cols) return NULL;
	Matrix *C = Mat_create(A->rows, A->cols); if (!C) return NULL;
	for (int i = 0, n = A->rows * A->cols; i < n; ++i) C->a[i] = B->a[i] != 0.0 ? A->a[i] / B->a[i] : 0.0;
	return C;
}

/**
 * 矩阵求逆：Gauss-Jordan 消元（含列主元部分选主元）。
 * A 必须为方阵；若奇异返回 NULL。
 */
static Matrix *Mat_inverse(const Matrix *A) { // Gauss-Jordan with partial pivoting
	if (!A || A->rows != A->cols) return NULL;
	int n = A->rows; const double EPS = 1e-12;
	int w = n * 2; // augmented width
	double *aug = (double *)calloc((size_t)n * w, sizeof(double)); if (!aug) return NULL;
	// 1) 组装增广矩阵 [A | I]
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) aug[i * w + j] = A->a[i * n + j];
		aug[i * w + (n + i)] = 1.0;
	}
	// 2) 高斯-约当消元（列主元部分选主元）
	for (int col = 0; col < n; ++col) {
		// 2.1 选择当前列绝对值最大的主元行
		int piv = col; double maxv = fabs(aug[piv * w + col]);
		for (int r = col + 1; r < n; ++r) {
			double v = fabs(aug[r * w + col]); if (v > maxv) { maxv = v; piv = r; }
		}
		if (maxv < EPS) { free(aug); return NULL; } // 奇异矩阵
		// 2.2 必要时交换当前行与主元行
		if (piv != col) {
			for (int c = 0; c < w; ++c) { double tmp = aug[col * w + c]; aug[col * w + c] = aug[piv * w + c]; aug[piv * w + c] = tmp; }
		}
		// 2.3 主元行归一化，使主元为 1
		double denom = aug[col * w + col];
		for (int c = 0; c < w; ++c) aug[col * w + c] /= denom;
		// 2.4 对其它行做消元，使该列除主元外均为 0
		for (int r = 0; r < n; ++r) if (r != col) {
			double factor = aug[r * w + col];
			if (factor != 0.0) for (int c = 0; c < w; ++c) aug[r * w + c] -= factor * aug[col * w + c];
		}
	}
	// 3) 取右半部分作为 A^{-1}
	Matrix *Inv = Mat_create(n, n); if (!Inv) { free(aug); return NULL; }
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			Inv->a[i * n + j] = aug[i * w + (n + j)];
	free(aug); return Inv;
}

// 右除：A / B := A * inv(B)（要求 B 为方阵且 A.cols == B.rows）
/** 右除实现，内部调用 Mat_inverse + Mat_mul */
static Matrix *Mat_rdivide(const Matrix *A, const Matrix *B) {
	if (!A || !B || B->rows != B->cols || A->cols != B->rows) return NULL;
	Matrix *Binv = Mat_inverse(B); if (!Binv) return NULL;
	Matrix *R = Mat_mul(A, Binv); Mat_destroy(Binv); return R;
}

// 左除：A \ B := inv(A) * B（要求 A 为方阵且 A.rows == B.rows）
/** 左除实现，内部调用 Mat_inverse + Mat_mul */
static Matrix *Mat_ldivide(const Matrix *A, const Matrix *B) {
	if (!A || !B || A->rows != A->cols || A->rows != B->rows) return NULL;
	Matrix *Ainv = Mat_inverse(A); if (!Ainv) return NULL;
	Matrix *R = Mat_mul(Ainv, B); Mat_destroy(Ainv); return R;
}

// ========================= 演示 =========================
int main(void) {
	// 以 4×4 为例
	const int n = 4;
	ElemType fullSym[16] = {
		4, 1, 2, 3,
		1, 5, 6, 7,
		2, 6, 8, 9,
		3, 7, 9, 10
	}; // 对称

	ElemType fullUpper[16] = {
		1, 2, 3, 4,
		0, 5, 6, 7,
		0, 0, 8, 9,
		0, 0, 0, 10
	};

	ElemType fullLower[16] = {
		1, 0, 0, 0,
		2, 3, 0, 0,
		4, 5, 6, 0,
		7, 8, 9, 10
	};

	ElemType diag4[4] = {11, 22, 33, 44};

	ElemType fullSparse[20] = { // 4×5
		0, 0, 7, 0, 0,
		0, 0, 0, 0, 0,
		5, 0, 0, 0, 2,
		0, 0, 0, 9, 0
	};

	// 构造并打印
	SymmetricMatrix *S = Symmetric_CreateFromFull(n, fullSym);
	UpperTriMatrix *U = Upper_CreateFromFull(n, fullUpper);
	LowerTriMatrix *L = Lower_CreateFromFull(n, fullLower);
	DiagMatrix *D = Diag_CreateFromArray(n, diag4);
	SparseCOO *SP = Sparse_FromFull(4, 5, fullSparse);

	printf("[Symmetric]\n"); Symmetric_PrintFull(S);
	printf("\n[Upper Triangular]\n"); Upper_PrintFull(U);
	printf("\n[Lower Triangular]\n"); Lower_PrintFull(L);
	printf("\n[Diagonal]\n"); Diag_PrintFull(D);
	printf("\n[Sparse (COO) 4x5]\n"); Sparse_PrintFull(SP);

	// ---- 使用十字交叉链表构造并验证（与上面稠密打印一致）----
	CrossList *CL = Cross_FromTriples(4, 5, SP->nnz, SP->a, 0); // SP 的三元组是 0 基
	printf("\n[Sparse (CrossList) 4x5 - Full View]\n");
	Cross_PrintFull(CL);
	printf("\n"); Cross_PrintRows(CL); printf("\n"); Cross_PrintCols(CL);

	// ---------------- 通用矩阵运算演示 ----------------
	{
		double Ad[9] = { 1,2,3, 0,1,4, 5,6,0 }; // 3x3
		double Bd[9] = { 2,0,1, 1,1,0, 0,1,1 }; // 可逆（行列式 3）
		Matrix *A = Mat_fromArray(3,3,Ad);
		Matrix *B = Mat_fromArray(3,3,Bd);
		Matrix *AT = Mat_transpose(A);
		Matrix *Binv = Mat_inverse(B);
		Matrix *AplusB = Mat_add(A,B);
		Matrix *AminusB = Mat_sub(A,B);
		Matrix *AB = Mat_mul(A,B);
		Matrix *ArdivB = Mat_rdivide(A,B); // A / B := A * inv(B)
		Matrix *AelemDivB = Mat_hadamard_div(A,B); // 同型逐元素除法

		Mat_print(A, "\n[Dense] A:");
		Mat_print(B, "[Dense] B:");
		Mat_print(AT, "[Dense] A^T:");
		Mat_print(Binv, "[Dense] inv(B):");
		Mat_print(AplusB, "[Dense] A + B:");
		Mat_print(AminusB, "[Dense] A - B:");
		Mat_print(AB, "[Dense] A * B:");
		Mat_print(ArdivB, "[Dense] A / B  (A * inv(B)):");
		Mat_print(AelemDivB, "[Dense] A ./ B (element-wise):");

		Mat_destroy(A); Mat_destroy(B); Mat_destroy(AT); Mat_destroy(Binv);
		Mat_destroy(AplusB); Mat_destroy(AminusB); Mat_destroy(AB);
		Mat_destroy(ArdivB); Mat_destroy(AelemDivB);
	}

	// ---------------- 广义表（Generalized List）演示 ----------------
	{
		const char *s1 = "(a,(b,c),d,(),(e,(f,g)))"; // 示例：含原子/子表/空表
		GList L1 = GList_CreateFromString(s1);
		printf("\n[Generalized List] source: %s\n", s1);
		printf("print: "); GList_Print(L1); printf("\n");
		printf("length=%d, depth=%d\n", GList_Length(L1), GList_Depth(L1));

		// 表头与表尾
		GList h = GList_Head(L1);
		GList t = GList_Tail(L1);
		printf("head: "); GList_Print(h); printf("\n");
		printf("tail: "); GList_Print(t); printf("\n");

		// 在表头插入一个子表 (x,y)
		GList sub = GList_CreateFromString("(x,y)");
		GList_InsertFirst(&L1, sub);
		printf("after insertFirst (x,y): "); GList_Print(L1); printf("\n");

		// 删除表头元素
		GList removed = NULL;
		if (GList_DeleteFirst(&L1, &removed)) {
			printf("deleted first: "); GList_Print(removed); printf("\n");
			printf("after deleteFirst: "); GList_Print(L1); printf("\n");
			GList_Destroy(removed);
		}

		// 复制与销毁
		GList copy = GList_Copy(L1);
		printf("copy:  "); GList_Print(copy); printf("\n");
		GList_Destroy(copy);
		GList_Destroy(L1);
	}

	// 资源释放
	Symmetric_Destroy(S); Upper_Destroy(U); Lower_Destroy(L); Diag_Destroy(D); Cross_Destroy(CL); Sparse_Destroy(SP);
	return 0;
}

