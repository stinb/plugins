// Custom

#include <cstdint>
#include <functional>

std::function<std::int32_t()> H()
{
  std::int32_t i = 12; // UndCC_Violation
	std::int32_t j = 12;
  return ([&i,j]() -> std::int32_t {
    i = 100;
		// j = 100 - i; // Compilation error
    return i;
  });
}

std::function<std::int32_t()> F()
{
  std::int32_t i = 12; // UndCC_Violation
  return ([&i]() -> std::int32_t {
    i = 100;
    return i;
  });
}

std::function<std::int32_t()> G()
{
  std::int32_t i = 12;
  return ([i]() mutable -> std::int32_t { return ++i; }); // UndCC_Valid
}

void Fn()
{
  auto lambda1 = F();
  std::int32_t i = lambda1(); // Undefined behavior
  auto lambda2 = G();
  i = lambda2(); // lambda2() returns 13
  auto lambda3 = H();
  i = lambda3(); // Undefined behavior
}
