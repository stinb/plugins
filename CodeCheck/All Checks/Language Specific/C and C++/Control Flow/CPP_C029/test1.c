// MISRA C 2012

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h> // define NULL

#define bool_t bool
#define MAX 10

bool_t f ( uint16_t n, char *p ) // UndCC_Violation(decl) - 2 early returns
{
  if ( n > MAX )
  {
    return false;                // UndCC_Violation(exit) - early return
  }
  if ( p == NULL )
  {
    return false;                // UndCC_Violation(exit) - early return
  }
  return true;
}
