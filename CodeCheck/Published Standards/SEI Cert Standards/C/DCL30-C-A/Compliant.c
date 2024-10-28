#include <stddef.h>

const char *p;
char local[10];

void init_array(char *array, size_t len)
{
    /* Initialize array */
    return;
}

int main(void)
{
    char array[10];
    init_array(array, sizeof(array) / sizeof(array[0]));
    /* ... */
    return 0;
}

void this_is_OK(void)
{
    const char c_str[] = "Everything OK";
    const char *p = c_str;
    /* ... */
}

void is_this_OK(void)
{
    const char c_str[] = "Everything OK?";
    p = c_str;
    /* ... */
    p = NULL;
}

void squirrel_away(char **ptr_param)
{
    /* Initialize array */
    *ptr_param = local;
}

void rodent(void)
{
    char *ptr;
    squirrel_away(&ptr);
    /* ptr is valid in this scope */
}