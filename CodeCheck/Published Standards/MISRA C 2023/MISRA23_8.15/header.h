#include <stdalign.h>
#include <stdint.h>

typedef float float32_t;

/* header.h - #included by both file1.c and file2.c */
extern alignas(16) int32_t a;
extern alignas(0) int32_t b;
extern int32_t c;
extern int32_t d;
extern alignas(16) int32_t e;
extern alignas(16) int32_t f;
extern int32_t g;
extern alignas(float) int32_t i;
extern alignas(float) int32_t j;
extern alignas(float) int32_t k;
extern alignas(float) int32_t l;
extern alignas(float32_t) int32_t m;
