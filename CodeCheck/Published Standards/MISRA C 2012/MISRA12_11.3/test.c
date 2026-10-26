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




typedef struct { int16_t re; int16_t im; } complex16_t;
typedef struct { uint8_t a : 1; uint8_t b : 7; } bits_t;
typedef struct { char *elts; } array_t;
#define U32 uint32_t
void takesI16(int16_t *p);
int16_t *getI16(void);

void customerCases(void)
{
  int16_t buffer[16] = {0};
  complex16_t cx[4];
  complex16_t *cp = &cx[0];
  uint8_t bytes[16] = {0};
  const bits_t *sp;
  int32_t *p32;
  int16_t *p16;
  uint8_t *p8;
  void *vp;
  int idx = 1;
  p32 = (int32_t *)&buffer[0];                    // UndCC_Violation - int16_t to int32_t
  takesI16((int16_t *)cp);                        // UndCC_Violation - in an argument
  p16 = (int16_t *)&cx[idx];                      // UndCC_Violation - structure to int16_t
  sp = (const bits_t *)&bytes[idx];               // UndCC_Violation - the exception is only towards character types
  p32 = (int32_t *)buffer;                        // UndCC_Violation - the array converts to int16_t *
  p32 = (U32 *)p32;                               // UndCC_Violation - through a type macro
  p32 = (int32_t *)getI16();                      // UndCC_Violation - the call returns int16_t *
  p16 = (int16_t *)(buffer + 2);                  // UndCC_Valid - same type
  p8 = (uint8_t *)sp;                             // UndCC_Valid - by exception
  p16 = (int16_t *)&((uint8_t *)cp)[2];           // UndCC_Violation - uint8_t to int16_t
  p16 = (int16_t *)vp;                            // UndCC_Valid - pointer to void is Rule 11.5
  p16 = (int16_t *)p16;                           // UndCC_Valid
  array_t arr;
  array_t *ap = &arr;
  p16 = (int16_t *)(*(&ap))->elts;                // UndCC_Violation - the member is a char pointer
  p8 = (uint8_t *)(*(&ap))->elts;                 // UndCC_Valid - by exception
}

void spellings(unsigned int *up, long *lp, int *ip, short *sp, long long *llp)
{
  unsigned *a = (unsigned *)up;                   // UndCC_Valid - unsigned and unsigned int are one type
  long int *b = (long int *)lp;                   // UndCC_Valid
  signed *c = (signed *)ip;                       // UndCC_Valid
  short int *d = (short int *)sp;                 // UndCC_Valid
  long long int *e = (long long int *)llp;        // UndCC_Valid
  long *g = (long *)ip;                           // UndCC_Violation - long and int are different types
  (void)a; (void)b; (void)c; (void)d; (void)e; (void)g;
}
