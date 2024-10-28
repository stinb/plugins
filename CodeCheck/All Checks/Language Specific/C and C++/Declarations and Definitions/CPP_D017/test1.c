// Custom

inline double discriminant_inline(double a, double b, double c) // UndCC_Violation
{
  return b * b - 4 * a * c;
}

double discriminant(double a, double b, double c)
{
  return b * b - 4 * a * c;
}

static inline double discriminant_static_inline(double a, double b, double c)
{
  return b * b - 4 * a * c;
}

static double discriminant_static(double a, double b, double c)
{
  return b * b - 4 * a * c;
}
