// MISRA C 2012 & 2023

#include <string.h>

void f ( void )
{
  int p[3];
  int q[3];
  int n;
  /* memcpy has restrict-qualified parameters */
  memcpy ( p, q, n );
}

void user_copy ( void * restrict p, void * restrict q, int n )  // UndCC_Violation
{
}
