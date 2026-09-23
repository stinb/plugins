#include <threads.h>
#include <stdint.h>

mtx_t Ra;
mtx_t Rb;

int32_t t1( void *ignore )
{
  mtx_lock( &Ra );
  mtx_lock( &Rb );                      /* UndCC_Violation - the reverse of t2 */
  mtx_unlock( &Rb );
  mtx_unlock( &Ra );
  return 0;
}

int32_t t2( void *ignore )
{
  mtx_lock( &Rb );
  mtx_lock( &Ra );                      /* UndCC_Violation - the reverse of t1 */
  mtx_unlock( &Ra );
  mtx_unlock( &Rb );
  return 0;
}

void deadlocking( void )
{
  thrd_t id1, id2;

  thrd_create( &id1, t1, NULL );
  thrd_create( &id2, t2, NULL );

  thrd_join( id1, NULL );
  thrd_join( id2, NULL );
}
