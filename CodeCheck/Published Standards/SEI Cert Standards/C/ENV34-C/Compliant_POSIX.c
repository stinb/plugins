#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void func(void)
{
    char *tmpvar;
    char *tempvar;

    const char *temp = getenv("TMP");
    if (temp != NULL)
    {
        tmpvar = strdup(temp);
        if (tmpvar == NULL)
        {
            /* Handle error */
        }
    }
    else
    {
        /* Handle error */
    }

    temp = getenv("TEMP");
    if (temp != NULL)
    {
        tempvar = strdup(temp);
        if (tempvar == NULL)
        {
            /* Handle error */
        }
    }
    else
    {
        /* Handle error */
    }

    if (strcmp(tmpvar, tempvar) == 0)
    {
        printf("TMP and TEMP are the same.\n");
    }
    else
    {
        printf("TMP and TEMP are NOT the same.\n");
    }
    free(tmpvar);
    tmpvar = NULL;
    free(tempvar);
    tempvar = NULL;
}