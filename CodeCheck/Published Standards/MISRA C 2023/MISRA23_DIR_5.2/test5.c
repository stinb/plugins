#include <stdint.h>
#include <stddef.h>

/* <threads.h> is missing on macOS, so declare what the tests use */
typedef unsigned long thrd_t;
typedef struct { int opaque; } mtx_t;
typedef int (*thrd_start_t)( void * );
enum { thrd_success, thrd_busy, thrd_error };
int thrd_create( thrd_t *thr, thrd_start_t func, void *arg );
int thrd_join( thrd_t thr, int *res );
int mtx_lock( mtx_t *mtx );
int mtx_trylock( mtx_t *mtx );
int mtx_unlock( mtx_t *mtx );

mtx_t Ta;
mtx_t Tb;

int32_t ta( void *ignore )
{
  /* Lock one and try for the next: the usual way to avoid a cycle. */
  mtx_lock( &Ta );
  if ( mtx_trylock( &Tb ) == thrd_success )  /* UndCC_Valid - a try never waits */
  {
    mtx_unlock( &Tb );
  }
  mtx_unlock( &Ta );
  return 0;
}

int32_t tb( void *ignore )
{
  mtx_lock( &Tb );
  mtx_lock( &Ta );                      /* UndCC_Valid - ta can never be waiting */
  mtx_unlock( &Ta );
  mtx_unlock( &Tb );
  return 0;
}

void avoided( void )
{
  thrd_t a, b;

  thrd_create( &a, ta, NULL );
  thrd_create( &b, tb, NULL );

  thrd_join( a, NULL );
  thrd_join( b, NULL );
}
