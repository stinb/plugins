#include <cstdint>
#include <cassert>

static_assert((sizeof(int) == 4), "Bad size"); // Rule does not apply

void f(int32_t i)
{
    assert(i < 1000); // UndCC_Valid - not constant

    if (i >= 0)
    {
        assert((sizeof(int) == 4) && "Bad size"); // UndCC_Violation - constant
    }
    else
    {
        assert(false && "i is negative"); // UndCC_Valid by exception
    }
}
