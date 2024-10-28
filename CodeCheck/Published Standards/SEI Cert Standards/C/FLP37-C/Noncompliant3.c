// Custom

#include <stdbool.h>
#include <string.h>

union U
{
    int i;
    long double d;
};

bool are_equal(const union U *u1, const union U *u2)
{
    if (!u1 && !u2)
        return true;
    else if (!u1 || !u2)
        return false;
    return 0 == memcmp(u1, u2, sizeof(union U)); // UndCC_Violation
}
