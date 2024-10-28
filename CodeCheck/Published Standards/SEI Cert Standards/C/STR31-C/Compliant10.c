#include <stdlib.h>
#include <string.h>

void func(void)
{
    char *buff;
    char *editor = getenv("EDITOR");
    if (editor == NULL)
    {
        /* EDITOR environment variable not set */
    }
    else
    {
        size_t len = strlen(editor) + 1;
        buff = (char *)malloc(len);
        if (buff == NULL)
        {
            /* Handle error */
        }
        memcpy(buff, editor, len);
        free(buff);
    }
}