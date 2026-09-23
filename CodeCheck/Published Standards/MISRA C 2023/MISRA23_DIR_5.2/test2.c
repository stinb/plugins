#include <threads.h>
#include <stdint.h>

mtx_t Ca;
mtx_t Cb;
mtx_t Cc;

int32_t c1( void *ignore )
{
  mtx_lock( &Ca );
  mtx_lock( &Cb );                      /* UndCC_Violation - closes a cycle with c2 and c3 */
  mtx_unlock( &Cb );
  mtx_unlock( &Ca );
  return 0;
}

int32_t c2( void *ignore )
{
  mtx_lock( &Cb );
  mtx_lock( &Cc );                      /* UndCC_Violation - closes a cycle with c1 and c3 */
  mtx_unlock( &Cc );
  mtx_unlock( &Cb );
  return 0;
}

int32_t c3( void *ignore )
{
  mtx_lock( &Cc );
  mtx_lock( &Ca );                      /* UndCC_Violation - closes a cycle with c1 and c2 */
  mtx_unlock( &Ca );
  mtx_unlock( &Cc );
  return 0;
}

void three_way( void )
{
  thrd_t a, b, c;

  thrd_create( &a, c1, NULL );
  thrd_create( &b, c2, NULL );
  thrd_create( &c, c3, NULL );

  thrd_join( a, NULL );
  thrd_join( b, NULL );
  thrd_join( c, NULL );
}
