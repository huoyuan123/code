// PTA 7.3 字符串关键字的散列映射
// 哈希值取关键字最后最多 3 个字符映射到整数，然后取余；冲突用平方探测

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hash_key(const char *s, int P) {
    int len = (int)strlen(s);
    int start = len > 3 ? len - 3 : 0;
    int v = 0;
    for (int i = start; i < len; ++i) {
        v = v * 32 + (s[i] - 'A');
    }
    return v % P;
}

int main(void) {
    int N, P;
    if (scanf("%d %d", &N, &P) != 2) return 0;

    unsigned char *used = (unsigned char *)calloc((size_t)P, 1);
    char *slots = (char *)malloc((size_t)P * 9); // 每槽最多8字符+\0
    if (!used || !slots) {
        free(used);
        free(slots);
        return 0;
    }

    char key[64];
    for (int i = 0; i < N; ++i) {
        if (scanf("%63s", key) != 1) {
            free(used);
            free(slots);
            return 0;
        }
        int h = hash_key(key, P);
        int pos = -1;
        for (int k = 0; k < P; ++k) {
            int idx;
            if (k == 0) {
                idx = h;
            } else if (k % 2 == 1) {
                int d = ((k + 1) / 2);
                d = (d * d) % P;
                idx = (h + d) % P; // 正方向：+i^2
            } else {
                int d = (k / 2);
                d = (d * d) % P;
                idx = (h + P - d) % P; // 负方向：-i^2，保证非负
            }
            if (!used[idx]) {
                // 空槽：插入并记录位置
                size_t len = strlen(key);
                if (len > 8) len = 8;
                memcpy(slots + idx * 9, key, len);
                slots[idx * 9 + len] = '\0';
                used[idx] = 1;
                pos = idx;
                break;
            } else if (strcmp(slots + idx * 9, key) == 0) {
                // 重复关键字：返回同一位置
                pos = idx;
                break;
            }
        }
        if (i) printf(" ");
        printf("%d", pos);
    }
    printf("\n");
    free(slots);
    free(used);
    return 0;
}
