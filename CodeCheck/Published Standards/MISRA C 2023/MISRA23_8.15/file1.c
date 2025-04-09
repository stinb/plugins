#include "header.h"

/* file1.c */
alignas(16) int32_t a; /* UndCC_Valid - same explicit alignment */
// alignas(16) int32_t b;        /* Error on strict parser - not consistently explicit */
alignas(16) int32_t c; /* UndCC_Violation - not consistently explicit */
int32_t d;             /* UndCC_Valid - not manually aligned */
// int32_t e;                    /* Error on strict parser - constraint violation */
alignas(16) int32_t f;        /* UndCC_Violation because of file2.c */
alignas(16) int32_t g;        /* UndCC_Violation, and undefined because of file2.c */
extern alignas(16) int32_t h; /* UndCC_Violation with file2.c */
alignas(float) int32_t i;     /* UndCC_Valid - same type used */
// alignas(double) int32_t j;    /* Error on strict parser - different type, and therefore may be a constraint violation */
alignas(4) int32_t k;         /* UndCC_Violation - regardless of the size of float */
alignas(float32_t) int32_t l; /* UndCC_Violation - potentially a different type on
a different platform */
alignas(float32_t) int32_t m; /* UndCC_Valid - same type used by name */
