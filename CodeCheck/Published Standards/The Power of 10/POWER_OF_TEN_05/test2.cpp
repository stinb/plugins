#include <cstdint>
#include <cassert>

const volatile int32_t int32_t_function(int32_t x, int32_t y)
{
    if (x == 0)
        return 0;
    else
        return x + y;
}

const volatile bool bool_function(int x, int y)
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
    assert(int32_t_function(x, y)); // UndCC_Violation - not a boolean condition

    // ...
    assert(bool_function(x, y));
}
