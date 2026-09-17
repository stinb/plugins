#include <stdint.h>

typedef int32_t Array[10];
typedef Array *ArrayP;
typedef int32_t Int;
typedef int32_t const CInt;
typedef int32_t volatile VInt;

void f( int32_t *p )
{
  ( void ) _Generic( p,
    int32_t * : 1,                      /* UndCC_Valid */
    ArrayP    : 2,                      /* UndCC_Valid */
    default   : 3 );

  ( void ) _Generic( *p,
    Int       : 1,                      /* UndCC_Valid */
    CInt      : 2,                      /* UndCC_Violation - const is dropped */
    default   : 3 );

  ( void ) _Generic( *p,
    Int       : 1,                      /* UndCC_Valid */
    VInt      : 2,                      /* UndCC_Violation - volatile is dropped */
    default   : 3 );

  ( void ) _Generic( *p,
    Array     : 1,                      /* UndCC_Violation - an array decays */
    default   : 2 );

  ( void ) _Generic( *p,
    _Atomic int32_t : 1,                /* UndCC_Violation - atomic is dropped */
    default         : 2 );

  ( void ) _Generic( *p,
    struct { int32_t x; } : 1,          /* UndCC_Violation - a distinct type */
    default               : 2 );
}

void g( int32_t const *p )
{
  /* The qualifier is on the pointed-to type, not the object type. */
  ( void ) _Generic( p,
    CInt *    : 1,                      /* UndCC_Valid */
    Int  *    : 2,                      /* UndCC_Valid */
    default   : 3 );
}

typedef struct { int32_t x; } Anon;
struct Tagged { int32_t x; };

void records( Anon a, struct Tagged t )
{
  /* A typedef gives the definition one name, so it can be matched. */
  ( void ) _Generic( a,
    Anon      : 1,                      /* UndCC_Valid */
    default   : 2 );

  ( void ) _Generic( t,
    struct Tagged : 1,                  /* UndCC_Valid */
    default       : 2 );
}

void pointer_qualifiers( int32_t *p )
{
  ( void ) _Generic( p,
    int32_t *const : 1,                 /* UndCC_Violation - const is dropped */
    int32_t *      : 2,                 /* UndCC_Valid */
    default        : 3 );
}

/* Every example in the rule is a macro, so each expansion is reported where it
   is used rather than at the definition. */
#define SIZE_OR_OTHER( X ) _Generic( ( X ), Array: 1, default: 2 )

void macro_uses( int32_t *p, int32_t i )
{
  ( void ) SIZE_OR_OTHER( p );          /* UndCC_Violation - an array decays */
  ( void ) SIZE_OR_OTHER( i );          /* UndCC_Violation - an array decays */
}
