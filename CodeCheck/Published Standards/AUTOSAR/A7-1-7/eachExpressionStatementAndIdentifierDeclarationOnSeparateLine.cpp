// AUTOSAR

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

struct Person
{
  std::int32_t x = 0; // UndCC_Valid
  std::int32_t y = 0; // UndCC_Valid
};

char preamble[200], a; // UndCC_Violation

typedef std::int32_t* ptr;  // UndCC_Valid
typedef std::int32_t *pointer, value; // UndCC_Violation
typedef std::array<std::string, 1000> arr; // UndCC_Valid

void Fn1() noexcept
{
  std::int32_t x = 0; // UndCC_Valid
  std::int32_t y = 7, *p1 = nullptr; // UndCC_Violation
  std::int32_t const *p2, z = 1; // UndCC_Violation
  std::int32_t a = x; // UndCC_Valid
}

void Fn2()
{
  std::vector<std::int32_t> v{1, 2, 3, 4, 5};
  for (auto iter{v.begin()}, end{v.end()}; iter != end;
        ++iter) // UndCC_Valid by exception
  {
    // ...
  }

  for (auto iter{v.begin()}, end{v.end()};
       iter != end; ++iter) // UndCC_Valid by exception
  {
    // ...
  }
}
void Fn3() noexcept
{
  std::int32_t x{5};
  std::int32_t y{15};
  x++;
  ++y;
  x++; y++; // UndCC_Violation

  for (std::int32_t i{0}; i < 100; ++i)
  {
    Fn2(); // UndCC_Valid
  }

  int psize = strlen(preamble); // UndCC_Valid
}
