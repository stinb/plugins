// MISRA C 2023

#include <stdint.h>

void fu8(uint8_t);
void fi(int);

#define M(x) _Generic( (x), uint8_t: fu8, default: fi )(x)

int main( void )
{
  M( UINT8_C( 100 ) );         /* UndCC_Violation - selects fi, not fu8 */
}
