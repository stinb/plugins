// AUTOSAR

#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>

struct A
{
  A() { throw std::runtime_error("example"); }
  A(std::uint8_t xx, std::uint8_t yy) : x(xx), y(yy) {}
  std::uint8_t x;
  std::uint8_t y;
};

void Foo(std::shared_ptr<A> a, std::shared_ptr<A> b) { }

int main(void)
{
  // UndCC_Valid
  std::shared_ptr<A> upA = std::make_shared<A>(4,6);
  //non-compliant
  std::shared_ptr<A> upA2 = std::shared_ptr<A>(new A(5,7)); // UndCC_Violation

  //non-compliant, potential memory leak, as A class constructor throws
  Foo(std::shared_ptr<A>(new A()), std::shared_ptr<A>(new A())); // UndCC_FalseNeg
  //non-compliant, potential memory leak, as A class constructor throws
  Foo(std::make_shared<A>(4,6), std::shared_ptr<A>(new A())); // UndCC_FalseNeg
  // UndCC_Valid, no memory leaks
  Foo(std::make_shared<A>(4,6), std::make_shared<A>(4,6));

  // UndCC_Valid by exception
  std::shared_ptr<A> ptr(new A(4,5), [](A* b) { delete b; } );

  return 0;
}
