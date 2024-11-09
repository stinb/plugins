#include <cstdint>
#include <cstring>

void f1(const uint8_t *src, uint8_t *dst, size_t len)
{
    memmove(dst, src, len); // UndCC_Violation
}

struct S
{
    bool m1;
    // There may be padding here
    int64_t m2;
};

void f2(S s1, S s2)
{
    if (memcmp(&s1, &s2, sizeof(S)) != 0) // UndCC_Violation
    {
    }
};

extern char buffer1[12];
extern char buffer2[12];

void f3()
{
    strcpy(buffer1, "abc");                             // Indeterminate contents in elements 4 to 11
    strcpy(buffer2, "abc");                             // Indeterminate contents in elements 4 to 11
    if (memcmp(buffer1, buffer2, sizeof(buffer1)) != 0) // UndCC_Violation
    {
    }
}
