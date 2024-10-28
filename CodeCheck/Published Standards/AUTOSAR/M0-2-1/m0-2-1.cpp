#include <cstdint>

struct s
{
    int16_t m1[32];
};
struct t
{
    int32_t m2;
    struct s m3;
};
void fn()
{
    union // Breaks Rule 9–5–1
    {
        struct s u1;
        struct t u2;
    } a;
    union // Breaks Rule 9–5–1
    {
        struct s u1;
        struct t u2;
    } b;
    a.u2.m3 = a.u1;    // UndCC_Violation
    b.u2 = a.u2;       // UndCC_Valid
    b.u2.m3 = a.u2.m3; // UndCC_Valid
}