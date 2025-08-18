#include <stdint.h>

typedef struct s
{
    uint8_t a;
    uint8_t b;
} s_t;

typedef union person
{
    uint8_t age;
} person;

_Atomic s_t astr;
_Atomic person p1;

int32_t main(void)
{
    s_t lstr = {7U, 42U};

    astr.b = 43U; /* UndCC_Violation */
    // lstr = atomic_load(&astr);  /* Compiler error */
    lstr.b = 43U;
    atomic_store(&astr, lstr); /* UndCC_Valid */
    lstr.a = 8U;
    astr = lstr; /* UndCC_Valid */
}
