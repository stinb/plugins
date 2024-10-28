// SEI CERT C++

#if __cplusplus >= 202002L // C++20

#include <new>

class Base {
  const int whichDerived;

protected:
  Base(int whichDerived) : whichDerived(whichDerived) {}

public:
  Base() : Base(0) {}
  void operator delete(Base *, std::destroying_delete_t);
};

struct Derived1 final : Base {
  Derived1() : Base(1) {}
};

struct Derived2 final : Base {
  Derived2() : Base(2) {}
};

void Base::operator delete(Base *b, std::destroying_delete_t) {
  switch (b->whichDerived) {
  case 0:
    b->~Base();
    break;
  case 1:
    static_cast<Derived1 *>(b)->~Derived1();
    break;
  case 2:
    static_cast<Derived2 *>(b)->~Derived2();
  }
  ::operator delete(b);
}

void f() {
  Base *b = new Derived1();
  // ...
  delete b; // UndCC_Valid
}

#endif // C++20
