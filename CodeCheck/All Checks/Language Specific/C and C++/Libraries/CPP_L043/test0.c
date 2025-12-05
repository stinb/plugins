// MISRA C 2023


#include <stdint.h>
#include <string.h>
#include <threads.h>


mtx_t Ra;
mtx_t Rb;
thrd_t id1;
thrd_t id2;
tss_t key;


int32_t t1( void *ignore )
{
  mtx_lock( &Ra );
  int32_t val;
  /* if ( id1 == id2 ) */            /* Error - use thrd_equal() */
  {
    Rb = Ra;                         /* UndCC_Violation */
    memcpy(&Rb, &Ra, sizeof(mtx_t)); /* UndCC_Violation */
  }

  if ( thrd_equal( id1, id2 ) )      /* UndCC_Valid */
  {
    // ...
  }
  /* key++; */                       /* Error - explicit manipulation of
                                                TSS pointer */
  tss_set( key, &val );              /* Undefined, value of key not returned by
                                                tss_create() */
}


int main( void )
{
  mtx_init   ( &Ra, mtx_plain );
  mtx_init   ( &Rb, mtx_plain );
  tss_create ( &key, NULL     );
  thrd_create( &id1, t1, NULL );
  thrd_create( &id2, t1, NULL );
  //  ...
}
