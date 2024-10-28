#include <stdio.h>

int main(void)
{
    FILE *my_stdout = stdout;
    if (fputs("Hello, World!\n", my_stdout) == EOF)
    {
        /* Handle error */
    }
    return 0;
}