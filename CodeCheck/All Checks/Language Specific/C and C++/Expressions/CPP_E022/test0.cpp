// MISRA C++ 2008

typedef char char_t;

void fn ( )
{
   const char_t a[ 2 ] = "\k";   // UndCC_Violation
   const char_t b[ 2 ] = "\b";   // UndCC_Valid
}
