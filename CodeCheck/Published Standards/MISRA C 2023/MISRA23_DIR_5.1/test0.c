#include <threads.h>
#include <stdatomic.h>
#include <stdint.h>

int32_t x;
int32_t a = 1;
int32_t b;
int32_t owned;
_Atomic int32_t counter;
atomic_int std_counter;
typedef _Atomic int32_t atomic_alias;
atomic_alias aliased_counter;
int32_t readonly = 7;
_Atomic int32_t *atomic_ptr;
const int32_t *shared_text;
int32_t guarded;
mtx_t lock;

int32_t t1( void *ignore )
{
  x = -1;                               /* UndCC_Violation - races with t2 */
  return 0;
}

int32_t t2( void *ignore )
{
  x = 0;                                /* UndCC_Violation - races with t1 */
  return 0;
}

int32_t t3( void *ignore )
{
  int32_t seen;

  if ( a != 0 )                         /* UndCC_Violation - races with t4 */
  {
    b += 1 / a;                         /* UndCC_Violation - races with t4 */
  }
  counter += 1;                         /* UndCC_Valid - atomic */
  std_counter += 1;                     /* UndCC_Valid - atomic_int */
  aliased_counter += 1;                 /* UndCC_Valid - a typedef of _Atomic */
  seen = readonly;                      /* UndCC_Valid - never written */
  shared_text = "t3";                   /* UndCC_Violation - races with t4 */
  atomic_ptr = &counter;                /* UndCC_Violation - the pointer is not atomic */
  return 0;
}

int32_t t4( void *ignore )
{
  int32_t seen;

  a = 0;                                /* UndCC_Violation - races with t3 */
  counter += 1;                         /* UndCC_Valid - atomic */
  std_counter += 1;                     /* UndCC_Valid - atomic_int */
  aliased_counter += 1;                 /* UndCC_Valid - a typedef of _Atomic */
  seen = readonly;                      /* UndCC_Valid - never written */
  shared_text = "t4";                   /* UndCC_Violation - races with t3 */
  atomic_ptr = &counter;                /* UndCC_Violation - the pointer is not atomic */
  return 0;
}

int32_t t5( void *ignore )
{
  owned = 1;                            /* UndCC_Valid - no other thread uses it */
  return 0;
}

int32_t t6( void *ignore )
{
  mtx_lock( &lock );
  guarded += 1;                         /* UndCC_Valid - a mutex is taken */
  mtx_unlock( &lock );
  return 0;
}

int32_t t7( void *ignore )
{
  mtx_lock( &lock );
  guarded += 1;                         /* UndCC_Valid - a mutex is taken */
  mtx_unlock( &lock );
  return 0;
}

void concurrent( void )
{
  thrd_t id1, id2, id3, id4, id5, id6, id7;

  thrd_create( &id1, t1, NULL );
  thrd_create( &id2, t2, NULL );
  thrd_create( &id3, t3, NULL );
  thrd_create( &id4, t4, NULL );
  thrd_create( &id5, t5, NULL );
  thrd_create( &id6, t6, NULL );
  thrd_create( &id7, t7, NULL );

  thrd_join( id1, NULL );
  thrd_join( id2, NULL );
  thrd_join( id3, NULL );
  thrd_join( id4, NULL );
  thrd_join( id5, NULL );
  thrd_join( id6, NULL );
  thrd_join( id7, NULL );
}
