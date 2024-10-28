
#include <stdlib.h>

void exit1(void)
{
    /* ... Cleanup code ... */
    return;
}

void exit2(void)
{
    extern int some_condition;
    if (some_condition)
    {
        /* ... More cleanup code ... */
        exit(0); // UndCC_Violation
    }
    return;
}

int main(void)
{
    if (atexit(exit1) != 0)
    {
        /* Handle error */
    }
    if (atexit(exit2) != 0)
    {
        /* Handle error */
    }
    /* ... Program code ... */
    return 0;
}