/*
二叉树两种存储结构示例：
- 顺序存储（数组，下标从 1 开始，i 的左右孩子分别是 2i 与 2i+1）
- 链式存储（二叉链表）
包含：构造、遍历、统计、序列化/反序列化、广义表输出与直接求高度等。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*======================
= 顺序存储：数组表示 =
=======================*/

typedef struct
{
    int capacity; // 容量：可用下标范围 1..capacity（data[0] 空置）
    char *data;   // 结点值数组；空结点用 '\0'
} SeqBiTree;

#define SBT_EMPTY '\0'

/*
函数：SBT_create
作用：创建容量为 cap 的顺序二叉树并置空
参数：cap - 预分配的最大下标（>=1）
返回：成功返回堆上对象指针，失败返回 NULL
复杂度：O(cap)（memset 置零）
注意：data[0] 未使用，实际可用下标 1..cap
*/
SeqBiTree *SBT_create(int cap)
{
    if (cap < 1)
        cap = 1;
    SeqBiTree *t = (SeqBiTree *)malloc(sizeof(SeqBiTree));
    if (!t)
        return NULL;
    t->capacity = cap;
    t->data = (char *)malloc((size_t)(cap + 1)); // data[0] 弃用,因为char类型占1字节,避免乘 sizeof(char)
    if (!t->data)
    {
        free(t);
        return NULL;
    }
    memset(t->data, 0, (size_t)(cap + 1)); // 用 memset 从 t->data 起始位置开始，把连续 cap + 1 个字节全部写成 0。
    // “memset” 是 C 标准库 <string.h> 中的内存填充函数，用来把一段连续内存按“字节”写成同一个值。
    // void *memset(void *s, int c, size_t n);
    /*s: 起始内存地址
      c: 要写入的字节值（会转为 unsigned char）
      n: 要写入的字节数*/
    return t;
}

/*
函数：SBT_destroy
作用：释放顺序二叉树对象与数组
参数：t - 树指针（可为 NULL）
返回：无
*/
void SBT_destroy(SeqBiTree *t)
{
    if (!t)
        return;
    free(t->data);
    free(t);
}

/*
函数：SBT_inRange（内部工具）
作用：判断下标 idx 是否在有效区间 1..capacity
参数：t - 树；idx - 下标
返回：1/0
*/
static int SBT_inRange(const SeqBiTree *t, int idx)
{
    return t && idx >= 1 && idx <= t->capacity;
}

/*
函数：SBT_parent / SBT_left / SBT_right
作用：返回父结点或左右孩子的“下标”；越界返回 0
参数：t - 树；i - 当前结点下标
返回：对应下标或 0
备注：仅计算下标，不保证该位置非空
*/
int SBT_parent(const SeqBiTree *t, int i) { return (i > 1 && SBT_inRange(t, i)) ? (i / 2) : 0; }
int SBT_left(const SeqBiTree *t, int i)
{
    int p = i * 2;
    return (SBT_inRange(t, i) && p <= t->capacity) ? p : 0;
}
int SBT_right(const SeqBiTree *t, int i)
{
    int p = i * 2 + 1;
    return (SBT_inRange(t, i) && p <= t->capacity) ? p : 0;
}

/*
函数：SBT_set / SBT_get
作用：设置/获取指定下标的结点值
参数：idx - 目标下标；val - 新值（'\0' 表示空）
返回：SBT_set 成功 1/失败 0；SBT_get 越界返回 SBT_EMPTY
*/
int SBT_set(SeqBiTree *t, int idx, char val)
{
    if (!SBT_inRange(t, idx))
        return 0;
    t->data[idx] = val;
    return 1;
}
char SBT_get(const SeqBiTree *t, int idx)
{
    if (!SBT_inRange(t, idx))
        return SBT_EMPTY;
    return t->data[idx];
}

/*
函数：SBT_clear
作用：清空整棵顺序二叉树为“空结点”
复杂度：O(capacity)
*/
void SBT_clear(SeqBiTree *t)
{
    if (!t)
        return;
    memset(t->data, 0, (size_t)(t->capacity + 1));
}

/*
函数：SBT_buildExample
作用：根据讲义图片构造固定示例：
            +
        *       /
      a   b   -   e
               c   d
*/
void SBT_buildExample(SeqBiTree *t)
{
    if (!t)
        return;
    SBT_clear(t);
    SBT_set(t, 1, '+');
    SBT_set(t, 2, '*');
    SBT_set(t, 3, '/');
    SBT_set(t, 4, 'a');
    SBT_set(t, 5, 'b');
    SBT_set(t, 6, '-');
    SBT_set(t, 7, 'e');
    SBT_set(t, 12, 'c');
    SBT_set(t, 13, 'd');
}

/*
函数：SBT_printTable
作用：按“索引行 + 值行”形式打印数组（空位用 .）
*/
void SBT_printTable(const SeqBiTree *t)
{
    if (!t)
        return;
    printf("Index: ");
    for (int i = 1; i <= t->capacity; ++i)
        printf("%2d ", i);
    printf("\nValue: ");
    for (int i = 1; i <= t->capacity; ++i)
    {
        char v = t->data[i];
        printf(" %c ", v ? v : '.');
    }
    printf("\n\n");
}

/*
函数：SBT_printLevels
作用：按层打印，每层个数翻倍（1,2,4,...），便于查看层次
*/
void SBT_printLevels(const SeqBiTree *t)
{
    if (!t)
        return;
    int level = 1, i = 1, cnt = 1;
    while (i <= t->capacity)
    {
        printf("Level %d: ", level);
        for (int k = 0; k < cnt && i <= t->capacity; ++k, ++i)
            printf("%c ", t->data[i] ? t->data[i] : '.');
        printf("\n");
        cnt <<= 1;
        ++level;
        /*<<= 是“左移并赋值”运算符，cnt <<= 1 等价于 cnt = cnt << 1。
        对无符号或正的整型值，左移 1 位就是乘以 2；左移 n 位相当于乘 2^n。
        二进制左移就是在末尾补 0。等价于把数在二进制位权上整体“向高位移动”，每移一位，权值都乘 2，所以数值也乘 2。
        本函数用它来实现“每层个数翻倍”的逻辑。*/
    }
    printf("\n");
}

/*
函数：SBT_printLeaves
作用：扫描所有非空下标，若左右孩子都为空则视为叶子并输出
      就是把顺序存储二叉树里的“叶子结点”依次输出。
*/
void SBT_printLeaves(const SeqBiTree *t)
{
    if (!t)
        return;
    printf("Leaves: ");
    for (int i = 1; i <= t->capacity; ++i)
    {
        char v = t->data[i];
        if (!v)
            continue;
        int L = SBT_left(t, i), R = SBT_right(t, i);
        char lv = (L ? t->data[L] : SBT_EMPTY);
        char rv = (R ? t->data[R] : SBT_EMPTY);
        if (!lv && !rv)
            printf("%c ", v);
    }
    printf("\n\n");
}

/*==========================
= 链式存储：二叉链表表示 =
==========================*/

typedef struct Tnode
{
    char data;         // 结点值
    struct Tnode *lc;  // 左孩子
    struct Tnode *rc;  // 右孩子
    struct Tnode *pre; // 先序后继线索（preorder successor thread），不占用左右孩子，一般二叉链表不需要这一项
} Tnode, *Tree;

/*
函数：BT_new
作用：创建一个值为 v 的结点，左右孩子置空
返回：结点指针或 NULL
*/
static Tnode *BT_new(char v)
{
    Tnode *p = (Tnode *)malloc(sizeof(Tnode));
    if (!p)
        return NULL;
    p->data = v;
    p->lc = p->rc = NULL;
    p->pre = NULL;
    return p;
}

/*
函数：BT_destroy
作用：释放整棵链式二叉树（后序递归）
*/
void BT_destroy(Tree t)
{
    if (!t)
        return;
    BT_destroy(t->lc);
    BT_destroy(t->rc);
    free(t);
}

/*
函数：BT_count
作用：统计结点总数
返回：结点个数
复杂度：O(n)
*/
int BT_count(Tree t)
{
    if (!t)
        return 0;
    return 1 + BT_count(t->lc) + BT_count(t->rc);
}

/*
函数：BT_countLeaves
作用：统计叶子结点数（左右孩子都为空）
*/
int BT_countLeaves(Tree t)
{
    if (!t)
        return 0;
    if (!t->lc && !t->rc)
        return 1;
    return BT_countLeaves(t->lc) + BT_countLeaves(t->rc);
}

/*
函数：BT_countLeaves_iter
作用：非递归统计叶子数（使用简单栈），与 BT_countLeaves 结果一致。
理由：展示迭代写法，避免极端退化树造成递归栈过深。
*/
int BT_countLeaves_iter(Tree t)
{
    if (!t)
        return 0;
    // 复用前面链栈实现
    LStack *st = LST_new();
    if (!st)
        return 0;
    int cnt = 0;
    LST_push(st, t);
    while (!LST_empty(st))
    {
        Tnode *p = LST_pop(st);
        if (!p->lc && !p->rc)
            ++cnt;
        if (p->lc)
            LST_push(st, p->lc);
        if (p->rc)
            LST_push(st, p->rc);
    }
    LST_del(st);
    return cnt;
}

/*
函数：BT_swapRoot
作用：仅交换根结点的左、右子树（不递归），若 t 为空无操作。
*/
void BT_swapRoot(Tree t)
{
    if (!t)
        return;
    Tnode *tmp = t->lc;
    t->lc = t->rc;
    t->rc = tmp;
}

/*
函数：BT_swapAll
作用：递归交换整棵树所有结点的左右孩子（镜像）。
*/
void BT_swapAll(Tree t)
{
    if (!t)
        return;
    Tnode *tmp = t->lc;
    t->lc = t->rc;
    t->rc = tmp;
    BT_swapAll(t->lc);
    BT_swapAll(t->rc);
}

/*
函数：BT_height
作用：求树高（空树为 0）
定义：高度 = 根到最深叶子的结点数
*/
int BT_height(Tree t)
{
    if (!t)
        return 0;
    int hl = BT_height(t->lc);
    int hr = BT_height(t->rc);
    return (hl > hr ? hl : hr) + 1;
}

/*
函数：BT_preorder / BT_inorder / BT_postorder
作用：先/中/后序遍历并输出结点字符（空格分隔）
*/
void BT_preorder(Tree t)
{
    if (!t)
        return;
    putchar(t->data);
    putchar(' ');
    BT_preorder(t->lc);
    BT_preorder(t->rc);
}
void BT_inorder(Tree t)
{
    if (!t)
        return;
    BT_inorder(t->lc);
    putchar(t->data);
    putchar(' ');
    BT_inorder(t->rc);
}
void BT_postorder(Tree t)
{
    if (!t)
        return;
    BT_postorder(t->lc);
    BT_postorder(t->rc);
    putchar(t->data);
    putchar(' ');
}

/* 简单循环队列用于层序遍历 */
typedef struct
{
    Tnode **a;
    int head, tail, cap;
} Q;

/*
Q_new/Q_del/Q_empty/Q_push/Q_pop
作用：最小队列实现（不做溢出检查，容量由调用者保证足够）
*/
static Q *Q_new(int cap)
{
    Q *q = (Q *)malloc(sizeof(Q));
    if (!q)
        return NULL;
    q->a = (Tnode **)malloc(sizeof(Tnode *) * cap);
    if (!q->a)
    {
        free(q);
        return NULL;
    }
    q->head = q->tail = 0;
    q->cap = cap;
    return q;
}
static void Q_del(Q *q)
{
    if (!q)
        return;
    free(q->a);
    free(q);
}
static int Q_empty(Q *q) { return q->head == q->tail; }
static void Q_push(Q *q, Tnode *x)
{
    if (!x)
        return;
    q->a[q->tail++ % q->cap] = x;
}
static Tnode *Q_pop(Q *q)
{
    if (Q_empty(q))
        return NULL;
    return q->a[q->head++ % q->cap];
}

/*
函数：BT_levelorder
作用：层序遍历并输出
实现：队列入根结点，循环弹出，再把左右孩子入队
*/
void BT_levelorder(Tree t)
{
    if (!t)
        return;
    int cnt = BT_count(t);     // 简单估计队列容量
    Q *q = Q_new(cnt * 2 + 1); // 留冗余避免环绕覆盖
    if (!q)
        return;
    Q_push(q, t);
    while (!Q_empty(q))
    {
        Tnode *p = Q_pop(q);
        if (!p)
            continue;
        putchar(p->data);
        putchar(' ');
        if (p->lc)
            Q_push(q, p->lc);
        if (p->rc)
            Q_push(q, p->rc);
    }
    Q_del(q);
}

/*---------- 非递归遍历所需：链栈（存放结点指针） ----------*/
/*
链栈用于模拟递归调用栈，保存“将来要回来的结点”。
选择链式结构（单链表）而不是顺序栈（数组）的好处：
    - 无需预估容量，结点多少就分配多少；
    - push/pop 皆为 O(1)；
    - 每个栈元素仅保存一个 Tnode* 指针，内存占用低。
*/
typedef struct LSNode
{
    Tnode *x;
    struct LSNode *next;
} LSNode;
typedef struct
{
    LSNode *top;
} LStack;

// 新建一个空栈：top 置为 NULL
static LStack *LST_new(void)
{
    LStack *s = (LStack *)malloc(sizeof(LStack));
    if (!s)
        return NULL;
    s->top = NULL;
    return s;
}
// 判空：栈不存在或 top==NULL 视为空
static int LST_empty(LStack *s) { return !s || s->top == NULL; }
// 入栈：头插到单链表（新结点成为新的 top）
static void LST_push(LStack *s, Tnode *x)
{
    if (!s || !x)
        return;
    LSNode *n = (LSNode *)malloc(sizeof(LSNode));
    if (!n)
        return;
    n->x = x;             // 保存要回到的树结点
    n->next = s->top;     // 新结点指向原栈顶
    s->top = n;           // 更新栈顶
}
// 出栈：弹出并返回当前栈顶保存的树结点指针
static Tnode *LST_pop(LStack *s)
{
    if (LST_empty(s))
        return NULL;
    LSNode *n = s->top;
    Tnode *x = n->x;
    s->top = n->next;     // 栈顶下移
    free(n);
    return x;
}
// 取栈顶但不弹出（用于观察下一步该处理的结点）
static Tnode *LST_peek(LStack *s) { return LST_empty(s) ? NULL : s->top->x; }
// 释放整个栈（逐个 free 结点）
static void LST_del(LStack *s)
{
    if (!s)
        return;
    while (s->top)
    {
        LSNode *n = s->top;
        s->top = n->next; // 下移并释放
        free(n);
    }
    free(s);
}

/*
先序（非递归）：根-左-右
一路向左：访问并压栈，回溯改走右。
*/
void BT_preorder_nonrec(Tree t)
{
    if (!t)
    {
        return;
    }
    LStack *st = LST_new();   // 辅助栈：记录回溯路径
    Tnode *p = t;             // 游标：沿着当前分支向下走
    while (p || !LST_empty(st))
    {
        // 一路向左：访问根 -> 压栈 -> 转到左孩子
        while (p)
        {
            putchar(p->data); // 先序：此刻访问根
            putchar(' ');
            LST_push(st, p);  // 将当前结点压栈，回头处理它的右子树
            p = p->lc;        // 继续深入到最左
        }
        // 走到左端后：回溯一个结点，改走其右子树
        p = LST_pop(st);
        p = p ? p->rc : NULL;
    }
    LST_del(st);
}

/*
中序（非递归）：左-根-右
一路向左压栈；回溯输出，再向右。
*/
void BT_inorder_nonrec(Tree t)
{
    if (!t)
    {
        return;
    }
    LStack *st = LST_new();   // 辅助栈
    Tnode *p = t;             // 游标
    while (p || !LST_empty(st))
    {
        // 一路向左：仅压栈不访问（中序要等左子树处理完再访问根）
        while (p)
        {
            LST_push(st, p);
            p = p->lc;
        }
        // 回溯一个结点：此时其左子树已处理，访问根
        p = LST_pop(st);
        putchar(p->data);
        putchar(' ');
        // 转向右子树，重复同样过程
        p = p->rc;
    }
    LST_del(st);
}

/*
后序（非递归）：左-右-根
单栈 + last 指针：当右子未处理时转向右，否则输出根。
*/
void BT_postorder_nonrec(Tree t)
{
    if (!t)
    {
        return;
    }
    LStack *st = LST_new();
    Tnode *p = t, *last = NULL; // last 记录“上一次被访问（输出）的结点”
    while (p || !LST_empty(st))
    {
        if (p)
        {
            // 先尽量走到最左：沿路压栈
            LST_push(st, p);
            p = p->lc;
        }
        else
        {
            // 查看栈顶但先不弹：决定是该转向右子，还是可以输出栈顶
            Tnode *node = LST_peek(st);
            // 若存在右子且右子尚未被处理，则转向右子
            if (node->rc && last != node->rc)
            {
                p = node->rc;
            }
            else
            {
                // 否则（无右子或右子处理过）即可输出栈顶作为“根”
                putchar(node->data);
                putchar(' ');
                last = node;     // 记录已处理的最后结点
                LST_pop(st);     // 弹出栈顶，继续回溯
            }
        }
    }
    LST_del(st);
}

/*
函数：BT_findParent
作用：按结点指针 x 查找其双亲；若 x 为根或不存在则返回 NULL。
方法：递归：若 root->lc==x 或 root->rc==x 则 root 为双亲；否则向左右子树继续查找。
复杂度：O(n) 最坏需要遍历整棵树。
*/
Tnode *BT_findParent(Tree root, Tnode *x)
{
    if (!root || !x || root == x)
        return NULL;
    if (root->lc == x || root->rc == x)
        return root;
    Tnode *p = BT_findParent(root->lc, x);
    if (p)
        return p;
    return BT_findParent(root->rc, x);
}

/*
函数：BT_findParentByValue
作用：按结点数据值 v 查找其双亲；若对应结点为根或不存在则返回 NULL。
假设结点值不唯一时，将返回“先序遍历遇到的第一个匹配”结点的双亲。
*/
Tnode *BT_findParentByValue(Tree root, char v)
{
    if (!root)
        return NULL;
    // 若任一直接孩子匹配，root 即为双亲
    if ((root->lc && root->lc->data == v) || (root->rc && root->rc->data == v))
        return root;
    Tnode *p = BT_findParentByValue(root->lc, v);
    if (p)
        return p;
    return BT_findParentByValue(root->rc, v);
}

/*
函数：BT_findParent_iter
作用：非递归按指针查找双亲，使用栈模拟先序。
*/
Tnode *BT_findParent_iter(Tree root, Tnode *x)
{
    if (!root || !x || root == x)
        return NULL;
    LStack *st = LST_new();
    if (!st)
        return NULL;
    Tnode *p = root;
    while (p || !LST_empty(st))
    {
        while (p)
        {
            if (p->lc == x || p->rc == x)
            {
                LST_del(st);
                return p;
            }
            LST_push(st, p);
            p = p->lc;
        }
        p = LST_pop(st);
        p = p ? p->rc : NULL;
    }
    LST_del(st);
    return NULL;
}

/*
函数：BT_buildExample
作用：按示例构造一棵链式表达式树（与顺序存储示例一致）
*/
Tree BT_buildExample(void)
{
    Tnode *plus = BT_new('+');
    Tnode *mul = BT_new('*');
    Tnode *divv = BT_new('/');
    Tnode *a = BT_new('a');
    Tnode *b = BT_new('b');
    Tnode *sub = BT_new('-');
    Tnode *e = BT_new('e');
    Tnode *c = BT_new('c');
    Tnode *d = BT_new('d');
    if (!plus || !mul || !divv || !a || !b || !sub || !e || !c || !d)
        return plus; // 省略容错
    plus->lc = mul;
    plus->rc = divv;
    mul->lc = a;
    mul->rc = b;
    divv->lc = sub;
    divv->rc = e;
    sub->lc = c;
    sub->rc = d;
    return plus;
}

/*---------- 从标准输入按先序读取并建树（# 表示空） ----------*/

/*
辅助：读取输入流中的下一个“非空白字符”。
返回该字符（unsigned char 范围内的 int）或 EOF。
*/
static int BT_getNonSpace(FILE *in)
{
    int ch;
    do
    {
        ch = fgetc(in);
    } while (ch != EOF && isspace((unsigned char)ch));
    return ch;
}

/*
递归：按“先序 + # 表示空”从流中读取并构造二叉链表。
允许输入中有空格/换行，会自动跳过。
例如以下三种输入等价：
  AB##C##
  A B # # C # #
  A  \nB  #  #  C  #  #
*/
static Tree BT_buildPreorderFromStream(FILE *in)
{
    int ch = BT_getNonSpace(in);
    if (ch == EOF)
        return NULL; // 输入耗尽：按空树处理
    if (ch == '#')
        return NULL; // 空结点
    Tnode *p = BT_new((char)ch);
    if (!p)
        return NULL;
    p->lc = BT_buildPreorderFromStream(in);
    p->rc = BT_buildPreorderFromStream(in);
    return p;
}

/*
对外：从标准输入读取（先序 + #）并返回构造好的树。
*/
Tree BT_buildFromPreorderInput(void)
{
    return BT_buildPreorderFromStream(stdin);
}

/*---------- 序列化（先序 + '#' 为空） ----------*/

/*
内部递归：BT_serRec
作用：把树按“先序 + 空指针用 #”写入 out
参数：cap - 缓冲区容量；idx - 当前写入位置（引用）
注意：简单防溢出（保证至少还能写 1 个字符）
*/
static void BT_serRec(Tree t, char *out, size_t cap, size_t *idx)
{
    if (!out || !idx)
        return;
    if (*idx + 1 >= cap)
        return;
    if (!t)
    {
        out[(*idx)++] = '#';
        return;
    }
    out[(*idx)++] = t->data;         // 根
    BT_serRec(t->lc, out, cap, idx); // 左
    BT_serRec(t->rc, out, cap, idx); // 右
}

/*
函数：BT_serializePreorder
作用：先序+# 序列化到 str（以 '\0' 结尾）
返回：写入的有效字符数（不含 '\0'）
*/
size_t BT_serializePreorder(Tree t, char *str, size_t cap)
{
    if (!str || cap == 0)
        return 0;
    size_t idx = 0;
    BT_serRec(t, str, cap, &idx);
    if (idx >= cap)
        idx = cap - 1;
    str[idx] = '\0';
    return idx;
}

/*---------- 非递归遍历所需：链栈（存放结点指针） ----------*/
/*
链栈 = 用单链表实现的“后进先出”结构，用于替代顺序栈（动态数组）。
为什么在这里用链栈？
  - 非递归遍历过程中，入栈/出栈次数和树大小相关，但不需要预估容量；
  - 链栈按需分配节点，没有扩容开销，也不受固定容量限制；
  - 每个栈元素仅保存“结点指针（Tnode*）”，避免大对象拷贝。

结构约定：
  - LSNode：链栈的结点，保存一个树结点指针 x，以及指向下一个栈结点的 next；
  - LStack：栈头结构，只维护一个 top 指针（指向栈顶 LSNode）。

时间复杂度：
  - push / pop / peek / empty 均为 O(1)。
内存管理：
  - 每次 push 会 malloc 一个 LSNode；对应地，pop 或 del 会 free 这些节点，避免泄漏。
*/

/*
函数：BT_buildFromString
作用：根据广义表串构造链式二叉树
规则：字母=结点； '(' 入栈并转到“左”； ',' 切换“右”； ')' 出栈；空白忽略
例子：A(B(a,b),C(D(d,),c))
实现：使用父结点栈 + 标志位（1=连左，2=连右）
*/
Tree BT_buildFromString(const char *str)
{
    if (!str)
        return NULL;
    size_t n = strlen(str);
    if (n == 0)
        return NULL;

    Tnode **stack = (Tnode **)malloc(sizeof(Tnode *) * (n + 1));
    int top = -1;
    int flag = 0; // 1: 左孩子；2: 右孩子
    Tree root = NULL;
    Tnode *p = NULL; // 最近创建结点

    for (size_t i = 0; i < n; ++i)
    {
        char ch = str[i];
        if (isspace((unsigned char)ch))
            continue;
        if (isalpha((unsigned char)ch))
        {
            p = BT_new(ch);
            if (!root)
                root = p;
            else if (top >= 0)
            {
                if (flag == 1)
                    stack[top]->lc = p;
                else if (flag == 2)
                    stack[top]->rc = p;
            }
        }
        else if (ch == '(')
        {
            if (p)
                stack[++top] = p; // p 成为父结点
            flag = 1;             // 接下来连左
        }
        else if (ch == ',')
        {
            flag = 2; // 切换连右
        }
        else if (ch == ')')
        {
            if (top >= 0)
                --top; // 子树结束
        }
        else
        {
            // 其他字符忽略
        }
    }

    free(stack);
    return root;
}

/*---------- 先序后继线索构建与按线索遍历 ----------*/

// 清空先序线索域（防止旧线索残留）
void BT_clearPreThread(Tree t)
{
    if (!t)
        return;
    t->pre = NULL;
    BT_clearPreThread(t->lc);
    BT_clearPreThread(t->rc);
}

static void BT_threadPreRec(Tree t, Tnode **pprev)
{
    if (!t)
        return;
    if (*pprev)
        (*pprev)->pre = t; // 将前驱的线索指向当前
    *pprev = t;            // 更新前驱
    BT_threadPreRec(t->lc, pprev);
    BT_threadPreRec(t->rc, pprev);
}

// 构造先序后继线索：按照先序遍历把每个结点的 pre 指向其先序后继
void BT_buildPreorderThread(Tree t)
{
    BT_clearPreThread(t);
    Tnode *prev = NULL;
    BT_threadPreRec(t, &prev);
}

// 仅沿 pre 线索遍历输出（不访问左右孩子）
void BT_traversePreThread(Tree t)
{
    for (Tnode *p = t; p; p = p->pre)
    {
        putchar(p->data);
        putchar(' ');
    }
}

/*---------- 输出为广义表字符串 ----------*/

/*
内部递归：BT_genRec
作用：把链式树输出为广义表（形如 A(B( , ),C( , ))）
要点：有孩子时输出 '(' left ',' right ')'；缺孩子则对应位置留空
*/
static void BT_genRec(Tree t, char *out, size_t cap, size_t *idx)
{
    if (!t || !out || !idx)
        return;
    if (*idx + 1 < cap)
        out[(*idx)++] = t->data; // 根
    if (t->lc || t->rc)
    {
        if (*idx + 1 < cap)
            out[(*idx)++] = '(';
        if (t->lc)
            BT_genRec(t->lc, out, cap, idx); // 左（可能为空）
        if (*idx + 1 < cap)
            out[(*idx)++] = ',';
        if (t->rc)
            BT_genRec(t->rc, out, cap, idx); // 右（可能为空）
        if (*idx + 1 < cap)
            out[(*idx)++] = ')';
    }
}

/*
函数：BT_toGeneralizedString
作用：把二叉链表转为广义表字符串（以 '\0' 结束）
返回：写入的字符数（不含 '\0'）
*/
size_t BT_toGeneralizedString(Tree t, char *str, size_t cap)
{
    if (!str || cap == 0)
        return 0;
    size_t idx = 0;
    if (t)
        BT_genRec(t, str, cap, &idx);
    if (idx >= cap)
        idx = cap - 1;
    str[idx] = '\0';
    return idx;
}

/*---------- 直接从字符串求高度 ----------*/

/*
函数：BT_heightFromString
作用：不建树，直接从广义表串计算树的高度
方法：open 记录当前括号嵌套层数；遇到字母则深度=open+1，取最大值
*/
int BT_heightFromString(const char *str)
{
    if (!str)
        return 0;
    int open = 0, maxh = 0;
    for (const char *p = str; *p; ++p)
    {
        unsigned char ch = (unsigned char)*p;
        if (isspace(ch))
            continue;
        if (isalpha(ch))
        {
            int h = open + 1;
            if (h > maxh)
                maxh = h;
        }
        else if (*p == '(')
        {
            ++open;
        }
        else if (*p == ')')
        {
            if (open > 0)
                --open;
        }
        else
        { /* 逗号等忽略 */
        }
    }
    return maxh;
}

/*=================
= 演示主函数 main =
==================*/
int main(void)
{
    /* --- 顺序存储演示 --- */
    SeqBiTree *T = SBT_create(15); // 4 层满二叉树最大下标=15
    if (!T)
    {
        fprintf(stderr, "内存分配失败\n");
        return 1;
    }
    SBT_buildExample(T);
    printf("顺序存储数组（与图片示例一致）：\n");
    SBT_printTable(T);
    printf("按层打印：\n");
    SBT_printLevels(T);
    SBT_printLeaves(T);

    /* --- 二叉链表演示 --- */
    Tree R = BT_buildExample();
    printf("二叉链表遍历：\n");
    printf("先序(递归): ");
    BT_preorder(R);
    printf("\n");
    printf("中序(递归): ");
    BT_inorder(R);
    printf("\n");
    printf("后序(递归): ");
    BT_postorder(R);
    printf("\n");
    printf("层序:       ");
    BT_levelorder(R);
    printf("\n");

    /* 新增：非递归遍历测试 */
    printf("先序(非递归): ");
    BT_preorder_nonrec(R);
    printf("\n");
    printf("中序(非递归): ");
    BT_inorder_nonrec(R);
    printf("\n");
    printf("后序(非递归): ");
    BT_postorder_nonrec(R);
    printf("\n");

    printf("结点数=%d, 叶子数=%d, 高度=%d\n",
           BT_count(R), BT_countLeaves(R), BT_height(R));

    /* --- 构造先序后继线索并按线索遍历 --- */
    BT_buildPreorderThread(R);
    printf("先序(线索):   ");
    BT_traversePreThread(R);
    printf("\n\n");

    /* --- 先序+# 序列化 --- */
    int n = BT_count(R);
    size_t cap = (size_t)(2 * n + 1 + 1); // 上界 2n+1，再 +1 存 '\0'
    char *str = (char *)malloc(cap);
    if (str)
    {
        BT_serializePreorder(R, str, cap);
        printf("先序序列化(空为#): %s\n\n", str);
        free(str);
    }

    /* --- 按广义表串构造并测试 --- */
    const char *gstr = "A(B(a,b),C(D(d,),c))";
    Tree T2 = BT_buildFromString(gstr);
    printf("由串构造的树 遍历：\n");
    printf("先序: ");
    BT_preorder(T2);
    printf("\n");
    printf("中序: ");
    BT_inorder(T2);
    printf("\n");
    printf("后序: ");
    BT_postorder(T2);
    printf("\n");
    printf("层序: ");
    BT_levelorder(T2);
    printf("\n");
    printf("结点数=%d, 叶子数=%d, 高度=%d\n", BT_count(T2), BT_countLeaves(T2), BT_height(T2));

    /* 对由串构造的树也生成并测试线索 */
    BT_buildPreorderThread(T2);
    printf("T2 先序(线索): ");
    BT_traversePreThread(T2);
    printf("\n\n");

    /* --- 链式树输出为广义表 --- */
    size_t cap2 = (size_t)(4 * n + 8);
    char *g1 = (char *)malloc(cap2);
    char *g2 = (char *)malloc(cap2);
    if (g1)
    {
        BT_toGeneralizedString(R, g1, cap2);
        printf("链表树R的广义表: %s\n", g1);
        free(g1);
    }
    if (g2)
    {
        BT_toGeneralizedString(T2, g2, cap2);
        printf("由串构造T2的广义表: %s\n\n", g2);
        free(g2);
    }
    BT_destroy(T2);

    /* --- 直接从字符串求高度 --- */
    printf("字符串 '%s' 的高度(直接计算) = %d\n", gstr, BT_heightFromString(gstr));

    /* --- 从标准输入按先序(#为空)构造链式树并测试 --- */
    printf("\n--- 从标准输入按先序(#为空)构造链式二叉链表并测试 ---\n");
    printf("示例: AB##C## 或 A B # # C # #，可包含空格/换行\n");
    printf("请输入: ");
    Tree T3 = BT_buildFromPreorderInput();
    if (T3)
    {
        printf("先序: ");
        BT_preorder(T3);
        printf("\n");
        printf("中序: ");
        BT_inorder(T3);
        printf("\n");
        printf("后序: ");
        BT_postorder(T3);
        printf("\n");
        printf("层序: ");
        BT_levelorder(T3);
        printf("\n");
        printf("结点数=%d, 叶子数=%d, 高度=%d\n", BT_count(T3), BT_countLeaves(T3), BT_height(T3));
        BT_destroy(T3);
    }
    else
    {
        printf("(输入为空或仅包含空节点)\n");
    }

    /* 收尾 */
    BT_destroy(R);
    SBT_destroy(T);
    return 0;
}
