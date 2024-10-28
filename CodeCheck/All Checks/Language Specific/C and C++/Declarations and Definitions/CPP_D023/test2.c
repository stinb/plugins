// Custom

const int x = 0;     // UndCC_Violation(strict) - used in 1 global object and 1 function
const int y = 1 + x; // UndCC_Violation - only used in 1 function
int z = 3;           // UndCC_Violation - only used in 1 function

int f()
{
  z = 10;
  z++;
  return x + y / z;
}
