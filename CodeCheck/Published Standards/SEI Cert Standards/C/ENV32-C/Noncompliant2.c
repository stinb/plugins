
#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;
int val;

void exit1(void)
{
    longjmp(env, 1); // UndCC_Violation
}

int main(void)
{
    if (atexit(exit1) != 0)
    {
        /* Handle error */
    }
    if (setjmp(env) == 0)
    {
        exit(0);
    }
    else
    {
        return 0;
    }
}