// MISRA C++ 2008

enum AnEnumType {};

struct S
{
   AnEnumType n : 2;   // UndCC_Violation
};
