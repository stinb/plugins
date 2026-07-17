//% $Id: A23-0-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
#include <vector>

void Fn1(std::vector<std::int32_t>& v) noexcept
{
  for (std::vector<std::int32_t>::const_iterator iter{v.cbegin()},
  end{v.cend()};
  iter != end;
  ++iter) // UndCC_Valid
  {
    // ...
  }
}

void Fn2(std::vector<std::int32_t>& v) noexcept
{
  for (auto iter{v.cbegin()}, end{v.cend()}; iter != end;
  ++iter) // UndCC_Valid
  {
    // ...
  }
}

void Fn3(std::vector<std::int32_t>& v) noexcept
{
  for (std::vector<std::int32_t>::const_iterator iter{v.begin()}, // UndCC_Violation
  end{v.end()}; // UndCC_Violation
  iter != end;
  ++iter) 
  {
    // ...
  }
}
