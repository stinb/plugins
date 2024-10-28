// Custom

#include <cstdint>
#include <type_traits>
#include <cctype>
#include <string>
#include <cassert>

void Fn(std::int32_t value) noexcept
{
  if (value > 0 && value < 3) // UndCC_Violation
  {
    // do some work
  }
  else if ((value > 1) && (value < 2)) // UndCC_Valid
  {
    // do some work
  }
  else if (value > 1 || (value < 2)) // UndCC_Violation
  {
    // do some work
  }
  else if ((value > 1) || value < 2) // UndCC_Violation
  {
    // do some work
  }
  else
  {
    // do some work
  }

  return;
}

bool bool_function(int);

void f()
{
  static_assert(std::is_arithmetic<char>::value && !std::is_same<char, bool>::value, "msg"); // UndCC_Valid
}

bool f2(int var1, int var2)
{
  return (bool_function(var1) || bool_function(var2)); // UndCC_Valid
}

void f3(std::string const &msg)
{
  assert(!msg.empty() && std::isupper(msg.front())); // UndCC_Valid
}
