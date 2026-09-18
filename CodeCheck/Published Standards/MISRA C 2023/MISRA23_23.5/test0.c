#include <stdint.h>

void handle_cpi( const int32_t * );
void handle_any( void * );
void handle_f( float );

#define handle_pointer1( X ) ( _Generic( ( X ),                               \
                 const int32_t *: handle_cpi,                                 \
                 default        : handle_any ) ( ( void * ) ( X ) ) )

#define handle_pointer2( X ) ( _Generic( ( X ),                               \
                 void           *: handle_any,                                \
                 const int32_t  *: handle_cpi,                                \
                 default         : handle_any ) ( ( void * ) ( X ) ) )

void f( const int32_t *cp, int32_t *mp, float *fp )
{
  handle_pointer1( cp );                /* UndCC_Valid - const int32_t * matches */
  handle_pointer1( mp );                /* UndCC_Violation - adds const to match */
  handle_pointer1( fp );                /* UndCC_Valid - no listed type accepts it */

  handle_pointer2( mp );                /* UndCC_Violation - void * would accept it */
  handle_pointer2( fp );                /* UndCC_Violation - void * would accept it */
}

/* The selection is judged as a whole, so each case is reported at its
   _Generic rather than at one association. */

void arithmetic( int32_t i )
{
  /* The rule only applies to selection on a pointer type. */
  ( void ) _Generic( i,                 /* UndCC_Valid */
    float     : 1,
    default   : 2 );
}

void exact_match( int32_t *p )
{
  ( void ) _Generic( p,                 /* UndCC_Valid - the default is not taken */
    int32_t * : 1,
    default   : 2 );
}

void unrelated_pointer( float *p )
{
  ( void ) _Generic( p,                 /* UndCC_Valid - no conversion exists */
    int32_t * : 1,
    default   : 2 );
}

void void_controlling( void *p )
{
  ( void ) _Generic( p,                 /* UndCC_Violation - void * would convert */
    int32_t * : 1,
    default   : 2 );
}

void qualifier_dropped( const int32_t *p )
{
  /* Dropping const is not an implicit conversion, so nothing would accept it. */
  ( void ) _Generic( p,                 /* UndCC_Valid */
    int32_t * : 1,
    default   : 2 );
}

typedef void ( *Handler )( void );

void function_pointer( Handler h )
{
  /* A function pointer does not convert to a pointer to void. */
  ( void ) _Generic( h,                 /* UndCC_Valid */
    void *    : 1,
    default   : 2 );
}

void array_controlling( void )
{
  int32_t arr[10];

  /* Decay gives an exact match, so the default is not taken. */
  ( void ) _Generic( arr,               /* UndCC_Valid */
    int32_t * : 1,
    default   : 2 );
}

void qualified_void( int32_t *p )
{
  ( void ) _Generic( p,                 /* UndCC_Violation - const void * would convert */
    const void * : 1,
    default      : 2 );
}

void restrict_controlling( int32_t *restrict p )
{
  /* The top-level qualifier is dropped, so this is an exact match. */
  ( void ) _Generic( p,                 /* UndCC_Valid */
    int32_t * : 1,
    default   : 2 );
}

void exact_match_wins( int32_t *p )
{
  /* const int32_t * is convertible, but the exact match is selected. */
  ( void ) _Generic( p,                 /* UndCC_Valid */
    int32_t *       : 1,
    const int32_t * : 2,
    default         : 3 );
}
