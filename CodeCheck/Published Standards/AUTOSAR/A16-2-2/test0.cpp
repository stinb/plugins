// $Id: A16-2-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <algorithm> // UndCC_Violation - nothing from algorithm header file is used
#include <array>     // UndCC_Violation - nothing from array header file is used
#include <cstdint>   // UndCC_Valid - std::int32_t, std::uint8_t are used
#include <iostream>  // UndCC_Valid - cout is used
#include <stdexcept> // UndCC_Valid - out_of_range is used
#include <vector>    // UndCC_Valid - vector is used

void Fn1() noexcept
{
  std::int32_t x = 0;
  // ...
  std::uint8_t y = 0;
  // ...
}

void Fn2() noexcept(false)
{
  try
  {
    std::vector<std::int32_t> v;
    // ...
    std::uint8_t idx = 3;
    std::int32_t value = v.at(idx);
  }
  catch (std::out_of_range& e)
  {
    std::cout << e.what() << '\n';
  }
}
