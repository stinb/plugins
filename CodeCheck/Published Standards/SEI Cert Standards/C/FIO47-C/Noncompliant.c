#include <stdio.h>

void func(void)
{
    const char *error_msg = "Resource not available to user.";
    int error_type = 3;
    /* ... */
    printf("Error (type %s): %d\n", error_type, error_msg); // UndCC_Violation
    /* ... */
}