// MISRA C 2012

typedef float float32_t;

#include <stdint.h>

void fn1 ( void )
{
  int16_t i;   // Declare an object "i"

  {
    int16_t i; // UndCC_Violation - hides previous "i"

    i = 3;     // Could be confusing as to which "i" this refers
  }
}

struct astruct
{
  int16_t m;
};

extern void g ( struct astruct *p );

int16_t xyz = 0;                // Declare an object "xyz"

void fn2 ( struct astruct xyz ) // UndCC_Violation - outer "xyz" is
                                // now hidden by parameter name
{
  g ( &xyz );
}

uint16_t speed;

void fn3 ( void )
{
  typedef float32_t speed; // UndCC_Violation - type hides object
}
