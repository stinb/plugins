#include <stdlib.h>
#include <string.h>

void process_data(void)
{
    /* Allocate memory using a standard library function */
    size_t len = 32U;
    char *buffer = (char *)malloc(len);

    if (buffer != NULL)
    {
        /* Use the allocated memory safely */
        (void)memset(buffer, 0, len);

        /* Free exactly the same pointer that malloc returned */
        free(buffer);
        buffer = NULL; /* Defensive reset to prevent accidental reuse */
    }
}

/* Another compliant example with realloc */
void resize_example(void)
{
    int *data = (int *)malloc(10U * sizeof(int));
    if (data != NULL)
    {
        int *temp = (int *)realloc(data, 20U * sizeof(int));
        if (temp != NULL)
        {
            data = temp;
        }
        /* Regardless of success, free the allocated block */
        free(data);
        data = NULL;
    }
}
