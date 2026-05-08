#ifndef MISRA12_DIR_4_7_TEST_H
#define MISRA12_DIR_4_7_TEST_H

#include <stdio.h>
#include <stdlib.h>

static inline void hdr_f(void)
{
    void *p;
    p = malloc(100);                           // UndCC_Valid
    malloc(100);                               // UndCC_Violation
    (void)p;
}

#endif
