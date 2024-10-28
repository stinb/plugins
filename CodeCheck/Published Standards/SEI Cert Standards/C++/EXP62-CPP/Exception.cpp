#include <cstring>

struct S
{
    int i : 10;
    int j;
};

void f(const S &s1)
{
    S s2;
    std::memcpy(&s2, &s1, sizeof(S));
}