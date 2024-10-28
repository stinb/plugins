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
        tmpvar = (char *)malloc(strlen(temp) + 1);
        if (tmpvar != NULL)
        {
            strcpy(tmpvar, temp);
        }
        else
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
        tempvar = (char *)malloc(strlen(temp) + 1);
        if (tempvar != NULL)
        {
            strcpy(tempvar, temp);
        }
        else
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
    free(tempvar);
}