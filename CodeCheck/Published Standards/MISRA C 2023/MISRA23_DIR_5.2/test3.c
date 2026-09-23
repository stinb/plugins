#include <threads.h>
#include <stdint.h>

mtx_t Recursive;

int32_t r1( void *ignore )
{
  /* Taking one mutex twice is not a cycle. */
  mtx_lock( &Recursive );
  mtx_lock( &Recursive );               /* UndCC_Valid */
  mtx_unlock( &Recursive );
  mtx_unlock( &Recursive );
  return 0;
}

int32_t r2( void *ignore )
{
  mtx_lock( &Recursive );               /* UndCC_Valid */
  mtx_unlock( &Recursive );
  return 0;
}

void recursive_use( void )
{
  thrd_t a, b;

  thrd_create( &a, r1, NULL );
  thrd_create( &b, r2, NULL );

  thrd_join( a, NULL );
  thrd_join( b, NULL );
}
