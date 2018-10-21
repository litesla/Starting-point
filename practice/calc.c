/*************************************************************************
	> File Name: 20.c
	> Author: x
	> Mail: x.com
	> Created Time: 2018年10月21日 星期日 14时50分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 0x3f3f3f3f

int calc(const char *str, int l, int r) {
    int pos = -1, temp_prior = 0, prior = INF - 1;
    for (int i = l; i <= r; i++) {
        int cur_prior = INF;
        switch (str[i]) {
            case '(' : temp_prior += 100; break;
            case ')' : temp_prior -= 100; break;
            case '+' : cur_prior = temp_prior + 1; break;
            case '-' : cur_prior = temp_prior + 1; break;
            case '*' : cur_prior = temp_prior + 2; break;
            case '/' : cur_prior = temp_prior + 2; break;
        }
        if (cur_prior <= prior) {
            prior = cur_prior;
            pos = i;
        }
    }
    if (pos == -1) {
        int num = 0;
        for (int i = l; i <= r; i++) {
            if (str[i] < '0' || str[i] > '9') continue;
            num = num * 10 + str[i] - '0';
        }
        return num;
    } else {
        int a = calc(str, l, pos - 1); 
        int b = calc(str, pos + 1, r);
        switch (str[pos]) {
            case '+' : return a + b;
            case '-' : return a - b;
            case '*' : return a * b;
            case '/' : return a / b;
        }
    }
    return 0;
}

int main() {
    char str[100];
    int i = 0;
    while (scanf("%[^\n]s", str) != EOF) {
        getchar();
        printf("%s = %d\n", str, calc(str, 0, strlen(str) - 1));
        memset(str, 0, sizeof(str));
    }
    return 0;
}
