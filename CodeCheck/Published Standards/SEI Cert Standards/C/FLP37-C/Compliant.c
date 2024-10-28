// SEI CERT C

#include <stdbool.h>
#include <string.h>

struct S
{
    int i;
    float f;
};

bool are_equal(const struct S *s1, const struct S *s2)
{
    if (!s1 && !s2)
        return true;
    else if (!s1 || !s2)
        return false;
    return s1->i == s2->i &&
           s1->f == s2->f;
}
