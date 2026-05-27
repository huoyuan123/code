// 串（字符串）——定长顺序存储与堆分配表示
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
函数名: StrLength（函数原型）
作  用: 返回以'\0'结尾的 C 字符串长度（不包含终止符）。
参  数: s - C 字符串指针
返  回: 字符串长度（int）
说  明: 在 main 中使用到，故提前声明；实现见文件末尾。
*/
int StrLength(const char *s);

/*
三种串的存储方式的区别：

1) 定长顺序存储表示（SString）
	- 结构：固定大小数组 + length；不依赖'\0' 终止。
	- 优点：随机访问 O(1)，结构简单，无堆分配/释放开销，局部性好。
	- 缺点：容量受 MAXSTRLEN 限制，超长需截断；短串可能浪费空间。
	- 适用：上界已知、长度较短或受限、性能敏感（数组访问友好）的场景。

2) 堆分配存储方式（HString）
	- 结构：在堆上按实际长度分配连续内存 + length（不依赖'\0' 终止）。
	- 优点：容量弹性大，按需分配更省空间；随机访问 O(1)。
	- 缺点：需要 malloc/free；联结等操作通常需重新分配并搬移 O(n)，可能产生碎片。
	- 适用：长度变化大、未知上界、需要节省静态空间的通用场景。

3) 块链存储方式（LString）
	- 结构：多个固定大小块（Chunk）链式连接 + 总 length。
	- 优点：减少对大块连续内存的需求，扩展/拼接较灵活，长串可降低一次性整体搬移成本。
	- 缺点：随机访问需遍历块，缓存局部性较差；有额外指针/管理开销，实现更复杂。
	- 适用：超长串、频繁拼接/扩展、系统可用大块连续内存受限的场景。

典型复杂度对比（简述）：
	- 访问第 i 个字符：SString/HString 为 O(1)；LString 需按块遍历，约 O(块数)。
	- 联结（Concat）：SString 需拷贝且可能截断；HString 需新分配并拷贝 O(|A|+|B|)；LString 可通过链接块降低整串拷贝成本（实现相关）。

备注：本文件的实现以教学演示为主，并未覆盖所有工程优化技巧。
*/

// ---------------- 定长顺序存储 ----------------

#define MAXSTRLEN 255
typedef struct {
	char ch[MAXSTRLEN];
	int length; // 实际有效字符数（0..MAXSTRLEN）
} SString;

/*
函数名: StrAssign_S
作  用: 将以'\0'结尾的 C 字符串赋值到定长顺序串 S 中（必要时截断）。
参  数: S - 目标定长串；cstr - 源 C 字符串
返  回: 无
复  杂: 时间 O(n)，空间 O(1)
注意事项: 超过 MAXSTRLEN 的部分将被截断；不自动补'\0'，以 length 管理。
*/
// 时间复杂度: O(n)  空间复杂度: O(1)
void StrAssign_S(SString *S, const char *cstr) {
	/*strlen 的返回类型是 size_t。
	 size_t 是一个无符号整数类型，用来表示对象大小，位宽与平台相关（32 位平台通常为 32 位无符号，64 位平台通常为 64 位无符号）。
	 头文件中由 <stddef.h> 定义，包含 <string.h> 也能用。*/
	int n = (int)strlen(cstr);
	if (n > MAXSTRLEN) n = MAXSTRLEN; // 截断到最大长度
	for (int i = 0; i < n; ++i) S->ch[i] = cstr[i];
	S->length = n;
}

/*
函数名: Concat_S
作  用: 定长顺序串联结：C = A ∘ B。
参  数: A, B - 源定长串；C - 输出定长串
返  回: 1 表示完整联结；0 表示发生截断（超过 MAXSTRLEN）。
复  杂: 时间 O(|A|+|B|)，空间 O(1)
注意事项: 超长将尾部截断以适配 MAXSTRLEN。
*/
// 时间复杂度: O(|A|+|B|)  空间复杂度: O(1)
int Concat_S(const SString *A, const SString *B, SString *C) {
	int total = A->length + B->length;
	int ok = 1;
	if (total > MAXSTRLEN) { total = MAXSTRLEN; ok = 0; }
	int k = 0;
	for (int i = 0; i < A->length && k < total; ++i) C->ch[k++] = A->ch[i];
	for (int j = 0; j < B->length && k < total; ++j) C->ch[k++] = B->ch[j];
	C->length = k;
	return ok;
}

/*
函数名: SubString_S
作  用: 取得定长顺序串 S 的子串 T = S[pos..pos+len-1]（pos 为 1 基）。
参  数: S - 源串；pos - 起始位置(1 基)；len - 子串长度；T - 输出子串
返  回: 1 表示成功；0 表示参数不合法（越界等）。
复  杂: 时间 O(len)，空间 O(1)
注意事项: 使用 1 基位置；不依赖'\0'，通过 length 维护。
*/
// 时间复杂度: O(len)  空间复杂度: O(1)
// 这里的pos是1基，S->ch是0基
int SubString_S(const SString *S, int pos, int len, SString *T) {
	if (pos < 1 || len < 0 || pos > S->length || pos - 1 + len > S->length) return 0;
	int start = pos - 1;
	for (int i = 0; i < len; ++i) T->ch[i] = S->ch[start + i];
	T->length = len;
	return 1;
}

/*
函数名: PrintS
作  用: 将定长顺序串按内容输出到标准输出（不依赖'\0'）。
参  数: S - 要打印的定长串
返  回: 无
复  杂: 时间 O(n)，空间 O(1)
*/
// 打印定长串（不依赖'\0'终止）
void PrintS(const SString *S) {
	for (int i = 0; i < S->length; ++i) putchar(S->ch[i]);
}
// ---------------- 堆分配存储 ----------------

/*堆是进程的动态内存区，程序在运行时可按需向操作系统申请/释放任意大小的内存块。
在 C 里通过 malloc/calloc/realloc 申请，通过 free 释放；在 C++ 里用 new/delete。*/

typedef struct {
	char *ch;    // 指向堆区字符数组（可能不以'\0'结尾）
	int length;  // 实际长度
} HString;

/*
函数名: StrAssign_H
作  用: 用 C 字符串内容构造/覆盖堆分配串 S。
参  数: S - 目标堆串；cstr - 源 C 字符串
返  回: 1 成功；0 内存分配失败或 S 为 NULL
复  杂: 时间 O(n)，空间 O(n)
注意事项: 会释放 S 原有缓冲并重新分配；不附加'\0'。
*/
// 时间复杂度: O(n)  空间复杂度: O(n)
int StrAssign_H(HString *S, const char *cstr) {
	if (!S) return 0;
	int n = (int)strlen(cstr);
	char *buf = (char *)malloc(n > 0 ? n : 1);
	if (!buf && n > 0) return 0;
	/*这里的堆串不以'\0'结尾、用 length 管理长度，而 strcpy 会复制直到并包含终止符，多拷贝1个字节；
	  当前只分配了 n 个字节，用 strcpy 会越界，且在拼接时源缓冲也不保证有 '\0'。*/
	if (n > 0) memcpy(buf, cstr, n);
	/*strcpy: 依赖源有 '\0'，复制到包含 '\0'，目标需有 n+1 空间。
	  strncpy: 最多复制 n 个字节，不保证结尾加 '\0'（需要手动补），且会在源更短时用 0 填充剩余字节，不推荐用来替代 memcpy。
 	  memcpy: 按字节数精确复制，不关心 '\0'，不处理重叠（重叠用 memmove）。*/
	if (S->ch) free(S->ch);
	/*S->ch 之前可能指向一块用 malloc 分配的旧数据；在本次重新赋值前，先把这块旧内存释放掉：if (S->ch) free(S->ch);。*/
	S->ch = buf;
	S->length = n;
	return 1;
}
/*浅拷贝
	只拷贝指针值，不复制指针指向的数据。
	修改其中一方的数据会影响另一方；释放时容易发生双重释放或非法释放。
	例（不安全示例）：S->ch = (char*)cstr; 仅别名同一块内存。
深拷贝
	申请新内存，把原数据内容复制过去，彼此独立管理。
	修改或释放任何一方都不影响另一方。
	例（你的代码）：在 StrAssign_H 中先 malloc 得到 buf，再 memcpy(buf, cstr, n)，最后把 S->ch = buf;。这就是深拷贝。
以下情况要用深拷贝：
	需要独立生命周期/所有权的资源（如字符串、缓冲区、复杂结构）。
	需要安全修改、独立释放，避免共享带来的副作用。*/
// 这里用buf中转就是在深拷贝,防止cstr和S->ch的内存所有权相同、可写性相同、生命周期相同

/*
函数名: Concat_H
作  用: 堆分配串联结：T = A ∘ B（重建 T 的缓冲区）。
参  数: T - 目标堆串；A, B - 源堆串
返  回: 1 成功；0 失败（空指针或分配失败）
复  束: 时间 O(|A|+|B|)，空间 O(|A|+|B|)
注意事项: 若 T 原先有缓冲将被释放；不以'\0'结尾，用 length 管理。
*/
// 时间复杂度: O(|A|+|B|)  空间复杂度: O(|A|+|B|)
int Concat_H(HString *T, const HString *A, const HString *B) {
	if (!T || !A || !B) return 0;
	int total = A->length + B->length;
	/*malloc 得到的是一块连续的内存，你把它转换成某种类型的指针后，就可以“像数组一样”用下标访问，本质上就是一个手工管理的“动态数组”。但它不是编译器层面的数组，很多事情要你自己负责。*/
	char *buf = (char *)malloc(total > 0 ? total : 1);
	if (!buf && total > 0) return 0;
	int k = 0;
	// 在内存里，‘\0’本身占据空间（1 个字节，对 char 而言）。
	// 在 C 字符串长度定义里（strlen 返回值），‘\0’不计入长度。
	if (A->length > 0) { memcpy(buf + k, A->ch, A->length); k += A->length; }
	if (B->length > 0) { memcpy(buf + k, B->ch, B->length); k += B->length; }
	if (T->ch) free(T->ch);
	T->ch = buf;
	T->length = total;
	return 1;
}

/*
函数名: Destroy_H
作  用: 释放堆分配串的缓冲并复位为长度 0。
参  数: S - 目标堆串
返  回: 无
复  杂: 时间 O(1)  空间 O(1)
*/
void Destroy_H(HString *S) {
	if (!S) return;
	if (S->ch) free(S->ch);
	S->ch = NULL; S->length = 0;
}

/*
函数名: PrintH
作  用: 输出堆分配串的内容到标准输出（不依赖'\0'）。
参  数: S - 要打印的堆串
返  回: 无
复  杂: 时间 O(n)  空间 O(1)
*/
void PrintH(const HString *S) {
	for (int i = 0; i < S->length; ++i) putchar(S->ch[i]);
}

// ---------------- 块链串（块结点 + 块链串类型定义） ----------------

#define CHUNKSIZE 8  // 每个块的字符容量，可按 PPT 要求调整
typedef struct Chunk {
	char ch[CHUNKSIZE];
	int len;              // 当前块已使用的字符个数（0..CHUNKSIZE）
	struct Chunk *next;
} Chunk;

typedef struct {
	Chunk *head;          // 头块指针
	Chunk *tail;          // 尾块指针
	int length;           // 串的总长度（所有块 len 之和）
} LString;                // 块链串（链式存储的串）

/*
函数名: StrAssign_L
作  用: 由 C 字符串构造块链串（按 CHUNKSIZE 分块）。
参  数: T - 输出块链串；cstr - 源 C 字符串
返  回: 1 成功；0 失败（分配失败或 T 为 NULL）
复  杂: 时间 O(n)  空间 O(n/CHUNKSIZE * 块大小)
注意事项: 这是演示性实现；空串会构造一个 len=0 的空块。
*/
int StrAssign_L(LString *T, const char *cstr) {
	if (!T) return 0;
	T->head = T->tail = NULL; T->length = 0;
	int n = (int)strlen(cstr), k = 0;
	while (k < n) {
		/*malloc 成功后，node 指向的一整块内存的内容是未初始化的“垃圾值”（不确定值）；只有对你显式赋值的成员才有确定值。*/
		Chunk *node = (Chunk *)malloc(sizeof(Chunk));
		/*它是在检查内存分配是否失败：如果 malloc(sizeof(Chunk)) 返回了 NULL（即 !node），就立刻返回 0 表示函数失败，避免后续对空指针解引用导致崩溃。*/
		if (!node) return 0;
		node->len = 0; node->next = NULL;
		while (node->len < CHUNKSIZE && k < n) {
			node->ch[node->len++] = cstr[k++];
		}
		if (!T->head) T->head = node; else T->tail->next = node;
		T->tail = node; T->length += node->len;
		// T的总长度是所有块的len之和
	}

	// 当源字符串为空、构造出来没有任何块时，用它来维持 LString 的不变式（head/tail 永远非空），以便后续操作可以少写分支、直接用 tail 进行链接。
	if (!T->head) { // 空串
		T->head = T->tail = (Chunk *)malloc(sizeof(Chunk));
		// if (!T->head) 表示前面没有分到任何块（源字符串长度为 0），当前是空串。
		if (!T->head) return 0;
		T->head->len = 0; T->head->next = NULL;
	}
	return 1;
}

/*
函数名: Destroy_L
作  用: 释放块链串的所有块并复位。
参  数: T - 目标块链串
返  回: 无
复  杂: 时间 O(块数)  空间 O(1)
*/
void Destroy_L(LString *T) {
	if (!T) return;
	Chunk *p = T->head;
	while (p) { Chunk *nxt = p->next; free(p); p = nxt; }
	T->head = T->tail = NULL; T->length = 0;
}

/*
函数名: PrintL
作  用: 顺序输出块链串的全部字符。
参  数: T - 要打印的块链串
返  回: 无
复  杂: 时间 O(n)  空间 O(1)
*/
void PrintL(const LString *T) {
	for (const Chunk *p = T->head; p; p = p->next) {
		for (int i = 0; i < p->len; ++i) putchar(p->ch[i]);
	}
}

// ---------------- 串的模式匹配算法（Brute-Force） ----------------

/*模式匹配：子串在主串中的定位称为模式匹配或串匹配(字符串匹配) 。
模式匹配成功是指在主串S中能够找到模式串T，否则，称模式串T在主串S中不存在。
*/
/*
函数名: Index_BF_Buffer（内部通用实现）
作  用: 在文本 T[0..n-1] 中寻找模式 P[0..m-1] 的首次匹配位置（BF 暴力匹配）。
参  数: T - 文本缓冲；n - 文本长度；P - 模式缓冲；m - 模式长度；pos1 - 起始检查位置(1 基)
返  回: 首次匹配的 1 基起始下标；未找到返回 0
复  杂: 时间 O((n-m+1)*m) 最坏；空间 O(1)
注意事项: pos1 < 1 视为从 1 开始；当 start > n-m 时直接失败。
*/
static int Index_BF_Buffer(const char *T, int n, const char *P, int m, int pos1) {
    if (!T || !P || m <= 0 || n <= 0) return 0;
    int start = pos1 < 1 ? 0 : pos1 - 1; // 起始检查位置（0 基）
	/*文本 T 的索引范围是 [0, n-1]，模式 P 的索引范围是 [0, m-1]。
	  当起点为 i 时，会访问到 T[i + (m - 1)]。
	  不越界条件：i + (m - 1) ≤ n - 1 ⇒ i ≤ n - m。*/
    if (start > n - m) return 0;
    for (int i = start; i <= n - m; ++i) {
        int j = 0;
        while (j < m && T[i + j] == P[j]) ++j;
        if (j == m) return i + 1; // 返回 1 基
    }
    return 0;
}

/*
函数名: IndexAll_BF_Buffer（内部通用实现，收集全部命中）
作  用: 在文本 T[0..n-1] 中寻找模式 P[0..m-1] 的所有匹配位置。
参  数: T - 文本缓冲；n - 文本长度；P - 模式缓冲；m - 模式长度；
		pos1 - 起始检查位置(1 基)；out - 输出位置数组（存 1 基位置）；
		cap - out 可容纳的最大元素个数；allowOverlap - 是否允许重叠匹配（1 允许 / 0 不允许）。
返  回: 实际找到并写入 out 的个数（可能 <= cap）。
复  杂: 时间 O((n-m+1)*m) 最坏；空间 O(1)（不含 out）。
注意事项: 不依赖 '\0'，仅按长度 n/m 工作；当 cap 为 0 或 out 为 NULL，将仅统计数量而不写入（仍返回计数）。
*/
static int IndexAll_BF_Buffer(const char *T, int n, const char *P, int m, int pos1,
							  int *out, int cap, int allowOverlap) {
	if (!T || !P || m <= 0 || n <= 0) return 0;
	int start = pos1 < 1 ? 0 : pos1 - 1; // 0 基
	if (start > n - m) return 0;
	int count = 0;

	// 若 cap < 0 视为 0，避免负值参与比较
	if (cap < 0) cap = 0;

	for (int i = start; i <= n - m; ) {
		int j = 0;
		while (j < m && T[i + j] == P[j]) ++j;
		if (j == m) {
			// 命中：记录 1 基位置
			if (out && count < cap) out[count] = i + 1;
			/*把这次命中的起点位置写入输出数组 out。
			  i 是 0 基，这里存“1 基”位置，所以写 i + 1。
			  只有当 out 非空且还没超出数组容量 cap 时才写入；即使数组满了，也会继续统计命中次数（见下面的 ++count）*/
			++count;
			// 根据是否允许重叠，前进不同的步长
			i += allowOverlap ? 1 : m;
		} else {
			++i;
		}
	}
	return count;
}
// 在调用的时候需要int out[cap],所以cap不能太大

// ---------------- 串的模式匹配算法（KMP） ----------------
/*
KMP 核心思想：当出现失配时，利用模式自身的前缀/后缀信息，避免回溯文本指针 i。
next[i] = P[0..i] 的“最长相等真前缀”的长度（也等价为下一次比较的 j 位置）。
构造 next 的常见写法如下（基于前缀函数）。
*/

// 计算模式 P 的前缀函数 next（长度为 m），next[i] ∈ [0..i]
static void KMP_BuildNext(const char *P, int m, int *next) {
	if (m <= 0) return;
	next[0] = 0;
	int j = 0; // 已匹配的前缀长度
	for (int i = 1; i < m; ++i) {
		while (j > 0 && P[i] != P[j]) j = next[j - 1];
		if (P[i] == P[j]) ++j;
		next[i] = j;
	}
}

/* 单次匹配：在 T[0..n-1] 中查找 P[0..m-1] 的首次出现（1 基返回；未找到 0） */
static int Index_KMP_Buffer(const char *T, int n, const char *P, int m, int pos1) {
	if (!T || !P || n <= 0 || m <= 0) return 0;
	int start = pos1 < 1 ? 0 : pos1 - 1; // 0 基起点
	if (start > n - m) return 0;
	int *next = (int *)malloc(sizeof(int) * m);
	if (!next) return 0;
	KMP_BuildNext(P, m, next);
	int j = 0; // 模式指针
	for (int i = start; i < n; ++i) {
		while (j > 0 && T[i] != P[j]) j = next[j - 1];
		if (T[i] == P[j]) ++j;
		if (j == m) { // 命中
			int pos1_out = i - m + 2; // 转 1 基
			free(next);
			return pos1_out;
		}
	}
	free(next);
	return 0;
}

/* 全部匹配：收集所有出现位置。allowOverlap=1 允许重叠；否则每次命中后将 j 置 0 跳过重叠 */
static int IndexAll_KMP_Buffer(const char *T, int n, const char *P, int m, int pos1,
							   int *out, int cap, int allowOverlap) {
	if (!T || !P || n <= 0 || m <= 0) return 0;
	int start = pos1 < 1 ? 0 : pos1 - 1; // 0 基
	if (start > n - m) return 0;
	if (cap < 0) cap = 0;
	int *next = (int *)malloc(sizeof(int) * m);
	if (!next) return 0;
	KMP_BuildNext(P, m, next);
	int j = 0, count = 0;
	for (int i = start; i < n; ++i) {
		while (j > 0 && T[i] != P[j]) j = next[j - 1];
		if (T[i] == P[j]) ++j;
		if (j == m) {
			// 命中：记录 1 基位置
			int pos1_hit = i - m + 2;
			if (out && count < cap) out[count] = pos1_hit;
			++count;
			// 允许重叠：按 KMP 规律继续 j = next[m-1]; 不允许：重置为 0
			j = allowOverlap ? next[m - 1] : 0;
		}
	}
	free(next);
	return count;
}

// SString/HString 的 KMP 封装
int Index_KMP_S(const SString *S, const SString *P, int pos) {
	return Index_KMP_Buffer(S->ch, S->length, P->ch, P->length, pos);
}
int IndexAll_KMP_S(const SString *S, const SString *P, int pos,
				   int *out, int cap, int allowOverlap) {
	return IndexAll_KMP_Buffer(S->ch, S->length, P->ch, P->length, pos,
							   out, cap, allowOverlap);
}
int Index_KMP_H(const HString *S, const HString *P, int pos) {
	return Index_KMP_Buffer(S->ch, S->length, P->ch, P->length, pos);
}
int IndexAll_KMP_H(const HString *S, const HString *P, int pos,
				   int *out, int cap, int allowOverlap) {
	return IndexAll_KMP_Buffer(S->ch, S->length, P->ch, P->length, pos,
							   out, cap, allowOverlap);
}

/*
函数名: Index_BF_S
作  用: 在定长串 S 中用 BF 查找模式 P 的首次出现位置（1 基）。
参  数: S - 文本；P - 模式；pos - 起始检查位置（1 基）
返  回: 首次匹配位置（1 基）；未找到返回 0
*/
int Index_BF_S(const SString *S, const SString *P, int pos) {
    return Index_BF_Buffer(S->ch, S->length, P->ch, P->length, pos);
}

/*
函数名: IndexAll_BF_S
作  用: 在定长串 S 中用 BF 查找模式 P 的所有出现位置（1 基）。
参  数: S - 文本；P - 模式；pos - 起始检查位置（1 基）；
		out - 输出位置数组（1 基）；cap - 数组容量；allowOverlap - 是否允许重叠。
返  回: 实际写入 out 的元素个数（可能 <= cap）。
*/
int IndexAll_BF_S(const SString *S, const SString *P, int pos,
				  int *out, int cap, int allowOverlap) {
	return IndexAll_BF_Buffer(S->ch, S->length, P->ch, P->length, pos,
							  out, cap, allowOverlap);
}

/*
函数名: Index_BF_H
作  用: 在堆分配串 S 中用 BF 查找模式 P 的首次出现位置（1 基）。
参  数: S - 文本；P - 模式；pos - 起始检查位置（1 基）
返  回: 首次匹配位置（1 基）；未找到返回 0
*/
int Index_BF_H(const HString *S, const HString *P, int pos) {
    return Index_BF_Buffer(S->ch, S->length, P->ch, P->length, pos);
}

/*
函数名: IndexAll_BF_H
作  用: 在堆分配串 S 中用 BF 查找模式 P 的所有出现位置（1 基）。
参  数: S - 文本；P - 模式；pos - 起始检查位置（1 基）；
		out - 输出位置数组（1 基）；cap - 数组容量；allowOverlap - 是否允许重叠。
返  回: 实际写入 out 的元素个数（可能 <= cap）。
*/
int IndexAll_BF_H(const HString *S, const HString *P, int pos,
				  int *out, int cap, int allowOverlap) {
	return IndexAll_BF_Buffer(S->ch, S->length, P->ch, P->length, pos,
							  out, cap, allowOverlap);
}

// ---------------- 测试 ----------------
/*
函数名: main
作  用: 演示定长串/堆串/块链串的赋值、联结、子串与 BF 匹配，以及 StrLength 测试。
参  数: 无
返  回: 程序退出码
*/
int main(void) {
	// 定长顺序存储：赋值、联结、子串
	SString A = {{0},0}, B = {{0},0}, C = {{0},0}, Sub = {{0},0};
	StrAssign_S(&A, "hello");
	StrAssign_S(&B, " world");
	int ok_concat = Concat_S(&A, &B, &C);
	printf("[fixed] A = \""); PrintS(&A); printf("\"\n");
	printf("[fixed] B = \""); PrintS(&B); printf("\"\n");
	printf("[fixed] C = A+B = \""); PrintS(&C); printf("\"  (complete=%s)\n", ok_concat?"yes":"no-truncated");
	if (SubString_S(&A, 2, 3, &Sub)) { // 从第2个字符开始，长度3 => "ell"
		printf("[fixed] Sub = SubString(A, pos=2, len=3) = \""); PrintS(&Sub); printf("\"\n");
	}

	// 堆分配：赋值、联结
	HString HA = {NULL,0}, HB = {NULL,0}, HC = {NULL,0};
	StrAssign_H(&HA, "data");
	StrAssign_H(&HB, "struct");
	Concat_H(&HC, &HA, &HB);
	printf("[heap] HA = \""); PrintH(&HA); printf("\"\n");
	printf("[heap] HB = \""); PrintH(&HB); printf("\"\n");
	printf("[heap] HC = HA+HB = \""); PrintH(&HC); printf("\"\n");

	// BF 模式匹配（SString）
	SString TS = {{0},0}, PS = {{0},0};
	StrAssign_S(&TS, "ababcabcacbab");
	StrAssign_S(&PS, "abcac");
	int posS = Index_BF_S(&TS, &PS, 1);
	printf("\n[BF] SString: T=\""); PrintS(&TS); printf("\"  P=\""); PrintS(&PS); printf("\"  -> pos=%d\n", posS);
	int posS_kmp = Index_KMP_S(&TS, &PS, 1);
	printf("[KMP] SString: T=\""); PrintS(&TS); printf("\"  P=\""); PrintS(&PS); printf("\"  -> pos=%d\n", posS_kmp);

	// BF 模式匹配（HString）
	HString TH = {NULL,0}, PH = {NULL,0};
	StrAssign_H(&TH, "aaaaab");
	StrAssign_H(&PH, "aaab");
	int posH = Index_BF_H(&TH, &PH, 1);
	printf("[BF] HString: T=\""); PrintH(&TH); printf("\"  P=\""); PrintH(&PH); printf("\"  -> pos=%d\n", posH);
	int posH_kmp = Index_KMP_H(&TH, &PH, 1);
	printf("[KMP] HString: T=\""); PrintH(&TH); printf("\"  P=\""); PrintH(&PH); printf("\"  -> pos=%d\n", posH_kmp);

	// BF 全部匹配（允许重叠 vs 不允许重叠）示例
	SString TAll = {{0},0}, PAll = {{0},0};
	StrAssign_S(&TAll, "aaaaa");
	StrAssign_S(&PAll, "aaa");
	int out1[8] = {0}, out2[8] = {0};
	int c1 = IndexAll_BF_S(&TAll, &PAll, 1, out1, 8, 1); // 允许重叠
	int c2 = IndexAll_BF_S(&TAll, &PAll, 1, out2, 8, 0); // 不允许重叠
	printf("\n[BF-all] SString: T=\""); PrintS(&TAll); printf("\"  P=\""); PrintS(&PAll); printf("\"\n");
	printf("  allowOverlap=1  count=%d  pos:", c1);
	for (int i = 0; i < c1; ++i) printf(" %d", out1[i]);
	printf("\n");
	printf("  allowOverlap=0  count=%d  pos:", c2);
	for (int i = 0; i < c2; ++i) printf(" %d", out2[i]);
	printf("\n");

	// KMP 全部匹配（同一案例）
	int out3[8] = {0}, out4[8] = {0};
	int k1 = IndexAll_KMP_S(&TAll, &PAll, 1, out3, 8, 1);
	int k2 = IndexAll_KMP_S(&TAll, &PAll, 1, out4, 8, 0);
	printf("\n[KMP-all] SString: T=\""); PrintS(&TAll); printf("\"  P=\""); PrintS(&PAll); printf("\"\n");
	printf("  allowOverlap=1  count=%d  pos:", k1);
	for (int i = 0; i < k1; ++i) printf(" %d", out3[i]);
	printf("\n");
	printf("  allowOverlap=0  count=%d  pos:", k2);
	for (int i = 0; i < k2; ++i) printf(" %d", out4[i]);
	printf("\n");

	// 块链串演示
	LString LS; StrAssign_L(&LS, "chunked-string");
	printf("\n[chunked] LS = \""); PrintL(&LS); printf("\"  (len=%d, CHUNKSIZE=%d)\n", LS.length, CHUNKSIZE);

    /* 测试 StrLength */
    {
        const char *cases[] = {"", "a", "hello", "测试", "long string with spaces"};
        int nc = sizeof(cases) / sizeof(cases[0]);
        for (int i = 0; i < nc; ++i) {
            printf("case %d: \"%s\" -> StrLength=%d, strlen=%zu\n",
                   i, cases[i], StrLength(cases[i]), strlen(cases[i]));
        }
    }

	Destroy_H(&HA); Destroy_H(&HB); Destroy_H(&HC);
	Destroy_H(&TH); Destroy_H(&PH);
	Destroy_L(&LS);
	return 0;
}

/*
函数名: StrLength
作  用: 计算以'\0'结尾的 C 字符串长度（教学用，自实现版 strlen）。
参  数: s - C 字符串指针
返  回: 字符串长度（int）
复  杂: 时间 O(n)，空间 O(1)
注意事项: 不对 NULL 做健壮检查；需确保 s 指向以'\0'终止的有效内存。
*/
int StrLength(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return (int)(p - s);
}
