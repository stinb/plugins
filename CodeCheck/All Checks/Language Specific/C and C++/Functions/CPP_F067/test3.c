// MISRA C 2023

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

_Noreturn void a ( void )
{
  return; /* UndCC_Violation - breaks _Noreturn contract */
}

_Noreturn void b ( void )
{
  while ( true ) /* Permitted by Rule 14.3 Exception 1 */
  {
    /* ... */
  }

  /* Compliant - function can never return */
}

_Noreturn void c ( void )
{
  abort(); /* Compliant - no return from abort() */
}

_Noreturn void d ( int32_t i )
{
  if ( i > 0 )
  {
    abort();
  }

} /* UndCC_Violation - returns if i <= 0 */
