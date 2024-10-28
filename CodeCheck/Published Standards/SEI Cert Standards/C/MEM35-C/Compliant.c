#include <stdlib.h>
#include <stdint.h>
#include <time.h>

struct tm *make_tm(int year, int mon, int day, int hour,
                   int min, int sec)
{
    struct tm *tmb;
    tmb = (struct tm *)malloc(sizeof(*tmb));
    if (tmb == NULL)
    {
        return NULL;
    }
    *tmb = (struct tm){
        .tm_sec = sec, .tm_min = min, .tm_hour = hour, .tm_mday = day, .tm_mon = mon, .tm_year = year};
    return tmb;
}

void function(size_t len)
{
    long *p;
    if (len == 0 || len > SIZE_MAX / sizeof(long))
    {
        /* Handle overflow */
    }
    p = (long *)malloc(len * sizeof(long));
    if (p == NULL)
    {
        /* Handle error */
    }
    free(p);
}

void function_2(size_t len)
{
    long *p;
    if (len == 0 || len > SIZE_MAX / sizeof(*p))
    {
        /* Handle overflow */
    }
    p = (long *)malloc(len * sizeof(*p));
    if (p == NULL)
    {
        /* Handle error */
    }
    free(p);
}