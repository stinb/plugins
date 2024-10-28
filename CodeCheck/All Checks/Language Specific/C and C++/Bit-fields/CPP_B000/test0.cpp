// MISRA C++ 2008

#include <cstdint>

struct S
{
     signed int   a : 2;   // UndCC_Valid
   unsigned int   b : 2;   // UndCC_Valid
            char  c : 2;   // UndCC_Violation
     signed char  d : 2;   // UndCC_Valid
   unsigned char  e : 2;   // UndCC_Valid
            short f : 2;   // UndCC_Violation
     signed short g : 2;   // UndCC_Valid
   unsigned short h : 2;   // UndCC_Valid
            int   i : 2;   // UndCC_Violation
            bool  j : 2;   // UndCC_Valid
         wchar_t  k : 2;   // UndCC_Violation
        uint32_t  l : 2;   // UndCC_Valid
          int8_t  m : 2;   // UndCC_Valid
};
