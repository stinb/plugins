#include <stdio.h>

int main(void)
{
    FILE my_stdout = *stdout;// UNDCC_Violation, by-value copy of stdout is being used in the call to fputs()
    if (fputs("Hello, World!\n", &my_stdout) == EOF) 
    {
        /* Handle error */
    }
    return 0;
}