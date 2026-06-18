/*
 * 装载问题 (Loading Problem) - 分支限界法
 * -------------------------------------------
 * 问题描述：同第5.2节。n 个集装箱 + 两艘船。
 * 求将尽可能多的重量装入第一艘船（≤ c₁ 前提下）。
 *
 * 算法思想（分支限界法 — 优先队列式）：
 *   每个节点代表部分装载方案（部分物品已决定装/不装）。
 *   用优先队列维护活节点，按限界值（上界）降序排列。
 *   限界函数（上界）：当前重量 + 剩余可装重量 = curW + sumRemaining
 *   每次取上界最大的节点扩展（最大优先队列）。
 *
 * 时间复杂度：最坏 O(2ⁿ)，通过限界剪枝减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 与回溯法区别：回溯通常深搜逐个生成儿子；分支限界往往用队列/优先队列，一次性生成儿子并按界排序扩展。
 *   - 活节点：节点里一般要保存当前层 i、当前装载量/剩余容量，以及用于估计的上界（bound）。
 *   - 易错点：界函数要和目标一致（最大化装载量或最小化剩余）；界过松会导致搜索爆炸。
 *
 * 【实现提示】
 *   - 活节点表：用优先队列按 bound 从大到小扩展，优先探索更有希望的节点。
 *   - 界函数：必须是“上界”（乐观估计），否则可能错剪最优解。
 *   - 状态拷贝：节点中 x/curW/level 要独立保存，避免相互影响。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第6.3节

 *
 * ═══════════════════════════════════════════════════════════════
 * 【约束函数 vs 限界函数 — 区别说明】
 * ═══════════════════════════════════════════════════════════════
 * 约束函数 (Constraint)：检查部分解是否满足问题的硬性约束条件。
 *   不满足 → 立即剪枝（该分支不可能产生可行解，继续搜无意义）。
 *   例：装载重量≦容量、皇后是否冲突、顶点颜色是否冲突。
 *
 * 限界函数 (Bounding)：估算部分解可能达到的最优目标值。
 *   不可能优于当前最优 → 剪枝（该子树不可能有更优解）。
 *   例：当前重量+剩余≦bestW、价值上界≦bestValue、curDist≧best。
 *
 * 简单记法：约束 = "能不能"（可行性）， 限界 = "值不值"（最优性）。
 * ═══════════════════════════════════════════════════════════════
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

// 变量说明：n = 集装箱数量；c1 = 第一艘船载重；w = 集装箱重量列表；bestW = 当前最优装载重量；bestX = 最优方案（哪些集装箱装入）
int n, c1;
vector<int> w;
int bestW = 0;
vector<bool> bestX;
// 节点结构：level = 当前层级（已考虑的集装箱数）；curW = 当前装载重量；bound = 上界；x = 当前部分解（哪些集装箱装入）
struct BBNode {
    int level;          // 当前层级（已考虑的集装箱数）
    int curW;           // 当前装载重量
    double bound;       // 上界（优先队列排序依据）,上界是当前重量 + 剩余可装重量
    vector<bool> x;     // 当前部分解，x[i] = true 表示集装箱 i 已装入

    BBNode(int l, int cw, double b, const vector<bool>& sol)
    //传递大对象是用引用，不拷贝整个 vector，直接传递原对象的别名
    // const	承诺在函数内部不会修改这个参数
        : level(l), curW(cw), bound(b), x(sol) {}
        /*语法格式：构造函数名(参数列表) : 成员1(初始值), 成员2(初始值), ... 
            {
                // 构造函数体
            }
            冒号 : 后面、花括号 {} 前面的部分，就是初始化列表。*/
    // 最大优先队列（上界大者优先）
    bool operator<(const BBNode& other) const {
        return bound < other.bound;
    }
    //如果存的是自定义结构体（比如你的 BBNode），编译器不知道按哪个成员排序，就必须告诉它。重载 operator< 就是告诉编译器：当比较两个 BBNode 时，应该比较它们的 bound 成员。因为 priority_queue 默认是最大堆，所以我们让 bound 大的节点被认为“更大”，这样它们就会被优先出队。
};


/**
 * 装载问题 - 分支限界法（最大优先队列）
 */
void MaxLoading() {
    int totalW = 0;
    for (int weight : w) totalW += weight;
    /*int weight	每次循环从 w 中拷贝一个元素到 weight
    :	分隔符，读作"in"
    w	被遍历的容器（这里是 vector<int> w）
    totalW += weight	循环体（只有一行，省略了花括号）
    */

    /*三种变体：
    for (int weight : w)        // 拷贝：修改 weight 不影响原数组
    for (int& weight : w)       // 引用：修改 weight 会改原数组，零拷贝
    for (const int& weight : w) // 只读引用：不能改，零拷贝（最常用）
    */
    priority_queue<BBNode> pq;
    //priority_queue 是 C++ 标准库中的优先队列容器，定义在 <queue> 头文件中。
    //优先级最高的先出

    vector<bool> initX(n, false);
    // bound 初始为所有集装箱重量总和，表示最乐观的上界
    // initX 全 false，表示初始状态没有装任何集装箱
    // 将根节点入队：level=0(还没考虑任何集装箱), curW=0(已装重量为0), bound=totalW(最乐观上界), x=全false(什么都没装)
    pq.push(BBNode(0, 0, totalW, initX));

    // 主循环：只要优先队列不为空（还有活节点没被探索），就继续
    while (!pq.empty()) {
        // 取出上界(bound)最大的节点，并把它从队列中删掉
        // 优先扩展上界大的节点，因为它更有可能包含最优解
        BBNode cur = pq.top(); pq.pop();

        // i = 当前已决定装/不装的集装箱个数（第 0 ~ i-1 个已处理，现在要决定第 i 个）
        int i = cur.level;

        // 如果 i == n，说明 n 个集装箱全都决策完了，当前节点是一个完整解（叶节点）
        if (i == n) {
            // 如果这个完整解的装载量比之前记录的最优解更好，就更新最优解
            if (cur.curW > bestW) {
                bestW = cur.curW;   // 更新最优装载重量
                bestX = cur.x;      // 更新对应的装载方案（哪些集装箱装入了）
            }
            // 叶节点没有子节点可扩展，跳过后续扩展逻辑，继续处理下一个活节点
            continue;
        }

        // 限界剪枝：如果当前节点的上界都不如已知最优解，那么以它为根的子树不可能有更优解，直接丢掉
        if (cur.bound <= bestW) continue;  // [限界函数] 上界不优于最优，剪枝（出队时）

        // ─────────── 扩展左儿子：装入第 i 个集装箱 ——————————————
        // 前提：装入后总重不超过第一艘船的载重 c1
        if (cur.curW + w[i] <= c1) {  // [约束函数] 检查装入后是否超载
            vector<bool> leftX = cur.x;        // 从父节点拷贝部分解
            //因为每个节点必须独立保存自己的方案，不能共享同一个 vector。如果直接改父节点的 x，那右儿子也会被影响。
            leftX[i] = true;                   // 标记第 i 个集装箱已装入
            // 计算左儿子的上界：已装重量 + 第i个重量 + 剩余可选的重量
            int remaining = 0;
            for (int j = i + 1; j < n; j++) remaining += w[j];
            double leftBound = cur.curW + w[i] + remaining;
            // 左儿子入队：层数+1，重量增加，携带更新后的部分解
            pq.push(BBNode(i + 1, cur.curW + w[i], leftBound, leftX));
        }

        // ─────────── 扩展右儿子：不装第 i 个集装箱 ——————————————
        //没有执行条件，是因为右儿子永远不装入，不会有超载的风险。
        {
            vector<bool> rightX = cur.x;       // 从父节点拷贝部分解
        /*不加 &（拷贝）：leftX 是独立副本，修改不影响原数据
        加了 &（引用）：leftX 和 cur.x 是同一个东西，改了 leftX 就改了 cur.x，接下来右儿子看到的 cur.x 也被污染了*/
            rightX[i] = false;                 // 标记第 i 个集装箱不装
            // 计算右儿子的上界：已装重量 + 跳过第i个后剩余的重量
            int remaining = 0;
            for (int j = i + 1; j < n; j++) remaining += w[j];
            double rightBound = cur.curW + remaining;
            // 右上界 > bestW 才入队（否则入队后也会被剪枝，提前过滤）
            if (rightBound > bestW) {  // [限界函数] 右儿子上界优于最优才入队（入队前过滤）
                pq.push(BBNode(i + 1, cur.curW, rightBound, rightX));
            }
        }
    }
}

int main() {
    cout << "=== 装载问题 (分支限界法) ===" << endl;

    n = 5;
    w = {30, 40, 50, 20, 10};
    c1 = 80;

    cout << "集装箱重量: ";
    for (int weight : w) cout << weight << " ";
    cout << endl;
    cout << "第一艘船载重 c1=" << c1 << endl;

    bestX.assign(n, false);
    MaxLoading();

    cout << "第一艘船最优装载重量: " << bestW << endl;
    cout << "装入的集装箱: ";
    int sum = 0;
    for (int i = 0; i < n; i++)
        if (bestX[i]) {
            cout << "集装箱" << (i+1) << "(重" << w[i] << ") ";
            sum += w[i];
        }
    cout << endl;
    cout << "验证总重: " << sum << endl;

    return 0;
}
