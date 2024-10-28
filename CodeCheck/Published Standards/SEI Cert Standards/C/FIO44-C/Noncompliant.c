
#include <stdio.h>
#include <string.h>

int opener(FILE *file)
{
    int rc;
    fpos_t offset;

    memset(&offset, 0, sizeof(offset));

    if (file == NULL)
    {
        return -1;
    }

    /* Read in data from file */

    rc = fsetpos(file, &offset);        //UndCC_Violation
    if (rc != 0)
    {
        return rc;
    }

    return 0;
}
