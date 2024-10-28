#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *out;
    FILE *in;
    size_t size;
    char *ptr;

    if (argc != 2)
    {
        /* Handle error */
    }

    in = fmemopen(argv[1], strlen(argv[1]), "r");
    /* Use in */
    printf(in); // UndCC_Violation

    out = open_memstream(&ptr, &size);
    /* Use out */
    printf(out); // UndCC_Violation

    return 0;
}