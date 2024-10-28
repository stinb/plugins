// $Id: A5-6-1.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
#include <cstdint>
#include <stdexcept>
std::int32_t Division1(std::int32_t a, std::int32_t b) noexcept
{
  int32_t c = a/(a + b); /* Violation Part A - flags both a and b since they both can cause the expression to equal zero */
  return (a / b); /* Violation Part A - value of b could be zero */

}
std::int32_t Division2(std::int32_t a, std::int32_t b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero error");
  }
  return (a / b); /* Valid Part A - value of b checked before division */
}
void Fn()
{
  std::int32_t x = 20 / 0; /* UNDCC_Violation Part B - undefined behavior */
  x = Division1(20, 0); // Undefined behavior
  x = Division2(20,
  0); // Preconditions check will throw a runtime_error from
  // division2() function
  std::int32_t remainder = 20 % 0; /* UNDCC_Violation Part B - undefined behavior */
  std::int32_t a = 20 / (5-5); /* UNDCC_Violation Part B - undefined behavior */
  
}
