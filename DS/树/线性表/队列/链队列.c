// 队列基本内容
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef int ElemType; // 定义元素类型
// 链队列
/*struct LinkNode（前面的那个）
这是“结构体标签”（tag）。它给这类结构体起了名字，便于在结构体内部自引用：struct LinkNode* next; 此时类型还是“不完全类型”，只能用于指针。
LinkNode（声明末尾的那个）
这是 typedef 起的“类型别名”。把 struct LinkNode 起了一个简名 LinkNode，之后可直接写 LinkNode p; 而不用每次都写 struct LinkNode p;。*/
typedef struct LinkNode {
    ElemType data;
    struct LinkNode* next;
} LinkNode;
typedef struct {
    LinkNode* front; // 队头指针
    LinkNode* rear;  // 队尾指针
    int size;      // 队列当前元素个数
} LinkQueue;
// 为了在结构体内部自引用 next 指针，必须先有 struct LinkNode 这个标签名，但是LinkQueue不用所以没有标签名
/*
函数名: InitLinkQueue
作  用: 初始化链队列
参  数: Q - 链队列指针
返  回: 无
复  杂: O(1)
注意事项: 创建头节点，初始化指针和大小
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
void InitLinkQueue(LinkQueue* Q) {
    LinkNode* node = (LinkNode*)malloc(sizeof(LinkNode));
    node->next = NULL;
    Q->front = Q->rear = node;
    Q->size = 0;
}
/*
函数名: EnLinkQueue
作  用: 元素 e 入队
参  数: Q - 链队列指针；e - 要入队的元素
返  回: 成功返回 1；内存分配失败返回 0
复  杂: O(1)
注意事项: 分配新节点，更新指针和大小
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
int EnLinkQueue(LinkQueue* Q, ElemType e) {
    LinkNode* node = (LinkNode*)malloc(sizeof(LinkNode));
    if (!node) return 0; // 内存分配失败
    node->data = e;
    node->next = NULL;
    Q->rear->next = node;
    Q->rear = node;
    Q->size++;
    return 1;
}
/*
函数名: DeLinkQueue
作  用: 元素出队到 e
参  数: Q - 链队列指针；e - 用于接收
返  回: 成功返回 1；队列空返回 0
复  杂: O(1)
注意事项: 检查队列是否为空，释放出队节点
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
int DeLinkQueue(LinkQueue* Q, ElemType* e) {
    if (Q->size == 0) return 0; // 队列空
    LinkNode* node = Q->front->next;
    *e = node->data;
    Q->front->next = node->next;
    if (Q->rear == node) {
        Q->rear = Q->front; // 队列变空，更新 rear 指针
    }
    free(node);
    Q->size--;
    return 1;
}

/*
函数名: DestroyLinkQueue
作  用: 销毁链队列并释放所有节点（包含头结点）
参  数: Q - 链队列指针
返  回: 无
复  杂: O(k)，k 为队列节点数
注意事项: 调用后 front/rear 置空，size 置 0
*/
// 时间复杂度: O(k)  空间复杂度: O(1)
void DestroyLinkQueue(LinkQueue* Q) {
    if (!Q) return;
    LinkNode* cur = Q->front;
    while (cur) {
        LinkNode* nxt = cur->next;
        free(cur);
        cur = nxt;
    }
    Q->front = Q->rear = NULL;
    Q->size = 0;
}

/*
函数名: GenerateAndPair_OddEven
作  用: 产生 n 个 [1,100] 的随机整数，按生成顺序分别入奇数队/偶数队；
        然后依次配对输出“1 个奇数 + 1 个偶数”，直到某一队列为空。
参  数: n - 要生成的整数个数
返  回: 无（直接打印）
复  束: 生成与归类 O(n)，配对输出 O(pairs)；总体 O(n)。
注意事项: 使用两个链队列保存顺序；结束后释放队列内存。
*/
// 时间复杂度: O(n)  空间复杂度: O(n)（最坏情况下约各存 n/2 个）
void GenerateAndPair_OddEven(int n) {
    if (n <= 0) {
        printf("n 必须为正数\n");
        return;
    }
    LinkQueue oddQ, evenQ;
    InitLinkQueue(&oddQ);
    InitLinkQueue(&evenQ);

    // 使用时间作为随机种子，确保每次运行序列不同
    /*固定种子（如 srand(42)）→ 每次运行得到完全相同的随机序列，便于复现/测试。
      时间种子（srand((unsigned)time(NULL))）→ 每次运行序列不同，适合一般随机需求。*/
    /*srand: 设定 C 标准库伪随机数生成器的种子（state）。后续的 rand() 会基于这个种子产生序列。
      time(NULL): 返回当前时间的秒数（自 Unix 纪元起），每次运行通常不同。
      (unsigned): 强制转换为 unsigned int，因为 srand 的参数类型是 unsigned int。
      种子就是伪随机数发生器的初始状态（起点）。srand 用来设置这个初始状态，后续的 rand() 会按确定的算法从该状态生成一串“看似随机”的数。
      相同种子 → 产生完全相同的 rand() 序列（可复现）
      不同种子 → 序列不同
      常用 time(NULL) 作为种子，让每次运行都不同
      一般只在程序启动时调用一次 srand
      srand 接受 unsigned int，(unsigned) 是匹配参数类型
      不调用 srand 时，很多实现使用固定默认种子，导致每次序列都一样
      rand/srand 不是安全随机，别用于安全场景*/
    srand((unsigned)time(NULL));
    /*srand(seed): 设定 C 标准库伪随机数发生器的“种子”（初始状态）。
      rand(): 基于当前内部状态生成下一个伪随机整数，并推进内部状态。
      简言之：先用 srand 初始化，后用 rand 取数；不设种则用实现的默认种子，序列每次都相同。*/
    /*需要的头文件：stdlib.h（srand/rand），time.h（time）。
      rand() 返回范围为[0,RAND_MAX]，常配合取模使用。*/

    printf("生成的序列: ");
    for (int i = 0; i < n; i++) {
        int x = rand() % 100 + 1; // [1,100]
        // 这行是在按格式打印一个整数 x，后面跟一个“分隔符”：要么是空格，要么是换行。
        printf("%d%s", x, (i == n - 1 ? "\n" : " "));
        if (x % 2) EnLinkQueue(&oddQ, x);
        else EnLinkQueue(&evenQ, x);
    }

    printf("配对输出(奇,偶):\n");
    int o, e;
    while (DeLinkQueue(&oddQ, &o) && DeLinkQueue(&evenQ, &e)) {
        printf("(%d, %d)\n", o, e);
    }

    // 可选：提示是否有剩余未配对元素
    if (oddQ.size > 0 || evenQ.size > 0) {
        printf("停止配对：%s还有剩余（奇数剩余 %d，偶数剩余 %d）\n",
               oddQ.size == 0 ? "偶数已用尽，" : "奇数已用尽，",
               oddQ.size, evenQ.size);
    }

    DestroyLinkQueue(&oddQ);
    DestroyLinkQueue(&evenQ);
}

/*
主函数: 测试本文件中的主要函数
内容:
 1) 测试链队列的入队/出队/判空/销毁
 2) 测试“随机生成并按顺序配对奇偶数”功能
 3) （可选）顺序队列简单演示
*/
int main(void) {
    // 1) 链队列基础功能测试
    printf("[链队列] 基础功能测试\n");
    LinkQueue q;
    InitLinkQueue(&q);
    printf("入队: ");
    for (int i = 1; i <= 5; i++) {
        EnLinkQueue(&q, i);
        printf("%d%s", i, (i == 5 ? "\n" : " "));
    }
    printf("当前大小: %d\n", q.size);
    printf("出队: ");
    ElemType x;
    while (DeLinkQueue(&q, &x)) {
        printf("%d ", x);
    }
    printf("\n是否空: %s\n", q.size == 0 ? "是" : "否");
    DestroyLinkQueue(&q);

    // 2) 奇偶按生成顺序配对输出
    printf("\n[奇偶配对] 生成 20 个随机数并成对输出\n");
    GenerateAndPair_OddEven(20);

    return 0;
}
