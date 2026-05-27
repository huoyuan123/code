// 4.1 修理牧场 —— 最小合并代价（Huffman 合并）
// 思路：每次取两段最短的木头合并，花费为两者之和，再把和放回；
//      重复直到只剩一段。用小根堆维护最小两段，时间 O(n log n)。

#include <stdio.h>
#include <stdlib.h>

typedef long long i64;

typedef struct {
	i64 *a;   // 1-based
	int n;    // size
	int cap;  // capacity
} MinHeap;

static void heap_swap(i64 *x, i64 *y) { i64 t = *x; *x = *y; *y = t; }

static void heap_down(MinHeap *h, int i) {
	while (1) {
		int l = i << 1, r = l + 1, m = i;
		if (l <= h->n && h->a[l] < h->a[m]) m = l;
		if (r <= h->n && h->a[r] < h->a[m]) m = r;
		if (m == i) break;
		heap_swap(&h->a[i], &h->a[m]);
		i = m;
	}
}

static void heap_up(MinHeap *h, int i) {
	while (i > 1) {
		int p = i >> 1;
		if (h->a[p] <= h->a[i]) break;
		heap_swap(&h->a[p], &h->a[i]);
		i = p;
	}
}

static void heap_build(MinHeap *h) {
	for (int i = h->n / 2; i >= 1; --i) heap_down(h, i);
}

static i64 heap_pop(MinHeap *h) {
	i64 ret = h->a[1];
	h->a[1] = h->a[h->n--];
	if (h->n >= 1) heap_down(h, 1);
	return ret;
}

static void heap_push(MinHeap *h, i64 v) {
	h->a[++h->n] = v;
	heap_up(h, h->n);
}

int main(void) {
	int n;
	if (scanf("%d", &n) != 1) return 0;
	if (n <= 0) { printf("0\n"); return 0; }

	MinHeap h;
	h.cap = n + 5;
	h.a = (i64 *)malloc(sizeof(i64) * (h.cap));
	if (!h.a) return 0;
	h.n = 0;

	for (int i = 0; i < n; ++i) {
		int x; scanf("%d", &x);
		h.a[++h.n] = (i64)x;
	}
	if (n == 1) { // 不需要锯
		printf("0\n");
		free(h.a);
		return 0;
	}

	heap_build(&h);
	i64 cost = 0;
	while (h.n > 1) {
		i64 a = heap_pop(&h);
		i64 b = heap_pop(&h);
		i64 c = a + b;
		cost += c;
		heap_push(&h, c);
	}
	printf("%lld\n", cost);

	free(h.a);
	return 0;
}

