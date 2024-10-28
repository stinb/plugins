#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern void log_message(const char *);

int utf8_to_wcs(wchar_t *wcs, size_t n, const char *utf8,
                size_t *size)
{
    if (NULL == size)
    {
        return -1;
    }
    const char *save = setlocale(LC_CTYPE, "en_US.UTF-8"); // UndCC_Valid
    if (NULL == save)
    {
        return -1;
    }

    *size = mbstowcs(wcs, utf8, n);
    if (NULL == setlocale(LC_CTYPE, save))
    {
        return -1;
    }
    return 0;
}

size_t read_at(FILE *file, long offset,
               void *buf, size_t nbytes)
{
    if (fseek(file, offset, SEEK_SET) != 0) // UndCC_Valid
    {
        /* Indicate error to caller */
        return 0;
    }
    return fread(buf, 1, nbytes, file);
}

void f(int i, int width, int prec)
{
    char buf[40];
    int n;
    n = snprintf(buf, sizeof(buf), "i = %*.*i", width, prec, i); // UndCC_Valid
    if (n < 0 || n >= sizeof(buf))
    {
        /* Handle snprintf() error */
        strcpy(buf, "unknown error");
    }
    log_message(buf);
}
