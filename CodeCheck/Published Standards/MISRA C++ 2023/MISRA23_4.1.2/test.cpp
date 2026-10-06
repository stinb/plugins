#include <cstdint>

/* MISRA C++ 2023 is written against C++17, so these are the features Annex D
   of that standard deprecates. A feature removed by C++17, such as the
   register keyword, is no longer a deprecation and is not reported. */

void dynamic_spec( ) throw( )           /* UndCC_Violation - depr.except.spec */
{
}

struct WithDtor
{
  std::int32_t v;
  ~WithDtor( ) { }                      /* UndCC_Violation - depr.impldec */
};

void implicit_copy( )
{
  WithDtor a { 1 };
  WithDtor b { a };
  ( void ) b;
}

struct DefaultedDtor
{
  std::int32_t v;
  ~DefaultedDtor( ) = default;          /* UndCC_Violation - a user-declared dtor */
};

void defaulted_dtor_copy( )
{
  DefaultedDtor a { 1 };
  DefaultedDtor b { a };
  ( void ) b;
}

struct UserCopy
{
  std::int32_t v;
  UserCopy( const UserCopy &other ) : v( other.v ) { }  /* UndCC_Violation - depr.impldec */
  UserCopy( std::int32_t n ) : v( n ) { }
};

void user_copy_assign( )
{
  UserCopy a { 1 };
  UserCopy b { 2 };
  b = a;                                /* UndCC_Valid - reported at the declaration */
  ( void ) b;
}

void writable_string( )
{
  char *s = "literal";                  /* UndCC_Violation - depr.str.strings */
  ( void ) s;
}

void volatile_use( )
{
  volatile std::int32_t v = 0;
  v += 1;                               /* UndCC_Valid - deprecated from C++20 */
}

void compliant( ) noexcept              /* UndCC_Valid */
{
}
