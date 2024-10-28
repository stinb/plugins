#include <assert.h>

typedef void (*handler_type)(int);

void execute_handler(handler_type handler, int value)
{
    handler(value);
}

static void assert_handler(int value)
{
    assert(value);
}

void func(int e)
{
    execute_handler(&assert_handler, e < 0);
}