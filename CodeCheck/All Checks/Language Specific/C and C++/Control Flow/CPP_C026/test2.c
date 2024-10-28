#include <stdbool.h>

void test2 ()
{
  bool expression = true;

  do     // UndCC_Violation(break, both) - 2 breaks
  {
    do { // UndCC_Valid - 1 break
      break;
    } while (expression);

    if (expression)
      break;
    else
      break;
  }
  while (expression);
}
