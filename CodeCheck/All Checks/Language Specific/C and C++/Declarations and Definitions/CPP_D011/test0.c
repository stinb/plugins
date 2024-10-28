// MISRA C 2012 - Rule 8.8

#include <stdint.h>

static int32_t x = 0;       /* definition: internal linkage  */
extern int32_t x;           /* UndCC_Violation               */

static int32_t f ( void );  /* declaration: internal linkage */
int32_t f ( void )          /* UndCC_Violation               */
{
  return 1;
}

static int32_t g ( void );  /* declaration: internal linkage */
extern int32_t g ( void )   /* UndCC_Violation               */
{
  return 1;
}
