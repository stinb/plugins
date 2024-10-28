#include <stdio.h>

enum
{
    BUFFERSIZE = 32
};

void func(void)
{
    char buf[BUFFERSIZE];
    int ch;
    size_t index = 0;
    size_t chars_read = 0;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (index < sizeof(buf) - 1)
        {
            buf[index++] = (char)ch;
        }
        chars_read++;
    }
    buf[index] = '\0'; /* Terminate string */
    if (ch == EOF)
    {
        /* Handle EOF or error */
    }
    if (chars_read > index)
    {
        /* Handle truncation */
    }
}