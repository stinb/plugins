// MISRA C 2004

#include <stdint.h>

void foo()
{
  int16_t i;
  {
    int16_t i; // UndCC_Violation - This is a different variable
    i = 3;     // It could be confusing as to which i this refers
  }
}
