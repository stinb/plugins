/* MISRA C 2012 & 2023 */

#include <stdint.h>

#define QUALIFIER volatile

#undef QUALIFIER             /* UndCC_Violation */

#define QUALIFIER volatile   /* Fix compilation error */

void f ( QUALIFIER int32_t p )
{
  while ( p != 0 )
  {
    ;                        /* Wait... */
  }
}
