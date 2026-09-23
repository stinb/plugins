#include <threads.h>
#include <stdint.h>

mtx_t Ja;
mtx_t Jb;

int32_t j1( void *ignore )
{
  mtx_lock( &Ja );
  mtx_lock( &Jb );                      /* UndCC_Valid - j2 has already finished */
  mtx_unlock( &Jb );
  mtx_unlock( &Ja );
  return 0;
}

int32_t j2( void *ignore )
{
  mtx_lock( &Jb );
  mtx_lock( &Ja );                      /* UndCC_Valid - j1 has not started yet */
  mtx_unlock( &Ja );
  mtx_unlock( &Jb );
  return 0;
}

void one_then_the_other( void )
{
  thrd_t id;

  thrd_create( &id, j2, NULL );
  thrd_join( id, NULL );

  thrd_create( &id, j1, NULL );
  thrd_join( id, NULL );
}
