#include <stdlib.h>

int values[] = { 88, 56, 100, 2, 25 };

void main()
{
  qsort (values, 5, 3, 2); // UndCC_Violation
  bsearch (2, values, 3, 4, 2); // UndCC_Violation
}

