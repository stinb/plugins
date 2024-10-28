// Custom

#include <stdint.h>
typedef int8_t * INTPTR;

// Both cv-qualifiers
void function( INTPTR *const *const par1,       // UndCC_Violation
               INTPTR *volatile *volatile par2) // UndCC_Violation
{
   INTPTR *const *const ptr1;                   // UndCC_Violation
   INTPTR *volatile *const ptr2;                // UndCC_Violation
   INTPTR *volatile *volatile ptr3;             // UndCC_Violation
   INTPTR *const *volatile ptr4;                // UndCC_Violation
}
