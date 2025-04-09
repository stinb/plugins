#include "header.h"

/* file2.c */
extern int32_t f; /* UndCC_FalseNeg - flagged on file1 - not consistent with
 either file1.c or header.h */

extern alignas(8) int32_t g; /* UndCC_Violation - undefined behaviour because of
inconsistency with file1.c */

extern alignas(8) int32_t h; /* UndCC_Violation - not consistent with file1.c
and undefined behaviour */
