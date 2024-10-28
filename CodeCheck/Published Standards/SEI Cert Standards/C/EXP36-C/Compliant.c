#include <assert.h>
#include <string.h>

struct foo_header
{
    int len;
    /* ... */
};

void func(void)
{
    char c = 'x';
    int i = c;
    int *ip = &i;

    assert(ip == &i);
}

int *loop_function(int *v_pointer)
{
    /* ... */
    return v_pointer;
}

void foo(int *loop_ptr)
{
    int *int_ptr = loop_function(loop_ptr);

    /* ... */
}

void foo_2(char *data, size_t offset)
{
    struct foo_header header;
    memcpy(&header, data + offset, sizeof(header));

    /* ... */
}
