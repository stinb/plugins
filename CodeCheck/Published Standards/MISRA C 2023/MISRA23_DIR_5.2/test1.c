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

mtx_t Ma;
mtx_t Mb;

int32_t s1( void *ignore )
{
  mtx_lock( &Ma );
  mtx_lock( &Mb );                      /* UndCC_Valid - both threads agree */
  mtx_unlock( &Mb );
  mtx_unlock( &Ma );
  return 0;
}

int32_t s2( void *ignore )
{
  mtx_lock( &Ma );
  mtx_lock( &Mb );                      /* UndCC_Valid - both threads agree */
  mtx_unlock( &Mb );
  mtx_unlock( &Ma );
  return 0;
}

int32_t s3( void *ignore )
{
  /* Released before the next is taken, so no order is imposed. */
  mtx_lock( &Mb );
  mtx_unlock( &Mb );
  mtx_lock( &Ma );                      /* UndCC_Valid */
  mtx_unlock( &Ma );
  return 0;
}

void ordered( void )
{
  thrd_t a, b, c;

  thrd_create( &a, s1, NULL );
  thrd_create( &b, s2, NULL );
  thrd_create( &c, s3, NULL );

  thrd_join( a, NULL );
  thrd_join( b, NULL );
  thrd_join( c, NULL );
}
