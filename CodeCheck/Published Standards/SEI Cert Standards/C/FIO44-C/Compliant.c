#include <stdio.h>
#include <string.h>

int opener(FILE *file)
{
    int rc;
    fpos_t offset;

    if (file == NULL)
    {
        return -1;
    }

    rc = fgetpos(file, &offset);
    if (rc != 0)
    {
        return rc;
    }

    /* Read in data from file */

    rc = fsetpos(file, &offset);  // UndCC_Valid
    if (rc != 0)
    {
        return rc;
    }

    return 0;
}
