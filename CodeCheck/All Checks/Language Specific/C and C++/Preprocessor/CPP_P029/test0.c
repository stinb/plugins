/* MISRA C 2012 & 2023 */

#include <stdint.h>

void use_int16 ( int16_t );

#define SIZE 4
#define DATA 3      /* UndCC_Violation - DATA not used */

void use_macro ( void )
{
  use_int16 ( SIZE );
}
