/* Custom */

typedef unsigned long long u64;
typedef unsigned int u32;

struct params
{
  u32 ptr;
  u32 list[ 4 ];
};

void f1 ( struct params *bp, unsigned int sig, int value )
{
  u64 a = ( u64 )bp->ptr << 32;        /* UndCC_Valid - the cast applies to bp->ptr */
  u64 b = ( ( u64 )bp->ptr ) << 40;    /* UndCC_Valid */
  u64 c = ( u64 )( bp->ptr ) << 40;    /* UndCC_Valid */
  u64 d = ( u64 )bp->list[ 1 ] << 40;  /* UndCC_Valid - the cast applies to the element */
  u32 e = bp->ptr << 32;               /* UndCC_Violation - u32 is 32 bits */

  unsigned int g = sig >> 20;          /* UndCC_Violation(0) - only when int is 16 bits */
  unsigned int h = sig >> 31;          /* UndCC_Violation(0) */
  unsigned int i = sig >> 32;          /* UndCC_Violation */
  int j = value << 16;                 /* UndCC_Violation(0) */
  int k = value << 15;                 /* UndCC_Valid */

  ( void )a;
  ( void )b;
  ( void )c;
  ( void )d;
  ( void )e;
  ( void )g;
  ( void )h;
  ( void )i;
  ( void )j;
  ( void )k;
}
