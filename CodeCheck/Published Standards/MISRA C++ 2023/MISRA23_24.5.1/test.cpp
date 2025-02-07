#include <cctype>
#include <locale>

void f(char c)
{
    if (std::isdigit(c)) // UndCC_Violation
    {
    }
    if (std::isdigit(c, std::locale{})) // UndCC_Valid version of the above
    {
    }
}
