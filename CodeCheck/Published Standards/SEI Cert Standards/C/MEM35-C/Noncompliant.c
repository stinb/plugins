#include <stdlib.h>
#include <stdint.h>
#include <time.h>

struct tm *make_tm(int year, int mon, int day, int hour,
                   int min, int sec)
{
    struct tm *tmb;
    tmb = (struct tm *)malloc(sizeof(tmb)); // UNDCC_Violation, inadequate space is allocated for a struct tm object
    if (tmb == NULL)
    {
        return NULL;
    }
    *tmb = (struct tm){
        .tm_sec = sec, .tm_min = min, .tm_hour = hour, .tm_mday = day, .tm_mon = mon, .tm_year = year};
    return tmb;
}
