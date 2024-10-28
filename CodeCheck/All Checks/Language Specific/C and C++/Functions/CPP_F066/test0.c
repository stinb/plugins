#include <stdio.h>

static unsigned depth;
static const unsigned interval = 1000;

static inline void dive() {
    depth += 1;
    if (depth % interval == 0) printf("%u\n", depth);
}

unsigned fac0(unsigned n)
{
    dive();
    if (n < 2) return 1;
    return n * fac0(n - 1); // UndCC_Violation - the return value is modified
}

unsigned fac1(unsigned n)
{
    dive();
    if (n < 2) return 1;
    unsigned acc = fac1(n - 1); // UndCC_Violation - the call is before the last statement
    return n * acc;
}

unsigned fac2_tailrec(unsigned acc, unsigned n)
{
    dive();
    if (n < 2) return acc;
    return fac2_tailrec(n * acc, n - 1); // UndCC_Valid - the recursive call is returned in the last statement
}
unsigned fac2(unsigned n)
{
    return fac2_tailrec(1, n);
}

int main(void)
{
    unsigned targetDepth = 1 << 18;

    depth = 0;
    fac0(targetDepth);

    depth = 0;
    fac1(targetDepth);

    depth = 0;
    fac2(targetDepth);

    return 0;
}
