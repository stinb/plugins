#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

enum
{
    BUFFERSIZE = 32
};

void func(void)
{
    char buf[BUFFERSIZE];

    if (gets_s(buf, sizeof(buf)) == NULL)
    {
        /* Handle error */
    }
}