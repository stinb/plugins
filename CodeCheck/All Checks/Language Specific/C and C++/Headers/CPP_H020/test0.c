#include <tgmath.h> // UndCC_Violation, tgmath lib header used

float f1, f2;

void f ( void )
{
  f1 = sqrt ( f2 );   /* generic sqrt from tgmath */
}

#include <math.h>

float f1, f2;

void func2 ( void )
{
  f1 = sqrtf ( f2 ); /* UndCC_Valid - float version of sqrt used */
}
