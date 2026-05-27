// 顺序队列（顺序存储）独立文件
// 本质是数组，用指针类型定义是因为顺序队列既可以用“下标”来表示队头/队尾，也可以直接用“指向数组元素的指针”来表示。两种写法本质等价，选指针只是实现风格，便于用指针算术做 front++/rear++。
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100 // 队列最大容量
typedef int ElemType; // 元素类型

/* 顺序队列（非循环版）
   - 用一段连续内存保存元素，通过两个指针 front/rear 标识区间 [front, rear)
   - 本实现不回绕（非循环队列）：front/rear 只向右移动，不在底层数组中取模。
     因此当 rear 到达 base+MAXSIZE 时，即使前面有空位也不能再入队。
     适用于演示；生产中建议使用“循环队列（取模回绕）”避免浪费空间。
*/
typedef struct {
    ElemType *base;   // 分配的起始基址（malloc 得到的首地址，用于释放）
    ElemType *front;  // 队头指针（指向当前第一个元素）
    ElemType *rear;   // 队尾指针（指向可写入位置的下一格）
    int size;         // 当前元素个数（辅助获取长度与判空/满）
} SqQueue;

/* 函数: InitQueue（初始化）
   作用: 初始化一个空队列，分配底层顺序存储空间，令 front=rear=base，size=0
   参数: Q - 指向待初始化的队列
   返回: 无（若内存分配失败，进程退出）
   复杂度: 时间 O(1)  空间 O(1)（除去底层数组）
*/
void InitQueue(SqQueue *Q) {
    Q->base = (ElemType *)malloc(MAXSIZE * sizeof(ElemType));
    if (!Q->base) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    Q->front = Q->rear = Q->base;
    Q->size = 0;
}

/* 函数: EnQueue
   作用: 入队，将元素 e 放到队尾
   参数: Q - 指向队列；e - 要入队的元素
   返回: 1 表示成功；0 表示失败（队列满或 rear 已到达物理尾部）
   复杂度: 时间 O(1)  空间 O(1)
   注意: 非循环实现，rear 不会回绕；rear>=base+MAXSIZE 时将防御性拒绝入队
*/
int EnQueue(SqQueue *Q, ElemType e) {
    if (Q->size >= MAXSIZE) return 0; // 队列满
    // 非循环实现：rear 不能越过 base + MAXSIZE
    if (Q->rear >= Q->base + MAXSIZE) return 0; // 防御性检查
    *(Q->rear) = e;
    // rear++ 会让指针移动到下一个元素位置，即地址增加 sizeof(ElemType)字节
    // 示例：若 ElemType 为 int(4 字节)，rear++ 实际地址加 4；若为 double(8 字节)，则加 8。
    Q->rear++;
    Q->size++;
    return 1;
}

/* 函数: DeQueue
   作用: 出队，弹出队头元素并通过 e 返回
   参数: Q - 指向队列；e - 出参，保存被弹出的元素
   返回: 1 表示成功；0 表示失败（队列空）
   复杂度: 时间 O(1)  空间 O(1)
*/
int DeQueue(SqQueue *Q, ElemType *e) {
    if (Q->size == 0) return 0; // 队列空
    *e = *(Q->front);
    Q->front++;
    Q->size--;
    return 1;
}

/* 函数: GetQueueSize
   作用: 获取当前队列中元素个数
   参数: Q - 按值传入（只读）
   返回: int，元素个数
   复杂度: 时间 O(1)  空间 O(1)
*/
int GetQueueSize(SqQueue Q) { return Q.size; }

/* 函数: IsEmptyQueue
   作用: 判断队列是否为空
   参数: Q - 按值传入（只读）
   返回: 1 表示空；0 表示非空
   复杂度: 时间 O(1)  空间 O(1)
*/
int IsEmptyQueue(SqQueue Q) { return Q.size == 0; }

/* 函数: ClearQueue
   作用: 将队列重置为空队列（不释放底层内存）
   参数: Q - 指向队列
   返回: 无
   复杂度: 时间 O(1)  空间 O(1)
   注意: 非循环实现里，这会把 front/rear 直接重置到 base，从头开始使用整段空间
*/
void ClearQueue(SqQueue *Q) {
    // 重置到初始状态（不释放内存）
    Q->front = Q->rear = Q->base;
    Q->size = 0;
}

/* 函数: DestroyQueue
   作用: 销毁队列，释放底层内存并清空指针/计数
   参数: Q - 指向队列
   返回: 无
   复杂度: 时间 O(1)  空间 O(1)
*/
void DestroyQueue(SqQueue *Q) {
    if (Q->base) free(Q->base);
    Q->base = Q->front = Q->rear = NULL;
    Q->size = 0;
}

/* 函数: PrintQueue（辅助）
   作用: 打印当前队列从队头到队尾-1 的元素，便于调试观察
   参数: Q - 指向队列
   返回: 无
   复杂度: 时间 O(k)，k 为当前元素个数  空间 O(1)
*/
static void PrintQueue(const SqQueue *Q) {
    printf("[");
    for (ElemType *p = Q->front; p < Q->rear; ++p) {
        printf("%d", *p);
        if (p + 1 < Q->rear) printf(", ");
    }
    printf("]\n");
}

/* 函数: main（示例）
   作用: 演示顺序队列的基本操作流程：初始化、入队、出队、清空、再次入队、销毁
*/
int main(void) {
    SqQueue q;
    InitQueue(&q);

    printf("入队 1..5: ");
    for (int i = 1; i <= 5; ++i) EnQueue(&q, i);
    PrintQueue(&q);
    printf("大小: %d\n", GetQueueSize(q));

    printf("出队 3 个: ");
    for (int k = 0; k < 3; ++k) {
        int x; if (DeQueue(&q, &x)) printf("%d ", x);
    }
    printf("\n剩余: ");
    PrintQueue(&q);
    printf("大小: %d\n", GetQueueSize(q));

    printf("清空并再次入队 7,8,9: ");
    ClearQueue(&q);
    EnQueue(&q, 7); EnQueue(&q, 8); EnQueue(&q, 9);
    PrintQueue(&q);

    DestroyQueue(&q);
    return 0;
}
