#include <string.h>

union u
{
    char c;
    int i;
    char buffer[13];
};

void compare2(const union u *left, const union u *right)
{
    if ((left && right) &&
        (0 == memcmp(left, right, sizeof(union u)))) /* UNDCC_Violation - memcmp used to compare contents of two unions */
    {
        /* ... */
    }
}
