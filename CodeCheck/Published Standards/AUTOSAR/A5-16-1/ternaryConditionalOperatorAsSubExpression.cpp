// $Id: A5-16-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>

constexpr bool Fn1(std::int32_t x)
{
  return (x > 0); // UndCC_Valid
}

std::int32_t Fn2(std::int32_t x)
{
  std::int32_t i = (x >= 0 ? x : 0); // UndCC_Valid
  
  std::int32_t j =
      x + (i == 0 ? (x >= 0 ? x : -x) : i); // UndCC_Violation - nested
                                            // conditional operator
                                            // and used as a
                                            // sub-expression
  return (
    i > 0
      ? (j > 0 ? i + j : i) // UndCC_Violation
      : (j > 0 ? j : 0)); 
}

int main()
{
  int x = 1;
  int z = 0;
  int y = (x >= 0) ? x : 0; // UndCC_Valid
  
  if ((y >= 0) ? 1 : 0) // UndCC_Valid
  {
    z = ((x >= y) ? x : y) + 9; // UndCC_Violation - sub-expression
    // ...
  }
  else if (((x >= 0) ? x : 0) && ((z >= 0) ? z : 0)) // UndCC_Violation - sub-expression
	{
		// ...
	}
	else
	{
		// ..
	}

  return 0;
}
