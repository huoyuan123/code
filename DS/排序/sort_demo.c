#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

/*
工具函数说明：
1) swap: 交换两个整数（O(1) 时间, O(1) 空间）
2) is_sorted: 判断数组是否非降序已排序（O(n) 时间, O(1) 空间）
3) print_array: 打印数组内容（O(n) 时间, O(1) 空间）
4) copy_array: 复制数组（O(n) 时间, O(1) 额外空间）
*/
/*
swap
作用：交换两个整数的值
时间复杂度：O(1)
空间复杂度：O(1)
稳定性：不适用（工具函数）
*/
static void swap(int *a, int *b){int t=*a;*a=*b;*b=t;}
/*
is_sorted
作用：判断数组是否已按非降序排列
时间复杂度：O(n)
空间复杂度：O(1)
稳定性：不适用（检查函数）
*/
static int is_sorted(const int *a,int n){for(int i=1;i<n;++i) if(a[i-1]>a[i]) return 0; return 1;}
/*
print_array
作用：打印数组内容，便于调试/展示
时间复杂度：O(n)
空间复杂度：O(1)
稳定性：不适用（输出函数）
*/
static void print_array(const char *name,const int *a,int n){printf("%s: ",name);for(int i=0;i<n;++i) printf("%d%s",a[i],i+1==n?"\n":", ");}
/*
copy_array
作用：将 src 复制到 dst
时间复杂度：O(n)
空间复杂度：O(1) 额外
稳定性：不适用（数据搬运）
*/
static void copy_array(int *dst,const int *src,int n){for(int i=0;i<n;++i) dst[i]=src[i];}

/**************** 基本排序 ****************/

/*
插入排序 insertion_sort
作用：将数组就地排序；对近乎有序数据表现良好且稳定。
时间复杂度：最好 O(n)（已排序），平均/最坏 O(n^2)
空间复杂度：O(1)
是否稳定：稳定
要点：将当前位置元素按顺序插入到前缀有序区
*/
void insertion_sort(int *a,int n){
    for(int i=1;i<n;++i){
        int key=a[i];                 /* 待插入的元素 */
        int j=i-1;                    /* 有序区末尾指针 */
        while(j>=0 && a[j]>key){      /* 向左移动比 key 大的元素 */
            a[j+1]=a[j];
            --j;
        }
        a[j+1]=key;                   /* 插入到正确位置 */
    }
}

/*
希尔排序 shell_sort（增量递减的分组插入排序）
作用：通过间隔分组降低逆序距离，加速插入排序。
时间复杂度：依赖 gap 序列；此处简单折半序列通常最坏 ~O(n^2)
这里的gap就是增量
空间复杂度：O(1)
是否稳定：否
*/
void shell_sort(int *a,int n){
    for(int gap=n/2;gap>0;gap/=2){            /* 逐步缩小步长 */
        for(int i=gap;i<n;++i){
            int temp=a[i];
            int j=i;
            while(j-gap>=0 && a[j-gap]>temp){ /* 分组内做插入排序 */
                a[j]=a[j-gap];
                // 将索引 j 向前移动 gap 个位置，通常用于希尔排序（Shell Sort）等分组插入排序算法中，
                // 以便比较和移动前一个分组元素，实现分组内的插入排序。
                j-=gap;
            }
            a[j]=temp;
        }
    }
}

/*
选择排序 selection_sort
作用：每轮选择剩余元素中的最小值放到已排序区末尾。
时间复杂度：最好/平均/最坏均 O(n^2)
空间复杂度：O(1)
是否稳定：否（存在交换跨越相同元素）
*/
void selection_sort(int *a,int n){
    for(int i=0;i<n-1;++i){
        int min=i;                           /* 当前未排序区的最小下标 */
        for(int j=i+1;j<n;++j)
            if(a[j]<a[min]) min=j;
        if(min!=i) swap(&a[i],&a[min]);      /* 将最小值放到位 */
    }
}

/*
冒泡排序 bubble_sort
作用：通过相邻交换将较大元素逐步“冒泡”到右端；含早停优化。
时间复杂度：最坏/平均 O(n^2)，最好 O(n)（已排序 + 早停）
空间复杂度：O(1)
是否稳定：稳定
*/
void bubble_sort(int *a,int n){
    for(int i=0;i<n-1;++i){
        int swapped=0;                       /* 若一趟无交换则已有序 */
        for(int j=0;j<n-1-i;++j){            /* 右侧 i 个已在正确位置 */
            if(a[j]>a[j+1]){                 /* 逆序则交换 */
                swap(&a[j],&a[j+1]);
                swapped=1;
            }
        }
        if(!swapped) break;                  /* 早停 */
    }
}

/*
地精排序 gnome_sort（相邻错就交换，退一步再比较）
时间复杂度：最好 O(n)，平均/最坏 O(n^2)
空间复杂度：O(1)
是否稳定：稳定
*/
void gnome_sort(int *a,int n){
    int i=0;
    while(i<n){
        if(i==0 || a[i-1]<=a[i]) i++;        /* 顺序正确则前进 */
        else { swap(&a[i-1],&a[i]); i--; }   /* 逆序则交换并后退 */
    }
}

/**************** 归并排序 ****************/

/*
merge（合并两个相邻有序段 [l..m], [m+1..r]）
时间复杂度：O(r-l+1)
空间复杂度：O(r-l+1) 使用外部 tmp 缓冲
稳定性：稳定
*/
static void merge(int *a,int l,int m,int r,int *tmp){
    int i=l,j=m+1,k=l;                        /* i 指左段，j 指右段，k 指 tmp 填充位置 */
    while(i<=m && j<=r){
        if(a[i]<=a[j]) tmp[k++]=a[i++];
        else           tmp[k++]=a[j++];
    }
    while(i<=m) tmp[k++]=a[i++];              /* 复制剩余元素 */
    while(j<=r) tmp[k++]=a[j++];
    for(int t=l;t<=r;++t) a[t]=tmp[t];         /* 回写到原数组 */
}
/*
自顶向下归并排序递归体
时间复杂度：O(n log n)
空间复杂度：O(n) 需要辅助数组
稳定性：稳定
*/
static void mergesort_rec(int *a,int l,int r,int *tmp){
    if(l>=r) return;
    int m=l+(r-l)/2;
    mergesort_rec(a,l,m,tmp);
    mergesort_rec(a,m+1,r,tmp);
    /* 可选优化：若 a[m] <= a[m+1] 则两段“已整体有序”，可跳过 merge（此处保持原始实现） */
    merge(a,l,m,r,tmp);
} 
/*
归并排序（包装函数） merge_sort
作用：对数组进行稳定排序（自顶向下递归 + 辅助数组）
时间复杂度：O(n log n)
空间复杂度：O(n)
是否稳定：稳定
*/
void merge_sort(int *a,int n){
    int *tmp=(int*)malloc(sizeof(int)*n);     /* 一次性分配 O(n) 缓冲 */
    mergesort_rec(a,0,n-1,tmp);
    free(tmp);
} 

/**************** 快速排序 ****************/

/*
Hoare 分区法 partition_hoare
作用：以中点值为枢轴，将区间分为 <=pivot 与 >=pivot 两边，返回分割点。
时间复杂度：O(r-l+1)
空间复杂度：O(1)
注意：返回的 j 位置满足 [l..j] 与 [j+1..r] 两段递归。
*/
static int partition_hoare(int *a,int l,int r){
    int pivot=a[l+(r-l)/2];                   /* 选取中间值做枢轴，缓解已序退化 */
    int i=l-1,j=r+1;                          /* i 从左向右找 >=pivot，j 从右向左找 <=pivot */
    for(;;){//无限循环，直到找到交错点返回
        do{i++;}while(a[i]<pivot);           /* 向右找第一个 >= pivot */
        do{j--;}while(a[j]>pivot);           /* 向左找第一个 <= pivot */
        if(i>=j) return j;                   /* 交错则返回分割点 */
        swap(&a[i],&a[j]);                   /* 交换到正确侧 */
    }
}
/*
快速排序 quick_sort（Hoare 分区 + 递归）
时间复杂度：平均 O(n log n)，最坏 O(n^2)
空间复杂度：平均 O(log n)（递归栈），最坏 O(n)
是否稳定：否
*/
static void quicksort_rec(int *a,int l,int r){
    if(l<r){
        int p=partition_hoare(a,l,r);
        quicksort_rec(a,l,p);
        quicksort_rec(a,p+1,r);
        //每次递归都在分区，把数组分成两部分，继续递归，直到每个区间只有一个元素。
    }
}
/*
快速排序（包装函数） quick_sort
作用：原地不稳定排序（Hoare 分区策略）
时间复杂度：平均 O(n log n)，最坏 O(n^2)
空间复杂度：平均 O(log n)，最坏 O(n)
是否稳定：否
*/
void quick_sort(int *a,int n){ quicksort_rec(a,0,n-1);} 

/**************** 堆排序 ****************/

/*
heapify（下滤/筛选）
作用：在以 i 为根的子堆上进行下滤，保持最大堆性质。
时间复杂度：O(log n)
空间复杂度：O(1)
*/
static void heapify(int *a,int n,int i){
    while(1){
        int largest=i;
        int l=2*i+1, r=2*i+2;                 /* 左右孩子 */
        if(l<n && a[l]>a[largest]) largest=l;
        if(r<n && a[r]>a[largest]) largest=r;
        if(largest==i) break;                 /* 已满足堆序则停止 */
        swap(&a[i],&a[largest]);               /* 交换并继续向下 */
        i=largest;
    }
}
/*
堆排序 heap_sort（原地、不稳定）
时间复杂度：建堆 O(n) + n 次删除最大 O(n log n) ≈ O(n log n)
空间复杂度：O(1)
是否稳定：否
*/
void heap_sort(int *a,int n){
    for(int i=n/2-1;i>=0;--i) heapify(a,n,i); /* 自底向上建最大堆 */
    for(int end=n-1;end>0;--end){             /* 依次将堆顶最大放到末尾 */
        swap(&a[0],&a[end]);
        heapify(a,end,0);                     /* 在缩小的堆中下滤 */
    } 
}

/**************** 计数排序（稳定，支持负数） ****************/

/*
计数排序 counting_sort（可处理负数，通过平移偏移）
时间复杂度：O(n + range) 其中 range = max-min+1
空间复杂度：O(n + range)
是否稳定：按逆序回填实现为稳定
*/
void counting_sort(int *a,int n){
    if(n<=1) return;
    int mn=a[0], mx=a[0];                     /* 扫描得到范围 */
    for(int i=1;i<n;++i){ if(a[i]<mn) mn=a[i]; if(a[i]>mx) mx=a[i]; }
    int range = mx - mn + 1;
    /* 注意：当值域 range 很大时，计数排序的空间和时间都会变大 */
    int *cnt=(int*)calloc(range,sizeof(int)); /* 统计每个值出现次数 */
    for(int i=0;i<n;++i) cnt[a[i]-mn]++;
    for(int i=1;i<range;++i) cnt[i]+=cnt[i-1];/* 前缀和得到稳定位置 */
    int *out=(int*)malloc(sizeof(int)*n);
    for(int i=n-1;i>=0;--i){                  /* 逆序遍历以保持稳定 */
        int pos=--cnt[a[i]-mn];
        out[pos]=a[i];
    }
    for(int i=0;i<n;++i) a[i]=out[i];         /* 回写结果 */
    free(cnt);
    free(out);
}

/**************** 树形选择排序（锦标赛排序） ****************/

/*
锦标赛排序 tournament_sort
思想：构建完全二叉“胜者树”，每次取根为最小值，再自底向上更新路径。
时间复杂度：建树 O(n)，每次选最小与更新 O(log n)，总 O(n log n)
空间复杂度：O(n)（树和输出缓冲）
是否稳定：否（相等元素的胜出依赖索引顺序）
*/
void tournament_sort(int *a,int n){
    if(n<=1) return;
    int m=1; while(m<n) m<<=1;                /* 叶子补齐到 2 的幂，便于完全二叉树表示 */
    int *tree=(int*)malloc(sizeof(int)* (2*m));/* 节点存放的是数组下标 */
    /* 叶子初始化：有效位置存下标，无效记为 -1 */
    for(int i=0;i<m;++i){ if(i<n) tree[m+i]=i; else tree[m+i]=-1; }
    /* 自底向上选出每对孩子中较小值的下标 */
    for(int i=m-1;i>=1;--i){ int li=tree[2*i], ri=tree[2*i+1]; if(li==-1) tree[i]=ri; else if(ri==-1) tree[i]=li; else tree[i]= (a[li]<=a[ri]?li:ri); }
    int *out=(int*)malloc(sizeof(int)*n);
    for(int k=0;k<n;++k){
        int minIdx=tree[1];                    /* 根为当前全局最小的下标 */
        out[k]=a[minIdx];
        a[minIdx]=INT_MAX;                     /* 将该位置设置为无穷大，表示已取出 */
        /* 从叶到根更新胜者路径 */
        int pos=m+minIdx;
        while(pos>=1){
            int i=pos/2; if(i==0) break;
            int li=tree[2*i], ri=tree[2*i+1];
            if(li==-1) tree[i]=ri;
            else if(ri==-1) tree[i]=li;
            else tree[i]= (a[li]<=a[ri]?li:ri);
            pos=i;
        }
    }
    for(int i=0;i<n;++i) a[i]=out[i];
    free(out);
    free(tree);
}

/**************** 链式基数排序（十进制，非负） ****************/

typedef struct Node{int key; struct Node *next;} Node;

/*
链式 LSD 基数排序 radix_sort_linked（基数 10，仅处理非负整数）
思想：按个位→十位→百位… 分配到 10 个桶，再按桶序连接。
时间复杂度：O(d*(n + B))，d 为位数，B=10
空间复杂度：O(n + B)（链表结点复用，额外桶头尾指针）
稳定性：稳定
*/
void radix_sort_linked(int *a,int n){
    if(n<=1) return;
    int max=a[0]; for(int i=1;i<n;++i) if(a[i]>max) max=a[i];
    if(max<0) return; /* 仅处理非负数 */
    /* 将数组转为链表，便于稳定分配与拼接 */
    Node *head=NULL,*tail=NULL;
    for(int i=0;i<n;++i){ Node *p=(Node*)malloc(sizeof(Node)); p->key=a[i]; p->next=NULL; if(!head){head=tail=p;} else {tail->next=p; tail=p;} }
    Node *bHead[10], *bTail[10];              /* 10 个桶的头尾指针 */
    for(int exp=1; max/exp>0; exp*=10){       /* 依次按个位、十位、百位... */
        for(int i=0;i<10;++i){ bHead[i]=bTail[i]=NULL; }
        /* 分配：按当前位数字入桶，保持相对顺序 */
        Node *cur=head; 
        while(cur){ 
            int d=(cur->key/exp)%10;
            if(!bHead[d]){ bHead[d]=bTail[d]=cur; }
            else { bTail[d]->next=cur; bTail[d]=cur; }
            Node *next=cur->next; cur->next=NULL; cur=next; 
        }
        /* 收集：按桶序连接成新链 */
        head=tail=NULL; 
        for(int i=0;i<10;++i){ 
            if(!bHead[i]) continue; 
            if(!head){ head=bHead[i]; tail=bTail[i]; } 
            else { tail->next=bHead[i]; tail=bTail[i]; } 
        }
    }
    /* 回写到数组并释放链表 */
    Node *cur=head; 
    for(int i=0;i<n;++i){ a[i]=cur->key; Node *next=cur->next; free(cur); cur=next; }
}

/**************** 测试 ****************/

int main(){
    /* 构造两组测试数据，覆盖负数和重复值场景 */
    int base1[] = {29,10,14,37,13,5,25,17,8,33,2,2,-1,0,19};
    int n1 = sizeof(base1)/sizeof(base1[0]);
    int *buf=(int*)malloc(sizeof(int)*n1);

    copy_array(buf,base1,n1); insertion_sort(buf,n1); printf("insertion: "); print_array("",buf,n1);
    copy_array(buf,base1,n1); shell_sort(buf,n1);     printf("shell    : "); print_array("",buf,n1);
    copy_array(buf,base1,n1); selection_sort(buf,n1); printf("selection: "); print_array("",buf,n1);
    copy_array(buf,base1,n1); bubble_sort(buf,n1);    printf("bubble   : "); print_array("",buf,n1);
    copy_array(buf,base1,n1); gnome_sort(buf,n1);     printf("gnome    : "); print_array("",buf,n1);
    copy_array(buf,base1,n1); merge_sort(buf,n1);     printf("merge    : "); print_array("",buf,n1);
    copy_array(buf,base1,n1); quick_sort(buf,n1);     printf("quick    : "); print_array("",buf,n1);
    copy_array(buf,base1,n1); heap_sort(buf,n1);      printf("heap     : "); print_array("",buf,n1);
    copy_array(buf,base1,n1); counting_sort(buf,n1);  printf("counting : "); print_array("",buf,n1);
    copy_array(buf,base1,n1); tournament_sort(buf,n1);printf("tournament: "); print_array("",buf,n1);

    int base2[] = {170,45,75,90,802,24,2,66};
    int n2 = sizeof(base2)/sizeof(base2[0]);
    int *buf2=(int*)malloc(sizeof(int)*n2);
    copy_array(buf2,base2,n2); radix_sort_linked(buf2,n2); printf("radix(linked, nonneg): "); print_array("",buf2,n2);

    // 验证：再次排序并检查有序性
    printf("sorted(n1, quick)= %s\n", is_sorted(buf,n1)?"true":"false");
    copy_array(buf,base1,n1); quick_sort(buf,n1); printf("sorted(n1, quick)= %s\n", is_sorted(buf,n1)?"true":"false");
    copy_array(buf2,base2,n2); radix_sort_linked(buf2,n2); printf("sorted(n2, radix)= %s\n", is_sorted(buf2,n2)?"true":"false");

    free(buf); free(buf2);
    return 0;
}
