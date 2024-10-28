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

void Foo(std::unique_ptr<A> a, std::unique_ptr<A> b) { }

int main(void)
{
  // UndCC_Valid
  std::unique_ptr<A> upA = std::make_unique<A>(4,6);
  //non-compliant
  std::unique_ptr<A> upA2 = std::unique_ptr<A>(new A(5,7)); // UndCC_Violation


  Foo(std::unique_ptr<A>(new A()), std::unique_ptr<A>(new A())); // UndCC_Violation

  Foo(std::make_unique<A>(4,6), std::unique_ptr<A>(new A())); // UndCC_Violation
  // UndCC_Valid, no memory leaks
  Foo(std::make_unique<A>(4,6), std::make_unique<A>(4,6));

  // UndCC_Valid by exception
  std::unique_ptr<A, std::function<void(A*)>> ptr(new A(4,5), [](A* b) { delete b; } );

  return 0;
}
