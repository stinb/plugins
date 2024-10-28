
#include <errno.h>
#include <stdio.h>
#include <string.h>

void f(FILE *fp)
{
    fpos_t pos;
    errno = 0;

    if (0 != fgetpos(fp, &pos))
    {
        char *errmsg = strerror(errno); // UNDCC_Violation, strerror() is not required to avoid data races
        printf("Could not get the file position: %s\n", errmsg);
    }
}