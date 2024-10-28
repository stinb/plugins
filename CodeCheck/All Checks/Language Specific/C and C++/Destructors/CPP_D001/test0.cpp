// SEI CERT C++

struct Base {
  virtual void f();
};

struct Derived : Base {};

void f() {
  Base *b = new Derived();
  // ...
  delete b; // UndCC_Violation
}
