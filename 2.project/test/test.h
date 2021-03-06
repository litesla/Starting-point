/*************************************************************************
	> File Name: test.h
	> Author: hug
	> Mail:   hug@haizeix.com
	> Created Time: 日 10/ 7 09:22:43 2018
 ************************************************************************/

#ifndef _TEST_H
#define _TEST_H
#include <stdlib.h>
#include <math.h>

struct TestFuncData {
    int total, expect;
};

typedef void (*test_func_t)(struct TestFuncData *);
struct FuncData {
    const char *a_str, *b_str;
    test_func_t func;
    struct FuncData *next;
};

void addFuncData(
    const char *a, 
    const char *b, 
    test_func_t func 
); 

#define TEST(a, b) \
    void a##_haizeix_##b(struct TestFuncData *); \
    __attribute__((constructor)) \
    void ADDFUNC_##a##_haizeix_##b() { \
        addFuncData(#a, #b, a##_haizeix_##b); \
    } \
    void a##_haizeix_##b(struct TestFuncData *__data)

#define EXPECT(a, b) ({ \
    int temp; \
    printf("%s = %d %s\n", #a, b, (temp = (a == b)) ? "True" : "False"); \
    __data->total += 1; \
    __data->expect += temp; \
})

#define EXPECT_lt(a, b) ({ \
    int temp; \
    printf("%lf = %lf %s\n", a, b, (temp = (fabs(b - a) < 1e-7)) ? "True" : "False");\
    __data->total += 1;\
    __data->expect += temp;\
})

int RUN_ALL_TEST();

#endif
