// Custom

struct Base2 {
  virtual ~Base2() = default;
};

struct Derived2 final : Base2 {};
struct Derived3 final : Base2 {};

void f() {
  Base2 *b = new Derived3[10];
  // ...
  delete [] b;  // UndCC_Violation
}
