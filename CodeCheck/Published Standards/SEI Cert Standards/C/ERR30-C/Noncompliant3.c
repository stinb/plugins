#include <errno.h>
#include <stdio.h>

void func(const char *filename)
{
    FILE *fileptr;

    errno = 0;
    fileptr = fopen(filename, "rb");  // UNDCC_FalseNeg - may fail to diagnose errors because fopen() might not 
                                      // set errno even if an error occurs
    if (errno != 0)
    {
        /* Handle error */
    }
}