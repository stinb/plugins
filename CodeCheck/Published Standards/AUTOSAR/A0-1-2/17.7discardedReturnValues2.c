#include <string.h>
#include <stdio.h>

typedef unsigned short uint16_t;

uint16_t func ( uint16_t para1 )
{
  return para1;
}

void vfunc ( void )
{
}

uint16_t ( *fp ) ( uint16_t ) = func;

#define CALL( x ) func ( x )
#define VCALL( x ) ( ( void ) func ( x ) )

uint16_t x;

void discarded ( uint16_t para2, int c )
{
  char buf[8];
  uint16_t i;

  vfunc ( );                           /* UndCC_Valid - void return type */
  fp ( para2 );                        /* UndCC_Violation - call through a pointer */
  ( *fp ) ( para2 );                   /* UndCC_Violation - call through a pointer */
  ( void ) fp ( para2 );               /* UndCC_Valid */
  CALL ( para2 );                      /* UndCC_Violation - call in a macro */
  VCALL ( para2 );                     /* UndCC_Valid - macro casts to void */
  strcpy ( buf, "a" );                 /* UndCC_Violation - library function */
  printf ( "x" );                      /* UndCC_Violation - library function */
  ( func ( para2 ) );                  /* UndCC_Violation - parenthesised */
  func ( para2 ), x = 1u;              /* UndCC_FalseNeg - left operand of comma */
  x = 1u, func ( para2 );              /* UndCC_FalseNeg - right operand of comma */
  c ? func ( para2 ) : func ( para2 ); /* UndCC_FalseNeg - conditional operands */

  for ( func ( para2 );                /* UndCC_Violation - for init */
        c != 0;
        func ( para2 ) )               /* UndCC_Violation - for increment */
  {
    c--;
  }

  if ( func ( para2 ) != 0u )          /* UndCC_Valid */
  {
  }

  x = ( uint16_t ) func ( para2 );     /* UndCC_Valid */
  i = func ( func ( para2 ) );         /* UndCC_Valid - used as an argument */
}
