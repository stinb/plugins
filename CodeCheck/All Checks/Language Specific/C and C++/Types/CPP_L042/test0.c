// MISRA C 2023


#include <stdint.h>
#include <stdio.h>


void f1 ( void )
{
  char ch;

  ch = ( char ) getchar ();

  /*
   * The following test is non-compliant. It will not be reliable as the
   * return value is cast to a narrower type before checking for EOF.
   */
  if ( EOF != ( int32_t ) ch ) // UndCC_Violation
  {
  }
}


void f2 ( void )
{
  char ch;

  ch = ( char ) getchar ();

  if ( !feof ( stdin ) )
  {
  }
}


void f3 ( void )
{
  int32_t i_ch;

  i_ch = getchar ();

  /*
   * The following test is compliant. It will be reliable as the
   * unconverted return value is used when checking for EOF.
   */
  if ( EOF != i_ch )
  {
    char ch;

    ch = ( char ) i_ch;
  }
}
