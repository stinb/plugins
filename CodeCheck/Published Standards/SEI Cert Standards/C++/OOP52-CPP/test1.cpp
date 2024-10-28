// SEI CERT C++

#include <memory>

struct Base {
  virtual void f();
};

struct Derived : Base {};

void f() {
  // std::unique_ptr<Base> b = std::make_unique<Derived()>(); // Violation - covered by compilation error
}
