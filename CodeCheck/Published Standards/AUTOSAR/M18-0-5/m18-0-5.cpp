#include <cstring>

void fn(const char *pChar)
{
    char array[10];
    strcpy(array, pChar); // UndCC_Violation
}
