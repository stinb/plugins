// MISRA C++ 2008

#include <cstdint>

int16_t i;
void foo ( void )
{
  int16_t i; // UndCC_Violation - This is a different variable
  i = 3;     // It could be confusing as to which i this refers
}

void fn ( int16_t i ) // UndCC_Violation
{
}
