#include <stdint.h>
#include <stdalign.h>

int32_t a;                        /* UndCC_Valid - no alignment specifier */
alignas(16) int32_t b;            /* UndCC_Valid - one alignment specifier */
alignas(16) alignas(8) int32_t c; /* UndCC_Violation - two alignment specifiers */
alignas(16) alignas(0) int32_t d; /* UndCC_Violation - also violates Rule 8.16 */
