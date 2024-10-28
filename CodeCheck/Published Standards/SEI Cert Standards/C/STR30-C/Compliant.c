#include <stddef.h>
#include <stdio.h>
#include <string.h>

char *get_dirname(const char *pathname, char *dirname, size_t size)
{
    const char *slash;
    slash = strrchr(pathname, '/');
    if (slash)
    {
        ptrdiff_t slash_idx = slash - pathname;
        if ((size_t)slash_idx < size)
        {
            memcpy(dirname, pathname, slash_idx);
            dirname[slash_idx] = '\0';
            return dirname;
        }
    }
    return 0;
}

int main(void)
{
    char dirname[260];
    if (get_dirname(__FILE__, dirname, sizeof(dirname)))
    {
        puts(dirname);
    }
    return 0;
}