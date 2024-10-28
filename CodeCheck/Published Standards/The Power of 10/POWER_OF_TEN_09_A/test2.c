#include <stdio.h>

#define deref(x) *x = 0 // UndCC_Valid - violation in other part

typedef int INT;
typedef INT *INTPTR; // UndCC_Valid - violation in other part

void fp(int x)
{
}

void foo()
{
    int a, b;
    int *ptr, *ptr2;
    int **ptr3, **ptr4;

    ptr = &a;
    ptr2 = &b;

    a = *ptr + *ptr2;

    a = **ptr3; // UndCC_Violation - more than one level of dereferencing (assignment)

    if (a == **ptr4) // UndCC_Violation - more than one level of dereferencing (usage)
        printf("");

    void (*func_ptr)(int) = fp;   // UndCC_Valid - violation in other part
    void (*func_ptr2)(int) = &fp; // UndCC_Valid - violation in other part
}
