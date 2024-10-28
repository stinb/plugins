#include <cstdint>
typedef char char_t;

// MISRA C++ 2008

#include <cstdlib>
int32_t f ( const char_t * numstr )
{
  return atoi ( numstr ); // UndCC_Violation
}
