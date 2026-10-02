#include <cstdint>
#include <exception>

void mayThrow( std::int32_t x )
{
  if ( x < 0 )
  {
    throw std::exception { };
  }
}

void f3( std::int32_t x ) noexcept
{
  mayThrow( x );                        /* UndCC_FalseNeg - propagates through a call */
}

void f4( std::int32_t x ) noexcept
{
  try
  {
    mayThrow( x );                      /* UndCC_Valid - caught locally */
  }
  catch ( ... )
  {
  }
}

struct S
{
  ~S( )
  {
    throw std::exception { };           /* UndCC_Violation - implicitly noexcept */
  }
};
