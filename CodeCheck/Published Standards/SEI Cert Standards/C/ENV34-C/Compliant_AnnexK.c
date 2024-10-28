#define __STDC_WANT_LIB_EXT1__ 1
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void func(void)
{
    char *tmpvar;
    char *tempvar;
    size_t requiredSize;
    int err;
    err = getenv_s(&requiredSize, NULL, 0, "TMP");

    if (err)
    {
        /* Handle error */
    }

    tmpvar = (char *)malloc(requiredSize);
    if (!tmpvar)
    {
        /* Handle error */
    }
    err = getenv_s(&requiredSize, tmpvar, requiredSize, "TMP");

    if (err)
    {
        /* Handle error */
    }
    err = getenv_s(&requiredSize, NULL, 0, "TEMP");
    if (err)
    {
        /* Handle error */
    }

    tempvar = (char *)malloc(requiredSize);
    if (!tempvar)
    {
        /* Handle error */
    }
    err = getenv_s(&requiredSize, tempvar, requiredSize, "TEMP");

    if (err)
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