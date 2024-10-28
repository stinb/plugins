#include <cstdio> // UndCC_Violation
#include <cwchar> // UndCC_Violation(wchar)

void fn()
{
    char array[10];
    std::fgets(array, 10, stdin); // Use fgets instead of gets to prevent buffer overflow
}
