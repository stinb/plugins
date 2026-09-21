#include <stdint.h>

enum E { A = 0, B = 1 };

void f( int16_t s16, int32_t i32, int64_t i64, uint16_t u16, float f32,
        enum E e, char c )
{
  ( void ) _Generic( s16,               /* UndCC_Valid */
    int16_t   : 1,
    default   : 2 );

  ( void ) _Generic( i32,               /* UndCC_Valid */
    int32_t   : 1,
    default   : 2 );

  /* Promotion makes this match int, but it is essentially short. */
  ( void ) _Generic( s16 + s16,         /* UndCC_Violation */
    int32_t   : 1,
    default   : 2 );

  ( void ) _Generic( u16 + u16,         /* UndCC_Violation */
    int32_t   : 1,
    default   : 2 );

  /* A character constant has standard type int. */
  ( void ) _Generic( 'c',               /* UndCC_Violation */
    int32_t   : 1,
    default   : 2 );

  ( void ) _Generic( c,                 /* UndCC_Valid */
    char      : 1,
    default   : 2 );

  /* An enumerated type cannot be told from its underlying type. */
  ( void ) _Generic( e,                 /* UndCC_Violation */
    int32_t   : 1,
    default   : 2 );

  ( void ) _Generic( A,                 /* UndCC_Violation */
    int32_t   : 1,
    default   : 2 );

  /* An explicit cast sets the essential type. */
  ( void ) _Generic( ( int32_t ) ( s16 + s16 ),  /* UndCC_Valid */
    int32_t   : 1,
    default   : 2 );

  ( void ) _Generic( i32 + i64,         /* UndCC_Valid */
    int64_t   : 1,
    default   : 2 );

  ( void ) _Generic( f32 + f32,         /* UndCC_Valid */
    float     : 1,
    default   : 2 );

  ( void ) _Generic( s16 > s16,         /* UndCC_Violation - essentially Boolean */
    int32_t   : 1,
    default   : 2 );
}

void constants( void )
{
  /* Compliant by exception - an integer constant expression. */
  ( void ) _Generic( 10u,               /* UndCC_Valid */
    uint32_t  : 1,
    default   : 2 );

  ( void ) _Generic( 250 + 350,         /* UndCC_Valid */
    int32_t   : 1,
    default   : 2 );
}

void outside_the_model( int32_t *p )
{
  /* A pointer has no essential type, so the rule does not apply. */
  ( void ) _Generic( p,                 /* UndCC_Valid */
    int32_t * : 1,
    default   : 2 );
}

void mixed_arithmetic( float f32, double f64, int64_t i64, uint64_t u64 )
{
  /* A floating type wins over an integer whatever their sizes. */
  ( void ) _Generic( f32 + i64,         /* UndCC_Valid */
    float     : 1,
    default   : 2 );

  ( void ) _Generic( i64 + f32,         /* UndCC_Valid */
    float     : 1,
    default   : 2 );

  ( void ) _Generic( f32 * u64,         /* UndCC_Valid */
    float     : 1,
    default   : 2 );

  ( void ) _Generic( f32 + f64,         /* UndCC_Valid */
    double    : 1,
    default   : 2 );
}

void enum_listed_as_itself( enum E e )
{
  /* Reported whichever of the two indistinguishable types is listed. */
  ( void ) _Generic( e,                 /* UndCC_Violation */
    enum E    : 1,
    default   : 2 );
}
