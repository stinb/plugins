#include <errno.h>
#include <stdio.h>

void func(FILE *fp)
{
    errno = 0;
    ftell(fp);  // UNDCC_FalseNeg - examines errno without first checking whether the out-of-band indicator
                // returned by ftell() indicates an error
    if (errno)
    {
        perror("ftell");
    }
}