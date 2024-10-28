#include <stdlib.h>

enum
{
    BUFFER_SIZE = 32
};

int f(void)
{
    char *text_buffer = (char *)malloc(BUFFER_SIZE);    // UndCC_Violation
    if (text_buffer == NULL)
    {
        return -1;
    }
    return 0;
}
