#include <stdio.h>

enum
{
    BUFFER_SIZE = 1024
};
void func(FILE *file)
{
    char buf[BUFFER_SIZE];

    if (fgets(buf, sizeof(buf), file) == NULL)  //UndCC_Violation
    {
        /* Set error flag and continue */
    }
}
