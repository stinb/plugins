// MISRA C++ 2008

struct S
{
   signed int   a : 1;   // UndCC_Violation
   signed int     : 1;   // UndCC_Valid
   signed int     : 0;   // UndCC_Valid
   signed int   b : 2;   // UndCC_Valid
   signed int     : 2;   // UndCC_Valid
};
