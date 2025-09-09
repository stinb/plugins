// $Id: A17-6-1.cpp 305588 2018-01-29 11:07:35Z michal.szczepankiewicz $
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

namespace std
{


pair<int, int> operator+(pair<int, int> const& x, pair<int, int> const& y) // UndCC_Violation
{
  return pair<int, int>(x.first + y.first, x.second + y.second);
}

} // namespace std

struct MyType
{
  int value;
};

namespace std
{


// non-arithmetic types [limits.numeric].
template <>
struct numeric_limits<MyType> : numeric_limits<int> // UndCC_Violation
{
};


// may not be specialized [meta.type.synop].
template <>
struct is_arithmetic<MyType> : true_type // UndCC_Violation
{
};


// specialization fulfills the requirements in [unord.hash].
template <>
struct hash<MyType> // UndCC_Violation
{
  using result_type = size_t; // deprecated in C++17
  using argument_type = MyType; // deprecated in C++17

  size_t operator()(MyType const& x) const noexcept
  {
    return hash<int>()(x.value);
  }
};

} // namespace std
