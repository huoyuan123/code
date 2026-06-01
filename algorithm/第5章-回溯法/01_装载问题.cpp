/*
 * 装载问题 (Loading Problem) - 回溯法
 * ---------------------------------------
 * 问题描述：n 个集装箱，重量分别为 wᵢ。有两艘船，载重分别为 c₁ 和 c₂。
 * 问能否将所有集装箱装上两艘船，并给出装载方案。
 *
 * 算法思想（回溯法）：
 *   等价于：求子集使得其重量和在不超过 c₁ 的前提下最大化，且剩余总重 ≤ c₂。
 *   转化为 0-1 背包的变体：使第一艘船的装载量最大。
 *   解空间：子集树（每个集装箱装或不装到第一艘船）
 *   剪枝策略：
 *     约束函数：当前装载量 + wᵢ ≤ c₁
 *     限界函数：当前装载量 + 剩余总重 ≤ best（已经不优于当前最优则剪枝）
 *
 * 时间复杂度：最坏 O(2ⁿ)，实际通过剪枝大幅减少
 *
 *
 * 【阅读指引 / 易错点】
 *   - 解空间：子集树（每个集装箱对第一艘船“装/不装”）；curX 是当前路径选择，bestX 是最优解快照。
 *   - 限界剪枝：suffixSum[i] 表示“从 i 到末尾最多还能增加的重量”；若 curW + suffixSum[i] <= bestW，则后续不可能更优。
 *   - 可行性判定：回溯只负责把第一艘船尽量装满，最后用 totalW-bestW 与 c2 判断第二艘船能否装下剩余。
 *
 * 【实现提示】
 *   - 回溯核心：进入分支前更新状态，返回前撤销状态（保持不同分支互不影响）。
 *   - 剪枝核心：若当前节点的“理论最好值”已不可能超过 best，则直接回溯。
 *   - 结果映射：若对输入做了排序/重排，输出时要用映射数组还原原编号。
 *
 * 算法来源：《计算机算法设计与分析(第5版)》第5.2节
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "../encoding_fix.h"
using namespace std;

int n;                    // 集装箱数
vector<int> w;            // 重量
int c1, c2;               // 两艘船的载重
int bestW = 0;            // 第一艘船的最优装载量
vector<bool> bestX;       // 最优解（第一艘船装哪些）
vector<bool> curX;        // 当前解
int totalW = 0;           // 所有集装箱总重
vector<int> suffixSum;    // 后缀和：从 i 到 n-1 的重量和

/**
 * 装载问题 - 回溯法（子集树）
 *
 * 【这棵“树”是什么？】
 *   这里的“子集树/搜索树”是一个“概念上的决策树”，并不会在代码里真的创建结点结构。
 *   - 第 i 层表示正在对第 i 个集装箱做决策。
 *   - 每个结点有两条分支：装到第一艘船（curX[i]=true）或不装（curX[i]=false）。
 *   - 当 i==n 时，说明对所有集装箱都已做完“装/不装到第一艘船”的选择，此时递归到达“叶节点”。
 *
 * 【函数做什么？】
 *   在不超过 c1 的前提下，枚举第一艘船可装的子集，并用剪枝快速排除“不可能超过当前最优”的分支；
 *   最终得到第一艘船的最大可装载重量 bestW，以及对应方案 bestX。
 * @param 参数名 参数说明
 * @param i    当前考察的集装箱编号
 * @param curW 当前第一艘船的装载重量
 */
//Backtrack 的作用：用回溯搜索第一艘船能装的所有子集，在不超过 c1 的前提下找到装载重量最大的方案。
void Backtrack(int i, int curW) {
// 1）i >= n 时为什么还需要 curW？
// i >= n 只是表示“所有集装箱都已经做过装/不装的选择”，但装了哪些只记录在 curX 和 curW 里。
// curW 是这条完整选择路径的总重量，所以到叶节点时必须用它来和 bestW 比较，判断这条方案是不是更好。
// 没有 curW，你就无法知道这条“已完成的方案”到底装了多重。
    if (i >= n) {
        // 到达“叶节点”：i>=n 表示已经对 0..n-1 所有集装箱都做完决策（概念树的最底层）
        // 此时 curX 记录了一条从根到叶的完整选择路径，对应一个“第一艘船装载子集”。
        if (curW > bestW) {
            bestW = curW;
            bestX = curX;
        }
        return;
    }

    // 限界剪枝：即使把 [i..n-1] 全部都装上（理论上能增加的最大重量是 suffixSum[i]）
    // 若 curW + suffixSum[i] 仍然不超过当前最优 bestW，则该结点及其子树不可能产生更优解。
    if (curW + suffixSum[i] <= bestW) return;
    // 这里的 return 是从当前这次 Backtrack 调用直接返回到它的上一级递归调用

    // remainW：这里是一个“分析用变量”（当前代码未参与后续计算），保留它有助于理解含义：
    //   - (curW + suffixSum[i]) 表示从当前状态出发第一艘船“最多可能达到”的重量
    //   - totalW - (curW + suffixSum[i]) 表示“最理想情况下”仍会留给第二艘船的重量下界
    int remainW = totalW - (curW + suffixSum[i]);  // 不装 i 的话第一艘船的剩余可能

    // 左分支（概念树的一条边）：选择把第 i 个集装箱装入第一艘船
    // 约束剪枝：只有当 curW + w[i] <= c1 时，这条分支才是可行的。
    if (curW + w[i] <= c1) {
        curX[i] = true;
        Backtrack(i + 1, curW + w[i]);
    }

    // 右分支：选择不把第 i 个集装箱装入第一艘船
    // 注意：这里通过覆盖 curX[i]=false 来“撤销/重置”上一个分支对 curX[i] 的选择（回溯中的状态恢复）。
    curX[i] = false;
    Backtrack(i + 1, curW);
}

/**
 * 装载问题求解
 * @return 是否可行（两艘船能否装下所有集装箱）
 */
bool Loading() {
    //准备回溯所需的数据，然后启动回溯，最后判断第二艘船是否能装下剩余。
    totalW = accumulate(w.begin(), w.end(), 0);
    //意思是把 w 里所有元素从 0 开始累加，得到总重量。
    //accumulate 在头文件 <numeric> 里。
    // 计算后缀和（按重量降序排序可以加速剪枝）
    suffixSum.resize(n + 1);
    //resize 是 vector 的大小调整函数。
    suffixSum[n] = 0;
    //这里的 n 是全局变量，在 main() 里被赋值
    //给suffixSum赋值
    for (int i = n - 1; i >= 0; i--) {
        suffixSum[i] = suffixSum[i + 1] + w[i];
    }

    bestW = 0;
    bestX.assign(n, false);
    curX.assign(n, false);
    //assign 用来把整个 vector 重新填充成指定大小和指定值。
    //作用是把 bestX 和 curX 都变成长度为 n，并且每个元素都是 false。

    Backtrack(0, 0);

    // 剩余重量是否能被第二艘船装下
    return (totalW - bestW) <= c2;
}

int main() {
    cout << "=== 装载问题 (回溯法) ===" << endl;

    n = 5;
    w = {30, 40, 50, 20, 10};
    c1 = 80;
    c2 = 90;

    cout << "集装箱重量: ";
    for (int weight : w) cout << weight << " ";
    cout << endl;
    cout << "第一艘船载重 c1=" << c1 << ", 第二艘船载重 c2=" << c2 << endl;

    // 按重量降序排列（提高剪枝效率）
    vector<int> origIdx(n);
    for (int i = 0; i < n; i++) origIdx[i] = i;
    //begin()：指向第一个元素
    //end()：指向最后一个元素的“下一个位置”（不能解引用）
    //迭代器可以理解成**“容器里的指针/游标”**，用来遍历 vector、list、map 等容器。
    //它让你不关心容器内部怎么存，只要能“往前走、取值”。 
    //按重量从大到小排序“下标数组” origIdx，不是直接排序重量数组 w，因为我们还需要保留原始的集装箱编号（下标）以便最后输出方案。
    sort(origIdx.begin(), origIdx.end(), [](int a, int b) { return w[a] > w[b]; });
    //[] 是 lambda 表达式的捕获列表。它决定这个匿名函数能不能“使用外部变量”。
    //[] 表示不捕获任何外部变量；[&] 表示按引用捕获所有外部变量；[=] 表示按值捕获所有外部变量；[this] 表示捕获当前对象的 this 指针。
    //sort 是算法库里的排序函数，这里用它来对 origIdx 进行排序，排序的依据是 w[origIdx[i]] 的值（即对应集装箱的重量），从大到小。
    //来自于<algorithm>
    //sort(起始迭代器, 结束迭代器);            // 默认升序
    //sort(起始迭代器, 结束迭代器, 比较函数);   // 自定义排序规则
    vector<int> sortedW(n);
    for (int i = 0; i < n; i++) sortedW[i] = w[origIdx[i]];
    w = sortedW;
    //这三行是在根据已经排好序的下标 origIdx，重建一个“按重量降序”的新重量数组，然后用它替换原来的 w：

    // w = {30, 40, 50, 20, 10}，对应原始编号分别是 1..5
    // origIdx 初始是 [0,1,2,3,4]
    // 按重量降序排序后：
    // w 中重量从大到小是 50, 40, 30, 20, 10
    // 对应的原始下标顺序是 2, 1, 0, 3, 4
    // 所以 origIdx = [2,1,0,3,4]
    // 接着用 origIdx 重建 sortedW：
    // sortedW[0] = w[2] = 50
    // sortedW[1] = w[1] = 40
    // sortedW[2] = w[0] = 30
    // sortedW[3] = w[3] = 20
    // sortedW[4] = w[4] = 10
    if (Loading()) {
        cout << "可行！第一艘船最优装载=" << bestW << ", 剩余=" << (totalW - bestW) << "≤c2=" << c2 << endl;
        cout << "第一艘船装的集装箱: ";
        for (int i = 0; i < n; i++)
            if (bestX[i]) cout << origIdx[i] + 1 << "(重量" << w[i] << ") ";
        cout << endl;
    } else {
        cout << "不可行，两艘船无法装下所有集装箱" << endl;
    }

    return 0;
}
