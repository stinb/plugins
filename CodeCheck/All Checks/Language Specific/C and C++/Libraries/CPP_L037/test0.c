// MISRA C 2023

#include <stdint.h>
#include <string.h>

/*
 * Is it intentional to only copy part of 's2'?
 */
void f ( uint8_t s1[ 8 ], uint16_t s2[ 8 ] )
{
  ( void ) memcpy ( s1, s2, 8 );    /* UndCC_Violation */
}
