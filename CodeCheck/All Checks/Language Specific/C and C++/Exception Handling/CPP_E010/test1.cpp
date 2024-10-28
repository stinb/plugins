// MISRA C++ 2008 - Rule 15-4-1

#include <cstdint>

// Translation unit B
// foo declared in this translation unit with a different exception
// specification
extern void foo ( ) throw ( int32_t ); // UndCC_Violation -
                                       // different specifier
void b ( ) throw ( int32_t )
{
  foo ( ); // The behaviour here is undefined.
}
