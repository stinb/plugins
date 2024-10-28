#include <stdbool.h>

void test5 ()
{
  int number = 3;
  bool expression = true;

  do // UndCC_Valid - 1 break
  {
    #include "test5.inc"
  }
  while (expression);

  do // UndCC_Violation(break, both) - 2 breaks
  {
    if (expression)
      break;

    #include "test5.inc"
  }
  while (expression);
}
