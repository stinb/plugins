#include <clocale>
#include <string>
#include <iostream>

void f1()
{
    const struct lconv *lc = localeconv();
    std::string copy{lc->int_curr_symbol};
    const char *res = std::setlocale(LC_MONETARY, "fr_FR");

    std::cout << lc->int_curr_symbol; // UndCC_Violation - use after setlocale called
    std::cout << copy;                // UndCC_Valid - copy made before call
    std::cout << res;                 // UndCC_Valid - no subsequent call before use
}
