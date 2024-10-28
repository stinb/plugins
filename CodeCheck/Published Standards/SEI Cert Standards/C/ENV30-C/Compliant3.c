#include <locale.h>
#include <stdlib.h>
#include <string.h>

void f2(void)
{
    const struct lconv *conv = localeconv();
    if (conv == NULL)
    {
        /* Handle error */
    }

    struct lconv *copy_of_conv = (struct lconv *)malloc(
        sizeof(struct lconv));
    if (copy_of_conv == NULL)
    {
        /* Handle error */
    }

    memcpy(copy_of_conv, conv, sizeof(struct lconv));

    if ('\0' == copy_of_conv->decimal_point[0])
    {
        copy_of_conv->decimal_point = ".";
    }
    /* ... */
    free(copy_of_conv);
}