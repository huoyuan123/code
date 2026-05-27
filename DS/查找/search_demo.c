#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/********************* 基础查找：顺序/折半/分块 *********************/

// 顺序查找：从头到尾依次查找目标元素，找到返回下标，否则返回-1

// 顺序查找：从头到尾依次查找目标元素，找到返回下标，否则返回-1
// 平均查找长度（ASL）：成功 ≈ (n+1)/2，失败 = n
// a: 待查找的数组
// n: 数组长度
// key: 要查找的目标值
int seq_search(const int *a, int n, int key) {
    // i: 当前遍历到的数组下标
    for (int i = 0; i < n; ++i)
        if (a[i] == key) return i; // 找到返回下标
    return -1; // 未找到返回-1
}

// 折半查找（二分查找）：有序数组中查找目标元素，返回下标或-1
// 平均查找长度（ASL）：成功/失败 ≈ log₂n
// a: 有序数组
// n: 数组长度
// key: 要查找的目标值
int bin_search(const int *a, int n, int key) {
    int l = 0;        // l: 当前查找区间左端点
    int r = n - 1;    // r: 当前查找区间右端点
    while (l <= r) {
        int mid = l + (r - l) / 2; // mid: 当前查找区间中点
        /*为什么要写成mid=left+(right-left)/2，而不是mid=(left+right)/2。
        因为会溢出！！此时的溢出指的是，mid可能会超出该数据类型的最大值*/
        if (a[mid] == key) return mid; // 找到返回下标
        if (a[mid] < key)
            l = mid + 1; // 目标在右侧，移动左端点
        else
            r = mid - 1; // 目标在左侧，移动右端点
        //如果直接写成 l = mid 或 r = mid，当区间只剩两个元素时，mid 可能一直是同一个值，导致无限循环，查找无法结束。
    }
    return -1; // 未找到
}

// 分块查找的块索引结构体
typedef struct {
    int maxKey;  // 索引块的最大键（或边界键）
    int start;   // 块开始下标
    int end;     // 块结束下标（包含）
} BlockIndex;

// 分块查找的块表结构体
typedef struct {
    BlockIndex *idx; // 块索引数组
    int idxCount;    // 块数量
    const int *arr;  // 原始数组
    int n;           // 数组长度
} BlockTable;

// 构建一般分块查找的块表：块内可乱序，块间有序（索引表存每块最大值）
// a: 原始数组（可乱序）
// n: 数组长度
// blockSize: 每块的大小
BlockTable build_blocks(const int *a, int n, int blockSize) {
    BlockTable bt;
    bt.arr = a;
    bt.n = n;
    bt.idxCount = (n + blockSize - 1) / blockSize;
    bt.idx = (BlockIndex *)malloc(sizeof(BlockIndex) * bt.idxCount);
    for (int i = 0; i < bt.idxCount; ++i) {
        int s = i * blockSize;
        int e = (s + blockSize - 1 < n) ? (s + blockSize - 1) : (n - 1);
        bt.idx[i].start = s;
        bt.idx[i].end = e;
        // 计算当前块的最大值
        int maxv = a[s];
        for (int j = s + 1; j <= e; ++j) {
            if (a[j] > maxv) maxv = a[j];
        }
        //这里不需要写成 temp=maxv; maxv=a[j]; a[j]=temp;，因为我们只是要找出当前块的最大值，并不需要交换数组元素的位置。
        bt.idx[i].maxKey = maxv;
    }
    return bt;
}

// 一般分块查找：块内可乱序，块间有序（索引表存每块最大值）
// bt: 分块表指针
// key: 要查找的目标值
int block_search(const BlockTable *bt, int key) {
    int k = -1;
    // 1. 找到第一个 maxKey >= key 的块
    for (int i = 0; i < bt->idxCount; ++i) {
        if (bt->idx[i].maxKey >= key) {
            k = i;
            break;
        }
    }
    if (k == -1) return -1; // 没有合适的块
    // 2. 在该块内顺序查找目标元素（块内可乱序）
    int s = bt->idx[k].start;
    int e = bt->idx[k].end;
    for (int j = s; j <= e; ++j) {
        if (bt->arr[j] == key) return j;
    }
    return -1; // 未找到
}

/********************* 二叉搜索树（BST） *********************/

// 二叉搜索树结点结构体
typedef struct BSTNode {
    int key;                  // 结点存储的键值
    struct BSTNode *left, *right; // 左右子树指针
} BSTNode;

// 创建一个新的二叉搜索树结点
BSTNode *bst_new(int key) {
    BSTNode *p = (BSTNode *)malloc(sizeof(BSTNode));
    p->key = key;
    p->left = p->right = NULL;
    return p;
}

// 插入新结点到二叉搜索树
BSTNode *bst_insert(BSTNode *root, int key) {
    if (!root) return bst_new(key); // 空树直接插入
    if (key < root->key)
        root->left = bst_insert(root->left, key); // 插入左子树
    else if (key > root->key)
        root->right = bst_insert(root->right, key); // 插入右子树
    return root; // 相等不插入（或可计数）
}

// 查找二叉搜索树中的目标键，找到返回结点指针，否则返回NULL
BSTNode *bst_search(BSTNode *root, int key) {
    while (root) {
        if (key == root->key) return root; // 找到
        root = (key < root->key) ? root->left : root->right; // 左右分支
    }
    return NULL; // 未找到
}

// 查找二叉搜索树最小值结点
static BSTNode *bst_min(BSTNode *root) {
    while (root && root->left)
        root = root->left;
    return root;
}

// 删除二叉搜索树中的目标键
BSTNode *bst_delete(BSTNode *root, int key) {
    if (!root) return NULL;
    //递归查找目标结点，父结点会用这个返回值更新自己的left或right指针，实现子树的自动连接。
    if (key < root->key)
        root->left = bst_delete(root->left, key); // 去左子树删除
    else if (key > root->key)
        root->right = bst_delete(root->right, key); // 去右子树删除
    // 找到目标结点
    else {
        // 找到要删除的结点
        if (!root->left) { // 只有右子树或无子树
            BSTNode *r = root->right;
            free(root);
            return r;
        }
        if (!root->right) { // 只有左子树
            BSTNode *l = root->left;
            free(root);
            return l;
        }
        // 左右子树都存在，找右子树最小结点替换
        BSTNode *s = bst_min(root->right);
        root->key = s->key;
        root->right = bst_delete(root->right, s->key);
    }
    return root;
}

// 释放二叉搜索树所有结点
void bst_free(BSTNode *root) {
    if (!root) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

/********************* AVL 平衡二叉树 *********************/

// ********************* AVL 平衡二叉树 *********************

// AVL树结点结构体
typedef struct AVLNode {
    int key;            // 结点存储的键值
    int height;         // 当前结点的高度
    struct AVLNode *left, *right; // 左右子树指针
} AVLNode;

// 获取结点高度，空结点高度为0
static int avl_h(AVLNode *n) { return n ? n->height : 0; }
// 计算结点的平衡因子（左子树高度 - 右子树高度）
static int avl_bf(AVLNode *n) { return n ? avl_h(n->left) - avl_h(n->right) : 0; }
// 更新结点高度（max(左高,右高) + 1）
static void avl_upd(AVLNode *n) { if (n) n->height = 1 + (avl_h(n->left) > avl_h(n->right) ? avl_h(n->left) : avl_h(n->right)); }

// 创建一个新的AVL树结点，初始高度为1
static AVLNode *avl_new(int key) {
    AVLNode *p = (AVLNode *)malloc(sizeof(AVLNode));
    p->key = key; p->height = 1; p->left = p->right = NULL; return p;
}

// 右旋操作（用于左重）：
//     y                x
//    / \\     ->      / \\
//   x   T3           T1  y
//  / \\                  / \\
// T1  T2               T2  T3
static AVLNode *rot_right(AVLNode *y) {
    AVLNode *x = y->left; AVLNode *T2 = x->right;
    x->right = y; y->left = T2; avl_upd(y); avl_upd(x); return x;
}

// 左旋操作（用于右重）
static AVLNode *rot_left(AVLNode *x) {
    AVLNode *y = x->right; AVLNode *T2 = y->left;
    y->left = x; x->right = T2; avl_upd(x); avl_upd(y); return y;
}

// 平衡调整：根据平衡因子判断并执行适当旋转
static AVLNode *avl_balance(AVLNode *n) {
    avl_upd(n);
    int bf = avl_bf(n);
    if (bf > 1) { // 左重
        if (avl_bf(n->left) < 0) n->left = rot_left(n->left); // LR 型
        return rot_right(n); // LL 型
    } else if (bf < -1) { // 右重
        if (avl_bf(n->right) > 0) n->right = rot_right(n->right); // RL 型
        return rot_left(n); // RR 型
    }
    return n; // 本身已平衡
}

// 插入：递归插入后自底向上回溯平衡调整
AVLNode *avl_insert(AVLNode *root, int key) {
    if (!root) return avl_new(key);
    if (key < root->key) root->left = avl_insert(root->left, key);
    else if (key > root->key) root->right = avl_insert(root->right, key);
    return avl_balance(root);
}

// 查找最小结点（用于删除时寻找后继）
static AVLNode *avl_min(AVLNode *n) { while (n && n->left) n = n->left; return n; }

// 删除：分三种情况处理（无子/单子/双子），删除后回溯调整平衡
AVLNode *avl_delete(AVLNode *root, int key) {
    if (!root) return NULL;
    if (key < root->key) root->left = avl_delete(root->left, key);
    else if (key > root->key) root->right = avl_delete(root->right, key);
    else {
        if (!root->left || !root->right) {
            AVLNode *tmp = root->left ? root->left : root->right;
            if (!tmp) { free(root); return NULL; } // 叶子直接删除
            else { *root = *tmp; free(tmp); }      // 单子，用子替换
        } else {
            AVLNode *s = avl_min(root->right); // 右子树最小作为后继
            root->key = s->key;
            root->right = avl_delete(root->right, s->key);
        }
    }
    return avl_balance(root);
}

// 查找：与 BST 相同的迭代查找
AVLNode *avl_search(AVLNode *root, int key) {
    while (root) { if (key == root->key) return root; root = (key < root->key) ? root->left : root->right; }
    return NULL;
}

// 释放 AVL 树所有结点（后序遍历）
void avl_free(AVLNode *root) { if (!root) return; avl_free(root->left); avl_free(root->right); free(root); }

/* ********************* 链地址哈希表 ********************* */

// 哈希表结点（链地址法）：每个 HashNode 存储一个 key 和指向下一个结点的指针
typedef struct HashNode { int key; struct HashNode *next; } HashNode;

// 哈希表结构：buckets 为指针数组，每个桶指向链表头；cap 为桶数量
typedef struct { HashNode **buckets; int cap; } HashTable;

// 初始化哈希表：分配桶数组并设置容量
HashTable hash_init(int cap) {
    HashTable ht; ht.cap = cap; ht.buckets = (HashNode **)calloc(cap, sizeof(HashNode *)); return ht;
}

// 哈希函数：将任意整数 key 映射到 [0, cap-1]
static int hfunc(const HashTable *ht, int key) { return (key % ht->cap + ht->cap) % ht->cap; }

// 插入：使用头插法，并避免重复插入
void hash_insert(HashTable *ht, int key) {
    int h = hfunc(ht, key);
    HashNode *p = ht->buckets[h];
    for (; p; p = p->next) if (p->key == key) return; // 不重复插入
    HashNode *n = (HashNode *)malloc(sizeof(HashNode)); n->key = key; n->next = ht->buckets[h]; ht->buckets[h] = n;
}

// 查找：遍历对应桶的链表
HashNode *hash_search(const HashTable *ht, int key) {
    int h = hfunc(ht, key); HashNode *p = ht->buckets[h];
    for (; p; p = p->next) if (p->key == key) return p; return NULL;
}

// 删除：从链表中摘除目标结点并释放
void hash_delete(HashTable *ht, int key) {
    int h = hfunc(ht, key); HashNode *p = ht->buckets[h], *prev = NULL;
    while (p) { if (p->key == key) { if (prev) prev->next = p->next; else ht->buckets[h] = p->next; free(p); return; } prev = p; p = p->next; }
}

// 释放哈希表：遍历每个桶释放链表结点，再释放桶数组
void hash_free(HashTable *ht) {
    for (int i = 0; i < ht->cap; ++i) { HashNode *p = ht->buckets[i]; while (p) { HashNode *q = p->next; free(p); p = q; } }
    free(ht->buckets);
}

/********************* B-树（CLRS样式，最小度 t=2） *********************/

// B-树结点结构（最小度 t=2，键容量 2*t-1 = 3）
typedef struct BTreeNode {
    int n;               // 当前键数量
    int leaf;            // 是否是叶子（1 表示叶子）
    int keys[3];         // 存储的键数组
    struct BTreeNode *child[4]; // 子指针数组，最多 2*t = 4 个子指针
} BTreeNode;

typedef struct { BTreeNode *root; } BTree;

// 分配并初始化 BTreeNode
static BTreeNode *bt_new_node(int leaf) {
    BTreeNode *x = (BTreeNode *)calloc(1, sizeof(BTreeNode));
    x->leaf = leaf; x->n = 0; return x;
}

// 创建空的 B-树
BTree bt_create() { BTree T; T.root = bt_new_node(1); return T; }

// 在 B-树 x 中查找键 k，存在返回包含该键的结点指针，否则返回 NULL
BTreeNode *bt_search(BTreeNode *x, int k) {
    if (!x) return NULL;
    int i = 0; while (i < x->n && k > x->keys[i]) i++;
    if (i < x->n && k == x->keys[i]) return x;
    if (x->leaf) return NULL; else return bt_search(x->child[i], k);
}

// 分裂子结点 y（位于 x->child[i]）: 将 y 一分为二，右半部分放入新结点 z，并把中间键上移到 x
static void bt_split_child(BTreeNode *x, int i) {
    BTreeNode *y = x->child[i];
    BTreeNode *z = bt_new_node(y->leaf);
    z->n = 1; // t-1 = 1
    // 复制右半键到 z
    z->keys[0] = y->keys[2];
    if (!y->leaf) { z->child[0] = y->child[2]; z->child[1] = y->child[3]; }
    y->n = 1; // 保留左边一个键
    // 在 x 中插入 z 为新的子指针并把 y 的中位键上移
    for (int j = x->n; j >= i + 1; --j) x->child[j + 1] = x->child[j];
    x->child[i + 1] = z;
    for (int j = x->n - 1; j >= i; --j) x->keys[j + 1] = x->keys[j];
    x->keys[i] = y->keys[1]; x->n++;
}

// 在非满结点 x 中插入键 k（递归）
static void bt_insert_nonfull(BTreeNode *x, int k) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && x->keys[i] > k) { x->keys[i + 1] = x->keys[i]; i--; }
        x->keys[i + 1] = k; x->n++;
        return;
    }
    while (i >= 0 && x->keys[i] > k) i--;
    int ci = i + 1;
    if (x->child[ci]->n == 3) { bt_split_child(x, ci); if (k > x->keys[ci]) ci++; }
    bt_insert_nonfull(x->child[ci], k);
}

// 插入到 B-树：若根满则分裂根，否则递归插入
void bt_insert(BTree *T, int k) {
    BTreeNode *r = T->root;
    // 不重复插入
    if (bt_search(r, k)) return;
    if (r->n == 3) {
        BTreeNode *s = bt_new_node(0); s->child[0] = r; T->root = s; bt_split_child(s, 0); int i = (k > s->keys[0]) ? 1 : 0; bt_insert_nonfull(s->child[i], k);
    } else bt_insert_nonfull(r, k);
}

// 删除实现（CLRS，t=2）：以下函数实现借/合并/替换等操作，适配小阶实现
static int bt_get_pred(BTreeNode *x, int idx) {
    BTreeNode *cur = x->child[idx];
    while (!cur->leaf) cur = cur->child[cur->n];
    return cur->keys[cur->n - 1];
}

static int bt_get_succ(BTreeNode *x, int idx) {
    BTreeNode *cur = x->child[idx + 1];
    while (!cur->leaf) cur = cur->child[0];
    return cur->keys[0];
}

static void bt_merge(BTreeNode *x, int idx) {
    BTreeNode *c = x->child[idx];
    BTreeNode *s = x->child[idx + 1];
    // 将 x->keys[idx] 下移到 c，合并 s 到 c
    c->keys[1] = x->keys[idx];
    c->keys[2] = s->keys[0];
    if (!c->leaf) { c->child[2] = s->child[0]; c->child[3] = s->child[1]; }
    c->n = 3;
    // 删除 x 中的 idx 键与右子指针
    for (int i = idx + 1; i < x->n; ++i) x->keys[i - 1] = x->keys[i];
    for (int i = idx + 2; i <= x->n; ++i) x->child[i - 1] = x->child[i];
    x->n--;
    free(s);
}

static void bt_borrow_from_prev(BTreeNode *x, int idx) {
    BTreeNode *c = x->child[idx]; BTreeNode *s = x->child[idx - 1];
    // 将 x->keys[idx-1] 下移到 c，将 s 的最后一个键上移到 x
    for (int i = c->n - 1; i >= 0; --i) c->keys[i + 1] = c->keys[i];
    if (!c->leaf) for (int i = c->n; i >= 0; --i) c->child[i + 1] = c->child[i];
    c->keys[0] = x->keys[idx - 1];
    if (!c->leaf) c->child[0] = s->child[s->n];
    x->keys[idx - 1] = s->keys[s->n - 1];
    s->n--; c->n++;
}

static void bt_borrow_from_next(BTreeNode *x, int idx) {
    BTreeNode *c = x->child[idx]; BTreeNode *s = x->child[idx + 1];
    // 将 x->keys[idx] 下移到 c，将 s 的第一个键上移到 x
    c->keys[c->n] = x->keys[idx];
    if (!c->leaf) c->child[c->n + 1] = s->child[0];
    x->keys[idx] = s->keys[0];
    for (int i = 1; i < s->n; ++i) s->keys[i - 1] = s->keys[i];
    if (!s->leaf) for (int i = 1; i <= s->n; ++i) s->child[i - 1] = s->child[i];
    s->n--; c->n++;
}

static void bt_fill(BTreeNode *x, int idx) {
    if (idx > 0 && x->child[idx - 1]->n >= 2) bt_borrow_from_prev(x, idx);
    else if (idx < x->n && x->child[idx + 1]->n >= 2) bt_borrow_from_next(x, idx);
    else { if (idx < x->n) bt_merge(x, idx); else bt_merge(x, idx - 1); }
}

static void bt_remove(BTreeNode *x, int k);

static void bt_remove_from_leaf(BTreeNode *x, int idx) {
    for (int i = idx + 1; i < x->n; ++i) x->keys[i - 1] = x->keys[i]; x->n--;
}

static void bt_remove_from_nonleaf(BTreeNode *x, int idx) {
    int k = x->keys[idx];
    if (x->child[idx]->n >= 2) {
        int pred = bt_get_pred(x, idx); x->keys[idx] = pred; bt_remove(x->child[idx], pred);
    } else if (x->child[idx + 1]->n >= 2) {
        int succ = bt_get_succ(x, idx); x->keys[idx] = succ; bt_remove(x->child[idx + 1], succ);
    } else {
        bt_merge(x, idx); bt_remove(x->child[idx], k);
    }
}

static void bt_remove(BTreeNode *x, int k) {
    int idx = 0; while (idx < x->n && x->keys[idx] < k) idx++;
    if (idx < x->n && x->keys[idx] == k) {
        if (x->leaf) bt_remove_from_leaf(x, idx);
        else bt_remove_from_nonleaf(x, idx);
    } else {
        if (x->leaf) return; // 不存在
        int flag = (idx == x->n);
        if (x->child[idx]->n < 2) bt_fill(x, idx);
        if (flag && idx > x->n) bt_remove(x->child[idx - 1], k);
        else bt_remove(x->child[idx], k);
    }
}

// 从 B-树中删除键 k，并处理根的特殊情况
void bt_delete(BTree *T, int k) {
    if (!T->root) return;
    bt_remove(T->root, k);
    if (T->root->n == 0) {
        BTreeNode *old = T->root;
        T->root = old->leaf ? bt_new_node(1) : old->child[0];
        if (!old->leaf) free(old); else { free(old); }
        if (!T->root) T->root = bt_new_node(1);
    }
}

// 释放 B-树节点（递归）
void bt_free_node(BTreeNode *x) { if (!x) return; if (!x->leaf) for (int i = 0; i <= x->n; ++i) bt_free_node(x->child[i]); free(x); }
void bt_free(BTree *T) { bt_free_node(T->root); }

/********************* B+ 树（简化实现，阶 M=4） *********************/

typedef struct BpNode {
    int isLeaf;
    int n;
    int keys[4];
    struct BpNode *child[5];
    struct BpNode *next; // 叶子层横向链
} BpNode;

typedef struct { BpNode *root; } BPlusTree;

static BpNode *bp_new(int isLeaf) { BpNode *x = (BpNode *)calloc(1, sizeof(BpNode)); x->isLeaf = isLeaf; return x; }

BPlusTree bp_create() { BPlusTree T; T.root = bp_new(1); return T; }

// 在节点 x 中找到插入/下钻的下标位置
static int bp_find_index(BpNode *x, int k) { int i = 0; while (i < x->n && k >= x->keys[i]) i++; return i; }

// 寻找包含键的叶子节点（下钻直到叶子）
BpNode *bp_search_leaf(BPlusTree *T, int k) {
    BpNode *x = T->root; while (!x->isLeaf) { int i = bp_find_index(x, k); x = x->child[i]; } return x;
}

// B+ 树查找：只在叶子层查找
int bp_search(BPlusTree *T, int k) {
    BpNode *leaf = bp_search_leaf(T, k); for (int i = 0; i < leaf->n; ++i) if (leaf->keys[i] == k) return 1; return 0;
}

// 分裂子结点：把 y 的右半部分复制到 z，并把 z 的第一个键上推到父节点
static void bp_split_child(BpNode *x, int i) {
    BpNode *y = x->child[i]; BpNode *z = bp_new(y->isLeaf);
    int mid = y->n / 2; // 简化拆分
    int zcnt = y->n - mid;
    for (int j = 0; j < zcnt; ++j) z->keys[j] = y->keys[mid + j];
    if (!y->isLeaf) for (int j = 0; j <= zcnt; ++j) z->child[j] = y->child[mid + j];
    z->n = zcnt; y->n = mid;
    if (y->isLeaf) { z->next = y->next; y->next = z; }
    for (int j = x->n; j >= i + 1; --j) x->child[j + 1] = x->child[j]; x->child[i + 1] = z;
    for (int j = x->n - 1; j >= i; --j) x->keys[j + 1] = x->keys[j]; x->keys[i] = z->keys[0]; x->n++;
}

// 在非满结点插入（B+ 树）：叶子直接插入，内节点递归下钻并必要时分裂
static void bp_insert_nonfull(BpNode *x, int k) {
    if (x->isLeaf) {
        int i = x->n - 1; while (i >= 0 && x->keys[i] > k) { x->keys[i + 1] = x->keys[i]; i--; }
        x->keys[i + 1] = k; x->n++;
    } else {
        int i = bp_find_index(x, k);
        if (x->child[i]->n == 4) { bp_split_child(x, i); if (k >= x->keys[i]) i++; }
        bp_insert_nonfull(x->child[i], k);
        // 若子是叶，父键可能需更新第 i 或 i+1 的边界；为简化不做细调（保持正确性即可）。
    }
}

// 插入到 B+ 树：避免重复插入，若根满则分裂根
void bp_insert(BPlusTree *T, int k) {
    if (bp_search(T, k)) return;
    BpNode *r = T->root; if (r->n == 4) { BpNode *s = bp_new(0); s->child[0] = r; T->root = s; bp_split_child(s, 0); int i = (k >= s->keys[0]) ? 1 : 0; bp_insert_nonfull(s->child[i], k); }
    else bp_insert_nonfull(r, k);
}

// 简化删除（仅叶层）：在叶子中删除键并左移，其它上层维护在示例中省略
static void bp_delete_from_leaf(BpNode *leaf, int k) {
    int pos = -1; for (int i = 0; i < leaf->n; ++i) if (leaf->keys[i] == k) { pos = i; break; }
    if (pos == -1) return; for (int i = pos + 1; i < leaf->n; ++i) leaf->keys[i - 1] = leaf->keys[i]; leaf->n--;
}

void bp_delete(BPlusTree *T, int k) {
    BpNode *leaf = bp_search_leaf(T, k); bp_delete_from_leaf(leaf, k);
    // 若出现下溢，需要从同层兄弟借或合并并更新父键。
    // 为示例简化，这里省略复杂的上层维护。测试时避免导致大规模下溢情形即可。
}

// 释放 B+ 树节点（递归）
void bp_free_node(BpNode *x) { if (!x) return; if (!x->isLeaf) for (int i = 0; i <= x->n; ++i) bp_free_node(x->child[i]); free(x); }
void bp_free(BPlusTree *T) { bp_free_node(T->root); }

/********************* 辅助：打印 *********************/

void print_array(const char *name, const int *a, int n) {
    // 打印数组内容，格式：name: v1, v2, ...
    printf("%s: ", name);
    for (int i = 0; i < n; ++i) printf("%d%s", a[i], i + 1 == n ? "\n" : ", ");
}

// 中序遍历 BST（递归），用于打印有序序列
void bst_inorder(BSTNode *r) { if (!r) return; bst_inorder(r->left); printf("%d ", r->key); bst_inorder(r->right); }
// 中序遍历 AVL（递归），同样用于打印有序序列
void avl_inorder(AVLNode *r) { if (!r) return; avl_inorder(r->left); printf("%d ", r->key); avl_inorder(r->right); }

void bt_print(BTreeNode *x, int depth) {
    if (!x) return; printf("[d=%d leaf=%d n=%d |", depth, x->leaf, x->n);
    for (int i = 0; i < x->n; ++i) printf(" %d", x->keys[i]); printf(" ]\n");
    if (!x->leaf) for (int i = 0; i <= x->n; ++i) bt_print(x->child[i], depth + 1);
}

void bp_print(BpNode *x, int depth) {
    if (!x) return; printf("<d=%d leaf=%d n=%d :", depth, x->isLeaf, x->n);
    for (int i = 0; i < x->n; ++i) printf(" %d", x->keys[i]); printf(" >\n");
    if (!x->isLeaf) for (int i = 0; i <= x->n; ++i) bp_print(x->child[i], depth + 1);
}

/********************* main 测试 *********************/

int main() {
    // 基础数组（部分用于有序查找）
    int arr_unsorted[] = {7, 3, 9, 1, 6, 2, 8, 5, 4};
    int n_unsorted = sizeof(arr_unsorted)/sizeof(arr_unsorted[0]);
    int arr_sorted[] = {1,2,3,4,5,6,7,8,9};
    int n_sorted = sizeof(arr_sorted)/sizeof(arr_sorted[0]);

    printf("==== 顺序/折半/分块查找 ====\n\n");
    print_array("unsorted", arr_unsorted, n_unsorted);
    print_array("sorted", arr_sorted, n_sorted);
    printf("seq_search 6 -> %d\n", seq_search(arr_unsorted, n_unsorted, 6));
    printf("bin_search 6 -> %d\n", bin_search(arr_sorted, n_sorted, 6));
    BlockTable bt = build_blocks(arr_sorted, n_sorted, 3);
    printf("block_search 6 -> %d\n\n", block_search(&bt, 6));
    free(bt.idx);

    printf("==== 二叉搜索树 BST ====\n\n");
    BSTNode *bst = NULL; int bst_vals[] = {5,2,8,1,3,6,9};
    for (int i = 0; i < 7; ++i) bst = bst_insert(bst, bst_vals[i]);
    printf("BST inorder: "); bst_inorder(bst); printf("\n");
    printf("BST search 6 -> %s\n", bst_search(bst, 6)?"found":"not found");
    bst = bst_delete(bst, 5); printf("BST delete 5, inorder: "); bst_inorder(bst); printf("\n\n");

    printf("==== AVL 平衡二叉树 ====\n\n");
    AVLNode *avl = NULL; int avl_vals[] = {10,20,30,40,50,25};
    for (int i = 0; i < 6; ++i) avl = avl_insert(avl, avl_vals[i]);
    printf("AVL inorder: "); avl_inorder(avl); printf("\n");
    avl = avl_delete(avl, 40); printf("AVL delete 40, inorder: "); avl_inorder(avl); printf("\n\n");

    printf("==== 链地址哈希表 ====\n\n");
    HashTable ht = hash_init(7);
    int hv[] = {14,21,3,10,17,24}; for (int i = 0; i < 6; ++i) hash_insert(&ht, hv[i]);
    printf("hash_search 17 -> %s\n", hash_search(&ht, 17)?"found":"not found");
    hash_delete(&ht, 17);
    printf("hash_search 17 -> %s\n\n", hash_search(&ht, 17)?"found":"not found");

    printf("==== B-树 (t=2) ====\n\n");
    BTree btree = bt_create(); int btv[] = {10,20,5,6,12,30,7,17};
    for (int i = 0; i < 8; ++i) bt_insert(&btree, btv[i]); bt_print(btree.root, 0);
    printf("bt_search 12 -> %s\n", bt_search(btree.root, 12)?"found":"not found");
    bt_delete(&btree, 6); printf("bt_delete 6\n"); bt_print(btree.root, 0);
    bt_delete(&btree, 7); printf("bt_delete 7\n"); bt_print(btree.root, 0);

    printf("==== B+ 树 (阶4，简化删除) ====\n\n");
    BPlusTree bpt = bp_create(); int bpv[] = {10,20,5,6,12,30,7,17};
    for (int i = 0; i < 8; ++i) bp_insert(&bpt, bpv[i]); bp_print(bpt.root, 0);
    printf("bp_search 12 -> %s\n", bp_search(&bpt, 12)?"found":"not found");
    bp_delete(&bpt, 6); printf("bp_delete 6\n"); bp_print(bpt.root, 0);

    // 资源释放
    bst_free(bst); avl_free(avl); hash_free(&ht); bt_free(&btree); bp_free(&bpt);
    return 0;
}
