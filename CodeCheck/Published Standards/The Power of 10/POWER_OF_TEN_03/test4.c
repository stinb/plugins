#include <stdlib.h>

void foo()
{
    int *ptr;
    int *ptr_2;
    int n;

    ptr = (int *)malloc(n * sizeof(int));  // UndCC_Violation - use of malloc
    ptr_2 = (int *)calloc(n, sizeof(int)); // UndCC_Violation - use of calloc

    // ...

    free(ptr);   // UndCC_Violation - use of free
    ptr_2 = ptr;

    // ...

    ptr_2 = realloc(ptr_2, n * sizeof(int)); // UndCC_Violation - use of realloc
}
