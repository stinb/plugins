#define __STDC_WANT_LIB_EXT1__ 1
#include <errno.h>
#include <stdio.h>
#include <string.h>

enum
{
    BUFFERSIZE = 64
};
void f(FILE *fp)
{
    fpos_t pos;
    errno = 0;

    if (0 != fgetpos(fp, &pos))
    {
        char errmsg[BUFFERSIZE];
        if (strerror_s(errmsg, BUFFERSIZE, errno) != 0)
        {
            /* Handle error */
        }
        printf("Could not get the file position: %s\n", errmsg);
    }
}