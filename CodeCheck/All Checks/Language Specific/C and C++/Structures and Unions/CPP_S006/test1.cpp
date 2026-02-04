// MISRA C++ 2023

#include <cstdint>
#include <variant>

union Data1                                    // UndCC_Violation
{
  int32_t i;
  float   j;
};

using Data2 = std::variant< int32_t, float >;  // Rule does not apply
