#include <stdio.h>
#include <stdlib.h>

/* ==========================================
   题目 1：顺序栈求长度
   ========================================== */
#define MaxSize 1000
typedef int elemtype; // 假设数据类型为 int

typedef struct stack {
    elemtype data[MaxSize];
    int top;
} SqStack;

/*
 * 算法思路：
 * 顺序栈中 top 指针指示栈顶元素在数组中的下标。
 * 栈空时 top = -1。
 * 因此，栈中元素个数 = top + 1。
 */
int LengthStack_Sq(SqStack *s) {
    return s->top + 1;
}

/* ==========================================
   题目 2：链栈求长度
   ========================================== */
typedef struct linkstack {
    elemtype data;
    struct linkstack *next;
} LinkStack;

/*
 * 算法思路：
 * 链栈通常用单链表表示，top 指向栈顶节点（首节点）。
 * 遍历链表统计节点个数即可。
 */
int LengthStack_Link(LinkStack *top) {
    int count = 0;
    LinkStack *p = top;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

/* ==========================================
   题目 3：带 tag 的循环队列
   ========================================== */
typedef int ElemType;
typedef struct queue {
    ElemType data[MaxSize];
    int rear;
    int front;
    int tag; // 0: 最近一次是出队（若 f==r 则空），1: 最近一次是入队（若 f==r 则满）
} SeQueue;

/*
 * 入队算法 Insert_SQ
 * 1. 检查是否队满：front == rear 且 tag == 1
 * 2. 放入元素
 * 3. 移动 rear
 * 4. 置 tag = 1
 */
int Insert_SQ(SeQueue *q, ElemType x) {
    // 判断队满
    if (q->front == q->rear && q->tag == 1) {
        return 0; // 队满，插入失败
    }
    q->data[q->rear] = x;
    q->rear = (q->rear + 1) % MaxSize;
    q->tag = 1; // 标志最后一次操作是入队
    return 1;   // 成功
}

/*
 * 出队算法 Delete_SQ
 * 1. 检查是否队空：front == rear 且 tag == 0
 * 2. 取出元素
 * 3. 移动 front
 * 4. 置 tag = 0
 */
int Delete_SQ(SeQueue *q, ElemType *x) {
    // 判断队空
    if (q->front == q->rear && q->tag == 0) {
        return 0; // 队空，删除失败
    }
    *x = q->data[q->front];
    q->front = (q->front + 1) % MaxSize;
    q->tag = 0; // 标志最后一次操作是出队
    return 1;   // 成功
}

/* ==========================================
   测试主函数
   ========================================== */
int main() {
    printf("=== 测试顺序栈 ===\n");
    SqStack s;
    s.top = -1; // 初始化空栈
    printf("空栈长度: %d\n", LengthStack_Sq(&s));
    s.data[++s.top] = 10;
    s.data[++s.top] = 20;
    printf("入栈2个元素后长度: %d\n", LengthStack_Sq(&s));

    printf("\n=== 测试链栈 ===\n");
    LinkStack *node1 = (LinkStack*)malloc(sizeof(LinkStack));
    LinkStack *node2 = (LinkStack*)malloc(sizeof(LinkStack));
    node1->data = 1; node1->next = node2;
    node2->data = 2; node2->next = NULL;
    printf("链栈长度: %d\n", LengthStack_Link(node1));
    free(node1); free(node2);

    printf("\n=== 测试循环队列 (tag法) ===\n");
    SeQueue q;
    q.front = q.rear = 0;
    q.tag = 0; // 初始为空

    // 填满队列测试
    printf("开始入队...\n");
    int i;
    for (i = 0; i < MaxSize; i++) {
        if (!Insert_SQ(&q, i)) {
            printf("入队失败于 i=%d\n", i);
            break;
        }
    }
    printf("入队 %d 个元素后，Front: %d, Rear: %d, Tag: %d\n", i, q.front, q.rear, q.tag);
    
    // 尝试再入队一个
    if (!Insert_SQ(&q, 9999)) {
        printf("再次入队失败，验证队满成功。\n");
    }

    // 出队测试
    ElemType val;
    if (Delete_SQ(&q, &val)) {
        printf("出队一个元素: %d, Front: %d, Rear: %d, Tag: %d\n", val, q.front, q.rear, q.tag);
    }

    return 0;
}
