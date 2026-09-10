// MISRA C 2023

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef bool bool_t;

bool_t f ( uint8_t a )
{
 return (    isdigit ( ( int32_t )  a )        /* UndCC_Valid */
          && isalpha ( ( int32_t ) 'b' )       /* UndCC_Valid */
          && islower (             EOF )       /* UndCC_Valid */
          && isalpha (             256 ) );    /* UndCC_Violation */
}
