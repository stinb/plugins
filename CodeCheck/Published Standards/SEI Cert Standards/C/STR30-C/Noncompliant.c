#include <stdio.h>
#include <string.h>

const char *get_dirname(const char *pathname)
{
    char *slash;
    slash = strrchr(pathname, '/');

    char *p = "string literal";
    p[0] = 'S'; /* UNDCC_Violation */

    if (slash)
    {
        *slash = '\0'; /* UNDCC_Violation, undefined behavior */
    }
    return pathname;
}

void func(void)
{
    mkstemp("/tmp/edXXXXXX"); // Erro in strict parser
}

int main(void)
{
    puts(get_dirname(__FILE__));
    return 0;
}
