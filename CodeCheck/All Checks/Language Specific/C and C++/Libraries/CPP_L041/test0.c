// MISRA C 2023

#include <string.h>

char buf1[  5 ] = "12345";
char buf2[ 10 ] = "1234567890";

void f ( void )
{
  if ( memcmp ( buf1, buf2, 5 ) == 0 ) /* UndCC_Valid */
  {
  }

  if ( memcmp ( buf1, buf2, 6 ) == 0 ) /* UndCC_Violation */
  {
  }
}
