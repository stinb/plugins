// MISRA C++ 2008

#include <stdint.h>

int16_t unusedtype()
{
   typedef int16_t local_Type; // UndCC_Violation
   return 67;
}
