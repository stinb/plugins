// MISRA C++ 2008

#include <cstddef>
#include <cstdint>

using char_t = char;

void f()
{
   try
   {
      throw ( NULL );       // UndCC_Violation
   }
   catch ( int32_t i )      // NULL exception handled here
   {
      // ...
   }
   catch ( const char_t * ) // Developer may expect it to be caught here
   {
      // ...
   }

   char_t * p = NULL;
   try
   {
      throw ( static_cast < const char_t * > ( NULL ) ); // UndCC_Valid,
                                                         // but breaks
                                                         // Rule 15-0-2
      throw ( p );                                       // UndCC_Valid
   }
   catch ( int32_t i )
   {
      // ...
   }
   catch ( const char_t * ) // Both exceptions handled here
   {
      // ...
   }
}
