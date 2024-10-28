// Custom

// C++11 standard 7.1.2
// "constexpr functions and constexpr constructors are implicitly inline"
constexpr double squared_inline(double number) // UndCC_Violation
{
  return number * number;
}

double squared(double number)
{
  return number * number;
}

static constexpr double squared_static_inline(double number)
{
  return number * number;
}

static double squared_static(double number)
{
  return number * number;
}
