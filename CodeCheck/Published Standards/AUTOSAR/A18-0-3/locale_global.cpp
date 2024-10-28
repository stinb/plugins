#include <locale>

void f1()
{
    std::locale::global(std::locale::classic()); // UndCC_Violation(1)
}
