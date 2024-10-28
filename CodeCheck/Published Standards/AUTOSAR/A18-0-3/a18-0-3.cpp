#include <clocale>  // UndCC_Violation(0)

int main()
{
    setlocale(LC_MONETARY, "en_US.utf8");  // UndCC_Violation
    struct lconv *lc = localeconv();
    return 0;
}
