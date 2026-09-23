#include <threads.h>
#include <stdint.h>

mtx_t Xe;
mtx_t Xf;
mtx_t Xg;
mtx_t Xh;

int32_t x1( void *ignore )
{
  mtx_lock( &Xe );
  mtx_lock( &Xf );                      /* UndCC_Violation - the reverse of x2 */
  mtx_unlock( &Xf );
  mtx_unlock( &Xe );
  return 0;
}

int32_t x2( void *ignore )
{
  mtx_lock( &Xf );
  mtx_lock( &Xe );                      /* UndCC_Violation - the reverse of x1 */
  mtx_unlock( &Xe );
  mtx_unlock( &Xf );
  return 0;
}

int32_t x3( void *ignore )
{
  mtx_lock( &Xg );
  mtx_lock( &Xh );                      /* UndCC_Violation - the reverse of x4 */
  mtx_unlock( &Xh );
  mtx_unlock( &Xg );
  return 0;
}

int32_t x4( void *ignore )
{
  mtx_lock( &Xh );
  mtx_lock( &Xg );                      /* UndCC_Violation - the reverse of x3 */
  mtx_unlock( &Xg );
  mtx_unlock( &Xh );
  return 0;
}

void two_cycles( void )
{
  thrd_t a, b, c, d;

  thrd_create( &a, x1, NULL );
  thrd_create( &b, x2, NULL );
  thrd_create( &c, x3, NULL );
  thrd_create( &d, x4, NULL );

  thrd_join( a, NULL );
  thrd_join( b, NULL );
  thrd_join( c, NULL );
  thrd_join( d, NULL );
}
