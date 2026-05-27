#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100 // 栈的最大容量

// 用于链栈的整型元素类型（进制转换使用）
typedef int ElemType;

// 坐标点结构体
typedef struct {
    int x, y;
} PosType;

// 顺序栈：专门用于存放坐标点 PosType（含 x、y 两个整型成员）的栈
typedef struct {
    PosType data[MAXSIZE]; // 存储空间
    int top; // 栈顶指针
} SqStackPos;

// 顺序栈操作（用于 PosType）
/*
函数名: InitStackPos
作  用: 初始化用于存放 PosType 的顺序栈
参  数: S - 顺序栈指针
返  回: 无
复  杂: O(1)
注意事项: 将栈顶设为 -1，表示空栈
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
void InitStackPos(SqStackPos *S) {
    // 关键点：顺序栈判空常用约定是 top == -1
    S->top = -1;
}

/*
函数名: IsEmptyPos
作  用: 判断顺序栈是否为空
参  数: S - 顺序栈(按值传入)
返  回: 空栈返回 1，非空返回 0
复  杂: O(1)
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
int IsEmptyPos(SqStackPos S) {
    return S.top == -1;
}

/*
函数名: PushPos
作  用: 将坐标点 e 压入顺序栈
参  数: S - 顺序栈指针；e - 要入栈的坐标点
返  回: 成功返回 1；栈满返回 0
复  杂: O(1)
注意事项: 入栈前检查是否达到最大容量(MAXSIZE)
*/

// 时间复杂度: O(1)  空间复杂度: O(1)
int PushPos(SqStackPos *S, PosType e) {
    /* 若栈顶指针已达到最大容量-1，说明栈已满，无法继续入栈，返回0表示失败 */
    // 注意：数组栈必须检查越界，否则会发生写越界导致未定义行为
    if (S->top >= MAXSIZE - 1) return 0;
    // 先将栈顶指针 top 自增 1，再把元素 e 存入新栈顶位置
    S->data[++(S->top)] = e;
    return 1;
}

/*
函数名: PopPos
作  用: 弹出顺序栈栈顶元素到 e
参  数: S - 顺序栈指针；e - 用于接收栈顶元素
返  回: 成功返回 1；栈空返回 0
复  杂: O(1)
注意事项: 空栈不可弹出
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
int PopPos(SqStackPos *S, PosType *e) {
    // 关键点：先判空，再访问栈顶元素
    if (IsEmptyPos(*S)) return 0;
    *e = S->data[(S->top)--];
    return 1;
}

// 链栈（用于整型，进制转换）
// typedef 关键字将 struct LinkNode 重命名为 LinkNode，
// 因此后续可以直接用 LinkNode 声明该结构体类型的变量（如 LinkNode *node;），无需重复写 struct。
typedef struct LinkNode {
    ElemType data;
    struct LinkNode *next;
} LinkNode, *LinkStack;
/*
函数名: InitLinkStack
作  用: 初始化链栈(整型元素)
参  数: top - 链栈栈顶指针的地址
返  回: 无
复  杂: O(1)
注意事项: 初始化为 NULL 表示空栈
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
void InitLinkStack(LinkStack *top) {
    *top = NULL;
}

/*
函数名: PushLink
作  用: 将整型元素 e 压入链栈
参  数: top - 链栈栈顶指针的地址；e - 要入栈的整数
返  回: 成功返回 1；内存分配失败返回 0
复  杂: O(1)
注意事项: 采用头插法入栈；注意 malloc 返回值判空
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
int PushLink(LinkStack *top, ElemType e) {
    LinkNode *node = (LinkNode *)malloc(sizeof(LinkNode));
    if (!node) return 0;             // 注意：malloc 判空，避免解引用空指针
    node->data = e;
    node->next = *top;   // 此时 *top 指向原栈顶节点（或 NULL 表示空栈）
    *top = node;         // 更新栈顶指针，使其指向新插入的节点
    return 1;
}

/*
函数名: PopLink
作  用: 弹出链栈栈顶元素到 e
参  数: top - 链栈栈顶指针的地址；e - 用于接收弹出的整数
返  回: 成功返回 1；空栈返回 0
复  束: O(1)
注意事项: 释放弹出的节点以避免内存泄漏
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
int PopLink(LinkStack *top, ElemType *e) {
    // 关键点：链栈判空即判断栈顶指针是否为 NULL
    if (*top == NULL) return 0;
    LinkNode *node = *top;
    *e = node->data;
    *top = node->next;
    free(node);                       // 注意：释放弹出的节点，防止内存泄漏
    return 1;
}

// 使用链栈将十进制数 n 转换为 d 进制并输出（支持 d 在 2..16 之间）
/*
函数名: ConvertBase_LinkStack
作  用: 将十进制整数 n 转换为 d 进制并输出
参  数: n - 十进制数；d - 目标进制(2..16)
返  回: 无(直接打印)
复  杂: O(位数)，约为 O(log_d n)
注意事项: d 不在 [2,16] 范围时给出提示；0 的特殊处理；大于 9 的位用 A-F 表示
*/
// 时间复杂度: O(log n)（位数）  空间复杂度: O(log n)

void ConvertBase_LinkStack(int n, int d) {
    if (d < 2 || d > 16) {
        printf("不支持的进制：%d\n", d);
        return;
    }
    if (n == 0) {
        printf("0\n");
        return;
    }
    LinkStack st;
/*   - 在“类型声明”里， * 用于声明“指针类型”
     - 例： LinkStack *top 表示“ top 是一个指向 LinkStack 的指针”。而 LinkStack 又是 LinkNode* 的别名，所以 top 的真实类型是 LinkNode** （双重指针）。
     - 记法提示： int *p; 中的 * 是类型说明的一部分，表示 p 是指针变量。
     - 在“表达式”里， * 是解引用操作符
     - 作用：访问指针指向的对象，例如 *p = 10; 或 x = *p;
     - 前提： p 必须非 NULL 且指向一块有效内存；否则未定义行为（可能段错误）。
     - 在“表达式”里， & 是取地址操作符
     - 作用：得到变量的地址，如果变量类型是 T ，那么 &var 的类型是 T* 。
     与“双指针”的关系： & 本身只是“取地址”。是否变成“双指针”取决于你对谁取地址：
     - int i; → &i 是 int* （单指针）
     - LinkNode *st; → &st 是 LinkNode** （双指针）     */
    InitLinkStack(&st);
    int num = n;
    // 第一段“入栈”把十进制数按目标进制拆成各位；
    // 关键点（进制转换核心）：不断“取余→入栈→除以基数”，实现从低位到高位的分解
    while (num > 0) {
        int rem = num % d;
        PushLink(&st, rem);
        num /= d;
    }
    // 弹出栈并打印（高位在栈顶）
    printf("%d 的 %d 进制表示：", n, d);
    ElemType val;
    // 第二段“出栈并打印”把位从高到低输出（含 A–F 的映射）。
    // 关键点：栈的“后进先出”特性把位序反转，从而从高位到低位输出
    while (PopLink(&st, &val)) {
        if (val < 10) printf("%d", val);
        else printf("%c", 'A' + (val - 10));
    }
    printf("\n");
}

/*
函数名: dfs_maze (文件内静态)
作  用: 通过顺序栈，使用 DFS 搜索迷宫路径，借助顺序栈保存当前路径
参  数: maze - 迷宫矩阵；m/n - 行/列；x/y - 当前坐标；
       visited - 访问标记矩阵；path - 路径栈；found - 是否已找到出口标志
       int* found:
       C 的参数传递是按值传递。若用普通 int，深层递归把 found 设为 1，只会改动它自己的副本，父/兄弟层看不到变化，无法提前剪枝，仍会继续探索。
       用 int* 指针，所有递归层都在读写同一块内存，一处置 1，全体生效，for 循环里的 if (!*found) 和 break 也才能起作用。
返  回: 无(通过 path 和 found 体现结果)
复  杂: 最坏 O(m*n)，每个格子最多进出一次
注意事项: 越界/障碍/已访问直接返回；找到出口后利用 found 剪枝停止继续递归
*/
// 时间复杂度: O(m*n)  空间复杂度: O(m*n)
static void dfs_maze(int maze[5][5], int m, int n, int x, int y, int visited[5][5], SqStackPos *path, int *found) {
    // 1) 越界直接返回：当前(x,y)不在网格内，没路可走
    if (x < 0 || x >= m || y < 0 || y >= n) return;

    // 2) 障碍或已访问过：剪枝返回（避免重复/回头路）
    if (maze[x][y] == 1 || visited[x][y]) return;

    // 3) 进入格子：标记“已访问/当前路径上”，防止后续重复进入
    //    采用“回溯标记”：进入时置 1，离开（回溯）时再置回 0
    visited[x][y] = 1;

    // 4) 将当前格子入栈到 path，以便命中出口时能把整条路径打印出来
    //    注意内部坐标是 0 基，这里打印想用 1 基更直观，所以 +1
    PosType pos = {x + 1, y + 1};
    PushPos(path, pos);

    // 5) 出口判定：如果当前格子就是终点 (m-1, n-1)
    if (x == m-1 && y == n-1) {
        // 5.1 打印整条路径（从栈底到栈顶顺序）
        for (int i = 0; i <= path->top; i++) {
            PosType p = path->data[i];
            printf("(%d,%d) ", p.x, p.y);
        }
        printf("\n");
        // 5.2 设置 found=1：全局剪枝——外层不再探索其他分支
        *found = 1;
    }

    // 6) 如果尚未找到出口，继续沿四个方向探索（右、下、左、上）
    if (!*found) {
        int dir[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
        for (int d = 0; d < 4; d++) {
            // 6.1 尝试走向新的相邻格
            dfs_maze(maze, m, n, x+dir[d][0], y+dir[d][1], visited, path, found);
            // 6.2 如果递归里找到了，立即 break，避免多余探索
            if (*found) break;
        }
    }

    // 7) 回溯阶段：对称操作
    // 7.1 路径出栈（撤销“走过这一步”的记录）
    PosType tmp;
    // tmp 是一个局部变量，类型是 PosType，用来“接收被弹出的那个元素的值”（拷贝）。
    PopPos(path, &tmp);
    // 7.2 恢复访问标记为“未访问”，给别的分支机会
    visited[x][y] = 0;
}
/*
函数名: MazeSolve
作  用: 入口(0,0)到出口(m-1,n-1)的路径搜索并打印
参  数: maze - 迷宫矩阵；m/n - 行/列
返  回: 无(直接打印一条可行路径或提示无通路)
复  束: O(m*n)
注意事项: 内部创建 visited 和 path；若无通路输出“无通路”
*/
// 时间复杂度: O(m*n)  空间复杂度: O(m*n)
void MazeSolve(int maze[5][5], int m, int n) {
    SqStackPos path;
    InitStackPos(&path);
    int visited[5][5] = {0};
    int found = 0;
    dfs_maze(maze, m, n, 0, 0, visited, &path, &found);
    if (!found) printf("无通路\n");
}

// 简单测试 main（可删除或替换为交互式输入）
/*
函数名: Priority
作  用: 返回四则运算符的优先级
参  数: op - 运算符字符('+', '-', '*', '/')
返  回: '*'/' 返回 2；'+'/'-' 返回 1；其他返回 0
复  杂: O(1)
*/
// 时间复杂度: O(1)  空间复杂度: O(1)
int Priority(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

/*
函数名: InfixToPostfix
作  用: 将中缀表达式转换为后缀表达式(逆波兰式)
参  数: expr - 中缀表达式字符串；postfix - 输出缓冲区
返  回: 无(结果写入 postfix)
复  杂: O(L)，L 为表达式长度
注意事项: 支持整数、+ - * /、括号；运算符栈用链栈实现；数字之间以空格分隔
*/
// 时间复杂度: O(L)  空间复杂度: O(L)

void InfixToPostfix(const char *expr, char *postfix) {
    LinkStack opStack;
    InitLinkStack(&opStack);
    int i = 0, k = 0;
    char ch;
    ElemType topOpInt; // 用整型接收出栈的操作符
    // 关键点（扫描-分流）：数字直接输出，左括号入栈，右括号弹到遇到'('，运算符按优先级处理
    // 注意‘\0’是结束符，‘0’才是字符
    while ((ch = expr[i]) != '\0') {
        if (ch >= '0' && ch <= '9') {
            // expr 是一个以 '\0' 结尾的 C 字符串，数字“123”在里面是三个字符依次存放：'1'、'2'、'3'，而不是一个整型数 123。
            while (expr[i] >= '0' && expr[i] <= '9') {
                postfix[k++] = expr[i++];
            }
            postfix[k++] = ' ';
            continue;
        } else if (ch == '(') {
            PushLink(&opStack, ch);
        } else if (ch == ')') {
            // 关键点：右括号处理——持续弹栈直至配对的'('但'('本身不输出）
            while (opStack != NULL) {
                PopLink(&opStack, &topOpInt);
                // 注意这里的topOpInt是整型，需要强制转换为字符再比较
                if ((char)topOpInt == '(') break;
                postfix[k++] = (char)topOpInt;
                postfix[k++] = ' ';
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 关键点：比较当前运算符与栈顶运算符的优先级
            // 若栈顶优先级更高或相等（且不是'('），先弹出再入栈当前运算符
            /*用表达式：8 - 3 * 2 + 4 举例（只展示处理运算符时栈和输出的变化；数字在别处已写入输出）：
            读到运算符 '-'：
            栈空，直接入栈 '-'
            栈: [-]，后缀: "8"
            读到运算符 '*'：
            栈顶是 '-'，且 Priority('*')=2 > Priority('-')=1，while 条件触发 break，不弹出
            将 '*' 入栈
            栈: [-, *]，后缀: "8 3"
            读到运算符 '+'：
            栈顶是 ''，Priority('+')=1 不大于 2 ⇒ 弹出 '' 到后缀
            继续看新栈顶 '-'，Priority('+')=1 不大于 1 ⇒ 再弹出 '-' 到后缀
            栈已空，推入 '+'
            栈: [+]，后缀: "8 3 2 * -"
            读到输入结束（或下一个更低/等优先级的操作符）：把栈剩余的 '+' 全部弹出
            最终后缀: "8 3 2 * - 4 +"*/
            while (opStack != NULL) {
                ElemType peekOp = opStack->data;
                // priority是前面的函数‘+’‘-’返回1，‘*’‘/’返回2，其他返回0
                if (peekOp == '(' || Priority(ch) > Priority(peekOp)) break;
                PopLink(&opStack, &topOpInt);
                postfix[k++] = (char)topOpInt;
                postfix[k++] = ' ';
            }
            PushLink(&opStack, ch);
        }
        i++;
    }
    // 扫描结束：把栈里剩余的运算符全部输出
    while (opStack != NULL) {
        PopLink(&opStack, &topOpInt);
        postfix[k++] = (char)topOpInt;
        postfix[k++] = ' ';
    }
    postfix[k] = '\0';
}

/*
函数名: EvalPostfix
作  用: 计算后缀表达式的值
参  数: postfix - 后缀表达式字符串
返  回: 表达式的整型计算结果
复  杂: O(L)，L 为表达式长度
注意事项: 用链栈保存操作数；遇到运算符时弹出两个数按顺序计算
*/
// 时间复杂度: O(L)  空间复杂度: O(L)

int EvalPostfix(const char *postfix) {
    LinkStack numStack;
    InitLinkStack(&numStack);
    int i = 0;
    while (postfix[i] != '\0') {
        if (postfix[i] == ' ') { i++; continue; }
        if (postfix[i] >= '0' && postfix[i] <= '9') {
            int num = 0;
            while (postfix[i] >= '0' && postfix[i] <= '9') {
                num = num * 10 + (postfix[i++] - '0');
            }
            PushLink(&numStack, num);
        } else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/') {
            int b, a, res;
            PopLink(&numStack, &b);   // 注意：先弹出的是右操作数 b
            PopLink(&numStack, &a);   // 再弹出左操作数 a
            switch (postfix[i]) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/': res = a / b; break;
            }
            PushLink(&numStack, res);
        }
        i++;
    }
    int result;
    PopLink(&numStack, &result);   // 最终结果应是栈中唯一元素
    return result;
}

/*
函数名: TestExpr
作  用: 演示中缀转后缀及求值流程
参  数: 无
返  回: 无(打印演示过程)
复  杂: O(L)
*/
// 时间复杂度: O(L)  空间复杂度: O(L)
void TestExpr() {
    char expr[] = "3+(2*5)-4";
    char postfix[100];
    InfixToPostfix(expr, postfix);
    printf("中缀: %s\n后缀: %s\n", expr, postfix);
    int val = EvalPostfix(postfix);
    printf("表达式结果: %d\n", val);
}
/*
下面用递归常用的五个关键词做简明解释，并各配一个小例子：
契约（Contract）
    含义：对子函数的承诺/规格。输入满足什么，函数保证返回什么。
    例：subset_sum(i, remain) 的契约是“从第 i 个开始，能否凑出 remain；返回1/0”。
基线（Base case/终止条件）
    含义：递归停止的条件，直接给出答案。
    例：subset_sum 中 remain==0 返回1；i==n 或 remain<0 返回0。hanoi 中 n==1 直接打印一步。
变小（Progress/规模减小）
    含义：每次递归都让问题规模严格减小或状态推进，避免无限递归。
    例：subset_sum 用 i+1 前进；dfs_maze 改变坐标到相邻格；hanoi 用 n-1。
组合（Combine/答案合成）
    含义：如何用子问题答案拼出当前答案（或/与/加总/最值等）。
    例：subset_sum 用“或”（选或不选任一成功即成功）；hanoi 用“三步恒等式”：搬(n-1)→搬最大→搬(n-1)。
不变式（Invariant）
    含义：递归/回溯过程中始终保持为真的关键条件，保证正确性。
    例：dfs_maze 中“path 始终是起点到当前格的路径；visited 标记当前路径上的点”。进入=push+置1，离开=pop+置0（对称回溯）。hanoi 中“每根柱子始终小盘在上”。*/
/*
函数名: hanoi
作  用: 递归求解汉诺塔移动步骤并打印
参  数: n - 盘子数量；from - 源柱；aux - 辅助柱；to - 目标柱
返  回: 无(直接打印每一步)
复  束: O(2^n) 次移动与调用
注意事项: n<=0 直接返回；n==1 时只需一次移动
*/
// 时间复杂度: O(2^n)  空间复杂度: O(n)
/*
示例输出：汉诺塔移动步骤
调用展开顺序：
hanoi(3, A,B,C)
    hanoi(2, A,C,B)
        hanoi(1, A,B,C) → 打印 A -> C
        打印 A -> B
        hanoi(1, C,A,B) → 打印 C -> B
    打印 A -> C
    hanoi(2, B,A,C)
        hanoi(1, B,C,A) → 打印 B -> A
        打印 B -> C
        hanoi(1, A,B,C) → 打印 A -> C
打印顺序（共 7 步）：A->C, A->B, C->B, A->C, B->A, B->C, A->C*/
void hanoi(int n, char from, char aux, char to) {
    if (n <= 0) return;
    if (n == 1) {
        printf("%c -> %c\n", from, to);
        return;
    }
    // 把上面的 n-1 个盘子从 from 移到 aux（借助 to）
    hanoi(n - 1, from, to, aux);
    // 移动最大的盘子从 from 到 to
    printf("%c -> %c\n", from, to);
    // 把 n-1 个盘子从 aux 移到 to（借助 from）
    hanoi(n - 1, aux, from, to);
}
/*
函数名: ackermann
数学定义:
    A(0, n) = n + 1
    A(m, 0) = A(m - 1, 1) (m > 0)
    A(m, n) = A(m - 1, A(m, n - 1)) (m, n > 0)
作  用: 计算 Ackermann 函数值 A(m,n)
参  数: m, n - 非负整数(较大会导致递归极深)
返  回: A(m,n) 的值
复  杂: 增长极快，递归深度对 m 非常敏感
注意事项: 仅对很小的 m/n 可安全计算；n==0 和 m==0 的边界处理
*/
// 时间复杂度: 超指数级（远超 2^n），仅对极小参数可计算；空间复杂度: 取决于递归深度
int ackermann(int m, int n) {
    // 关键点：Ackermann 增长极快，递归层数很深，仅测试很小的 (m,n)
    if (m == 0) return n + 1;          // 基线 1
    if (n == 0) return ackermann(m - 1, 1);  // 基线 2
    return ackermann(m - 1, ackermann(m, n - 1));
}

/*
函数名: Knapsack01_MaxValue
作  用: 子集和（Subset Sum）判定：从 n 个物品中选若干个，使得权重之和恰好等于容量 kn_W。
        要求使用递归实现。本函数返回 1 表示“存在解”，返回 0 表示“无解”。
参  数: w[]   - 重量数组（长度 kn_n）
        v[]   - 价值数组（本题不会用到，可传任意同长度数组以复用现有接口）
        kn_n  - 物品个数
        kn_W  - 目标容量（期望恰好凑出的总重量）
返  回: 1 表示存在恰好等于 kn_W 的选取；0 表示不存在
复  束: 纯递归最坏时间复杂度 O(2^n)，空间复杂度 O(n)（递归栈）
注意事项（递归思维关键点）：
  1) 明确“状态”和“返回值”：
     - 状态用 (i, remain) 表示：从第 i 个物品开始，是否能恰好凑出剩余容量 remain。
     - 返回 1/0 表示能/不能。
  2) 明确“终止条件”：
     - 若 remain == 0：说明前面的选择正好凑满 -> 返回 1（成功）。
     - 若 i == kn_n（没有物品可选了）或 remain < 0：无法继续且没凑满 -> 返回 0（失败）。
  3) 明确“递推关系”（做选择）：
     - 尝试“选第 i 个”（remain 减去 w[i]）或“跳过第 i 个”（remain 不变），二者只要有一个可行即成功。
  4) 递归风险与优化：
     - 纯递归在 n 大时会非常慢；如遇到较大输入，建议加记忆化（备忘录）或改 DP。
     - C 语言递归层数受栈空间限制，n 很大时可能栈溢出（Stack Overflow）。
*/
// 时间复杂度: O(2^n)  空间复杂度: O(n)
/*有 static：
    只能在本 .c 文件中调用 subset_sum_dfs
    其他文件不可见、不可链接
无 static：
    成为外部链接符号，其他文件可声明后调用
    多文件同名实现会导致链接冲突*/
static int subset_sum_dfs(const int w[], int kn_n, int i, int remain) {
    // 终止条件 1：已经恰好凑满
    if (remain == 0) return 1;
    // 终止条件 2：物品用尽 或 剩余容量为负数（剪枝）
    // 剪枝就是在搜索/递归/回溯中，提前丢弃“不可能成功”或“没必要继续”的分支，以减少探索的状态空间。
    if (i == kn_n || remain < 0) return 0;

    // 选择分支 A：选第 i 个物品（注意先判断避免 remain 变负）
    // 若选了第 i 个能成功，直接返回成功
    if (subset_sum_dfs(w, kn_n, i + 1, remain - w[i])) return 1;

    // 选择分支 B：不选第 i 个物品
    if (subset_sum_dfs(w, kn_n, i + 1, remain)) return 1;

    // 两种选择都不行，返回失败
    return 0;
}
/*例子

数组 w = [2, 4, 6, 3]，目标 remain = 9
调用 subset_sum_dfs(w, 4, 0, 9)
深度优先先“选 A 分支（选当前元素）”，失败就“回溯”，再试“B 分支（不选当前元素）”
关键路径（含回溯）
i=0, remain=9
    选 2 → i=1, remain=7
        选 4 → i=2, remain=3
            选 6 → i=3, remain=-3（剪枝，失败，回溯撤销 6）
        不选 6 → i=3, remain=3
            选 3 → remain=0 命中，向上层一路返回 1（后续同层“不选 3”分支不会再探索）*/
// Knapsack01_MaxValue 函数：入口函数，调用递归子函数是subset_sum_dfs的外包装函数
// 时间复杂度: O(2^n)  空间复杂度: O(n)
int Knapsack01_MaxValue(const int w[], const int v[], int kn_n, int kn_W) {
    // 基本入参合法性检查
    if (!w || kn_n < 0 || kn_W < 0) return 0;
    // 递归从 i=0 开始，目标是“恰好凑出 kn_W”
    return subset_sum_dfs(w, kn_n, 0, kn_W);
}
/*
函数名: main
作  用: 演示各功能模块：进制转换、迷宫、表达式、Ackermann、汉诺塔
参  数: 无
返  回: 程序退出码
复  杂: 取决于各模块的调用次数和输入规模
注意事项: 示例为固定输入；Ackermann 与汉诺塔仅示范小规模
*/
int main(void) {
    // 示例：125 转为 2、8、16 进制
    ConvertBase_LinkStack(125, 2);
    ConvertBase_LinkStack(125, 8);
    ConvertBase_LinkStack(125, 16);

    // ---- 迷宫问题测试 ----
    // 迷宫示例（0可通，1障碍）
    int maze[5][5] = {
        {0,0,1,0,0},
        {1,0,1,0,1},
        {0,0,0,0,0},
        {0,1,1,1,0},
        {0,0,0,1,0}
    };
    printf("迷宫路径：\n");
    MazeSolve(maze, 5, 5);

    // ---- 算术表达式转后缀表达式及求值测试 ----
    TestExpr();

    // ---- Ackermann 测试 ----
    unsigned long long am, an;

    am = 0; an = 5;
    printf("A(%llu, %llu) = %llu\n", am, an, ackermann(am, an));

    am = 1; an = 3;
    printf("A(%llu, %llu) = %llu\n", am, an, ackermann(am, an));

    am = 2; an = 2;
    printf("A(%llu, %llu) = %llu\n", am, an, ackermann(am, an));

    am = 3; an = 2;
    printf("A(%llu, %llu) = %llu\n", am, an, ackermann(am, an));

    // ---- 汉诺塔测试（确保在 return 之前）----
    int hn = 3;
    printf("汉诺塔测试，层数: %d\n", hn);
    hanoi(hn, 'A', 'B', 'C');

    // ---- 子集和（恰好装满）测试（沿用原测试数组）----
    {
        int w_arr[] = {2, 2, 6, 5, 4};
        int v_arr[] = {6, 3, 5, 4, 6};
        int kn_n = (int)(sizeof(w_arr)/sizeof(w_arr[0]));
        int kn_W = 10;
        // 现在 Knapsack01_MaxValue 用于“是否能恰好凑出 kn_W”，返回 1/0
        int ok = Knapsack01_MaxValue(w_arr, v_arr, kn_n, kn_W);
        printf("是否存在恰好装满容量 %d 的选取: %s\n", kn_W, ok ? "是" : "否");
    }

    // 可按需修改 m,n 测试其它小值（建议 m<=3, n<=6）
    return 0;
    int n = 3;
    printf("汉诺塔测试，层数: %d\n", n);
    hanoi(n, 'A', 'B', 'C');
}
