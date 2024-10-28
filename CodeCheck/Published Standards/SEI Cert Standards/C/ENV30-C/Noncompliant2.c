#include <locale.h>

void f2(void)
{
    struct lconv *conv = localeconv();

    if ('\0' == conv->decimal_point[0])
    {
        conv->decimal_point = ".";      // UndCC_Violation
    }
}
