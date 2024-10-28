#include <stdlib.h>
#include <string.h>

void func(void)
{
    char buff[256];
    char *editor = getenv("EDITOR");
    if (editor == NULL)
    {
        /* EDITOR environment variable not set */
    }
    else
    {
        strcpy(buff, editor); // UndCC_Violation(Win,Lin)
    }
}
