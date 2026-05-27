// 顺序储存结构
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100 // 顺序表的最大长度
typedef int ElemType; // 定义元素类型
typedef struct {
    ElemType data[MAXSIZE]; // 存储空间
    int length; // 当前长度
} SqList;
// 初始化顺序表
void InitList(SqList *L) {
    L->length = 0;
}
// 在顺序表的第i个位置插入元素e
int ListInsert(SqList *L, int i, ElemType e) {
    if (i < 1 || i > L->length + 1) {
        return 0; // 插入位置不合法
    }
    if (L->length >= MAXSIZE) {
        return 0; // 顺序表已满
    } 
    // 从最后一个已用位置向前移动，避免访问越界（当 length == MAXSIZE 时也安全）
    for (int j = L->length - 1; j >= i - 1; j--) {
        L->data[j + 1] = L->data[j]; // 元素后移
    }
    L->data[i - 1] = e; // 插入元素
    L->length++; // 长度加1
    return 1; // 插入成功
}
// 删除顺序表的第i个位置的元素
int ListDelete(SqList *L, int i, ElemType *e) {
    if (i < 1 || i > L->length) {
        return 0; // 删除位置不合法
    }
    *e = L->data[i - 1]; // 保存被删除的元素
    for (int j = i; j < L->length; j++) {
        L->data[j - 1] = L->data[j]; // 元素前移，这里的data[j - 1]是指第i个位置的元素.
    }
    L->length--; // 长度减1
    return 1; // 删除成功
}
// 获取顺序表中第i个位置的元素
int GetElem(SqList L, int i, ElemType *e) {
    if (i < 1 || i > L.length) {
        return 0; // 位置不合法
    }
    *e = L.data[i - 1]; // 获取元素
    return 1; // 获取成功
}
// 主函数测试顺序表操作
int main() {
    SqList L;
    ElemType e;
    InitList(&L);
    ListInsert(&L, 1, 10);
    ListInsert(&L, 2, 20);
    ListInsert(&L, 3, 30);
    printf("顺序表元素：");
    for (int i = 1; i <= L.length; i++) {
        GetElem(L, i, &e);
        printf("%d ", e);
    }
    printf("\n");
    ListDelete(&L, 2, &e);
    printf("删除的元素：%d\n", e);
    printf("删除后顺序表元素：");
    for (int i = 1; i <= L.length; i++) {
        GetElem(L, i, &e);
        printf("%d ", e);
    }
    printf("\n");
    return 0;
}