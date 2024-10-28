// Custom

#include <cstddef>

class A {
public:
  int a;
  class B {
  public:
    int b;
  };
  virtual void c() {}
};

void d()
{
  offsetof(A, a);    // UndCC_Violation
  offsetof(A::B, b); // UndCC_Valid
}
