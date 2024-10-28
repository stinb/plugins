
#include <stdio.h>

void func_3(const char *file_name)
{
    FILE *fptr;

    int c = getc(fptr = fopen(file_name, "r")); // UNDCC_Violation, calls the getc() function with an expression as the stream argument
    if (feof(fptr) || ferror(fptr))
    {
        /* Handle error */
    }

    if (fclose(fptr) == EOF)
    {
        /* Handle error */
    }
}

