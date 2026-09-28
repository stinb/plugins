#include <stdint.h>
#include <stddef.h>

/* <threads.h> is missing on macOS, so declare what the tests use */
typedef unsigned long thrd_t;
typedef int (*thrd_start_t)( void * );
int thrd_create( thrd_t *thr, thrd_start_t func, void *arg );
int thrd_join( thrd_t thr, int *res );

int32_t sequential;

int32_t s1( void *ignore )
{
  sequential = 1;                       /* UndCC_Valid - the threads never overlap */
  return 0;
}

int32_t s2( void *ignore )
{
  sequential = 2;                       /* UndCC_Valid - the threads never overlap */
  return 0;
}

void one_at_a_time( void )
{
  thrd_t id;

  thrd_create( &id, s1, NULL );
  thrd_join( id, NULL );

  thrd_create( &id, s2, NULL );
  thrd_join( id, NULL );
}
