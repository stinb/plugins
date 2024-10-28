#include <time.h>

enum
{
    maxsize = 26
};

void func(struct tm *time)
{
    char s[maxsize];
    /* Current time representation for locale */
    const char *format = "%c";

    size_t size = strftime(s, maxsize, format, time);
}