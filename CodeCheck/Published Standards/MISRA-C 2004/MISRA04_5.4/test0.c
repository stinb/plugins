// MISRA C 2004

#include <stdint.h>

struct stag { uint16_t a; uint16_t b; };

struct stag a1 = { 0, 0 };      /* UndCC_Valid - compatible with above   */
/* union stag a2 = { 0, 0 }; */ /* Analysis error - not compatible with
                                   previous declarations                 */

void foo(void)
{
   struct stag { uint16_t a; }; /* UndCC_Violation - tag stag redefined */
}
