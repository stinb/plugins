
#include <stdlib.h>

enum
{
    BUFFER_SIZE = 32
};

int f(void)
{
    static char *text_buffer = NULL;
    if (text_buffer == NULL)
    {
        text_buffer = (char *)malloc(BUFFER_SIZE);
        if (text_buffer == NULL)
        {
            return -1;
        }
    }
    return 0;
}