#include <stdio.h>
#include <string.h>

void f ( const char *string )
{
    int length0 = strlen(string); // UndCC_Violation

    int length1;
    length1 = strlen(string);     // UndCC_Violation
}
