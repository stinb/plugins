#include <threads.h>
#include <stdint.h>

int32_t stats;
int32_t queue;
int32_t outside;
mtx_t qlock;

static void bump_stats( void )
{
  stats += 1;                           /* UndCC_Violation - races with the other worker */
}

int32_t w1( void *ignore )
{
  mtx_lock( &qlock );
  queue += 1;                           /* UndCC_Valid - inside the lock region */
  mtx_unlock( &qlock );
  outside += 1;                         /* UndCC_Violation - the lock is released */
  bump_stats( );
  return 0;
}

int32_t w2( void *ignore )
{
  mtx_lock( &qlock );
  queue += 1;                           /* UndCC_Valid - inside the lock region */
  mtx_unlock( &qlock );
  outside += 1;                         /* UndCC_Violation - the lock is released */
  bump_stats( );
  return 0;
}

int32_t wrapped;
mtx_t wlock;

static void enter( void )
{
  mtx_lock( &wlock );
}

static void leave( void )
{
  mtx_unlock( &wlock );
}

int32_t d1( void *ignore )
{
  enter( );
  wrapped += 1;                         /* UndCC_Valid - the locking is delegated */
  leave( );
  return 0;
}

int32_t d2( void *ignore )
{
  enter( );
  wrapped += 1;                         /* UndCC_Valid - the locking is delegated */
  leave( );
  return 0;
}

void workers( void )
{
  thrd_t a, b, c, d;

  thrd_create( &a, w1, NULL );
  thrd_create( &b, w2, NULL );
  thrd_create( &c, d1, NULL );
  thrd_create( &d, d2, NULL );

  thrd_join( a, NULL );
  thrd_join( b, NULL );
  thrd_join( c, NULL );
  thrd_join( d, NULL );
}

int32_t both;
typedef mtx_t my_lock_t;
my_lock_t alias_lock;

int32_t g1( void *ignore )
{
  mtx_lock( &alias_lock );
  both = 1;                             /* UndCC_Valid - inside the region */
  mtx_unlock( &alias_lock );
  both = 2;                             /* UndCC_Violation - the lock is released */
  return 0;
}

int32_t other_thing;

int32_t g2( void *ignore )
{
  mtx_lock( &alias_lock );
  other_thing += 1;                     /* UndCC_Valid - inside the region */
  mtx_unlock( &alias_lock );
  both = 3;                             /* UndCC_Violation - races with g1 */
  return 0;
}

void forgetful( void )
{
  thrd_t e, f;

  thrd_create( &e, g1, NULL );
  thrd_create( &f, g2, NULL );

  thrd_join( e, NULL );
  thrd_join( f, NULL );
}
