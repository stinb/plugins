// MISRA C 2012

#include <stdint.h>

int16_t glob = 0;
void proc ( int16_t para )
{
  para = glob;               // UndCC_Violation
}
void f ( char *p, char *q )
{
  p = q;                     // UndCC_Violation
  *p = *q;                   // UndCC_Valid
}
