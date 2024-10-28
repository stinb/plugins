// MISRA C++ 2008

#include <cstdint>
#define char_t char

void fn()
{
   char_t ch = 't';                      // UndCC_Valid
   uint8_t v;
   if ( ( ch >= 'a' ) && ( ch <= 'z' ) ) // UndCC_Violation
   {
   }
   if ( ( ch >= '0' ) && ( ch <= '9' ) ) // UndCC_Valid by exception
   {
      v = ch - '0';                      // UndCC_Valid by exception
      v = ch - '1';                      // UndCC_Violation
   }
   else
   {
      // ...
   }
   ch = '0' + v;                         // UndCC_Valid by exception
   ch = 'A' + v;                         // UndCC_Violation
}
