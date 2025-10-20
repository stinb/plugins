// MISRA C 2023


#include <stdatomic.h>
#include <stdint.h>


typedef struct s {
  uint8_t a;
  uint8_t b;
} s_t;
_Atomic s_t astr;


void main( void )
{
  s_t lstr = {7, 42};

  atomic_init( &astr, lstr );

  lstr = atomic_load( &astr );                                /* UndCC_Valid */
  lstr = atomic_load_explicit( &astr, memory_order_relaxed ); /* UndCC_Violation */

  lstr.b = 43;
  atomic_store_explicit( &astr, lstr, memory_order_release ); /* UndCC_Violation */
}
