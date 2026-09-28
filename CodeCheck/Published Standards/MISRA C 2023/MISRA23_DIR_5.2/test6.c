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

mtx_t Sc;
mtx_t Sd;

int32_t inconsistent( void *ignore )
{
  mtx_lock( &Sc );
  mtx_lock( &Sd );                      /* UndCC_Violation - the other order is below */
  mtx_unlock( &Sd );
  mtx_unlock( &Sc );

  mtx_lock( &Sd );
  mtx_lock( &Sc );                      /* UndCC_Violation - the other order is above */
  mtx_unlock( &Sc );
  mtx_unlock( &Sd );
  return 0;
}

int32_t no_locks( void *ignore )
{
  return 0;
}

void self_conflict( void )
{
  thrd_t a, b;

  thrd_create( &a, inconsistent, NULL );
  thrd_create( &b, no_locks, NULL );

  thrd_join( a, NULL );
  thrd_join( b, NULL );
}
