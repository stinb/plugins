// AUTOSAR C++ 2014

#include <array>
#include <cstdint>
#include <iostream>

void Fn1() noexcept
{
  constexpr std::int32_t arraySize = 16;
  std::int32_t array[arraySize]{0}; // UndCC_FalsePos - not a violation according to the standard

  std::int32_t elem1 = array[0]; // UndCC_Valid - access with constant literal that
                                 // is less than ArraySize
  std::int32_t elem2 = array[12]; // UndCC_Valid - access with constant literal that
                                  // is less than ArraySize
  for (std::int32_t idx = 0; idx < 20; ++idx)
  {
    std::int32_t elem3 = array[idx]; // UndCC_Violation - access beyond ArraySize
                                     // bounds, which has 16 elements
  }

  std::int32_t shift = 25;
  std::int32_t elem4 = *(array + shift); // UndCC_FalseNeg - access beyond ArraySize bounds

  std::int32_t index = 0;
  std::cin >> index;
  std::int32_t elem5 = array[index]; // UndCC_Violation - index may exceed the ArraySize bounds
  if (index < arraySize)
  {
    std::int32_t elem6 = array[index]; // UndCC_FalsePos - range check coded
  }
}
void Fn2() noexcept
{
  constexpr std::int32_t arraySize = 32;
  std::array<std::int32_t, arraySize> array;
  array.fill(0);

  std::int32_t elem1 = array[10]; // UndCC_Valid - access with constant literal that
                                  // is less than ArraySize
  std::int32_t index = 40;
  std::int32_t elem2 = array[index]; // UndCC_FalseNeg - access beyond ArraySize bounds
  try
  {
    std::int32_t elem3 =
    array.at(50); // UndCC_Valid - at() method provides a
                  // range check, throwing an exception if
                  // input exceeds the bounds
  }
  catch (std::out_of_range&)
  {
    // Handle an error
  }

  for (auto&& e : array) // The std::array provides a possibility to iterate
                         // over its elements with range-based loop
  {
    // Iterate over all elements
  }
}
