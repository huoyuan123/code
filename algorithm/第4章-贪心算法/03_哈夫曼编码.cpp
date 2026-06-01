/*
 * 哈夫曼编码 (Huffman Coding)
 * -------------------------------
 * 问题描述：给定 n 个字符及其出现频率 fᵢ，为每个字符构造一个前缀码
 * （即没有任何码字是另一个码字的前缀），使得编码总长度最小。
 *
 * 算法思想（贪心算法）：
 *   贪心策略：每次选择频率最低的两个节点合并为新节点，
 *   新节点的频率为两者之和。重复直到只剩一个节点。
 *   使用最小堆（优先队列）维护当前频率最小的节点。
 *   合并过程形成一棵哈夫曼树，左分支标 0，右分支标 1，
 *   从根到叶的路径即为该字符的编码。
 *
 * 时间复杂度：O(n log n) —— 堆操作
 * 最优性：哈夫曼编码是最优前缀码
 *
 * 【阅读指引 / 易错点】
 *   - 选择规则：每次合并当前频率最小的两个节点（局部最优），最终得到全局最优前缀码。
 *   - 关键数据结构：最小堆维护“当前频率最小的两个节点”；每次出堆 2 个、入堆 1 个，共 n-1 轮。
 *   - 编码生成：只有叶子节点对应字符；从根到叶的 0/1 路径即码字。若字符数为 1，需要约定其编码长度。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第4.4节
 */

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

// 哈夫曼树节点
struct Node {
    char ch;        // 字符（叶节点有效）
    int  freq;      // 频率
    Node *left;     // 左孩子（对应编码 0）
    Node *right;    // 右孩子（对应编码 1）

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// 优先队列比较器（最小频率优先）
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;  // 小顶堆
    }
};

/**
 * 递归生成编码
 * @param root  当前节点
 * @param code  当前路径对应的编码
 * @param codes 编码表 (字符 -> 编码)
 */
void GenerateCodes(Node* root, string code,
                   vector<pair<char, string>>& codes) {
    if (!root) return;

    if (!root->left && !root->right) {
        // 叶节点：记录编码
        codes.push_back({root->ch, code});
    }

    GenerateCodes(root->left,  code + "0", codes);   // 左 0
    GenerateCodes(root->right, code + "1", codes);   // 右 1
}

/**
 * 哈夫曼编码
 * @param chars  字符列表
 * @param freqs  对应频率
 * @param codes  返回的编码表
 */
Node* HuffmanCoding(const vector<char>& chars, const vector<int>& freqs,
                    vector<pair<char, string>>& codes) {
    int n = chars.size();

    // 最小优先队列
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // 为每个字符建叶节点并入队
    for (int i = 0; i < n; i++) {
        pq.push(new Node(chars[i], freqs[i]));
    }

    // 合并直到只剩一个节点
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();   // 最小频率
        Node* right = pq.top(); pq.pop();  // 次小频率

        // 创建新内部节点
        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    Node* root = pq.top();

    // 从根节点生成编码
    GenerateCodes(root, "", codes);

    return root;  // 返回树根（调用者负责释放内存）
}

/**
 * 计算总编码长度（加权外部路径长度）
 */
int TotalCost(const vector<pair<char, string>>& codes,
              const vector<char>& chars, const vector<int>& freqs) {
    int total = 0;
    for (size_t i = 0; i < chars.size(); i++) {
        for (auto& p : codes) {
            if (p.first == chars[i]) {
                total += freqs[i] * p.second.length();
                break;
            }
        }
    }
    return total;
}

int main() {
    cout << "=== 哈夫曼编码 ===" << endl;

    vector<char> chars = {'a', 'b', 'c', 'd', 'e', 'f'};
    vector<int>  freqs = {45, 13, 12, 16, 9,  5};

    cout << "字符及频率:" << endl;
    for (size_t i = 0; i < chars.size(); i++)
        cout << "  " << chars[i] << ": " << freqs[i] << endl;

    vector<pair<char, string>> codes;
    Node* root = HuffmanCoding(chars, freqs, codes);

    cout << "\n哈夫曼编码:" << endl;
    // 按编码长度排序输出
    sort(codes.begin(), codes.end(),
         [](auto& a, auto& b) { return a.second.length() < b.second.length(); });

    for (auto& p : codes) {
        cout << "  " << p.first << " : " << p.second;
        cout << " (长度 " << p.second.length() << ")" << endl;
    }

    cout << "总编码长度: " << TotalCost(codes, chars, freqs) << " bits" << endl;

    return 0;
}
