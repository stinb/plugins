#include <stdlib.h>

int main()
{
    if (system(NULL)) // UndCC_Violation
        puts("Ok");
    else
        exit(EXIT_FAILURE);
}