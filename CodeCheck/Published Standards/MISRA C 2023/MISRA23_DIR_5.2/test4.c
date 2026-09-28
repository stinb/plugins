#include <stdint.h>
#include <stddef.h>

/* <threads.h> is missing on macOS, so declare what the tests use */
typedef unsigned long thrd_t;
typedef struct { int opaque; } mtx_t;
typedef int (*thrd_start_t)( void * );
int thrd_create( thrd_t *thr, thrd_start_t func, void *arg );
int thrd_join( thrd_t thr, int *res );
int mtx_lock( mtx_t *mtx );
int mtx_unlock( mtx_t *mtx );

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
