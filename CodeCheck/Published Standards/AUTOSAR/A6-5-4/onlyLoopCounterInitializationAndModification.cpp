// $Id: A6-5-4.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
#include <cstdint>

int MAX = 0;

void Fn() noexcept
{
  for (std::int32_t x = 0, MAX=10; x < MAX; x++)   // UndCC_Violation
                                                 
  {
    // ...
  }

  for (std::int32_t x = 0, MAX=10; x < MAX; x++, MAX++) // UndCC_Violation - AV Rule 199
  {
    // ...
  }

  for (std::int32_t x = 0; x < MAX; x++) // UndCC_Valid
  {
    // ...
  }

  for (std::int32_t y = 0; y < 10; y++) // UndCC_Valid
  {
    // ...
  }
}
