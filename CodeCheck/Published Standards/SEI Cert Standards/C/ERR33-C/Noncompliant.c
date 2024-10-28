#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

extern void log_message(const char *);

int utf8_to_wcs(wchar_t *wcs, size_t n, const char *utf8,
                size_t *size)
{
    if (NULL == size)
    {
        return -1;
    }
    setlocale(LC_CTYPE, "en_US.UTF-8"); // UndCC_Violation
    *size = mbstowcs(wcs, utf8, n);
    return 0;
}

size_t read_at(FILE *file, long offset,
               void *buf, size_t nbytes)
{
    fseek(file, offset, SEEK_SET); // UndCC_Violation
    return fread(buf, 1, nbytes, file);
}

void f(int i, int width, int prec)
{
    char buf[40];
    snprintf(buf, sizeof(buf), "i = %*.*i", width, prec, i); // UndCC_Violation(win,lin)
    log_message(buf);
    /* ... */
}
