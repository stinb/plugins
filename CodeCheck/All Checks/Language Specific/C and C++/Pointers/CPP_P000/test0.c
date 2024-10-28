// MISRA C 2004 and MISRA C++ 2008

#include <stdint.h>
typedef int8_t * INTPTR;

struct s {
   int8_t *   s1;                               // UndCC_Valid
   int8_t **  s2;                               // UndCC_Valid
   int8_t *** s3;                               // UndCC_Violation
};

struct s *   ps1;                               // UndCC_Valid
struct s **  ps2;                               // UndCC_Valid
struct s *** ps3;                               // UndCC_Violation

int8_t **  (  *pfunc1)();                       // UndCC_Valid
int8_t **  ( **pfunc2)();                       // UndCC_Valid
int8_t **  (***pfunc3)();                       // UndCC_Violation
int8_t *** ( **pfunc4)();                       // UndCC_Violation

void function( int8_t *   par1,                 // UndCC_Valid
               int8_t **  par2,                 // UndCC_Valid
               int8_t *** par3,                 // UndCC_Violation
               INTPTR *   par4,                 // UndCC_Valid
               INTPTR *   const * const par5,   // UndCC_Violation
               int8_t *   par6[],               // UndCC_Valid
               int8_t **  par7[])               // UndCC_Violation
{
   int8_t *   ptr1;                             // UndCC_Valid
   int8_t **  ptr2;                             // UndCC_Valid
   int8_t *** ptr3;                             // UndCC_Violation
   INTPTR *   ptr4;                             // UndCC_Valid
   INTPTR *   const * const ptr5;               // UndCC_Violation
   int8_t *   ptr6[10];                         // UndCC_Valid
   int8_t **  ptr7[10];                         // UndCC_Valid
}
