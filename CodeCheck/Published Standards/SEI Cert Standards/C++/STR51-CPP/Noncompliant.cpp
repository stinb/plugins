#include <cstdlib>
#include <string>

void f()
{
    std::string tmp(std::getenv("TMP"));   // UndCC_Violation
    std::string tmp2 = std::getenv("TMP"); // UndCC_Violation
    if (!tmp.empty())
    {
        // ...
    }
}
