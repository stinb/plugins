// Custom

#include <cstddef>

class A {
public:
  int a;
  class B {
  public:
    int b;
  };
  void c() {}
};

void d()
{
  offsetof(A, a);    // UndCC_Valid
  offsetof(A::B, b); // UndCC_Valid
}
