#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum
{
    MAX_ALLOCATION = 1000
};

int main(int argc, const char *argv[])
{
    char *c_str = NULL;
    size_t len;

    if (argc == 2)
    {
        len = strlen(argv[1]) + 1;
        if (len > MAX_ALLOCATION)
        {
            /* Handle error */
        }
        c_str = (char *)malloc(len);
        if (c_str == NULL)
        {
            /* Handle error */
        }
        strcpy(c_str, argv[1]);
    }
    else
    {
        printf("%s\n", "usage: $>a.exe [string]");
        return EXIT_FAILURE;
    }
    free(c_str);
    return 0;
}