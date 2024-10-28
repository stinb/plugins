#include <stdlib.h>

void exit1(void)
{
    return;
}

int main(void)
{
    if (atexit(exit1) != 0)
    {
        /* Handle error */
    }
    return 0;
}