#include <cstring>

struct S
{
    unsigned char buffType;
    int size;
};

void f(const S &s1, const S &s2)
{
    if (!std::memcmp(&s1, &s2, sizeof(S)))      // UndCC_Violation
    {
        // ...
    }
}
