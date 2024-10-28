#include <assert.h>
#include <stdbool.h>

int int_function(int x, int y)
{
    if (x == 0)
        return 0;
    else
        return x + y;
}

bool bool_function(int x, int y)
{
    if (x == 0)
        return 0;
    else
        return x + y;
}

_Bool _Bool_function(int x, int y)
{
    if (x == 0)
        return 0;
    else
        return x + y;
}

void test_func(int x, int y)
{
    // ...
    assert(x == y);
    // ...
}

void test_func2(int x, int y)
{
    // ...
    assert(x == y);

    // ...
    assert(int_function(x, y)); // UndCC_Violation - not a boolean condition

    // ...
    assert(bool_function(x, y));

    // ...
    assert(_Bool_function(x, y));
}
