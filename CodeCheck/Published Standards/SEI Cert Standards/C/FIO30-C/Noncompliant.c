#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void incorrect_password(const char *user)
{
    int ret;
    /* User names are restricted to 256 or fewer characters */
    static const char msg_format[] = "%s cannot be authenticated.\n";
    size_t len = strlen(user) + sizeof(msg_format);
    char *msg = (char *)malloc(len);
    if (msg == NULL)
    {
        /* Handle error */
    }
    ret = snprintf(msg, len, msg_format, user);
    if (ret < 0)
    {
        /* Handle error */
    }
    else if (ret >= len)
    {
        /* Handle truncated output */
    }
    fprintf(stderr, msg); // UndCC_Violation
    free(msg);
}