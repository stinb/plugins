#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>

enum
{
    maxsize = 26
};

void func(struct tm *time_tm)
{
    char buffer[maxsize];

    if (asctime_s(buffer, maxsize, &time_tm))
    {
        /* Handle error */
    }
}