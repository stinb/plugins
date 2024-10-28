// $Id: A6-5-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <iterator>
void Fn() noexcept
{
  constexpr std::int8_t arraySize = 7;
  std::uint32_t array[arraySize] = {0, 1, 2, 3, 4, 5, 6};
  std::int8_t idx2;
  for (std::int8_t idx = 0; idx < arraySize; ++idx) // UndCC_Valid
  {
    array[idx] = idx;
  }
  
  for (std::int8_t idx = 0; idx < arraySize / 2;
      ++idx) // UndCC_Valid - for does not loop though all elements
  {
    // ...
  }
  
  for (std::uint32_t* iter = std::begin(array); iter != std::end(array); // UndCC_Violation
      ++iter) 
  {
    // ...
  }
  
  for (std::int8_t idx = 0; idx < arraySize; ++idx) // UndCC_Violation
  {
      // ...
  }
  
  for (std::uint32_t value :
      array) // UndCC_Valid - equivalent to non-compliant loops above
  {
    // ...
  }
  
  for (std::int8_t idx = 0; idx < arraySize; ++idx) // UndCC_Valid
  {
    if ((idx % 2) == 0)
    {
      // ...
    }
  }
  
  for (idx2 = 0; idx2 < arraySize; ++idx2) // UndCC_Valid
  {
    array[idx2] = idx2;
  }
  
  for (idx2 = 0; idx2 < arraySize; ++idx2) // UndCC_Valid
    array[idx2] = idx2;

}
