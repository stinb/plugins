#include <stdint.h>

uint8_t *p1;
uint32_t *p2;
/* Non-compliant - possible incompatible alignment */
//p2 = ( uint32_t * ) p1; // Commented out due to error
extern uint32_t read_value ( void );
extern void print ( uint32_t n );
void f ( void )
{
  uint32_t u = read_value ( );
  uint16_t *hi_p = ( uint16_t * ) &u; /* UndCC_Violation even though
* probably correctly aligned */
  *hi_p = 0; /* Attempt to clear high 16-bits on big-endian machine */
  print ( u ); /* Line above may appear not to have been performed */
  
  const short *p;
  const volatile short *q;
  q = ( const volatile short * ) p; // UndCC_Valid
  int * const * pcpi;
  const int * const * pcpci;
  pcpci = ( const int * const * ) pcpi; // UndCC_Violation
}



