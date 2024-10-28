#include <stdlib.h>
#include <string.h>

struct s
{
    char *c_str;
    /* Other members */
};

struct s *create_s(const char *c_str)
{
    struct s *ret;
    size_t len = strlen(c_str) + 1;

    ret = (struct s *)malloc(sizeof(struct s) + len);
    if (ret != NULL)
    {
        ret->c_str = (char *)(ret + 1);
        memcpy(ret + 1, c_str, len);
    }
    return ret;
}