#include <stdio.h>

enum
{
    BUF_LENGTH = 1024
};

void get_data(void)
{
    char buf[BUF_LENGTH];
    if (1 != fscanf(stdin, "%1023s", buf))
    {
        /* Handle error */
    }

    /* Rest of function */
}