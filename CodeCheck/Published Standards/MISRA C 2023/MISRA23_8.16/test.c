#include <stdint.h>
#include <stdalign.h>

int32_t a;                                     /* UndCC_Valid: no alignment specification */
alignas(16) int32_t b;                         /* UndCC_Valid: explicit non-zero alignment specification */
alignas(0) int32_t c;                          /* UndCC_Violation: zero-alignment specification */
alignas(sizeof(long) - sizeof(int)) int32_t d; /* UndCC_FalseNeg on platforms where sizeof (int) == sizeof (long) */
