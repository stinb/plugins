#include <stdbool.h>

bool b;
void f ( void )
{
  unsigned short x = 0;  // UndCC_Violation - line splicing turns following line of code into a comment \\
  if ( b )
  {
    ++x; /* This is always executed */
  }
}
