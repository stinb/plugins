// MISRA C 2023

#include <stdint.h>

int main( void )
{
  uint8_t a = UINT8_C( 100 );  /* UndCC_Violation - typically expands as plain 100
                                                   i.e. as a signed int */
}
