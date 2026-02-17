/* MISRA C 2012 & 2023 */

#include <stdint.h>

void f1 ( int32_t a )
{
  if ( a <= 0 )
  {
    goto L2;             /* UndCC_Violation */
  }

  goto L1;               /* UndCC_Valid */

  if ( a == 0 )
  {
    goto L1;             /* UndCC_Valid */
  }

  goto L2;               /* UndCC_Violation */

L1:
  if ( a > 0 )
  {
  L2:
    ;
  }
}

void f2 ( int32_t x, int32_t y )
{
  switch ( x )
  {
    case 0:
      if ( x == y )
      {
        goto L1;         /* UndCC_Violation */
      }
      break;
    case 1:
      y = x;
  L1:
      ++x;
      break;
    default:
      break;
  }
}
