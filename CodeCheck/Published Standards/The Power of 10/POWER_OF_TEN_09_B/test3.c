#include <stdio.h>

#define deref_1(x) *x = 0 // UndCC_Violation(0) - dereference operation in macro definition

typedef int INT;
typedef INT *INTPTR; // UndCC_Violation - dereferencing in typedef

void fp(int x)
{
}

void foo()
{
    int a, b;
    int *ptr, *ptr2;
    int **ptr3, **ptr4; // UndCC_Violation(1,2)

    ptr = &a;
    ptr2 = &b;

    a = *ptr + *ptr2;

    a = **ptr3; // UndCC_Valid - violation in other part

    if (a == **ptr4) // UndCC_Valid - violation in other part
        printf("");

    void (*func_ptr)(int) = fp;   // UndCC_Violation - function pointer
    void (*func_ptr2)(int) = &fp; // UndCC_Violation - function pointer
}
