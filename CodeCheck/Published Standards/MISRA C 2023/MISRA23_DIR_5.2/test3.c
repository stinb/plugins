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
