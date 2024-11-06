#include <cstdint>

typedef int32_t INT;
using Index = int32_t;

INT i;
extern int32_t i; // UndCC_Violation

INT j;
extern INT j; // UndCC_Valid

void g(int32_t i);
void g(Index const i); // UndCC_Violation - int32_t vs. Index
void h(Index i);
void h(Index const index); // UndCC_Valid - Index used consistently
void h(int32_t *i);        // Rule does not apply - different overload
