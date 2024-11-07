#include <stdint.h>
#include <stddef.h>
extern void f ( uint8_t *p );
extern void f1 (uint8_t *p);
#define MY_NULL_1 0
#define MY_NULL_2 ( void * ) 0

int main() {
  int32_t *p1 = 0; /* UndCC_Violation */
  int32_t *p2 = ( void * ) 0; /* UndCC_Valid */
  if ( p1 == MY_NULL_1 ) /* UndCC_Violation */
  {
  }
  if ( p2 == MY_NULL_2 ) /* UndCC_Valid */
  {
  }

f ( NULL ); // UndCC_Valid
f (0); // UndCC_Violation
}

