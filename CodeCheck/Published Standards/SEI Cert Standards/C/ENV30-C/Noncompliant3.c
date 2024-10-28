#include <locale.h>
#include <stdio.h>
#include <time.h>


void trstr2(char *c_str, char orig, char rep)
{
    while (*c_str != '\0')
    {
        if (*c_str == orig)
        {
            *c_str = rep;
        }
        ++c_str;
    }
}

void func2(void)
{
    time_t currtime;
    struct tm *timer;
    char buffer[80];

    time( &currtime );
    timer = localtime( &currtime );

    char *locale = setlocale(LC_ALL, "en_GB");
    if (locale == NULL)
    {
        /* Handle error */
    }
    trstr2(locale, '"', '_');       // UndCC_Violation
}
