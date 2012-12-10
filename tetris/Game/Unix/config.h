#include<stdio.h>

typedef unsigned char Boolean;

/*
#define assert(x) { if ((x) == 0) \
        { printf("Assert failed: "); } }
//        { printf("Assert failed: "); exit(1); } }

*/

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif
#define nil NULL

#ifdef __cplusplus
extern "C" int rand();
#endif

/* EOF */
