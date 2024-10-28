#include <stdio.h>
#include <wchar.h>

enum
{
    BUFFER_SIZE = 1024
};
void func(FILE *file)
{
    wchar_t buf[BUFFER_SIZE];

    if (fgetws(buf, sizeof(buf), file) == NULL)  //UndCC_Violation
    {
        /* Set error flag and continue */
    }
}
