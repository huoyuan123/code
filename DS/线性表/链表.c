// 链式存储结构
#include <stdio.h>
#include <stdlib.h>
typedef int ElemType; // 定义元素类型
// 定义链表结点结构
typedef struct LNode {
    ElemType data; // 数据域
    struct LNode *next; // 指针域
} LNode;  // 定义 LNode 为结构体类型，可用于声明链表结点变量
typedef struct LNode *LinkList;  // 定义 LinkList 为指向 LNode 结构体的指针类型，通常用于表示链表头指针
// 双向链表结点定义
typedef struct DNode {
    ElemType data;
    struct DNode *prev;
    struct DNode *next;
} DNode, *DLinkList;
// 初始化链表
// L 是 “指针的地址”，*L 是 “指针本身”，而 *L 存储的值才是 “头结点的地址”。
void InitList(LinkList *L) {
    *L = (LinkList)malloc(sizeof(LNode)); // 创建头结点
    (*L)->next = NULL; // 头结点指针域置空,(*L)->next 是 “一级指针指向的头节点的成员”
}

// 在链表的第i个位置插入元素e
int ListInsert(LinkList L, int i, ElemType e) {
    LinkList p = L; // 指向头结点
    int j = 0;
    // 寻找第i-1个结点，链表的逻辑位置是从1开始的，所以要找第i-1个结点。
    while (p != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p == NULL || j > i - 1) {
        return 0; // 插入位置不合法
    }
    LinkList s = (LinkList)malloc(sizeof(LNode)); // 创建新结点
    s->data = e; // 赋值
    s->next = p->next; // 插入结点
    p->next = s;//接入前面的结点
    return 1; // 插入成功
}

// 删除链表的第i个位置的元素
int ListDelete(LinkList L, int i, ElemType *e) {
    LinkList p = L; // 指向头结点
    int j = 0;
    // 寻找第i-1个结点
    while (p->next != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p->next == NULL || j > i - 1) {
        return 0; // 删除位置不合法
    }
    LinkList q = p->next; // 指向被删除结点
    *e = q->data; // 保存被删除的元素
    p->next = q->next; // 删除结点
    free(q); // 释放内存
    return 1; // 删除成功
}
// 获取链表中第i个位置的元素
int GetElem(LinkList L, int i, ElemType *e) {   
    LinkList p = L->next; // 指向第一个结点
    int j = 1;
    // 寻找第i个结点
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    if (p == NULL || j > i) {
        return 0; // 位置不合法
    }
    *e = p->data; // 获取元素
    return 1; // 获取成功
}
// 函数原型：在链表的第 pos 个位置后插入 n 个元素
// 主函数测试链表操作
// 在链表的第 pos 个位置后插入 n 个元素（values 数组中按顺序插入）
// 如果 pos 为 0，则表示在头结点后（即作为第1个结点）插入
// 返回插入成功的元素个数（若发生内存分配失败则相应减少）
int InsertN(LinkList L, int pos, int n, ElemType values[]) {
    if (pos < 0) return 0; // 不接受负位置（0 表示头结点后）
    LinkList p = L; // 从头结点开始
    int j = 0;
    // 寻找第 pos 个结点（pos 为 0 时 p 保持为头结点）
    while (p != NULL && j < pos) {
        p = p->next;
        j++;
    }
    if (p == NULL || j > pos) {
        return 0; // 位置不合法
    }
    int inserted = 0;
    // 依次插入 values[0..n-1]
    for (int k = 0; k < n; k++) {
        LinkList s = (LinkList)malloc(sizeof(LNode));
        if (s == NULL) {
            // 内存分配失败，返回已插入的数量
            return inserted;
        }
        s->data = values[k];
        s->next = p->next;
        p->next = s;
        p = s; // 新插入节点成为之后插入的前驱（保持顺序）
        inserted++;
    }
    return inserted;
}
// 打印链表中所有元素（从第1个结点开始），每个元素后跟一个空格
void PrintList(LinkList L) {
    LinkList p = L->next; // 跳过头结点
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

// 合并两个按值递增的链表 A 和 B，生成一个按值递增的新链表并返回其头结点。
// 本函数通过重用 A、B 中的结点（不分配新结点）来完成合并。
// 要求：A 和 B 均为带头结点的链表，且各自的数据域按非降序排列。
// 返回值：合并后的链表 C 的头结点（需要调用者负责后续释放）。
// 注意：合并后 A->next 和 B->next 会被置为 NULL（原节点已被拼接到 C）。
LinkList MergeSorted(LinkList A, LinkList B) {
    if (A == NULL || B == NULL) return NULL;
    LinkList pa = A->next;
    LinkList pb = B->next;
    // 创建新头结点 C
    LinkList C = (LinkList)malloc(sizeof(LNode));
    if (C == NULL) return NULL; // 内存分配失败
    LinkList pc = C;
    pc->next = NULL;

    // 交替拼接较小的节点
    while (pa != NULL && pb != NULL) {
        if (pa->data <= pb->data) {
            pc->next = pa;
            pa = pa->next;
            pc = pc->next;
            pc->next = NULL; // 断开尾部，保持正确链表终结
        } else {
            pc->next = pb;
            pb = pb->next;
            pc = pc->next;
            pc->next = NULL;
        }
    }
    // 连接剩余部分
    if (pa != NULL) pc->next = pa;
    if (pb != NULL) pc->next = pb;

    // 原来 A, B 的头结点不再指向数据节点
    A->next = NULL;
    B->next = NULL;
    return C;
}

// 将两个带头结点的循环单链表 A 和 B 连接成一个循环链表并返回新的头结点（使用 A 作为结果头）。
// 约定：A 和 B 都是带头结点的循环链表（空表时头结点的 next 指向自身），
// 函数会把 B 的数据节点拼接到 A 的尾部并释放 B 的头结点，最终返回 A（如果 A 为 NULL 返回 NULL）。
// 行为要点：
// 若 A 或 B 为 NULL，则返回 NULL（不做修改）。
// 若 A 为空（A->next == A 或 A->next == NULL），则把 B 的环接入 A 的头后并返回 A。
// 若 B 为空，则释放 B 的头结点并返回 A（A 保持不变）。
LinkList ConnectCircular(LinkList A, LinkList B) {
    if (A == NULL || B == NULL) return NULL;

    // 找到 A 的尾结点（尾结点满足 next 指向头结点 A 或 next 为 NULL）
    LinkList tailA = A;
    while (tailA->next != A && tailA->next != NULL) tailA = tailA->next;
    // 找到 B 的尾结点
    LinkList tailB = B;
    while (tailB->next != B && tailB->next != NULL) tailB = tailB->next;

    // 若 A 是空表（满足 A->next == NULL 或 A->next == A），则 firstA 赋值为 NULL，表示 A 中没有数据节点。
    // 若 A 是非空表（A->next 指向第一个数据节点），则 firstA 赋值为 A->next，即指向 A 的第一个数据节点。
    LinkList firstA = (A->next == NULL || A->next == A) ? NULL : A->next;
    LinkList firstB = (B->next == NULL || B->next == B) ? NULL : B->next;

    // 两个都是空表
    if (firstA == NULL && firstB == NULL) {
        A->next = A; // 形成空的循环
        free(B);
        return A;
    }
    // A 为空，结果使用 A 作为头，将 B 的环接入 A
    if (firstA == NULL) {
        // tailB->next 指向 B（头），我们需要让尾部指向 A，且 A->next 指向 firstB
        tailB->next = A;
        A->next = firstB;
        // 释放 B 链表的头结点，因为此时 B 的数据节点已并入 A 链表，头结点不再使用
        free(B);
        return A;
    }
    // B 为空，只需释放 B 的头结点
    if (firstB == NULL) {
        free(B);
        // 确保 A 是循环形式
        // 若 A 原本不是循环链表（tailA->next 为 NULL），将其指向头结点 A 使其构成循环；
        // 若 A 原本就是循环链表（tailA->next 指向头结点 A），保持原有指向维持现有结构。
        tailA->next = (tailA->next == NULL) ? A : tailA->next;
        return A;
    }

    // 两个都非空：把 A 的尾接 B 的第一个结点，把 B 的尾接回 A 的第一个结点
    tailA->next = firstB;
    tailB->next = firstA;

    // 释放 B 的头结点（数据节点已并入 A 的环）
    
    free(B);
    return A;
}
// 函数原型：在链表的第 pos 个位置后插入 n 个元素

// 打印循环链表中最多 n 个元素（避免无限循环），从头结点后的第一个数据结点开始
void PrintCircularList(LinkList L, int n) {
    // L == NULL	链表头指针无效（无任何节点）	   直接返回，避免操作非法指针
    // p == NULL	头结点存在，但无数据节点（链断裂）	无需打印，直接返回
    if (L == NULL) return;
    LinkList p = L->next;
    if (p == NULL) return;
    int cnt = 0;
    while (p != NULL && cnt < n) {
        printf("%d ", p->data);
        p = p->next;
        cnt++;
    }
    printf("\n");
}

// ---------- 双向链表操作 ----------
// 在双向链表（带头结点）第 pos 个位置后插入元素 e（pos 从 0 开始，0 表示头结点）
// 返回 1 成功，0 失败
int DListInsert(DLinkList head, int pos, ElemType e) {
    // 检查输入参数的合法性，若头指针为空或位置为负数，则插入失败
    if (head == NULL || pos < 0) return 0;
    // 初始化指针 p 指向头结点
    DNode *p = head;
    // 初始化计数器 j
    int j = 0;
    // 遍历链表，找到第 pos 个结点
    while (p != NULL && j < pos) {
        p = p->next;
        j++;
    }
    // 若未找到第 pos 个结点，说明插入位置不合法，插入失败
    if (p == NULL) return 0;
    // 为新结点分配内存
    DNode *s = (DNode *)malloc(sizeof(DNode));
    // 若内存分配失败，返回插入失败
    if (!s) return 0;
    // 将元素 e 赋值给新结点的数据域
    s->data = e;
    // 将新结点的 next 指针指向第 pos 个结点的下一个结点
    s->next = p->next;
    // 将新结点的 prev 指针指向第 pos 个结点
    s->prev = p;
    // 若第 pos 个结点的下一个结点存在，更新其 prev 指针指向新结点
    if (p->next) p->next->prev = s;
    // 将第 pos 个结点的 next 指针指向新结点
    p->next = s;
    // 必须放在最后，否则会提前切断 p 与原后继 q 的联系，导致步骤 2 和 4 无法正确访问 q。
    // 插入成功，返回 1
    return 1;
}

// 删除双向链表中第 pos 个位置的元素（pos 从 1 开始，1 表示第一个数据结点）
// 被删除元素值保存在 *e 中，成功返回 1，失败返回 0
int DListDelete(DLinkList head, int pos, ElemType *e) {
    if (head == NULL || pos < 1) return 0;
    DNode *p = head->next;
    int j = 1;
    while (p != NULL && j < pos) {
        p = p->next;
        j++;
    }
    if (p == NULL) return 0;
    *e = p->data;
    if (p->prev) p->prev->next = p->next;
    if (p->next) p->next->prev = p->prev;
    free(p);
    return 1;
}

// 打印双向链表（从第一个数据结点开始）
void PrintDList(DLinkList head) {
    if (head == NULL) return;
    DNode *p = head->next;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

// 查找单链表倒数第k个节点，返回该节点指针（带头结点，k>=1，若不存在返回NULL）
// 原理：使用双指针法，定义快指针 fast 和慢指针 slow，初始都指向第一个数据节点。
// 先让 fast 指针向前移动 k-1 步，此时 fast 和 slow 相隔 k-1 个节点。
// 然后 fast 和 slow 同时向后移动，直到 fast 指针到达链表末尾。
// 此时 slow 指针所指向的节点就是倒数第 k 个节点。
LNode* FindKthFromEnd(LinkList L, int k) {
    // 若链表头指针为空或 k 小于 1，说明输入不合法，直接返回 NULL
    if (L == NULL || k < 1) return NULL;
    // 初始化快指针 fast 和慢指针 slow，都指向第一个数据节点
    LNode *fast = L->next, *slow = L->next;
    // fast 指针先走 k-1 步，使得 fast 和 slow 相隔 k-1 个节点
    for (int i = 1; i < k; i++) {
        // 如果 fast 指针在移动过程中为 NULL，说明链表长度小于 k，返回 NULL
        if (fast == NULL) return NULL;
        fast = fast->next;
    }
    // 如果 fast 指针移动 k-1 步后为 NULL，说明链表长度小于 k，返回 NULL
    if (fast == NULL) return NULL;
    // fast 和 slow 同时向后移动，直到 fast 指针到达链表末尾
    while (fast->next != NULL) {
        fast = fast->next;
        slow = slow->next;
    }
    // 此时 slow 指针指向的就是倒数第 k 个节点，返回该节点指针
    return slow;
}

// 约瑟夫环问题：构造一个包含 1..n 的循环链表（不带头结点），
// 从 start 位置开始，每次数到 m 就删除该结点，直到全部删除。
// 该函数直接打印出出列顺序并释放节点。
// 参数：n - 节点总数，start - 起始位置（1..n），m - 报数的间隔（>=1）
void JosephusSolve(int n, int start, int m) {
    if (n <= 0 || m <= 0 || start < 1 || start > n) {
        printf("Josephus: 参数不合法\n");
        return;
    }
    // 构造循环链表（无头结点），节点值为 1..n
    LinkList first = (LinkList)malloc(sizeof(LNode));
    first->data = 1;
    LinkList prev = first;
    for (int i = 2; i <= n; i++) {
        LinkList node = (LinkList)malloc(sizeof(LNode));
        node->data = i;
        prev->next = node;
        prev = node;
    }
    prev->next = first; // 成环

    // 定位到起始位置
    LinkList cur = first;
    LinkList pprev = prev; // pprev 指向 cur 的前驱，初始为尾结点
    // 定位到起始位置 start, 从当前位置移动 start - 1 次，将 cur 定位到起始位置 start 对应的节点。
    for (int i = 1; i < start; i++) {
        pprev = cur;
        cur = cur->next;
    }

    printf("Josephus 出列顺序：");
    int remaining = n;
    while (remaining > 0) {
        // 报数 m 次，将 cur 移动 m-1 次，pprev 跟随，将 cur 定位到要出列的结点。
        for (int cnt = 1; cnt < m; cnt++) {
            // pprev 移动到 cur 的位置
            pprev = cur;
            // cur 移动到下一个结点
            cur = cur->next;
        }
        // cur 指向要出列的结点
        // 打印当前要出列的结点的值
        printf("%d ", cur->data);
        // 从环中移除 cur
        // 将 pprev 的 next 指针指向 cur 的下一个结点，跳过 cur 结点
        // “cur 从环中被移除” 的关键是 切断环中指向 cur 的 “入口”（即 pprev->next 改指向 cur->next），
        //而非处理 cur 自身的 next 指针。因为 cur 即将被释放，其内部指针无需修改，
        //且环的完整性已通过 pprev 的调整得到保证。
        pprev->next = cur->next;
        // 创建临时指针 tofree 指向要释放的 cur 结点
        LinkList tofree = cur;
        // cur 移动到 pprev 的下一个结点，即原 cur 的下一个结点
        cur = pprev->next;
        // 释放 tofree 指向的结点，回收内存
        free(tofree);
        // 剩余结点数量减 1
        remaining--;
    }
    printf("\n");
}

int main() {
    LinkList L;
    ElemType e;
    InitList(&L);
    ListInsert(L, 1, 10);
    ListInsert(L, 2, 20);
    ListInsert(L, 3, 30);
    printf("链表元素：");
    PrintList(L);
    ListDelete(L, 2, &e);
    printf("删除的元素：%d\n", e);
    printf("删除后链表元素：");
    PrintList(L);

    // 演示 InsertN: 在第1个位置后插入 3 个元素 100,101,102
    ElemType vals[] = {100, 101, 102};
    int cnt = InsertN(L, 1, 3, vals); // 在第1个结点后插入
    printf("InsertN 插入了 %d 个元素\n", cnt);
    // 打印插入后的链表（当前应有 2 + 3 = 5 个有效结点）
    printf("插入后链表元素：");
    // 为了遍历方便，我们从第1个结点开始，一直遍历直到 NULL
    LinkList p = L->next;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");

    // ---- MergeSorted 测试 ----
    // 构造两个有序链表 A 和 B
    LinkList A, B;
    // &A 表示取指针变量 A 的地址，其类型是 LinkList *（与 InitList 的参数类型匹配）。
    InitList(&A);
    InitList(&B);
    // A: 1,3,5
    ListInsert(A, 1, 1);
    ListInsert(A, 2, 3);
    ListInsert(A, 3, 5);
    // B: 2,4,6
    ListInsert(B, 1, 2);
    ListInsert(B, 2, 4);
    ListInsert(B, 3, 6);
    printf("A: "); PrintList(A);
    printf("B: "); PrintList(B);
    LinkList C = MergeSorted(A, B);
    printf("合并后的链表 C: "); PrintList(C);
    
    // ---- ConnectCircular 测试 ----
    // 构造两个带头结点的循环链表 A_c 和 B_c
    LinkList A_c, B_c;
    InitList(&A_c);
    InitList(&B_c);
    // A_c: 1,3,5 并构成循环
    ListInsert(A_c, 1, 1);
    ListInsert(A_c, 2, 3);
    ListInsert(A_c, 3, 5);
    // 使 A_c 尾结点指回头结点
    LinkList ta = A_c;
    while (ta->next != NULL) ta = ta->next;
    ta->next = A_c;
    // B_c: 2,4 并构成循环
    ListInsert(B_c, 1, 2);
    ListInsert(B_c, 2, 4);
    LinkList tb = B_c;
    while (tb->next != NULL) tb = tb->next;
    tb->next = B_c;
    printf("A_c before connect: "); PrintCircularList(A_c, 10);
    printf("B_c before connect: "); PrintCircularList(B_c, 10);
    LinkList AC = ConnectCircular(A_c, B_c);
    printf("A_c after connect: "); PrintCircularList(AC, 10);

    // ---- Josephus 测试 ----
    JosephusSolve(7, 2, 3); // 常见测试：n=7, start=2, m=3

    // ---- 双向链表测试 ----
    DLinkList dhead = (DLinkList)malloc(sizeof(DNode));
    dhead->prev = NULL;
    dhead->next = NULL; // 作为带头结点的双向链表
    // 插入 10,20,30 在头后
    DListInsert(dhead, 0, 10);
    DListInsert(dhead, 1, 20);
    DListInsert(dhead, 2, 30);
    printf("双向链表插入后："); PrintDList(dhead);
    // 定义一个 ElemType 类型的变量 de，用于存储从双向链表中删除节点时该节点的数据值
    ElemType de;
    DListDelete(dhead, 2, &de); // 删除第2个数据结点（应删除20）
    printf("删除的元素：%d\n", de);
    printf("删除后双向链表："); PrintDList(dhead);

    // ---- 查找倒数第k个节点测试 ----
    int k = 2;
    LNode* kth = FindKthFromEnd(L, k);
    if (kth)
        printf("单链表倒数第%d个节点的值为：%d\n", k, kth->data);
    else
        printf("单链表倒数第%d个节点不存在\n", k);
    return 0;
}

