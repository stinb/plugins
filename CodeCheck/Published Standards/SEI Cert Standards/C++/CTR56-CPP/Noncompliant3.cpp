// Custom

#include <iostream>

int globI;
double globD;

class S
{
public:
  int i;
  S() : i(globI++) {}
};

class T : public S
{
public:
  double d;
  T() : S(), d(globD++) {}
};

void f(const S *someSes, std::size_t count)
{
  const S *lastSes = someSes + (count - 1);
}

int main()
{
  T test[5];
  f(test, 5); // UndCC_Violation
}
