// 循环队列（Circular Queue）
#include <stdio.h>
#include <stdlib.h>

// 与课件一致：顺序存储 + 首尾相接 + 牺牲 1 个存储单元区分满/空
// 约定：当 (rear + 1) % N == front 时为“满”；当 front == rear 时为“空”。

typedef int ElemType;
#define N 8  // 队列容量上限（可修改）；最大可容纳元素数为 N-1

typedef struct {
	ElemType elem[N]; // N: 元素最大个数
	int front;        // 队首指针（指向队首元素位置）
	int rear;         // 队尾指针（指向可写入位置）
} Squeue;

// 构造空队列：front = rear = 0
void InitQueue(Squeue *Q) {
	Q->front = Q->rear = 0;
}

// 判空：front == rear
int IsEmpty(const Squeue *Q) { return Q->front == Q->rear; }

// 判满：(rear + 1) % N == front（牺牲 1 个单元）
/*判满条件：(rear + 1) % N == front
  当 front == 0 时，rear = 5 表示满（(5+1)%6 == 0）
  元素个数 = (rear - front + N) % N = (5 - 0 + 6) % 6 = 5*/
int IsFull(const Squeue *Q) { return (Q->rear + 1) % N == Q->front; }

// 入队：向队尾插入一个数据元素 x
int EnQueue(Squeue *Q, ElemType x) {
	if (IsFull(Q)) return 0;         // 队满
	Q->elem[Q->rear] = x;            // 插入 x
	// 最后%N是为了防止 rear==N-1时越界，
	// Q->rear = (Q->rear + 1) % N 把下标加一并在到达 N-1 后回到 0；如果去掉 %N，当 Q->rear == N-1 时执行 rear++ 会变成 N，下一次访问 Q->elem[rear] 就越界（未定义行为）。
	Q->rear = (Q->rear + 1) % N;     // 队尾指针后移（环）
	return 1;
}

// 出队：删除队首数据元素，并用 *x 返回
int DeQueue(Squeue *Q, ElemType *x) {
	if (IsEmpty(Q)) return 0;        // 队空
	*x = Q->elem[Q->front];          // 取出队首
	Q->front = (Q->front + 1) % N;   // 队首指针后移（环）
	return 1;
}

// 获取当前元素个数（便于打印与测试），环绕”就是数组下标到达末尾后继续前进时回到数组起点的行为；在循环队列中通过取模或条件分支实现，使下标始终落在 [0, N-1]。
/*rear >= front（未环绕）
	元素就是下标 front, front+1, …, rear-1，共 rear - front 个。
	公式：(rear - front + N) % N = (rear - front) % N = rear - front。
rear < front（发生环绕）
	元素分两段：front..N-1（有 N - front 个）和 0..rear-1（有 rear 个），总数 = (N - front) + rear = rear - front + N。
	公式：(rear - front + N) % N = rear - front + N（再对 N 取模仍为该值，因为 < N）。*/ 
int GetSize(const Squeue *Q) {
	return (Q->rear - Q->front + N) % N;
}

// 清空队列（不改写数据，仅重置指针）
void ClearQueue(Squeue *Q) { Q->front = Q->rear = 0; }

// 查看队首元素（不出队），成功返回 1
int GetFront(const Squeue *Q, ElemType *x) {
	if (IsEmpty(Q)) return 0;
	*x = Q->elem[Q->front];
	return 1;
}

// 打印队列内容（从队首到队尾-1）
void PrintQueue(const Squeue *Q) {
	printf("[");
	int sz = GetSize(Q);
	for (int i = 0; i < sz; i++) {
		int idx = (Q->front + i) % N;
		printf("%d", Q->elem[idx]);
		if (i + 1 < sz) printf(", ");
	}
	printf("] (size=%d, front=%d, rear=%d)\n", sz, Q->front, Q->rear);
}

// 课件 3.2.4 循环队列：基本操作演示
int main(void) {
	Squeue Q; InitQueue(&Q);
	printf("-- 初始 --\n");
	PrintQueue(&Q);

	// 1) 入队若干元素，观察尾指针移动
	printf("入队 1..5:\n");
	for (int i = 1; i <= 5; ++i) {
		EnQueue(&Q, i);
	}
	PrintQueue(&Q);

	// 2) 出队 3 个，观察首指针移动
	printf("出队 3 个: ");
	for (int k = 0; k < 3; ++k) {
		int x; DeQueue(&Q, &x); printf("%d ", x);
	}
	printf("\n");
	PrintQueue(&Q);

	// 3) 继续入队，触发环绕
	printf("继续入队 6,7,8,9（注意 N=%d，最多容纳 N-1=%d 个）\n", N, N-1);
	for (int v = 6; v <= 9; ++v) {
		int ok = EnQueue(&Q, v);
		printf("EnQueue(%d) -> %s\n", v, ok ? "OK" : "FULL");
	}
	PrintQueue(&Q);

	// 4) 查看队首元素
	int frontVal;
	if (GetFront(&Q, &frontVal)) {
		printf("当前队首: %d\n", frontVal);
	}

	// 5) 全部出队
	printf("全部出队: ");
	int x;
	while (DeQueue(&Q, &x)) printf("%d ", x);
	printf("\n");
	PrintQueue(&Q);

	// 6) 清空后再次入队
	ClearQueue(&Q);
	printf("清空后入队 42, 43:\n");
	EnQueue(&Q, 42); EnQueue(&Q, 43);
	PrintQueue(&Q);

	return 0;
}
