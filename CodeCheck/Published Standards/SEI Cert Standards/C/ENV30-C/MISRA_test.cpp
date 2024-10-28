#include <clocale>
#include <cstring>

void f1()
{
    char *s1 = setlocale(LC_ALL, 0);
    struct lconv *conv = localeconv();
    s1[1] = 'A';               // UndCC_Violation
    conv->decimal_point = "^"; // UndCC_Violation
}

void f2()
{
    char str[128];
    (void)strcpy(str,
                 setlocale(LC_ALL, 0));
    // strcpy takes a const char *
    const struct lconv *conv = localeconv();
    // conv->decimal_point = "^";               // Ill-formed
}

void f3()
{
    const struct lconv *conv = localeconv();
    conv->grouping[2] = 'x'; // UndCC_Violation
}