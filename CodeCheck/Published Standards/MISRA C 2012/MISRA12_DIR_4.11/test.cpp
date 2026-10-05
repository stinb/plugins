#include <cmath>
#include <cstdint>
#include <vector>

namespace mylib
{
  double sqrt( double );
}

float f( float a )
{
  return std::fmod( a, 0.0f );          /* UndCC_Violation - the divisor is zero */
}

double domain( )
{
  return std::sqrt( -1.0 );             /* UndCC_Violation - negative argument */
}

double in_domain( double x )
{
  return std::sqrt( x );                /* UndCC_Valid - not a constant */
}

double own_namespace( )
{
  return mylib::sqrt( -1.0 );           /* UndCC_Valid - not the Standard Library */
}

std::int32_t b1( std::vector< std::int32_t > const &v )
{
  return v.front( );                    /* Precondition: v is not empty */
}

std::int32_t b2( )
{
  std::vector< std::int32_t > v;
  return b1( v );                       /* UndCC_FalseNeg - a user-defined precondition */
}
